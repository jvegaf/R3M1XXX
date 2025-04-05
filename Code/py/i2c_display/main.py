from machine import Pin, I2C
from sh1106 import SH1106_I2C
import dseg7
from writer import Writer
from time import sleep

i2c = I2C(0,scl=Pin(1), sda=Pin(0), freq=400000)
oled = SH1106_I2C(128, 64, i2c, addr=0x3c, rotate=180)


def print_screen(display, string_to_display):
    font_writer = Writer(display, dseg7)
    font_writer.set_textpos(display,10,22)
    font_writer.printstring(string_to_display)
    display.show()
    
def display_clear(display):
    display.fill(0)
    display.show()

while True:
    display_clear(oled)
    print_screen(oled, "   8")
    sleep(1)
    print_screen(oled, "16")
    sleep(1)
    print_screen(oled, "32")
    sleep(1)
    print_screen(oled, "64")
    sleep(1)