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
    else if (icon_txt == 1) //夜晚晴
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
    else if (icon_txt >= 21 && icon_txt < 100) //下雪或者其他天气
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
