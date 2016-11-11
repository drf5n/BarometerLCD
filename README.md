# BarometerLCD

This sketch combines a BMP085 pressure sensor with a 16X2 based on HD44780 chip and a Teensy2.0

The BMP085 sensor I used was on a 10DOF Geeetech IMU.

The Teensy2.0 is from https://www.pjrc.com/store/teensy.html 

I saw code like http://arduinolearning.com/code/bmp085-temperature-readings-lcd.php but used the updated Adafruit unified library from  https://github.com/adafruit/Adafruit_BMP085_Unified

I also added some conversions to meters above standard SLP, and a EWMA smoother to mitigate the noise.



