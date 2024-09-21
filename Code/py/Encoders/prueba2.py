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



def read_value(pinCx):
    s0.value(multiplexor[pinCx][0])
    s1.value(multiplexor[pinCx][1])
    s2.value(multiplexor[pinCx][2])
    lectura = [a1.value(), b1.value()]


    return lectura

pinCx = list(range(0,7))

print("Begining")

while True:

    encoder0 = read_value(0)
    encoder1 = read_value(1)

    print("encoder 0: {}".format(encoder0))
    print("encoder 1: {}".format(encoder1))

    time.sleep_ms(200)