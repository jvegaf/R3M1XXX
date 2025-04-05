from machine import Pin
import time

DATA_WIDTH = 8


DATA_PIN = Pin(11, Pin.IN)
CLK_PIN = Pin(12, Pin.OUT)
LATCH_PIN = Pin(13, Pin.OUT)


def read_shift_registers() -> int:
    """Three 74HC165 reads data from Shift Register."""
    LATCH_PIN.low()
    time.sleep_us(5)
    LATCH_PIN.high()
    time.sleep_us(5)

    register_values = 0
    for _ in range(DATA_WIDTH):  # 8 Bit Reading for Every Shift Register
        register_values <<= 1
        if DATA_PIN.value():
            register_values |= 1
    CLK_PIN.low()
    time.sleep_us(5)
    CLK_PIN.high()

    return register_values


while True:
    states = read_shift_registers()
    print(states)
    time.sleep(0.5)
