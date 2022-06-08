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
