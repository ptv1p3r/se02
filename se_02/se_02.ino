//LIVRARIAS
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 2          //pin DHT sensor 
#define DHTTYPE DHT22     //tipo de sensor
#define OLED_ADDR   0x3C  // endereço i2c oled
#define LDRPIN A0         //pin DHT sensor 

DHT dht(DHTPIN, DHTTYPE); //instacia sensor

// reset pin not used on 4-pin OLED module
Adafruit_SSD1306 display(-1);  // -1 = no reset pin

// 128 x 64 pixel display
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//VARIAVEIS
float humidityValue = 0;    // humidade
float temperatureValue = 0; // temperatura
float heatIndexValue = 0;   // heat index
int lightValue = 0;         // light value

void setup() {
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(27,30);
  display.print("Hello, world!");
  
  display.display();
  
  Serial.begin(9600);
  Serial.println(F("teste leituras:"));

  dht.begin();

}

void loop() {
  delay(2000); // dois segundos entre as medições (dth22 specs)

  humidityValue = dht.readHumidity(); // valor humidade em celcius
  temperatureValue = dht.readTemperature(); // valor temperatura celcius 
  lightValue = analogRead(LDRPIN);

  // valida valores de sensores.
  if (isnan(humidityValue) || isnan(temperatureValue)) {
    Serial.println(F("Falha na leitura de sensor (DHT)!"));
    return;
  }
  // calcula heat index em celcius
  heatIndexValue = dht.computeHeatIndex(temperatureValue, humidityValue, false);

  Serial.print(F("Humidity: "));
  Serial.print(humidityValue);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperatureValue);
  Serial.print(F("°C "));
  Serial.print(F("  Heat index: "));
  Serial.print(heatIndexValue);
  Serial.print(F("°C "));
  Serial.print(lightValue);
  Serial.println(F("ldr "));
}
