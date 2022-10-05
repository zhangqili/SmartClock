#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <PubSubClient.h>
#include <AliyunIoTSDK.h>

/* 连接您的WIFI SSID和密码 */
//#define WIFI_SSID "HUAWEI-402"
//#define WIFI_SSID "HUAWEI P10 Plus"
#define WIFI_SSID "Xiaomi 12S Ultra"
//#define WIFI_PASSWD "cai12345"
#define WIFI_PASSWD "19260817"

/* 设备的三元组信息*/
#define PRODUCT_KEY       "h9xwJufiIbz"
#define DEVICE_NAME       "ESP8266"
#define DEVICE_SECRET     "8bd8058ac54afa15bcda89a04cb86ae9"
#define REGION_ID         "cn-shanghai"

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 14, 2, U8X8_PIN_NONE);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time2.aliyun.com", 60 * 60 * 8, 30 * 60 * 1000);
WiFiClient espClient;
PubSubClient  client(espClient);

const char* iphost = "116.62.81.138";  //ip-api.com
const char* weatherHost = "116.62.81.138";  //api.seniverse.com
const char* weatherAPI = "api.seniverse.com";
const char* weatherURL = "/v3/weather/now.json?key=SUDMpGa0L48Bm_Oh4&location=";
const uint16_t port = 80 ;
char *ipurl = "/json/";

String inputString = "";
unsigned char weatherCode;
unsigned char outsideTemp = 0;
unsigned char hour = 0;
unsigned char minute = 0;
unsigned char second = 0;
unsigned char LEDSwitch = 0;
unsigned char checksum = 0;
unsigned long lastMs = 0;

String lat;
String lon;
String location;

int alarmTimespan;
int nowTimespan=0;
int count = 3600;

float ftemperature = 0.0;
float fhumidity = 0.0;
char codeline[16] = {0};

String UART_Rx_Buf;

void setup()
{
  u8g2.begin();

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_10x20_tf);
  u8g2.drawStr(0, 20, "Connecting...");
  u8g2.sendBuffer();

  Serial.begin(9600);
  wifiInit();

  AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);
  AliyunIoTSDK::bindData("LEDSwitch", ledSwitchCallback);
  AliyunIoTSDK::bindData("AlarmTimeMinutes", AlarmTimeMinutesCallback);

  timeClient.begin();
  Serial.println("WiFi Connected");
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_10x20_tf);
  u8g2.drawStr(0, 20, "Connected");
  u8g2.sendBuffer();

  count = 100;
  GetLocation();
  GetWeather();
}

void loop() {
  AliyunIoTSDK::loop();
  ReceiveData();
  count++;
  //SendLEDData();
  if (count >= 60)
  {
    GetWeather();
    UpdateTime();
    count = 0;
  }
  switch (count % 30)
  {
    case 0:
      UpdateTime();
      SendData(SendTimeData);
      break;
    case 10:
      SendData(SendWeatherData);
      break;
    case 20:
      SendData(SendTempData);
      break;
  }
  if (count % 10 == 0)
  {
    AliyunIoTSDK::send("CurrentTemperature", ftemperature);
    AliyunIoTSDK::send("CurrentHumidity", fhumidity);
  }
  char tempCountStr[10];
  sprintf(tempCountStr, "%d", count);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_10x20_tf);
  u8g2.drawStr(0, 20, tempCountStr);
  u8g2.sendBuffer();
  delay(1000);
}

void GetLatLon()
{
  HTTPClient http;
  String locationjson;
  http.begin(espClient, "ip-api.com", port, "/json?fields=lat,lon");
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    locationjson = http.getString();
    DynamicJsonDocument doc(200);
    DeserializationError error = deserializeJson(doc, locationjson);
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    String lat1 = doc["lat"];
    String lon1 = doc["lon"];
    lat = lat1;
    lon = lon1;
    Serial.println(lat);
    Serial.println(lon);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void GetLocation()
{
  HTTPClient http;
  String locationjson;
  http.begin(espClient, "restapi.amap.com", port, "/v3/ip?key=235435c9386a0830e87266c1c25b0f64");
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    locationjson = http.getString();
    DynamicJsonDocument doc(400);
    DeserializationError error = deserializeJson(doc, locationjson);
    // Test if parsing succeeds.
    if (error) {
      Serial.print("location fetch error");
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    String lat1 = doc["province"];
    String lon1 = doc["city"];
    location = lat1 + lon1;
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void GetWeather()
{
  HTTPClient http;
  String weatherjson;
  http.begin(espClient, weatherAPI, port, weatherURL + location);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    weatherjson = http.getString();
    //Serial.println(weatherjson);
    DynamicJsonDocument doc(1000);
    DeserializationError error = deserializeJson(doc, weatherjson);
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    String codeStr = doc["results"][0]["now"]["code"];
    String tempStr = doc["results"][0]["now"]["temperature"];

    weatherCode = codeStr.toInt();
    outsideTemp = tempStr.toInt();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void UpdateTime()
{
  timeClient.update();
  nowTimespan = timeClient.getEpochTime()%86400;
}

void ReceiveData()
{
  char c;
  while (Serial.available() > 0)
  {
    c=Serial.read();
    if(c=='\n')
    {
      UART_Rx_Buf.trim();
      PraseCommand();
      UART_Rx_Buf="";
    }
    else
      UART_Rx_Buf+=c;
    
  }
}

void PraseCommand()
{
  char recBUF[32];
  char cmd[32];
  char sData[32];
  float tempData;
  UART_Rx_Buf.toCharArray(recBUF,32,0);
  sscanf(recBUF,"%s %f",cmd,&tempData);
  if (!strcmp(cmd,"SETTEMP"))
  {
    ftemperature = tempData;
    sprintf(sData,"gettemp %f\n",tempData);
    Serial.print(sData);
  }
  if (!strcmp(cmd,"SETHUMI"))
  {
    fhumidity = tempData;
    sprintf(sData,"gethumi %f\n",tempData);
    Serial.print(sData);
  }
}

void SendWeatherData()
{
  sprintf(codeline, "SETW %d\n", weatherCode);
  Serial.print(codeline);
}


void SendLEDData()
{
  sprintf(codeline, "SETLED %d\n", LEDSwitch);
  Serial.print(codeline);
}


void SendTimeData()
{
  sprintf(codeline, "SETTIME %d\n", nowTimespan);
  Serial.print(codeline);
}

void SendTempData()
{
  sprintf(codeline, "SETTEMP %d\n", outsideTemp);
  Serial.print(codeline);
}

void wifiInit()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);   //连接WiFi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("WiFi not Connect");
  }
  Serial.println("Connected to AP");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void ledSwitchCallback(JsonVariant p)
{
  LEDSwitch = p["LEDSwitch"];
  SendData(SendLEDData);
}

void AlarmTimeMinutesCallback(JsonVariant p)
{
  alarmTimespan = p["AlarmTimeMinutes"];
  //SendAlarmData();
}

void SendAlarmData()
{

}

void SendData(void (*data)())
{
  String recBUF = "";
  while (1)
  {
    data();
    delay(100);
    while (Serial.available() > 0)
    {
      char c = Serial.read();
      if (c == '\n')
        break;
      else
        recBUF += c;
    }
    recBUF.trim();
    if (recBUF == "OVER")
    {
      return;
    }
    else
    {
      recBUF = "";
    }
  }
}
