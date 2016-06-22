/*THIET BI QUAN TRAC MOI TRUONG PRODUCT BY NGOCKHANH-IT
  NGAY 22_06_2016
  LE NGOC KHANH
  ngockhanh111021994@gmail.com
*/

//thuvien khai bao su dung cua Arduino
#include <SoftwareSerial.h>
// thu vien khai bao dung, khi chay chuong trinh có đính kèm theo cùng thư mục
#include "DHT.h"

// khai vao Pin ket noi GPRS 2 và 3
SoftwareSerial GPRS(2, 3);

// Pin 4 lay du lieu DHT22
const int DHTPIN = 4;
//loai cam bien DHT22
const int DHTTYPE = DHT22;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // bat dau GPRS
  GPRS.begin(9600);
  // bat dau chuong trinh
  Serial.begin(9600);
  Serial.println("--->>>CHO KHOI DONG CHUONG TRINH<<<---");
  //khai vao led xuat ra
  pinMode(5, OUTPUT);
  delay(3000);

}

void loop()
{
  //bat,tat led kiem tra
  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(5, LOW);
  // lay doi tuong nhiet do va do am
  String h = (String)dht.readHumidity();
  String t = (String)dht.readTemperature();
  // xuat ra thong tin nhiet do va do am
  Serial.print("Nhiet Do:");
  Serial.println(t);
  Serial.print("Do Am:");
  Serial.println(h);
  // gui toiSQL
  SendSQL(h, t);
  //bat,tat led kiem tra
  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(5, LOW);
  // cho delay de gui du lieu tiep theo sau 10s
  delay(600000);
}

void SendSQL(String Humidity, String Temperature)
{
  String h = Humidity, t = Temperature, tmp = (String)random(100);
  // xuat thong tin nhiet do va do am
  Serial.println("--->>> Dang gui .... a= " + t + " ;b= " + h + " ;c=: " + tmp);
  // tao ket noi GPRS
  GPRS.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(500);
  GPRS.println("AT+SAPBR=3,1,\"APN\",\"www.quantracmoitruong.16mb.com\"");
  delay(500);
  GPRS.println("AT+SAPBR=1,1");
  delay(500);
  GPRS.println("AT+SAPBR=2,1");
  delay(3000);
  GPRS.println("AT+HTTPINIT");
  delay(500);
  GPRS.println("AT+HTTPPARA=\"CID\",1");
  delay(500);
  // gui du lieu den MySQL
  GPRS.println("AT+HTTPPARA=\"URL\",\"http://quantracmoitruong.esy.es/add_data.php?first_name=" + t + "&last_name=" + h + "&email_address=" + tmp + "\"");
  delay(3000);
  GPRS.println("AT+HTTPACTION=0");
  delay(15000);
  GPRS.println("AT+HTTPREAD");
  delay(5000);
  // dong tien ttinh va ngat ket noi
  GPRS.println("AT+HTTPTERM");
  delay(500);
  GPRS.println("AT+SAPBR=0,1");
  delay(500);
  // xac nhan gui thanh cong
  Serial.println("GUI OK");
}

