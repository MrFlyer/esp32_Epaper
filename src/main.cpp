/*
    目前完成进度：1.WiFi链接已完成，后续上传还在做
                2.使用SPIFFS存储book文件已完成，并且可以完美上下页并且提示到达上限或者下限
                3.墨水屏使用B/W驱动
                4.支持中文输入
                5.支持存储目前页数保存当前进度
                6.支持WEB上传文件（还没测试
                7.支持时钟显示

    已知bug：单双击问题没解决，
    当到整点时候会重复全局刷新(已经修好)
*/

#include <Arduino.h>
#include <string.h>
#include <SPIFFS.h> //闪存相关
#include <FS.h>
#include <GxEPD2_BW.h> // 黑白二色库
#include <GxEPD2_3C.h> // 红黑白三色库
#include <Adafruit_I2CDevice.h>
#include <U8g2_for_Adafruit_GFX.h> //字体库
#include <EEPROM.h>                //存储当前页数
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Button2.h> //单击双击按键库
#include <WiFi.h>
#include <WebUpload.h> //web上传
#include <GetBatVal.h> //获取当前电量

//定义使用的汉朔2.13拆机屏(这屏幕纯纯贴贵物)
// 目前使用某宝购买的2.13寸GxEPD2_213_B74驱动屏幕，简直nm太清楚了
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(/*CS=D8*/ SS, /*DC=D3*/ 13, /*RST=D4*/ 14, /*BUSY=D2*/ 19)); // GDEH0213B72
// GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display(GxEPD2_213_Z98c(/*CS=D8*/ SS, /*DC=D3*/ 13, /*RST=D4*/ 14, /*BUSY=D2*/ 19)); // GDEH0213B72

//定义字体
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

Button2 MethodChoose, NextWord, LastWord;

HTTPClient https;

StaticJsonDocument<512> doc;

// WiFiUDP ntpUDP; // 创建一个WIFI UDP连接

// NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 60 * 60 * 8, 30 * 60 * 1000);

using namespace std;

int i = 0;

int flag = 1;

vector<String> v;

String data;

int temp = 0;

struct WetherData
{
  char city[32];
  char weather[64];
  char code[32];
  char temperature[32];
};

//启动wifi打印链接ip
void WiFi_Booting()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setRotation(1);
  display.setTextSize(2);
  // comment out next line to have no or minimal Adafruit_GFX code
  do
  {
    display.setCursor(40, 60);
    display.println("BOOTING SYSTEM");
  } while (display.nextPage());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }
  display.fillScreen(GxEPD_WHITE);
}
//获取书目列表并且存入动态大小数组V中
void GetBookList()
{
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  File dataFile = SPIFFS.open("/book.txt", "r");

  while (dataFile.available())
  {
    v.push_back(dataFile.readStringUntil('\n'));
  }
  dataFile.close();
}

//从文件获取每行单词并将其打印
void PrintWord(int num)
{
  String line = "";
  String word = "";
  if (num % 10 != 0)
  {
    display.setPartialWindow(0, 0, display.width(), display.height());
  }
  line = v[num];
  Serial.println(line);
  display.fillScreen(GxEPD_WHITE);
  display.drawLine(5, 35, 245, 35, 0); //画水平线
  word = line.substring(0, line.indexOf("["));
  u8g2Fonts.setFont(u8g2_font_logisoso18_tr);
  u8g2Fonts.setCursor(10, 27);
  u8g2Fonts.print(word);
  u8g2Fonts.setFont(u8g2_font_unifont_t_extended);
  u8g2Fonts.print(line.substring(line.indexOf("["), line.indexOf("]") + 1));
  display.nextPage();
  int k = line.indexOf("]") + 1;
  String s4 = "", s5 = "";
  int x = 5, y = 53;
  u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312b);
  while (k < line.length())
  {
    if ((k != 0 && line[k - 1] != ' ') || line[k] != ' ')
    {
      s4 += line[k];
    }
    if ((k <= line.length() - 2 && (line[k + 1] == ' ') && line[k + 2] == ' ' || k == line.length() - 1))
    {
      if (s4.length() != 1)
      {
        /*
          2.13寸屏：48
          2.66寸屏：60
          2.9寸屏：69
        */
        if (s4.length() <= 48)
        {
          Serial.println(s4);
          u8g2Fonts.setCursor(x, y);
          u8g2Fonts.print(s4);
          y += 18;
        }
        else if (s4.length() > 48)
        {
          for (int m = 0; m < s4.length(); m++)
          {
            s5 += s4[m];
            if (m == 48)
            {
              u8g2Fonts.setCursor(x, y);
              u8g2Fonts.print(s5);
              Serial.println(s5);
              y += 18;
              s5 = "";
            }
          }
          u8g2Fonts.setCursor(x, y);
          u8g2Fonts.print(s5);
          Serial.println(s5);
          y += 18;
          s5 = "";
        }
      }
      s4 = "";
    }
    k++;
  }
  display.nextPage();
}

//当从web上传文件之后更新本地动态大小数组V
void UpdateBook(Button2 &btn)
{
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 40);
    display.println("UPLOADING NEW BOOK");
  } while (display.nextPage());
  v.clear();
  File dataFile = SPIFFS.open("/book.txt", "r");

  while (dataFile.available())
  {
    v.push_back(dataFile.readStringUntil('\n'));
  }
  dataFile.close();
  delay(2000);
  display.fillScreen(GxEPD_WHITE);
  i = 0;
  PrintWord(i);
  flag = 2;
  EEPROM.write(10, i);
  delay(1);
  EEPROM.commit();
}

//使用arduino自带库进行同步NTP服务器与本地的RTC时钟
void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  data = timeinfo.tm_hour;
  data += ":";
  data += timeinfo.tm_min;
  if (timeinfo.tm_sec == 0) //当时间到达0秒的时候局刷分
  {
    display.firstPage();
    display.setPartialWindow(0, 0, display.width(), display.height());
    u8g2Fonts.setFont(u8g2_font_inb53_mr);
    u8g2Fonts.setCursor(13, 90);
    // display.setCursor(30,34);
    // display.setTextSize(8);
    do
    {
      u8g2Fonts.println(data.c_str());
    } while (display.nextPage());
    Serial.println(data.c_str());
  }
  if (timeinfo.tm_min % 10 == 0 && temp == 0) //当分钟达到整时时候，做一次全刷
  {
    temp = 1;
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.firstPage();
    u8g2Fonts.setFont(u8g2_font_inb53_mr);
    u8g2Fonts.setCursor(13, 90);
    // display.setCursor(30,34);
    // display.setTextSize(8);
    do
    {
      u8g2Fonts.println(data.c_str());
    } while (display.nextPage());
  }
  if (timeinfo.tm_min % 10 != 0)
  {
    temp = 0;
  }
}

//按键事件检测连带文件溢出检测
void Button(int page)
{
  switch (page)
  {
  case 1:
    printLocalTime();
    break;
  case 3:
    if (digitalRead(25) == LOW) //下一页
    {
      delay(200);
      if (digitalRead(25) == LOW)
      {
        Serial.print("进入下一页判断");
        if (i == v.size()) //文件顶溢出检测
        {
          do
          {
            display.setFullWindow();
            display.firstPage();
            display.setCursor(10, 40);
            display.println("HAD READ ALL BOOK");
          } while (display.nextPage());
          delay(2000);
          display.fillScreen(GxEPD_WHITE);
          Serial.print(i);
          PrintWord(v.size());
          i = v.size();
        }
        else
        {
          // display.setFullWindow();
          // display.firstPage();
          // do
          // {
          //   display.fillScreen(GxEPD_WHITE);
          // } while (display.nextPage());
          i++;
          Serial.print("i++完成");
          PrintWord(i);
          Serial.print(i);
          EEPROM.write(10, i);
          delay(1);
          Serial.print("\n");
          Serial.print(EEPROM.read(10), DEC);
          EEPROM.commit();
        }
      }
    }
    if (digitalRead(26) == LOW) //上一页
    {
      delay(200);
      if (digitalRead(26) == LOW)
      {
        if (i == 0) //文件底溢出检测
        {
          do
          {
            display.setFullWindow();
            display.firstPage();
            display.setCursor(10, 40);
            display.println("Already the beginning");
          } while (display.nextPage());
          delay(2000);
          display.fillScreen(GxEPD_WHITE);
          Serial.print(i);
          PrintWord(0);
          i = 0;
        }
        else
        {
          // display.setFullWindow();
          // display.firstPage();
          // do
          // {
          //   display.fillScreen(GxEPD_WHITE);
          // } while (display.nextPage());
          i--;
          PrintWord(i);
          Serial.print(i);
          EEPROM.write(10, i);
          delay(1);
          Serial.print("\n");
          Serial.print(EEPROM.read(10), DEC);
          EEPROM.commit();
        }
      }
    }
    break;
  }
}

//显示时间
void GetTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  data = timeinfo.tm_hour;
  data += ":";
  data += timeinfo.tm_min;
  display.firstPage();
  display.setPartialWindow(0, 0, display.width(), display.height());
  u8g2Fonts.setFont(u8g2_font_inb53_mr);
  u8g2Fonts.setCursor(13, 90);
  // display.setCursor(30,34);
  // display.setTextSize(8);
  do
  {
    u8g2Fonts.println(data.c_str());
  } while (display.nextPage());
  Serial.println(data.c_str());
}

void PrintWeather(String now_icon, String now_temp, String city, String weather)
{
  Serial.println("进入绘画天气");
  u8g2Fonts.setFont(u8g2_font_open_iconic_weather_8x_t);
  int icon_txt = now_icon.toInt();
  Serial.println(icon_txt);
  display.setPartialWindow(0, 0, display.width(), display.height());
  // display.firstPage();
  // do
  // {
  if (icon_txt == 0) //白天晴
  {
    u8g2Fonts.drawGlyph(15, 90, 0x0045);
  }
  else if (icon_txt == 150) //夜晚晴
  {
    u8g2Fonts.drawGlyph(15, 90, 0x0042);
  }
  else if (icon_txt == 4 || icon_txt == 9) //多云
  {
    u8g2Fonts.drawGlyph(15, 90, 0x0040);
  }
  else if (icon_txt == 5 || icon_txt == 6 || icon_txt == 7 || icon_txt == 8) //少云
  {
    u8g2Fonts.drawGlyph(15, 90, 0x0041);
  }
  else if (icon_txt >= 10 && icon_txt <= 20) //下雨
  {
    u8g2Fonts.drawGlyph(15, 90, 0x0043);
  }
  else if (icon_txt >= 400 && icon_txt < 1000) //下雪或者其他天气
  {
    u8g2Fonts.drawGlyph(15, 90, 0x0044);
  }

  u8g2Fonts.setFont(u8g2_font_logisoso78_tn);
  u8g2Fonts.setCursor(100, 90);
  String wea_temp = now_temp;
  u8g2Fonts.print(wea_temp);
  // wea_temp += "度";
  display.drawCircle(210, 60, 6, GxEPD_BLACK);
  u8g2Fonts.setFont(u8g2_font_inr24_mr);
  u8g2Fonts.setCursor(217, 82);
  u8g2Fonts.print("C");
  u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312b);
  u8g2Fonts.setCursor(130, 120);
  u8g2Fonts.print(city);
  u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312b);
  u8g2Fonts.setCursor(170, 120);
  u8g2Fonts.print(weather);
  display.nextPage();

  // } while (display.nextPage());
}

void GetWeath()
{
  // https.begin("https://devapi.qweather.com/v7/weather/now?location=101030100&key=30625228fb9340a1a538fa03449cb08d");
  https.begin("https://api.seniverse.com/v3/weather/now.json?key=SR7QxU9_RUOvkrbza&location=tianjin&language=zh-Hans&unit=c");
  https.setUserAgent("Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.82 Safari/537.36");
  int httpCode = https.GET();
  if (httpCode == HTTP_CODE_OK)
  {
    String payload = https.getString();
    deserializeJson(doc, payload);
    // JsonObject obj1 = doc.as<JsonObject>();
    // String code = doc["code"];
    // String now_icon = doc["now"]["icon"];
    // delay(500);
    Serial.println(httpCode);
    Serial.println(payload);
    // String a = "150";
    // PrintWeather(a);
  }
  https.end();
}

void AnalogData()
{
  Serial.println("进入获取天气数据");
  struct WetherData weatherdata = {0};
  strcpy(weatherdata.city, doc["results"][0]["location"]["name"].as<const char *>());
  strcpy(weatherdata.weather, doc["results"][0]["now"]["text"].as<const char *>());
  strcpy(weatherdata.code, doc["results"][0]["now"]["code"].as<const char *>());
  strcpy(weatherdata.temperature, doc["results"][0]["now"]["temperature"].as<const char *>());

  Serial.println("City");
  Serial.println(weatherdata.city);
  Serial.println("weather");
  Serial.println(weatherdata.weather);
  Serial.println("CODE");
  Serial.println(weatherdata.code);
  Serial.println("temperature");
  Serial.println(weatherdata.temperature);
  PrintWeather(weatherdata.code, weatherdata.temperature, weatherdata.city, weatherdata.weather);
}

//双击按键事件中断出发函数
void handler(Button2 &btn)
{
  flag++;
  if (flag == 4)
  {
    flag = 1;
  }
  Serial.println("判断完成");
  switch (flag)
  {
  case 1:
    display.setFullWindow();
    display.firstPage();
    do
    {
      display.fillScreen(GxEPD_WHITE);
    } while (display.nextPage());
    GetTime();
    break;

  case 2:
    display.fillScreen(GxEPD_WHITE);
    AnalogData();
    break;
  case 3:
    display.setFullWindow();
    display.firstPage();
    do
    {
      display.fillScreen(GxEPD_WHITE);
    } while (display.nextPage());
    PrintWord(i);
    break;
  }
  // if (flag == 1)
  // {
  //   flag = 2;
  //   display.setFullWindow();
  //   display.firstPage();
  //   do
  //   {
  //     display.fillScreen(GxEPD_WHITE);
  //   } while (display.nextPage());
  //   PrintWord(i);
  // }
  // else if (flag == 2)
  // {
  //   flag = 1;
  //   display.setFullWindow();
  //   display.firstPage();
  //   do
  //   {
  //     display.fillScreen(GxEPD_WHITE);
  //   } while (display.nextPage());
  //   GetTime();
  // }
}

//按键初始化
void Button_init()
{
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);
  MethodChoose.begin(27, PULLUP);
  MethodChoose.setPressedHandler(handler);
  // MethodChoose.setDoubleClickHandler(UpdateBook);
}

//程序总启动
void setup()
{
  Serial.begin(115200);
  display.init();
  WiFi_Booting();
  EEPROM.begin(1024);
  u8g2Fonts.begin(display);                  //将u8g2连接到display
  u8g2Fonts.setForegroundColor(GxEPD_BLACK); // 设置前景色
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
  Button_init();
  i = EEPROM.read(10);
  GetBookList();
  // PrintWord(i);
  // CorrectAdc();
  // web_setup();
  // timeClient.begin();
  configTime(8 * 3600, 0, "ntp1.aliyun.com", "ntp2.aliyun.com", "ntp3.aliyun.com");
  GetTime();
  GetWeath();
  // AnalogData();
}

/* 将时钟设置为flag为1
  天气设置flag为2
  词典设置为FLAG为3

*/

//程序总循环
void loop()
{
  MethodChoose.loop();
  Button(flag);
  // GetVoltage();
  server.handleClient();
}
