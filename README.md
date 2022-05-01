# esp32_Epaper

目前完成进度：1.WiFi链接已完成，后续上传还在做
               			2.使用SPIFFS存储book文件已完成，并且可以完美上下页并且提示到达上限或者下限
                		   3.墨水屏使用B/W驱动
                           4.支持中文输入
                           5.支持存储目前页数保存当前进度
                          6.支持WEB上传文件（还没测试

通过淘宝购买了GxEPD2_213_B74驱动全新墨水屏，比nm辣鸡汉朔拆机好太多倍了

目前预期：做出页面切换(已经完成)，做出多种显示功能等等（目前支持两种已经），实现控制灯具之类

目前使用库：
    zinggjm/GxEPD2@^1.4.5
	olikraus/U8g2@^2.32.15
	olikraus/U8g2_for_Adafruit_GFX@^1.8.0
	arduino-libraries/NTPClient@^3.1.0
	lennarthennigs/Button2@^2.0.1

