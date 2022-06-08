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
