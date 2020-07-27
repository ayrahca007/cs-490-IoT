import serial
import time

serial_port = '/dev/ttyACM1';
baudrate=9600;
write_to_file_path = "output.txt";

output_file = open(write_to_file_path, "w+");
ser = serial.Serial(serial_port, baudrate)

for i in range(1): 
    line = ser.readline();
    line = line.decode("utf-8")
    print(line);
    output_file.write(line);

output_file.close();
