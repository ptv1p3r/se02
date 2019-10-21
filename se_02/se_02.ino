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
/*  RL=500/lux
 *  V0=5*(RL/(RL+R))
 *  V0=LDR_value*ADC_value
 *  lux=(250/V0)-50
 */
float humidityValue = 0;      // humidade
float temperatureValue = 0;   // temperatura
float heatIndexValue = 0;     // heat index
float lightValue = 0;         // light value
float luxValue = 0;           // lux value
float adcValue =0.0048828125;
int rainMax = 1024;           // rain max value
int rainMin = 0;              // rain min value
int rainValue = 0;            // rain value
int rainMap = 0;              // rain converted map values
String rainDescription;       // rain description

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(2,0);
  display.print("Weather Station Rev.1");
  display.setCursor(4,18);
  display.print("A iniciar sistema...");
  display.setCursor(20,32);
  display.print("Lendo dados...");
  
  display.display();
  
  pinMode(FC37PIND, INPUT);

  dht.begin();
  
/*  Serial.begin(9600);
  Serial.println(F("teste leituras:"));*/
}

void loop() {
  delay(2000); // dois segundos entre as medições (dth22 specs)

  humidityValue = dht.readHumidity(); // valor humidade em celcius
  temperatureValue = dht.readTemperature(); // valor temperatura celcius 
  lightValue = analogRead(LDRPIN);
  luxValue = (250.000000/(adcValue * lightValue))- 50.000000; // formula p conversao para lux
  rainValue = analogRead(FC37PINA);
  rainMap = map(rainValue, rainMin, rainMax, 0, 3);

  // valida valores de sensores.
  if (isnan(humidityValue) || isnan(temperatureValue)) {
    display.clearDisplay();
    display.setCursor(0,32);
    display.print("Falha na leitura de sensor (DHT)!");
    display.display();
    return;
  }
  // calcula heat index em celcius
  heatIndexValue = dht.computeHeatIndex(temperatureValue, humidityValue, false);

  // le valor digital
  if (digitalRead(FC37PIND) == HIGH){ //seco
    rainDescription = "Sem chuva";
  } else {
    switch (rainMap) {
      case 0:    
        rainDescription = "Forte";
        break;
      case 1:    
        rainDescription = "Fraca";
        break;
    }
  }
  
  ScreenUpdate(temperatureValue, humidityValue, heatIndexValue, luxValue, rainDescription);
  
/* DEBUG 
 *Serial.print(F("Humidity: "));
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
  Serial.print("sensor: ");
  Serial.print(rainValue);
  Serial.print(" Mapa: ");
  Serial.print(rainMap);
  Serial.print(" Texto: ");
  Serial.println(rainDescription);*/
}

void ScreenUpdate(float tempValue,float humValue, float heatIndex, float luxValue, String precipValue){
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  // header
  display.setCursor(2,0);
  display.print("Weather Station Rev.1");
  
  // temperatura
  display.setCursor(0,16);
  display.print("Temp:");
  display.setCursor(30,16);
  display.print(tempValue, 2);
  display.setCursor(63,16);
  display.print((char)247);
  display.setCursor(69,16);
  display.print("C");

  // heat index
  display.setCursor(0,26);
  display.print("HI:");
  display.setCursor(18,26);
  display.print(heatIndex, 2);
  display.setCursor(50,26);
  display.print((char)247);
  display.setCursor(56,26);
  display.print("C");

  // humidade
  display.setCursor(0,36);
  display.print("Humidade:");
  display.setCursor(53,36);
  display.print(humValue, 2);
  display.setCursor(85,36);
  display.print("%");

  // lux
  display.setCursor(0,46);
  display.print("Lux:");
  display.setCursor(23,46);
  display.print(luxValue, 2);

  // precipitacao
  display.setCursor(0,56);
  display.print("Chuva:");
  display.setCursor(35,56);
  display.print(precipValue);
  
  display.display();
}
