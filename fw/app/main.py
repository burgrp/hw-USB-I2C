from machine import Pin, UART, I2C
import binascii

led = Pin(7, Pin.OUT)
alert = Pin(6, Pin.IN, Pin.PULL_UP)

i2c = I2C(0, scl = Pin(5), sda = Pin(4), freq = 400000)

commands = {
    "S": lambda: ",".join(str(addr) for addr in i2c.scan()),
    "R": lambda addr, len: binascii.hexlify(i2c.readfrom(int(addr), int(len))).decode("ascii"),
    "W": lambda addr, data: i2c.writeto(int(addr), binascii.unhexlify(data)),
    "A": lambda: str("F" if alert.value() == 1 else "T")
}

def do(cmd):
    led.value(1)
    try:
        cmd = cmd.split(",")
        cmd = [x.strip() for x in cmd]

        if cmd[0] in commands:
            result = commands[cmd[0]](*cmd[1:])
            return cmd[0] + str(result)
        else:
            raise Exception("Unknown command. Try S, R, W, A.")

    finally:
        led.value(0)

# print(do("S"))
# print(do(" R , 79 , 2 "))
# print(do(" W , 79, 0000 "))
# print(do(" A "))
# print(do(" W , 10, 0000 "))

while True:
    try:
        print(do(input()))
    except Exception as e:
        print("!" + str(e))
