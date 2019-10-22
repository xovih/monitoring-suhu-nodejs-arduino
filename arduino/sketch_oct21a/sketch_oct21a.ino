#include "DHT.h" //library sensor yang telah diimportkan
#include <SPI.h>
#include <Ethernet.h>
 
#define DHTPIN 4     //Pin apa yang digunakan
#define DHTTYPE DHT11   // DHT 11

byte mac[] = { 0x28, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
IPAddress ipAddress(192,168,196,55);
IPAddress ipDns(192,168,196,36);
IPAddress ipSubnet(255,255,252,0);
IPAddress ipGateway(192,168,196,10);
 
DHT dht(DHTPIN, DHTTYPE);

EthernetClient client;
char server[] = "192.168.197.208"; // ip penangkap
 
void setup() {
  Serial.begin(9600); //baud komunikasi serial
  Serial.println("Pengujian DHT11!"); //penulisan di serial monitor
  dht.begin(); //prosedur memulai pembacaan module sensor
  Ethernet.begin(mac, ipAddress, ipDns, ipGateway, ipSubnet);
}
 
void loop() {
  delay(2000);  //menunggu beberapa detik untuk pembacaan
 
  //pembacaan sensor membutuhkan waktu 250ms
 
  //Pembacaan untuk data kelembaban
  float humidity_1 = dht.readHumidity();
  //Pembacaan dalam format celcius (c)
  float celcius_1 = dht.readTemperature();
  //pembacaan dalam format Fahrenheit
  float fahrenheit_1 = dht.readTemperature(true);
 
  //mengecek pembacaan apakah terjadi kegagalan atau tidak
  if (isnan(humidity_1) || isnan(celcius_1) || isnan(fahrenheit_1)) {
    Serial.println("Pembacaan data dari module sensor gagal!");
    return;
  }
 
  float htof = dht.computeHeatIndex(fahrenheit_1, humidity_1); 
  //Prosedur pembacaaan data indeks panas dalam bentuk fahreheit
  float htoc = dht.computeHeatIndex(celcius_1, humidity_1, false);
 //Prosedur pembacaaan data indeks panas dalam bentuk celcius
 
  //pembacaan nilai pembacaan data kelembaban
  Serial.print("Kelembaban: ");
  Serial.print(humidity_1);
  Serial.print(" %\t");

  // tambah ini
  delay(1000);
  delay(1000);
  Serial.println("connecting...");

  //pembacaan nilai pembacaan data suhu
  Serial.print("Suhu : ");
  Serial.print(celcius_1); //format derajat celcius
  Serial.print(" 'C ");
  Serial.print(fahrenheit_1); //format derajat fahrenheit
  Serial.print(" 'F\t");
 
  Serial.print("Indeks Panas: ");
  Serial.print(htof); 
  Serial.println(" *F");
 
  Serial.print(htoc);
  Serial.println(" *C ");

  if (client.connect("192.168.197.208",2810)) {    
    String temp = "suhuc=";
    temp += celcius_1;
    temp += "&suhuf=";
    temp += fahrenheit_1;
    temp += "&kelembaban=";
    temp += humidity_1;  
                               
    Serial.println("Sending to Server: ");                   
    client.println("POST /insert-suhu HTTP/1.1");           
    Serial.println("POST /insert-suhu HTTP/1.1");           
    client.println("Host: 192.168.197.208");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(temp.length());
    client.println();
    client.print(temp);
    client.println();      
    Serial.println("We have connection");                                     
  } else {
    Serial.println("Cannot connect to Server");               
  }

  if(client.connected()) {    
    client.stop();
  }

  // sampai sini  
  
 
}
