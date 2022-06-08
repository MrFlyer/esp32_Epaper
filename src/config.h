
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(/*CS=D8*/ SS, /*DC=D3*/ 13, /*RST=D4*/ 14, /*BUSY=D2*/ 19)); // GDEH0213B72
// GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display(GxEPD2_213_Z98c(/*CS=D8*/ SS, /*DC=D3*/ 13, /*RST=D4*/ 14, /*BUSY=D2*/ 19)); // GDEH0213B72
//定义使用的汉朔2.13拆机屏(这屏幕纯纯贴贵物)
// 目前使用某宝购买的2.13寸GxEPD2_213_B74驱动屏幕，简直nm太清楚了
// GxEPD2_BW<GxEPD2_213_B72, GxEPD2_213_B72::HEIGHT> display(GxEPD2_213_B72(/*CS=D8*/ SS, /*DC=D3*/ 13, /*RST=D4*/ 14, /*BUSY=D2*/ 19)); // GDEH0213B72

//定义字体
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
Button2 MethodChoose, NextWord, LastWord;
StaticJsonDocument<512> doc;
HTTPClient https;
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
