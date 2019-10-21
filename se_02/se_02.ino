//LIVRARIAS
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 2          //pin DHT sensor 
#define DHTTYPE DHT22     //tipo de sensor
#define OLED_ADDR   0x3C  // endereço i2c oled
#define LDRPIN A0         //pin DHT sensor
#define FC37PINA A2       //pin FC37 analogico sensor
#define FC37PIND 7        //pin FC37 digital sensor 

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
int rainMax = 1024;         // rain max value
int rainMin = 0;            // rain min value
int rainValue = 0;          // rain value

void setup() {
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20,32);
  display.print("Lendo dados...");
  
  display.display();
  
  Serial.begin(9600);

  pinMode(FC37PIND, INPUT);

  dht.begin();
  Serial.println(F("teste leituras:"));
}

void loop() {
  delay(2000); // dois segundos entre as medições (dth22 specs)

  humidityValue = dht.readHumidity(); // valor humidade em celcius
  temperatureValue = dht.readTemperature(); // valor temperatura celcius 
  lightValue = analogRead(LDRPIN);
  rainValue = analogRead(FC37PINA);

  // valida valores de sensores.
  if (isnan(humidityValue) || isnan(temperatureValue)) {
    Serial.println(F("Falha na leitura de sensor (DHT)!"));
    return;
  }
  // calcula heat index em celcius
  heatIndexValue = dht.computeHeatIndex(temperatureValue, humidityValue, false);

  ScreenUpdate(temperatureValue, humidityValue, heatIndexValue, lightValue, rainValue);
  
  Serial.print(F("Humidity: "));
  Serial.print(humidityValue);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperatureValue);
  Serial.print(F("°C "));
  Serial.print(F("  Heat index: "));
  Serial.print(heatIndexValue);
  Serial.print(F("°C "));
  Serial.print(lightValue);
  Serial.print(F("ldr "));
  Serial.print(rainValue);
  Serial.println(F(" rain "));
}

void ScreenUpdate(float tempValue,float humValue, float heatIndex, int luxValue, float precipValue){
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  // temperatura
  display.setCursor(0,16);
  display.print("Temp:");
  display.setCursor(30,16);
  display.print(tempValue);

  // heat index
  display.setCursor(0,25);
  display.print("HI:");
  display.setCursor(18,25);
  display.print(heatIndex);

  // humidade
  display.setCursor(0,35);
  display.print("Humidade:");
  display.setCursor(53,35);
  display.print(humValue);

  // lux
  display.setCursor(0,45);
  display.print("Lux:");
  display.setCursor(23,45);
  display.print(luxValue);

  // precipitacao
  display.setCursor(0,55);
  display.print("Chuva:");
  display.setCursor(35,55);
  display.print(precipValue);
  
  display.display();
}
