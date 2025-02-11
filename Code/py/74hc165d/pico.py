import time
import os
from SN74HC165N import SN74HC165N
from Registers import Registers

# Shift Register Pin Definitions
PIN_LATCH = 13
PIN_CLOCK = 14
PIN_DATA = 6

register = SN74HC165N(PIN_LATCH, PIN_CLOCK, PIN_DATA)
registerGrid = Registers(1, 5, register)

while True:
    print(registerGrid.getText())
    time.sleep(1)
    print ("\033c", end="")
