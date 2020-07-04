import sys
from twython import Twython
import serial
import time
        
# your twitter consumer and access information goes here
apiKey = 'Udop9tfuG1pLl2yjeWTeIRVNy'
apiSecret = 'xdhgC1PVLHZN70ZFKaVvoA7Zz0YLxn2ks481btSJZrTZUjkgDg'
accessToken = '239771981-NvxUyC5qrYj5g4vlQyoenCgbhWaYaR8zCAHgsw3p'
accessTokenSecret = 'DdLF1sCY1i7wtNafJzN2kzxDpBLXHDwDzUc3fiplwzv1c'

api = Twython(apiKey,apiSecret,accessToken,accessTokenSecret)

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=6)
    ser.flush()
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            api.update_status(status=line)
            print(line)