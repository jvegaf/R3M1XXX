from machine import Pin
import time

class SN74HC165N():
    def __init__(self, latch_pin: int, clock_pin: int, data_pin: int):
        self.PIN_LATCH = Pin(latch_pin, Pin.OUT)
        self.PIN_CLOCK = Pin(clock_pin, Pin.OUT)
        self.PIN_DATA = Pin(data_pin, Pin.IN)
        
    def __setLow__(self, Pin: Pin):
        Pin.low()
        time.sleep_us(5)
        return Pin
    
    def __setHigh__(self, Pin: Pin):
        Pin.high()
        time.sleep_us(5)
        return Pin
    
    def doLowHigh(self, Pin: Pin):
        self.__setLow__(Pin)
        self.__setHigh__(Pin)
        return Pin
    

    