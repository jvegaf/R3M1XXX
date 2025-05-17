import board
import busio
import time

ENCODER_ADDR = 0x36  # Dirección I2C

i2c = busio.I2C(scl=board.GP1, sda=board.GP0)


def write_register(register, data):
    while not i2c.try_lock():
        pass
    i2c.writeto(ENCODER_ADDR, bytes([register]) + data)
    i2c.unlock()


def read_register(length=1):
    result = bytearray(length)
    while not i2c.try_lock():
        pass
    i2c.readfrom_into(ENCODER_ADDR, result)
    i2c.unlock()
    return result


def encoder_set(rmin, rmax, rstep, rval, rloop):
    payload = bytearray(
        [
            rmin & 0xFF,
            (rmin >> 8) & 0xFF,
            rmax & 0xFF,
            (rmax >> 8) & 0xFF,
            rstep & 0xFF,
            (rstep >> 8) & 0xFF,
            rval & 0xFF,
            (rval >> 8) & 0xFF,
            rloop,
        ]
    )
    write_register(
        0x00, payload
    )  # Asumiendo que el registro 0x00 inicia la configuración


def encoder_get_value():
    data = read_register(2)  # Registro 0x10 hipotético
    return int.from_bytes(data, "little")

def encoder_is_pressed():
    data = read_register(1)  # Registro 0x40 hipotético para botón
    return bool(data[0] & 0x01)


# Inicialización
while not i2c.try_lock():
    pass
i2c.unlock()

print("I2C Encoder Test")
encoder_set(-50, 50, 1, 0, 1)
print("Encoder set")

last_value = None
last_pressed = None

while True:
    value = encoder_get_value()
    if value != last_value:
        print("Value changed:", value)
        last_value = value

    pressed = encoder_is_pressed()
    if pressed != last_pressed:
        if pressed:
            print("Switch was pressed")
        last_pressed = pressed

    time.sleep(0.02)
