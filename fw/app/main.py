from machine import Pin, UART
import io
import uos

print("Nazdar!")

led = Pin(7, Pin.OUT)

#led.value(1)

# for line in sys.stdin:
#     if 'Exit' == line.rstrip():
#         break
#     print(f'Processing Message from sys.stdin *****{line}*****')

# class My:
#     def read(self, size):
#         pass
#     def write(self, data):
#         pass
#     def readinto(self, buf, nbytes):
#         pass
#     def seek(self, pos, whence):
#         pass
#     def flush(self):
#         pass
#     def close(self):
#         pass
#     pass

while True:
    line = input()
    print("=", line)