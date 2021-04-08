# BarometerLCD

This sketch combines a BMP085 pressure sensor with a 16X2 based on HD44780 chip and a Teensy2.0

![Sensor, LCD and Teensy on Breadboard](https://raw.githubusercontent.com/drf5n/BarometerLCD/master/Breadboard.png "Sensor, LCD and Teensy on Breadboard")

The BMP085 sensor I used was on a 10DOF Geeetech IMU per http://www.geeetech.com/wiki/index.php/10DOF_ITG3200/ADXL345/HMC5883L/BMP085_sensor_breakout

The Teensy2.0 is from https://www.pjrc.com/store/teensy.html 

I saw code like http://arduinolearning.com/code/bmp085-temperature-readings-lcd.php but used the updated Adafruit unified library from  https://github.com/adafruit/Adafruit_BMP085_Unified

I also added some conversions to meters above standard SLP to make it into an altimiter, and a EWMA smoother to mitigate the noise.


One thing I tried to do was choose the pins to minimize extra connections by aligning the LCD and the sensor with the sides of the chip.  I supply power to the devices directly with the pins of the Teensy development board and add just a couple wires on the breadboard to handle the extra signals.  The sensor is powered and grounded directly by pins adjacent to the SDA and SCL pins.

I am starting to add some complexity to retain a historical pressure and possibly display other units, e.g. Pa, hPa,  mmHg, inH2O, meters, degC, degF, etc.  I think this could work easily with an encoder to scroll through the various options. 



