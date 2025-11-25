# MicroPython driver for HT16K33 (ajustado para Raspberry Pi Pico)
# Basado en lpaseen/ht16k33 (https://github.com/lpaseen/ht16k33)
# Requisitos: MicroPython con módulo 'machine' (I2C).
#
# Uso:
#   from machine import I2C, Pin
#   i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=400000)  # Pico: SDA=GP0, SCL=GP1 (I2C0)
#   display = HT16K33(i2c, address=0x70, auto_write=False)
#   display.clear(); display.write_display()
#
# Nota: el mapeo de la RAM y del punto decimal es el más común para los backpacks HT16K33;
# puede necesitar ajuste si tu módulo es distinto.

from machine import I2C
import time

_DEFAULT_ADDRESS = 0x70

# HT16K33 commands
_CMD_SYSTEM_SETUP = 0x20
_CMD_DISPLAY_SETUP = 0x80
_CMD_BRIGHTNESS = 0xE0

class HT16K33:
    def __init__(self, i2c: I2C, address=_DEFAULT_ADDRESS, auto_write=True):
        """
        i2c: instancia machine.I2C ya inicializada
        address: dirección I2C del HT16K33 (por defecto 0x70)
        auto_write: si True, operaciones que cambian buffer llaman write_display() automáticamente
        """
        self.i2c = i2c
        self.address = address
        self.auto_write = auto_write
        # Display RAM: 16 bytes
        self.buffer = bytearray(16)
        # Inicializar chip: System on (oscillator on)
        self._write_cmd(_CMD_SYSTEM_SETUP | 0x01)
        # Display on, no blink
        self.set_display(True)
        # Default brightness medio
        self.brightness(8)

    def _write_cmd(self, cmd):
        """
        Envía un solo byte de comando al dispositivo.
        Intentamos writeto (común en MicroPython), y en caso de fallo intentamos writeto_mem.
        """
        try:
            self.i2c.writeto(self.address, bytes([cmd]))
        except Exception:
            try:
                # Algunas implementaciones usan writeto_mem(addr, memaddr, buf)
                self.i2c.writeto_mem(self.address, 0x00, bytes([cmd]))
            except Exception:
                # No podemos hacer más aquí; silenciamos, pero en general no debe ocurrir.
                pass

    def set_display(self, on=True, blink_rate=0):
        """
        Enciende/apaga la pantalla y configura blink (0=off, 1=2Hz, 2=1Hz, 3=0.5Hz)
        """
        if blink_rate < 0 or blink_rate > 3:
            blink_rate = 0
        cmd = _CMD_DISPLAY_SETUP | (blink_rate << 1) | (1 if on else 0)
        self._write_cmd(cmd)

    def brightness(self, brightness):
        """
        brightness: 0..15
        """
        if brightness < 0:
            brightness = 0
        if brightness > 15:
            brightness = 15
        self._write_cmd(_CMD_BRIGHTNESS | brightness)

    def clear(self):
        for i in range(16):
            self.buffer[i] = 0x00
        if self.auto_write:
            self.write_display()

    def write_display(self):
        """
        Escribe el buffer completo al HT16K33.
        El registro de inicio es 0x00 y se envían 16 bytes.
        """
        try:
            # writeto_mem(addr, memaddr, buf) es la forma preferida si está disponible
            self.i2c.writeto_mem(self.address, 0x00, self.buffer)
        except Exception:
            # Fallback: enviar 0x00 seguido de los 16 bytes
            data = bytes([0x00]) + bytes(self.buffer)
            try:
                self.i2c.writeto(self.address, data)
            except Exception:
                # error de comunicación: ignoramos aquí (el usuario puede atrapar excepciones desde arriba)
                pass

    # --- 8x8 matrix helpers (primer pase) ---
    def set_pixel(self, x, y, value):
        """
        Coloca/unsetea un pixel en coordenadas x(0..7), y(0..7).
        Mapeo asumido: cada columna usa la dirección idx = x*2, y corresponde al bit (0..7).
        Este mapeo puede necesitar ajustes según tu hardware/backpack.
        """
        if not (0 <= x <= 7 and 0 <= y <= 7):
            return
        idx = x * 2
        if value:
            self.buffer[idx] |= (1 << y)
        else:
            self.buffer[idx] &= ~(1 << y)
        if self.auto_write:
            self.write_display()

    def set_column(self, x, value_byte):
        """Escribe toda la columna x con un byte (bits 0..7 -> rows)"""
        if not (0 <= x <= 7):
            return
        idx = x * 2
        self.buffer[idx] = value_byte & 0xFF
        if self.auto_write:
            self.write_display()

    def set_row(self, y, value_byte):
        """
        Escribe una fila (y) con value_byte en forma de columnas.
        Nota: dependiendo del mapeo del módulo puede que necesites girar/transponer.
        """
        if not (0 <= y <= 7):
            return
        for x in range(8):
            bit = (value_byte >> x) & 0x01
            self.set_pixel(x, y, bit)
        # set_pixel ya hace write_display si auto_write=True

    # --- 7-seg helpers ---
    # Tabla de segmentos básica para 7-seg común (gfedcba)
    _SEGMENTS = {
        ' ': 0x00,
        '-': 0x40,
        '_': 0x08,
        '0': 0x3F,
        '1': 0x06,
        '2': 0x5B,
        '3': 0x4F,
        '4': 0x66,
        '5': 0x6D,
        '6': 0x7D,
        '7': 0x07,
        '8': 0x7F,
        '9': 0x6F,
        'A': 0x77,
        'B': 0x7C,
        'C': 0x58,
        'D': 0x5E,
        'E': 0x79,
        'F': 0x71,
        'a': 0x5F,
        'b': 0x7C,
        'c': 0x58,
        'd': 0x5E,
        'e': 0x7B,
        'f': 0x71,
        'H': 0x76,
        'h': 0x74,
        'L': 0x38,
        'P': 0x73,
        'o': 0x5C,
        'r': 0x50,
    }

    def _encode_char(self, ch):
        if ch in self._SEGMENTS:
            return self._SEGMENTS[ch]
        # fallback: space
        return 0x00

    def set_digit_raw(self, position, segments, dot=False):
        """
        position: 0..3 (para displays de 4 dígitos). Mapea en buffer en posiciones habituales.
        segments: byte con segmentos (gfedcba)
        dot: si True activa el punto decimal (añade bit 0x80)
        """
        if not (0 <= position <= 3):
            return
        if dot:
            segments |= 0x80
        idx = position * 2
        self.buffer[idx] = segments
        if self.auto_write:
            self.write_display()

    def set_digit(self, position, ch, dot=False):
        seg = self._encode_char(ch)
        self.set_digit_raw(position, seg, dot)

    def print(self, text, justify_left=True):
        """
        Imprime hasta 4 caracteres en 7-seg. Maneja puntos decimales que siguen a un carácter,
        por ejemplo "12.3" -> '1' '2.' '3' (si cabe).
        Si justify_left es False, se alinea a la derecha.
        """
        s = str(text)
        # Expand/trim logic: remove unexpected characters? dejamos tal cual
        # Primero compactar secuencias como "1.": queremos unir '.' al carácter anterior.
        # Implementación: iterar por el string y combinar.
        chars = []
        i = 0
        while i < len(s) and len(chars) < 4:
            ch = s[i]
            if ch == '.':
                # si '.' aparece al inicio o sin anterior, convertimos en '.' solo no mostrado; mejor añadir al anterior si existe
                if len(chars) == 0:
                    # nada que hacer, saltar
                    i += 1
                    continue
                # marcar dot en el anterior
                prev = chars[-1]
                if isinstance(prev, tuple):
                    # ya tenía dot -> dejar
                    pass
                else:
                    chars[-1] = (prev, True)
                i += 1
                continue
            # si el siguiente carácter es '.' lo unimos
            dot = False
            if i + 1 < len(s) and s[i+1] == '.':
                dot = True
                i += 1  # el '.' será consumido ahora
            if dot:
                chars.append((ch, True))
            else:
                chars.append((ch, False))
            i += 1

        # Rellenar/truncar a 4 y justificar
        # Convertir a lista de 4 elementos (caracter, dot)
        digits = [(' ', False)] * 4
        if justify_left:
            for idx, item in enumerate(chars):
                if isinstance(item, tuple):
                    ch, dot = item
                else:
                    ch, dot = item, False
                if idx < 4:
                    digits[idx] = (ch, dot)
        else:
            # right justify
            start = 4 - len(chars)
            if start < 0:
                start = 0
            for j, item in enumerate(chars[-4:]):
                idx = start + j
                if isinstance(item, tuple):
                    ch, dot = item
                else:
                    ch, dot = item, False
                digits[idx] = (ch, dot)

        # Escribir los dígitos en buffer
        for pos in range(4):
            ch, dot = digits[pos]
            self.set_digit(pos, ch, dot)

# End of ht16k33.py