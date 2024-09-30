from machine import Pin, I2C
import ssd1306

#You can choose any other combination of I2C pins
i2c = I2C(1,scl=Pin(3), sda=Pin(2), freq=400000)

oled_width = 128
oled_height = 32
oled = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)

oled.text('Hello, World 1!', 0, 0)
oled.text('Hello, World 2!', 0, 10)
oled.text('Hello, World 3!', 0, 20)

oled.show()