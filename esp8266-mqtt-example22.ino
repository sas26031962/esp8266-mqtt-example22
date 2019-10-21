// Встроенный светодиод подлкючен к выводу D4 контроллера
// SSL port

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>
 
//
// Константы
//

#define BUILT_IN_LED D4
#define DATA_REPEATE 300

const char *ssid1 = "WiFi-DOM.RU-3915"; // Имя WiFi точки доступа
const char *pass1 = "FzXbgYwPhU"; // Пароль от WiFi точки доступа

const char *ssid2 = "sas26031962"; // Имя WiFi точки доступа
const char *pass2 = "11111111"; // Пароль от WiFi точки доступа

/*
//Andy26031962 
const char *mqtt_server = "soldier.cloudmqtt.com"; // Имя сервера MQTT
const int mqtt_port = 15103; // Порт для подключения к серверу MQTT
//const int mqtt_port = 25103; // SSL порт для подключения к серверу MQTT
const char *mqtt_user = "xebuduol"; // Логи от сервер
const char *mqtt_pass = "C7uo-MebBHo1"; // Пароль от сервера

*/

//Andy 20190919
const char *mqtt_server = "soldier.cloudmqtt.com"; // Имя сервера MQTT
const int mqtt_port = 13381; // Порт для подключения к серверу MQTT
//const int mqtt_port = 23381; // SSL порт для подключения к серверу MQTT
const char *mqtt_user = "xvlfkffd"; // Логи от сервер
const char *mqtt_pass = "TmV4WHEGgXSk"; // Пароль от сервера

String sTopic_1 = "wemosd1r1_ledX";
String sTopic_2 = "wemosd1r1_ledY";

String sTopic_3 = "wemosd1r1_count";



String sConnection = "Andy20190919";

#define BUFFER_SIZE 100

//
// Глобальные переменные
//
//-----------------------

bool LedState = false;
int tm = DATA_REPEATE;
float temp=0;

int Counter = 0;

WiFiClient wclient;
PubSubClient client(wclient, mqtt_server, mqtt_port);

//
// Функция получения данных от сервера
//
void callback(const MQTT::Publish& pub)
{
  Serial.print(pub.topic()); // выводим в сериал порт название топика
  Serial.print(" => ");
  Serial.print(pub.payload_string()); // выводим в сериал порт значение полученных данных

  String payload = pub.payload_string();

  if(String(pub.topic()) == sTopic_1)  // проверяем из нужного ли нам топика пришли данные
  {
    int stled = payload.toInt();      // преобразуем полученные данные в тип integer
    Serial.print(">>" + String(stled,DEC)+ "\n");
    digitalWrite(LED_BUILTIN,stled); // включаем или выключаем светодиод в зависимоти от полученных значений данных
    LedState = (stled > 0);
  }

  if(String(pub.topic()) == sTopic_2)  // проверяем из нужного ли нам топика пришли данные
  {
    int stled = payload.toInt();      // преобразуем полученные данные в тип integer
    Serial.print(">>" + String(stled,DEC)+ "\n");
    digitalWrite(LED_BUILTIN,stled); // включаем или выключаем светодиод в зависимоти от полученных значений данных
    LedState = (stled > 0);
  }
}//End of void callback(const MQTT::Publish& pub)

//
// Программа начальной установки
//
void setup() 
{
  
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  
  pinMode(LED_BUILTIN, OUTPUT);

  // Start the I2C interface
  Wire.begin();

}//End of void setup()

//
// Главный цикл программы
//
void loop() 
{
  // подключаемся к wi-fi
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("Connecting to ");
    Serial.print(ssid2);
    Serial.println("...");
    WiFi.begin(ssid2, pass2);

    if (WiFi.waitForConnectResult() == WL_CONNECTED)
    {
      Serial.println("WiFi connected to sas26031962");
    }
    else
    {
      return;
    }
  }

  // подключаемся к MQTT серверу
  if (WiFi.status() == WL_CONNECTED) 
  {
    if (!client.connected()) 
    {
      Serial.println("Connecting to MQTT server");
      if (client.connect(MQTT::Connect(sConnection).set_auth(mqtt_user, mqtt_pass))) 
      {
        Serial.println("Connected to MQTT server");
        client.set_callback(callback);
        client.subscribe(sTopic_1); // подписываемся по топик 1
        client.subscribe(sTopic_2); // подписываемся по топик 2
      } 
      else 
      {
        Serial.println("Could not connect to MQTT server");
      }
    }

    if (client.connected())
    {
      client.loop();
      DataSend();
    }

  }//End of if (WiFi.status() == WL_CONNECTED)

  else
  {
    Serial.print(".");
  }

}//End of void loop()

//
// Функция публикации данных
//
void DataSend()
{
  if (tm==0)
  {
    //... здесь помещается блок отправки данных
    client.publish(sTopic_3,String(Counter)); // отправляем в топик для термодатчика значение температуры
    Serial.println("Counter:" + String(Counter, DEC)); // эхо в порт
    
    //Реализация счётчика публикаций
    Counter++;
    
    tm = DATA_REPEATE; // установка пауза между публикациями данных, единица измерения 10 мс
  }
  tm--;
  delay(10);//Данное значение уменьшать категорически не рекомедуется!!!
  
}//End of void DataSend()
