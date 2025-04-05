from machine import Pin
from time import sleep_ms

select_pin_nums = [6,7,8]
s_pins = [Pin(i, Pin.OUT) for i in select_pin_nums]
a_pin = Pin(10, Pin.IN, Pin.PULL_UP)

def select_pin(p, pins):
    for i in range(3):
        pins[i].value((p>>i)&1)

def read_buttons():
    btns = 0
    for i in range(8):
        select_pin(i, s_pins)
        btns += a_pin.value() << (i)
    return btns

last = 255

while True:
    b = read_buttons()
    if b != last:
        print('{:08b}'.format(b))
    last = b
    sleep_ms(250)
