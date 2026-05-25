import tkinter as tk
from tkinter import ttk
import serial
import serial.tools.list_ports
import threading
import time
import queue

class DRUGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("DRU Subsystem - Drone Hangar Control")

        self.serial_conn = None
        self.running = False
        self.queue = queue.Queue()

        # Stato interno per bloccare comandi in pre-allarme/allarme
        self.pre_alarm = False
        self.alarm = False

        # Layout GUI

        # Porta seriale
        ttk.Label(root, text="Porta seriale:").grid(row=0, column=0, padx=5, pady=5)
        self.port_var = tk.StringVar()
        self.port_menu = ttk.Combobox(root, textvariable=self.port_var, values=self.get_ports(), width=20)
        self.port_menu.grid(row=0, column=1, padx=5, pady=5)

        self.connect_btn = ttk.Button(root, text="Connetti", command=self.connect_serial)
        self.connect_btn.grid(row=0, column=2, padx=5, pady=5)

        # Stati visualizzati
        labels = [
            ("Stato Drone:", "drone_state_var"),
            ("Temperatura (°C):", "temp_var"),
            ("Distanza da terra (m):", "distance_var"),
            ("Allarme:", "alarm_state_var"),
        ]

        for i, (text, var_name) in enumerate(labels, start=1):
            ttk.Label(root, text=text).grid(row=i, column=0, sticky='e')
            setattr(self, var_name, tk.StringVar(value="-"))
            ttk.Label(root, textvariable=getattr(self, var_name), width=15, relief="sunken").grid(row=i, column=1, sticky='w')

        # Pulsanti comandi
        self.takeoff_btn = ttk.Button(root, text="Take-off", command=lambda: self.send_command("takeoff"))
        self.land_btn = ttk.Button(root, text="Landing", command=lambda: self.send_command("land"))

        self.takeoff_btn.grid(row=7, column=0, padx=5, pady=10)
        self.land_btn.grid(row=7, column=1, padx=5, pady=10)

        # Log messaggi
        self.log = tk.Text(root, height=12, width=60, state='disabled')
        self.log.grid(row=8, column=0, columnspan=3, padx=5, pady=5)

        # Avvia aggiornamento GUI dalla coda
        self.root.after(100, self.process_queue)

    def get_ports(self):
        return [p.device for p in serial.tools.list_ports.comports()]

    def connect_serial(self):
        if self.serial_conn and self.serial_conn.is_open:
            self.running = False
            self.serial_conn.close()
            self.connect_btn.config(text="Connetti")
            self.log_message("Disconnesso")
            self.set_controls_state(False)
            return

        port = self.port_var.get()
        if not port:
            self.log_message("Seleziona una porta COM")
            return

        try:
            self.serial_conn = serial.Serial(port, 9600, timeout=1)
            self.running = True
            self.connect_btn.config(state="disabled")
            self.log_message(f"Connesso a {port}. In attesa di handshake...")
            self.set_controls_state(False)
            threading.Thread(target=self.wait_for_ready, daemon=True).start()
        except Exception as e:
            self.log_message(f"Errore connessione: {e}")
    
    def wait_for_ready(self):
        """Attende il messaggio READY da Arduino prima di abilitare i controlli"""
        start_time = time.time()
        while self.running and time.time() - start_time < 15:  # Timeout di 15 secondi
            try:
                if self.serial_conn.in_waiting:
                    line = self.serial_conn.readline().decode('utf-8').strip()
                    if line == "READY":
                        self.log_message("Arduino pronto!")
                        self.connect_btn.config(text="Disconnetti", state="normal")
                        self.set_controls_state(True)
                        threading.Thread(target=self.read_serial, daemon=True).start()
                        return
                    elif line:
                        self.queue.put(line)
                time.sleep(0.1)
            except Exception as e:
                self.log_message(f"Errore durante handshake: {e}")
                break
        
        if self.running:
            self.log_message("Timeout: Arduino non risponde")
            self.running = False
            self.serial_conn.close()
            self.connect_btn.config(state="normal")
            self.connect_btn.config(text="Connetti")

    def set_controls_state(self, connected):
        state = "normal" if connected else "disabled"
        self.takeoff_btn.config(state=state)
        self.land_btn.config(state=state)
        

    def send_command(self, cmd):
        if self.serial_conn and self.serial_conn.is_open:
            if self.pre_alarm and cmd in ("takeoff", "land"):
                self.log_message("Operazioni sospese: sistema in pre-allarme.")
                return
            if self.alarm and cmd in ("takeoff", "land"):
                self.log_message("Operazioni sospese: sistema in allarme.")
                return
            try:
                self.serial_conn.write((cmd + "\n").encode())
                self.log_message(f"Inviato comando: {cmd}")
            except Exception as e:
                self.log_message(f"Errore invio comando: {e}")
        else:
            self.log_message("Non connesso")

    def read_serial(self):
        while self.running:
            try:
                if self.serial_conn.in_waiting:
                    line = self.serial_conn.readline().decode('utf-8').strip()
                    if line:
                        self.queue.put(line)
                time.sleep(0.1)
            except Exception as e:
                self.queue.put(f"ERRORE: {e}")
                break

    def process_queue(self):
        try:
            while True:
                line = self.queue.get_nowait()
                self.handle_message(line)
        except queue.Empty:
            pass
        self.root.after(100, self.process_queue)

    def handle_message(self, msg):

        if msg.startswith("DRONE STATE:"):
            # Parse drone state: "DRONE STATE: TAKING_OFF", "DRONE STATE: REST", etc.
            try:
                state = msg.split(":")[1].strip().upper()
                self.drone_state_var.set(state)
                self.log_message(f"Drone state updated: {state}")
            except Exception as e:
                self.log_message(f"Errore parsing DRONE STATE: {e}")

        elif msg.startswith("RESET"):
            # Reset message: restore hangar state from ALARM to NORMAL
            self.pre_alarm = False
            self.alarm = False
            self.alarm_state_var.set("NORMAL")
            self.set_controls_state(True)
            self.log_message("Sistema resettato, hangar state ritornato a NORMAL")

        elif msg.startswith("DISTANCE:"):
            try:
                distance = msg.split(":")[1].strip().upper();
                self.distance_var.set(distance);
            except Exception as e:
                self.log_message(f"Errore parsing DISTANCE: {e}")

        elif msg.startswith("TEMP:"):
            try:
                temp = msg.split(":")[1].strip().upper();
                self.temp_var.set(temp);
            except Exception as e:
                self.log_message(f"Errore parsing DISTANCE: {e}")
        
        elif msg.startswith("PREALARM"):
            self.pre_alarm = True;
            self.alarm_state_var.set("PRE-ALARM")
            self.log_message("Sistema in stato di pre-allarme")
        
        elif msg.startswith("ALARM"):
            self.alarm = True;
            self.alarm_state_var.set("ALARM")
            self.log_message("Sistema in stato di allarme")

        else:
            self.log_message("Arduino: " + msg)



    def log_message(self, text):
        self.log.configure(state='normal')
        self.log.insert(tk.END, text + "\n")
        self.log.see(tk.END)
        self.log.configure(state='disabled')

if __name__ == "__main__":
    root = tk.Tk()
    app = DRUGUI(root)
    root.mainloop()
