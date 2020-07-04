# Reads serial monitor information and posts it to Twitter
import sys
from twython import Twython
import serial
import time
        
# Twitter API Information; From Project Details on Twitter Developers
apiKey = 'xxxxxxxxxxxxxx'
apiSecret = 'xxxxxxxxxxxxxx'
accessToken = 'xxxxxxxxxxxxxx'
accessTokenSecret = 'xxxxxxxxxxxxxx'

api = Twython(apiKey,apiSecret,accessToken,accessTokenSecret)

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=6) #R.Pi Input 
    ser.flush()
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            api.update_status(status=line)
            print(line)
