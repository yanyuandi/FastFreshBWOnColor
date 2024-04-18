#include <GxEPD2_3C.h>
#include <GxEPD2_BW.h>
#include <U8g2_for_Adafruit_GFX.h>
//#include "shuma.c"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <time.h>

//#include "jpg.h"

#define NORMAL_FONT u8g2_font_wqy16_t_gb2312a  //设置NORMAL_FONT默认字体

const char* ssid = "yanyuandi";
const char* password = "beipiaoguizu";

// NTP服务器相关信息
#define TZ 8      // 时区偏移值，以 UTC+8 为例
#define DST_MN 0  // 夏令时持续时间（分钟）

#define TZ_SEC ((TZ)*3600)     // 时区偏移值（秒）
#define DST_SEC ((DST_MN)*60)  // 夏令时持续时间（秒）

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 2> display(GxEPD2_750c_Z08(/*CS=D8*/ 4, /*DC=D3*/ 27, /*RST=D4*/ 26, /*BUSY=D2*/ 25));  //即可。GxEPD2_420c_1680就是驱动程序文件名。
//GxEPD2_BW<GxEPD2_750_YT7, GxEPD2_750_YT7::HEIGHT> display(GxEPD2_750_YT7(/*CS=D8*/ 4, /*DC=D3*/ 27, /*RST=D4*/ 26, /*BUSY=D2*/ 25)); // GDEY075T7 800x480, UC8179 (GD7965)
//GxEPD2_BW<GxEPD2_583_T8, GxEPD2_583_T8::HEIGHT> display(GxEPD2_583_T8(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=D8*/ 4, /*DC=D3*/ 27, /*RST=D4*/ 26, /*BUSY=D2*/ 25)); // GDEW075T8 640x384, UC8159c (IL0371)
//GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT / 2> display(GxEPD2_750_T7(/*CS=D8*/ 4, /*DC=D3*/ 27, /*RST=D4*/ 26, /*BUSY=D2*/ 25)); // GDEW075T7 800x480, EK79655 (GD7965)


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("UI 初始化");
  SPI.end();
  SPI.begin(13, 14, 14, 4);

  // 连接WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  if (time(nullptr) < 1000000000) {
    // 如果没有获取过时间，重新获取时间
    uint8_t i = 0;
    configTime(TZ_SEC, DST_SEC, "ntp1.aliyun.com", "ntp2.aliyun.com");
    while ((time(nullptr) < 1000000000) & (i < 20)) {
      i++;
      Serial.print(".");
      delay(500);
    }
    Serial.println("时间同步成功");
  }




  display.init(115200, true, 2, false);
  display.setRotation(0);  //  对这块 4.2 寸屏幕而言，2 是横向（排线在上方）

  u8g2Fonts.begin(display);
  u8g2Fonts.setFontDirection(0);
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // 设置背景色
  u8g2Fonts.setFont(NORMAL_FONT);

  display.setFullWindow();
  display.firstPage();
  do {
    display.fillRect(0, 30, 800, 100, GxEPD_RED);  //屏幕顶部画一个红色的矩形
    u8g2Fonts.setFont(u8g2_font_fub42_tf);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setCursor(101, 285);
    u8g2Fonts.print("8");

    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setBackgroundColor(GxEPD_RED);
    u8g2Fonts.setForegroundColor(GxEPD_WHITE);
    u8g2Fonts.setCursor(268, 84);
    u8g2Fonts.print("此区域为7.5寸三色屏幕的红色刷新");

    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setCursor(61, 189);
    u8g2Fonts.print("三色的黑色刷新");

    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setCursor(242, 189);
    u8g2Fonts.print("局部白底黑字快刷");

    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setCursor(434, 189);
    u8g2Fonts.print("局部黑底白字快刷");

    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setCursor(610, 189);
    u8g2Fonts.print("三色屏幕本身局部刷新");

    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setCursor(258, 24);
    u8g2Fonts.print("7.5寸三色屏幕局部黑白快刷测试DEMO");

    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setForegroundColor(GxEPD_RED);
    u8g2Fonts.setCursor(374, 468);
    u8g2Fonts.print("BY YYD");



  } while (display.nextPage());


   for (int i = 0; i < 3; i++) {
     //display.fillScreen(GxEPD_BLACK);
     display.setPartialWindow(248, 204, 120, 120);
     //display.fillScreen(GxEPD_WHITE);
     display.firstPage();
     u8g2Fonts.setFont(u8g2_font_fub42_tf);
  
  
     do {
       u8g2Fonts.setForegroundColor(GxEPD_BLACK);
       u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
       u8g2Fonts.setCursor(286, 285);
       u8g2Fonts.print(i);
       Serial.println(i);
     } while (display.nextPageBW());
   }
  
   for (int i = 0; i < 3; i++) {
  
     display.setPartialWindow(434, 204, 120, 120);
     display.firstPage();
     display.fillScreen(GxEPD_BLACK);
     u8g2Fonts.setFont(u8g2_font_fub42_tf);
  
  
     do {
       u8g2Fonts.setForegroundColor(GxEPD_WHITE);
       u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
       u8g2Fonts.setCursor(474, 285);
       u8g2Fonts.print(i);
       Serial.println(i);
     } while (display.nextPageBW());
   }
  
   for (int i = 0; i < 1; i++) {
  
     display.setPartialWindow(620, 204, 120, 120);
     display.firstPage();
     //display.fillScreen(GxEPD_WHITE);
     u8g2Fonts.setFont(u8g2_font_fub42_tf);
  
  
     do {
       u8g2Fonts.setForegroundColor(GxEPD_BLACK);
       u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
       u8g2Fonts.setCursor(660, 285);
       u8g2Fonts.print(i);
       Serial.println(i);
     } while (display.nextPage());
   }
  // 显示“时间获取中...”
  display.setPartialWindow(175, 329, 450, 120);
  display.firstPage();
  do {
    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setCursor(347, 376);
    u8g2Fonts.print("时间获取中...");
  } while (display.nextPageBW());

  delay(3000);  // 延迟3秒


  display.powerOff();
}

void loop() {
  static int prevHour = -1;  // 用于存储上一次获取的小时
  static int prevMin = -1;   // 用于存储上一次获取的分钟

  time_t now = time(nullptr);        // 获取当前时间
  struct tm* ltm = localtime(&now);  // 将时间转换为本地时间结构体

  // 如果获取时间失败，打印错误信息并返回
  if (now == (time_t)-1) {
    Serial.println("Failed to obtain time");
    return;
  }

  // 仅当小时或分钟发生变化时才刷新显示
  if (ltm->tm_hour != prevHour || ltm->tm_min != prevMin) {
    // 更新上一次获取的小时和分钟
    prevHour = ltm->tm_hour;
    prevMin = ltm->tm_min;

    // 显示时间
    display.setPartialWindow(175, 329, 450, 120);
    display.firstPage();
    do {
      char timeString[20];
      sprintf(timeString, "%02d:%02d", ltm->tm_hour, ltm->tm_min);  // 将时间格式化为字符串
      u8g2Fonts.setFont(u8g2_font_logisoso50_tn);
      u8g2Fonts.setForegroundColor(GxEPD_BLACK);
      u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
      u8g2Fonts.setCursor(355, 415);
      u8g2Fonts.print(timeString);  // 显示时间字符串
      u8g2Fonts.setForegroundColor(GxEPD_BLACK);
      u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
      u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
      u8g2Fonts.setCursor(261, 415);
      u8g2Fonts.print("当前时间：");  // 显示时间字符串
    } while (display.nextPageBW());
  }

  // 等待一段时间再继续循环
  delay(1000);
}
