from SN74HC165N import SN74HC165N
class Registers:
    def __init__(self, row: int, col: int, register: SN74HC165N) -> None:
        self.row_count = row
        self.column_count = col
        self.total_count = row * col
        self.register: SN74HC165N  = register

    def read_shift_registers(self) -> int:
        """Three 74HC165 reads data from Shift Register."""
        self.register.doLowHigh(self.register.PIN_LATCH)

        register_values = 0
        for _ in range(self.total_count * 8):  # 8 Bit Reading for Every Shift Register
            register_values <<= 1
            if self.register.PIN_DATA.value():
                register_values |= 1
        self.register.doLowHigh(self.register.PIN_CLOCK)

        return register_values

    def getText(self) -> str:
        string = "".join([f"| {i} " for i in range(1,self.column_count+1)]) + "|\n"
        values = self.read_shift_registers()
        colors = {
            "red": "\033[0;31m",
            "green": "\033[1;32m",
            "end": "\033[0m",
        }
        for i in range(0, self.total_count * 8, 8):
            condition = values & (1 << i)
            string += "| {c}X{e} ".format(c=colors["green"] if condition else colors["red"], e=colors["end"])
            if(i // 8 % self.column_count == self.column_count - 1): # On every column lnegth to break line
                string += "|\n"
        return string
