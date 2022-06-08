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
#include "config.h"
#include "EngWord.h"
#include "GetWeather.h"
#include "GetTime.h"

//启动wifi打印链接ip
void WiFi_Booting()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  // display.setRotation(1);
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
    // display.setFullWindow();
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
  display.setRotation(3);
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
