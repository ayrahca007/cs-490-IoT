import serial
from Adafruit_IO import RequestError, Client, Feed


ADAFRUIT_IO_USERNAME = 'Exl111'
ADAFRUIT_IO_KEY = 'aio_JTap22ybmVZwH6UJWgVbamw4aN9C'

serial_port = '/dev/ttyACM0';
baudrate=9600;

ser = serial.Serial(serial_port, baudrate)
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

list = [];

for i in range(20):

    list.append(ser.readline())
    list[i] = list[i].decode('utf-8')
    


print(list[1])
print(list[4])
print(list[7])


test = aio.feeds('temp')
aio.send_data(test.key, list[4])

test1 = aio.feeds('humidity')
aio.send_data(test1.key, list[1])

test1 = aio.feeds('airsensor')
aio.send_data(test1.key, list[7])


