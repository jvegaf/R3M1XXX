#How to use 4067 analog multiplexer with MicroPython and NodeMCU
# Check the blog page https://www.gsampallo.com/blog/?p=475
from machine import Pin
import time

s0 = Pin(6, Pin.OUT)
s1 = Pin(7, Pin.OUT)
s2 = Pin(8, Pin.OUT)
a1 = Pin(10, Pin.IN, Pin.PULL_UP)
b1 = Pin(11, Pin.IN, Pin.PULL_UP)


multiplexor = [
    [0,0,0], #0
    [1,0,0], #1
    [0,1,0], #2
    [1,1,0], #3
    [0,0,1], #4
    [1,0,1], #5
    [0,1,1], #6
    [1,1,1], #7
]

def debounce_pin(pin):
    # wait for pin to change value
    # it needs to be stable for a continuous 20ms
    cur_value = pin.value()
    active = 0
    while active < 20:
        if pin.value() != cur_value:
            active += 1
        else:
            active = 0
            return cur_value
        time.sleep_ms(1)

def read_value(pinCx):
    s0.value(multiplexor[pinCx][0])
    s1.value(multiplexor[pinCx][1])
    s2.value(multiplexor[pinCx][2])

    lectura = [debounce_pin(a1), debounce_pin(b1)]

    return lectura


encoder0 = [0,0]
encoder1 = [0,0]
encoder2 = [0,0]

while True:

    encoder0 = read_value(0)
    encoder1 = read_value(1)
    encoder2 = read_value(2)

    print("encoder 0: {}".format(encoder0))
    print("encoder 1: {}".format(encoder1))
    print("encoder 2: {}".format(encoder2))

    time.sleep_ms(1000)