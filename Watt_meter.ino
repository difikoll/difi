
/************************** Configuration ***********************************/
#include "config.h"
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>
#include <config.h>

Adafruit_INA219 ina219;
Adafruit_INA219 ina219b (0X41);
Adafruit_INA219 ina219c (0x44);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire); 
/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;

//Set vind1, 2 and 3 feed
AdafruitIO_Feed *vind_1 = io.feed("Vind 1 Watt");
AdafruitIO_Feed *vind_2 = io.feed("Vind 2 Watt");
AdafruitIO_Feed *vind_3 = io.feed("Vind 3 Watt");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C address 0x3C (for the 128x32)
  Serial.println("Hello!");
  
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.clearDisplay(); // Clear the display
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  ina219b.begin();
  ina219c.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();
  Serial.println("Measuring voltage and current with INA219 ...");
  
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();


  float shuntvoltage = ina219.getShuntVoltage_mV()-0.88;
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();
  float power_mW = ina219.getPower_mW();
  float loadvoltage = busvoltage + (shuntvoltage / 1000);

  float SV2 = ina219b.getShuntVoltage_mV();
  float BV2 = ina219b.getBusVoltage_V();
  float C_mA2 = ina219b.getCurrent_mA();
  float P_mW2 = ina219b.getPower_mW();
  float LV2 = BV2 + (SV2 / 1000);

  float SV3 = ina219c.getShuntVoltage_mV();
  float BV3 = ina219c.getBusVoltage_V();
  float C_mA3 = ina219c.getCurrent_mA();
  float P_mW3 = ina219c.getPower_mW();
  float LV3 = BV3 + (SV3 / 1000);
  
  Serial.print("Bus Voltage 1:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage 1: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage 1:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current 1:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power 1:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  Serial.print("Bus Voltage 2:   "); Serial.print(BV2); Serial.println(" V");
  Serial.print("Shunt Voltage 2: "); Serial.print(SV2); Serial.println(" mV");
  Serial.print("Load Voltage 2:  "); Serial.print(LV2); Serial.println(" V");
  Serial.print("Current 2:       "); Serial.print(C_mA2); Serial.println(" mA");
  Serial.print("Power 2:         "); Serial.print(P_mW2); Serial.println(" mW");
  Serial.println("");
  
  Serial.print("Bus Voltage 3:   "); Serial.print(BV3); Serial.println(" V");
  Serial.print("Shunt Voltage 3: "); Serial.print(SV3); Serial.println(" mV");
  Serial.print("Load Voltage 3:  "); Serial.print(LV3); Serial.println(" V");
  Serial.print("Current 3:       "); Serial.print(C_mA3); Serial.println(" mA");
  Serial.print("Power 3:         "); Serial.print(P_mW3); Serial.println(" mW");
  Serial.println("");
  
  display.setCursor(1,1);
  display.clearDisplay();
  display.print ("Amper: "); display.print(current_mA); display.println("mA");
  display.print ("Effekt: "); display.print(power_mW); display.println("mW");
  display.print ("Shuntvoltage: "); display.print(shuntvoltage); display.println("mV");
  display.display();

  
  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");;
 
  vind_1->save(power_mW);
  vind_2->save(P_mW2);
  vind_3->save(P_mW3);

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(6500);
  

}
