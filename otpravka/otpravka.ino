#include <AM2320.h>


#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
RF24 radio(8,10);
const uint32_t pipe = 111156789; // адрес рабочей трубы;
AM2320 sensor;
int up=900;
void setup() {

pinMode(4, OUTPUT); // объявляем пин 13 как выход
 radio.begin();                // инициализация
  delay(2000);
  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.setCRCLength(RF24_CRC_8); // размер контрольной суммы 8 bit или 16 bit
  radio.setPALevel(RF24_PA_MAX); // уровень питания усилителя RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  radio.setChannel(0x6f);         // установка канала
  radio.setAutoAck(false);       // автоответ
  radio.powerUp();               // включение или пониженное потребление powerDown - powerUp
  radio.stopListening();  //радиоэфир не слушаем, только передача
  radio.openWritingPipe(pipe);   // открыть трубу на отправку
 sensor.begin();
}

void loop() { 
  int data[1];

sensor.measure();
int temper= sensor.getTemperature();
data[0]=temper;
radio.write(&data, 1);

if(up==0){
  
  digitalWrite(4, HIGH);
  up=900;
  delay(200);
  digitalWrite(4, LOW);;
}else{
  up=up-1;
}
}
