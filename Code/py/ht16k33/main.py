# Ejemplo para Raspberry Pi Pico usando HT16K33 MicroPython driver
# Asegúrate de copiar ht16k33.py al Pico (p. ej. con Thonny, rshell o ampy)

from machine import Pin, I2C
import time
from ht16k33 import HT16K33

# Raspberry Pi Pico: I2C0 -> SDA=GP0 (Pin1), SCL=GP1 (Pin2)
i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=400000)

display = HT16K33(i2c, address=0x70, auto_write=False)

# Inicializar
display.clear()
display.set_display(True)
display.brightness(8)
display.write_display()

# Mostrar un contador con punto decimal
try:
    n = 0
    while True:
        s = "{:>4}".format(str(n % 10000))
        # ejemplo con decimal en el segundo dígito: "12.3" -> ajusta según lo que quieras
        display.print(s, justify_left=False)
        display.write_display()
        time.sleep(0.25)
        n += 1
except KeyboardInterrupt:
    display.clear()
    display.write_display()