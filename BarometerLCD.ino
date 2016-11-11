#include <Encoder.h>

// Connect a 10DOF http://www.geeetech.com/wiki/index.php/10DOF_ITG3200/ADXL345/HMC5883L/BMP085_sensor_breakout
// with a BMP085 pressure sensor to a 
// LCD screen to report pressure and temperature


   #include <Wire.h>
   #include <SPI.h>
   #define SLP 101325 //Sea Level Pressure definition
   #include <Adafruit_Sensor.h>
   #include <Adafruit_BMP085_U.h>
   #include <LiquidCrystal.h>
   #define SENSE_VCC 4
   #define SENSE_GND 3
   #define SAVE_BUTTON 0
   #define EWMA 0.2
   
     Encoder menu(7,8);
   
//   #include <Adafruit_Sensor.h>
 //   #include <Adafruit_BMP085.h>
//    Adafruit_BMP085 bmp;  // 10DOF stick Pins GND=3 VCC=4 SCL=5 SDA=6
     Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
     
     LiquidCrystal lcd(11, 12, 13, 18, 19, 20, 21); //RS(4),EN6(6),D4,D5,D6,D7 (11-14)

     // LCD16<>220<>GND LCD15<>+5  for LED
     //
     // LCD1<>GND  LCD2<>+5 LCD3=PotWiper10K LCD5=
     //
     //      LCD     Teensy2.0
     // LCD1 GND     
     // LCD2 VDD
     // LCD3 VD          Contrast voltage (use 10K pot wiper or PWM on 14 or 15?)    
     // LCD4 RS     11
     // LCD5 R/~W   12   (hold low for write)
     // LCD6 EN     13
     // LCD7 D0     14
     // LCD8 D1     15
     // LCD9 D2     16
     // LCD10 D3    17
     // LCD11 D4    18
     // LCD12 D5    19
     // LCD13 D6    20
     // LCD14 D7    21
     // LCD15 LED+  +5   backlight
     // LCD16 LED-  220 ohm -> GND
                   
   float altZ = -9999.0;
   float pressure_save = 101325;
   float pressureZ = -9999.0;
     
    void setup(void)
    {
      /* put the LCD in write mode */
    //  pinMode(12,OUTPUT);  // LCD R/~W
    //  digitalWrite(12,LOW);
      pinMode(SAVE_BUTTON,INPUT_PULLUP);
      pinMode(SENSE_VCC,OUTPUT); // Sensor
      pinMode(SENSE_GND,OUTPUT);
      pinMode(14,OUTPUT);
      analogWrite(14,80);
      digitalWrite(SENSE_VCC,HIGH);
      digitalWrite(SENSE_GND,LOW);
      Serial.begin(9600);
      Serial.println("Pressure Sensor Test"); Serial.println("");
      /* Initialise the sensor */
      if(!bmp.begin()){
        while(1){
          Serial.print("Ooops, no BMP085 detected ... Check wiring per https://github.com/drf5n/BarometerLCD");
          delay(1000);
        }
      }
       
      lcd.begin(16,2); //Init for a 16x2 LCD
      lcd.clear(); //Just to be sure...
      menu.write(0);
    }
     
    void loop(void)
    {
        
        /* Display atmospheric pressure in hPa in console*/
        /* First we get the current temperature from the BMP085 */
        float temperature, pressure;
        digitalWrite(SENSE_VCC,HIGH);
        bmp.getTemperature(&temperature);
        bmp.getPressure(&pressure);
        if (pressureZ == -9999.0) {  // Initialize EWMA quickly
            pressureZ = pressure;  
        } else {
            pressureZ = (1.0-EWMA)*pressureZ + EWMA * pressure;
        }
        pressure=pressureZ; // Use smoothed pressure
        if (!digitalRead(SAVE_BUTTON)) 
             pressure_save=pressure;
        digitalWrite(SENSE_VCC,LOW);

        /* Then convert the atmospheric pressure, SLP and temp to altitude */
        float seaLevelPressure = SLP;
        float altitude;
        //altitude=bmp.readAltitude(seaLevelPressure);
        altitude=bmp.pressureToAltitude(seaLevelPressure,pressure);
        if (altZ < -999.0) { // Fast EWMA initialization
           altZ = altitude;
           pressure_save=pressure;
        }
        else
        {
            altZ = (1.0-EWMA)*altZ + EWMA*altitude;
        }
        
        
        //Display data on console
        Serial.print("Temp: ");
        Serial.print(temperature);
        Serial.println(" C");
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println(" Pa");
        //Display Altitude on console
        Serial.print("Alt: ");
        Serial.print(altZ);
        Serial.println(" m");
        Serial.println("");
        
        //Display temperature on LCD
        lcd.setCursor(0,0);
        lcd.print("     C         m"); //We do it this way so that The "C" is always at the same place on the screen
        lcd.setCursor(0,0);
        lcd.print(temperature,1); //Some of the digits will overwrite the spaces left before "C"
        lcd.setCursor(7,0);
        lcd.print(altZ,2); //Some of the digits will overwrite the spaces left before "C"
       
        //Display Altitude on LCD
        lcd.setCursor(0,1);
        lcd.print("    .   d");
        lcd.setCursor(0,1);
        lcd.print(pressure/100.0,2);
        lcd.setCursor(11,1);
        lcd.print("  hPa");
        lcd.setCursor(8,1);
        lcd.print((pressure-pressure_save)/100.0,2);
       
      Serial.println("End of loop");
      delay(1000);
    //  digitalWrite(13,LOW);
    }
