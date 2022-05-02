#include <ArduinoJson.h>
#include <HTTPClient.h>

HTTPClient https;
StaticJsonDocument<300> doc;

void StartClient()
{
    https.begin("https://devapi.qweather.com/v7/weather/now?location=101030100&key=30625228fb9340a1a538fa03449cb08d");
}

void GetData(String input)
{
    DeserializationError error = deserializeJson(doc, input);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    const char *code = doc["code"];             // "200"
    const char *updateTime = doc["updateTime"]; // "2022-05-02T14:52+08:00"

    JsonObject now = doc["now"];

    const char *now_temp = now["temp"];           // "25"
    const char *now_feelsLike = now["feelsLike"]; // "22"
    const char *now_icon = now["icon"];           // "100"
    const char *now_text = now["text"];           // "晴"
    const char *now_wind360 = now["wind360"];     // "225"
    const char *now_windDir = now["windDir"];     // "西南风"
    const char *now_windScale = now["windScale"]; // "3"
    const char *now_windSpeed = now["windSpeed"]; // "17"
    const char *now_humidity = now["humidity"];   // "25"
    const char *now_precip = now["precip"];       // "0.0"
    const char *now_pressure = now["pressure"];   // "1014"
    const char *now_vis = now["vis"];             // "30"
    const char *now_cloud = now["cloud"];         // "0"
    const char *now_dew = now["dew"];             // "5"

}
