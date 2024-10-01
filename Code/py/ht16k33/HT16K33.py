import time, sys
from machine import Pin, I2C

class HT16K33:

    def __init__(self, i2cAddress, i2cBus = 1):

        # set up i2c connection
        # self.i2cAddress = i2cAddress
        self.bus = I2C(0, scl=Pin(1), sda=Pin(0), freq=400000)


        # set initial HT16K33 state
        self.set_system_oscillator('ON')
        self.display_status()       # turn display on
        self.set_brightness(15)     # set initial display brightness
        self.set_interrupt()        # turn interrupt off

        return

    def set_system_oscillator(self, state = 'ON'):
        """set_system_oscillator, function to turn the HT16K33 system
        oscillator on/off"""

        if state == 'ON':
            self.bus.writeto(self.i2cAddress, 0x21) # turn system oscillator on
        else:
            self.bus.writeto(self.i2cAddress, 0x20) # turn system oscillator off

        return

    def display_status(self, state = 'ON', blinkRate = 0):
        """display_status, function to turn the display on/off and set
        the blink rate (0.5, 1 or 2 Hz)"""

        stateBit = 0b0
        blinkBits = 0b00

        if state == 'ON':
            stateBit = 0b1

        if blinkRate == 0.5:
            blinkBits = 0b11
        elif blinkRate == 1:
            blinkBits = 0b10
        elif blinkRate == 2:
            blinkBits = 0b01

        displayByte = 0x80 | ((blinkBits<<1) + stateBit)

        # write values to HT16K33
        self.bus.writeto(self.i2cAddress, displayByte)

        return

    def set_brightness(self, level = 15):
        """set_brightnesss, function to set the display brightness
        with a range of 0-15"""

        # check that level is an integer and force it to
        # be in the range of 0 - 15
        if isinstance(level, int):
            if level < 0:
                level = 0
            elif level > 15:
                level = 15
        else:
            level = 0
            print("Error: Non integer brightness level passed")

        brightnessByte = 0xE0 | level

        # write values to HT16K33
        self.bus.writeto(self.i2cAddress, brightnessByte)

        return

    def set_interrupt(self, interrupt = False, active = 'HIGH'):
        """set_interrupt, function to set the interrupt on/off and
        active high/low"""

        if interrupt == True:
            if active == 'HIGH':
                self.bus.writeto(self.i2cAddress, 0xA3) # interrupt on, active high
            else:
                self.bus.writeto(self.i2cAddress, 0xA1) # interrupt on, active low
        else:
            self.bus.writeto(self.i2cAddress, 0xA0) # interrupt off

        return

    def read_keyscan(self):
        """read_keyscan, function to read the 6 keyscan data registers and
        return a list of the register data"""

        # read 6 registers starting at 0x40
        keyscan = self.bus.read_i2c_block_data(self.i2cAddress, 0x40, 6)

        return keyscan



class HT16K33_LED_MATRIX(HT16K33):

    def __init__(self, i2cAddress, i2cBus = 1, size = [8, 8], adafruit = False):

        # call super class HT16K33 to set up common
        # variables and methods
        HT16K33.__init__(self, i2cAddress, i2cBus)

        self.cols = size[0]  # number of columns in LED matrix
        self.rows = size[1]  # number of rows in LED matrix
        self.adafruit = adafruit

        # set up list to hold the individual LED states
        # 0 - off, 1 - on
        self.matrix = self.create_matrix(self.cols, self.rows)

        return

    def create_matrix(self, cols, rows):
        """create_matrix, function to create a list to hold the
        individual LED states.  Set all initially to 0 - off."""

        matrixSize = int(cols*rows)
        matrix = [0 for i in range(matrixSize)]

        return matrix

    def fill_matrix(self, value = 1):
        """fill_matrix, function to set the list holding the LED
        states to either all on - 1, or all off - 0"""

        for i in range (len(self.matrix)):
            self.matrix[i] = value

        return

    def invert_matrix(self):
        """invert_matrix, function invert all the values in the
        list holding the LED states. 1s become 0s, 0s become 1s"""

        for i in range(len(self.matrix)):
            if self.matrix[i] == 1:
                self.matrix[i] = 0
            else:
                self.matrix[i] = 1

        return

    def __setitem__(self, key, value):
        """__setitem__, function to set an indivdual or
        a range of LEDs values in the matrix list"""

        self.matrix[key] = value

        return

    def __getitem__(self, key):
        """__getitem__, function to get/return an indivdual or
        a range of LEDs values from the matrix list"""

        return self.matrix[key]

    """def rotate_matrix(self, numTimes = 1):
        #rotate_matrix, function to rotate the LED matrix values
        #90 degrees counter clockwise numTimes

        print(self.rows, self.cols)

        tempArray = np.array(self.matrix).reshape(self.rows, self.cols)
        tempArray = np.rot90(tempArray, numTimes, axes = (0,1))

        self.matrix = list(tempArray.flatten())

        work in progress

        return"""

    def rotate_matrix(self, numTurns = 1):
        """rotate_matrix, function to rotate the LED matrix
        90 degrees clockwise per turn.  For 8x8 matrixes"""

        for n in range(numTurns):
            tempMatrix = [0 for i in range(64)]
            led = 0
            for j in range(7, -1, -1):
                for i in range(0,8):
                    tempMatrix[j+i*8] = self.matrix[led]
                    led += 1
            self.matrix = tempMatrix

        return

    def show_matrix(self):
        """show_matrix, function to write data to the display to
        show what is stored in self.matrix, the list holding what
        all the individual LED states should be."""

        matrix = self.matrix
        cols =  self.cols
        rows = self.rows

        # list to store all data to write to the display
        # will end up being 15 or 16 bytes in size
        data_to_write = [0]

        row = 0
        column = 0

        # assume LED matrixes are either 8 or 16 columns.
        # gather individual LED data for a row and combine into
        # one (8 columns) or two (16 columns) bytes of data that
        # can then be written to the display
        if cols == 8:
            for i in range(len(matrix)):
                if column == 8:
                    column = 0
                    row += 2
                    data_to_write.append(0x00) # write all zeros to columns 9-16 for 8 column display
                    data_to_write.append(0)

                data_to_write[row] = data_to_write[row] + (matrix[i]<<column)
                column += 1
                #print(hex(data_to_write[row]))

            data_to_write.append(0x00)

        else:
            for i in range(len(matrix)):
                if column == 8:
                    column = 0
                    row += 1
                    data_to_write.append(0)

                data_to_write[row] = data_to_write[row] + (matrix[i]<<column)
                column += 1
                #print(hex(data_to_write[row]))

        if self.adafruit == True:
            for i in range(len(data_to_write)):
                tempValue = (data_to_write[i] & 0x01) << 7
                data_to_write[i] = data_to_write[i] >> 1 | tempValue

        # write values to display/HT16K33
        self.bus.write_i2c_block_data(self.i2cAddress, 0x00, data_to_write)

        return


class HT16K33_7_SEGMENT(HT16K33):

    def __init__(self, i2cAddress, i2cBus = 1, justification = "RIGHT"):

        # call super class HT16K33 to set up common
        # variables and methods
        HT16K33.__init__(self, i2cAddress, i2cBus)

        # set display justification
        self.set_justification(justification)

        # Dictionary of valid characters
        self.numberList = {'0':0x3F, '1':0x06,
                           '2':0x5B, '3':0x4F,
                           '4':0x66, '5':0x6D,
                           '6':0x7C, '7':0x07,
                           '8':0x7F, '9':0x67,
                           ':':0x02, '-':0x40}

        return

    def set_justification(self, justification = "RIGHT"):
        """set_justification, set display justification to either
        left or right"""

        if justification == "LEFT":
            self.justification = justification
        else:
            self.justification = "RIGHT"

        return


    def write_numbers(self,value, colonOn = False):
        """write_numbers, function to write data to the HT16K33
        to display the value given"""

        decimalPoint = -1
        numList = [0,0,0,0]
        indexOffset = 0

        # if value is None, all 0x00s will be written to the display
        # except for colon value
        if value != None:

            # check that value is a number otherwise raise an error
            if not isinstance(value,(float,int)):
                raise TypeError("Expected: float, int")

            # check that value falls with the displayable range
            # of the 4 number display
            if value > 9999:
                value = 9999
            elif value < -999:
                value = -999

            # check for a decimal point and determine
            # if and where it can be displayed
            if isinstance(value, float):
                value = str(value)
                decimalPoint = value.find(".")
                if (decimalPoint > 0 and decimalPoint <=4):
                    decimalPoint -= 1
                    value = value.replace(".","")

                # check that the float number can be displayed on
                # the 4 number display
                if len(value) >4:
                    value = value[:4]

            elif isinstance(value,int):
                # convert value to string
                value = str(int(value))

            # if value is less than 4 characters long set justification

            if len(value) < 4:
                if self.justification == "RIGHT":
                    indexOffset = 4 - len(value)

            # break value into individual digits to display
            for i in range(0,len(value)):
                numList[i+indexOffset] = self.numberList[value[i]]
                if decimalPoint == i:
                    numList[i+indexOffset] = numList[i+indexOffset] | 0x80 # add decimal point

        # set colon state
        if colonOn == True:
            colon = self.numberList[":"]
        else:
            colon = 0x00

        # write values to display
        self.bus.write_i2c_block_data(self.i2cAddress, 0x00, [numList[0], 0x00,
                                                              numList[1], 0x00,
                                                              colon, 0x00,
                                                              numList[2], 0x00,
                                                              numList[3]])

        return


if __name__ == "__main__":

    mode = 0

    if mode == 0:

        ledMatrix = HT16K33_LED_MATRIX(0x70, 1, [8,8], True)
        ledMatrix.fill_matrix(1)
        ledMatrix.show_matrix()
        time.sleep(2)
        ledMatrix.set_brightness(4)
        ledMatrix.display_status('ON', 1) # turn blinking on at 1Hz
        time.sleep(4)
        ledMatrix.display_status('ON', 0) # turn blinking off

        ledMatrix.matrix = [0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0,
                            0, 1, 1, 0, 0, 1, 1, 0,
                            0, 1, 1, 0, 0, 1, 1, 0,
                            0, 0, 0, 0, 0, 0, 0, 0,
                            0, 1, 0, 0, 0, 0, 1, 0,
                            0, 0, 1, 1, 1, 1, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0]
        ledMatrix.show_matrix()
        time.sleep(2)
        ledMatrix.invert_matrix()
        ledMatrix.show_matrix()
        time.sleep(2)
        ledMatrix.invert_matrix()
        ledMatrix.show_matrix()
        time.sleep(2)

        #Keyscan demo
        ledMatrix.set_interrupt(True, 'HIGH')
        startTime = time.time()
        loopLength = 10

        while (time.time()-startTime)<loopLength:
            print(ledMatrix.read_keyscan())
            time.sleep(0.2)

        #Turn HT16K33 off
        ledMatrix.set_interrupt(False, 'HIGH')
        ledMatrix.display_status('OFF')
        ledMatrix.set_system_oscillator('OFF')

    elif mode == 1:

        sevenSegment = HT16K33_7_SEGMENT(0x70, 1)
        sevenSegment.set_brightness(7)
        #sevenSegment.set_justification("LEFT")
        #sevenSegment.write_numbers("1234") # raises error as intended
        #time.sleep(2)
        sevenSegment.write_numbers(-256.7)
        time.sleep(2)
        sevenSegment.write_numbers(91.3)
        time.sleep(2)
        sevenSegment.write_numbers(17.96)
        time.sleep(2)
        sevenSegment.write_numbers(None) # clear display
        time.sleep(2)
        sevenSegment.write_numbers(-8.5)
        time.sleep(2)

        # turn colon on and off every 1 second
        for i in range(0,5):
            sevenSegment.write_numbers(1004)
            time.sleep(1)
            sevenSegment.write_numbers(1004, True)
            time.sleep(1)

        #Turn HT16K33 off
        sevenSegment.write_numbers(None)
        sevenSegment.display_status('OFF')
        sevenSegment.set_system_oscillator('OFF')




