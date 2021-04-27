#include <MQ135.h>

#include <DHT.h>

#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#define Gas A1
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
#define DHTPIN 3
LiquidCrystal_I2C lcd(0x27,16,2); 
RF24 radio(8, 10);
int data[1];
const uint32_t pipe = 111156789; // адрес рабочей трубы;
DHT dht(DHTPIN, DHTTYPE);
MQ135 gasSensor = MQ135(Gas); // инициализация объекта датчика
///////////////////////////////
int Tgatchick(){
  float t = dht.readTemperature();
  return t;
}
int Hdatchick()
{
  int h=dht.readHumidity();
  return h;
}
void setup() {
  // put your setup cod 
 radio.begin();  // инициализация
  delay(2000);
  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.setCRCLength(RF24_CRC_8); // размер контрольной суммы 8 bit или 16 bit
  radio.setChannel(0x6f);         // установка канала
  radio.setAutoAck(false);       // автоответ
  radio.openReadingPipe(1, pipe); // открыть трубу на приём
  radio.startListening();        // приём

  dht.begin();
  lcd.init();                       //  Инициируем работу с LCD дисплеем
    lcd.backlight();                  //  Включаем подсветку LCD дисплея
    lcd.setCursor(0, 0); 
     
  
    Serial.begin(9600);         
}

void loop() {
  float rzero = gasSensor.getRZero(); // чтение калибровочных данных
  Serial.println(rzero);
  // put your main code here, to run repeatedly:
lcd.clear();
lcd.print("t1=");
lcd.print(int(dht.readTemperature()));
lcd.print("C");
lcd.setCursor(6, 0);
lcd.print(" CO2=");
lcd.print(int(gasSensor.getPPM()));
lcd.print("p");
lcd.setCursor(0, 1);
lcd.print("t2=");
if(radio.available())
   {                                // Если в буфере имеются принятые данные
        radio.read(data, 1);
        Serial.println(data[0]);
        lcd.print(data[0]);
        lcd.print("C");
   }
   else {
    Serial.println("error-");
   lcd.print("E") ;
   }
   
lcd.setCursor(6,1);
lcd.print("  h=");
lcd.print(int(dht.readHumidity()));
lcd.print("%");
delay(2000);
Serial.println("---------------------------------------------------------------------------------");

}
