#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <Ticker.h>
#include <SPI.h>

File fsUploadFile; // 建立文件对象用于闪存文件上传

Ticker ticker;

WebServer server(80);
const char *ssid = "ASUS_33_5G";     // WiFi名子
const char *password = "3214569987"; // WiFi密码
// const char *ssid = "NEWIFI3_3BE7";     // WiFi名子
// const char *password = "3214569987"; // WiFi密码

const char* file_name = "/book.txt";


//回复状态码 200 给客户端
void respondOK()
{
    server.send(200);
}

// 处理上传文件函数
void handleFileUpload()
{

    HTTPUpload &upload = server.upload();

    if (upload.status == UPLOAD_FILE_START)
    { // 如果上传状态为UPLOAD_FILE_START

        String filename = upload.filename; // 建立字符串变量用于存放上传文件名
        if (!filename.startsWith("/"))
            filename = "/" + filename;            // 为上传文件名前加上"/"
        Serial.println("File Name: " + filename); // 通过串口监视器输出上传文件的名称
        file_name = filename.c_str();

        fsUploadFile = SPIFFS.open(filename, "w"); // 在SPIFFS中建立文件用于写入用户上传的文件数据
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    { // 如果上传状态为UPLOAD_FILE_WRITE

        if (fsUploadFile)
            fsUploadFile.write(upload.buf, upload.currentSize); // 向SPIFFS文件写入浏览器发来的文件数据
    }
    else if (upload.status == UPLOAD_FILE_END)
    { // 如果上传状态为UPLOAD_FILE_END
        if (fsUploadFile)
        {                                                   // 如果文件成功建立
            fsUploadFile.close();                           // 将文件关闭
            Serial.println(" Size: " + upload.totalSize);   // 通过串口监视器输出文件大小
            server.sendHeader("Location", "/success.html"); // 将浏览器跳转到/success.html（成功上传页面）
            server.send(303);                               // 发送相应代码303（重定向到新页面）
        }
        else
        {                                                                // 如果文件未能成功建立
            Serial.println("File upload failed");                        // 通过串口监视器输出报错信息
            server.send(500, "text/plain", "500: couldn't create file"); // 向浏览器发送相应代码500（服务器错误）
        }
    }
}

// 获取文件类型
String getContentType(String filename)
{
    if (filename.endsWith(".htm"))
        return "text/html";
    else if (filename.endsWith(".html"))
        return "text/html";
    else if (filename.endsWith(".css"))
        return "text/css";
    else if (filename.endsWith(".js"))
        return "application/javascript";
    else if (filename.endsWith(".png"))
        return "image/png";
    else if (filename.endsWith(".gif"))
        return "image/gif";
    else if (filename.endsWith(".jpg"))
        return "image/jpeg";
    else if (filename.endsWith(".ico"))
        return "image/x-icon";
    else if (filename.endsWith(".xml"))
        return "text/xml";
    else if (filename.endsWith(".pdf"))
        return "application/x-pdf";
    else if (filename.endsWith(".zip"))
        return "application/x-zip";
    else if (filename.endsWith(".gz"))
        return "application/x-gzip";
    return "text/plain";
}


bool handleFileRead(String path)
{ //处理浏览器HTTP访问

    if (path.endsWith("/"))
    {                         // 如果访问地址以"/"为结尾
        path = "/index.html"; // 则将访问地址修改为/index.html便于SPIFFS访问
    }

    String contentType = getContentType(path); // 获取文件类型

    if (SPIFFS.exists(path))
    {                                         // 如果访问的文件可以在SPIFFS中找到
        File file = SPIFFS.open(path, "r");   // 则尝试打开该文件
        server.streamFile(file, contentType); // 并且将该文件返回给浏览器
        file.close();                         // 并且关闭文件
        return true;                          // 返回true
    }
    return false; // 如果文件未找到，则返回false
}

// 处理用户浏览器的HTTP访问
void handleUserRequest()
{
    // 获取用户请求网址信息
    String webAddress = server.uri();

    // 通过handleFileRead函数处处理用户访问
    bool fileReadOK = handleFileRead(webAddress);

    // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
    if (!fileReadOK)
    {
        server.send(404, "text/plain", "404 Not Found");
    }
}





//####################################################################################################
//  web服务初始化
//####################################################################################################
void web_setup()
{
    server.on("/upload.html",    // 如果客户端通过upload页面
                HTTP_POST,         // 向服务器发送文件(请求方法POST)
                respondOK,         // 则回复状态码 200 给客户端
                handleFileUpload); // 并且运行处理文件上传函数

    server.onNotFound(handleUserRequest);

    server.begin(); // 启动网站服务
    Serial.println("HTTP server started");
}

//###############
