//原作者: Jean-Marc Zingg Library: https://github.com/ZinggJM/GxEPD2
//本驱动经过魔改现支持黑白双色的局部快刷，可以很轻松的在三色墨水屏上显示时间等
//覆盖请注意备份原有驱动
//作者：YYD  同时感谢@su @游牧 @BlackCat
//QQ交流群：1051455459  
//欢迎进群交流！！
//GitHub地址：https://github.com/yanyuandi/FastFreshBWOnColor
//─────────────────────────────────────────────────────────────────────────────────────────────
//─██████████████───████████──████████────████████──████████─████████──████████─████████████───
//─██░░░░░░░░░░██───██░░░░██──██░░░░██────██░░░░██──██░░░░██─██░░░░██──██░░░░██─██░░░░░░░░████─
//─██░░██████░░██───████░░██──██░░████────████░░██──██░░████─████░░██──██░░████─██░░████░░░░██─
//─██░░██──██░░██─────██░░░░██░░░░██────────██░░░░██░░░░██─────██░░░░██░░░░██───██░░██──██░░██─
//─██░░██████░░████───████░░░░░░████────────████░░░░░░████─────████░░░░░░████───██░░██──██░░██─
//─██░░░░░░░░░░░░██─────████░░████────────────████░░████─────────████░░████─────██░░██──██░░██─
//─██░░████████░░██───────██░░██────────────────██░░██─────────────██░░██───────██░░██──██░░██─
//─██░░██────██░░██───────██░░██────────────────██░░██─────────────██░░██───────██░░██──██░░██─
//─██░░████████░░██───────██░░██────────────────██░░██─────────────██░░██───────██░░████░░░░██─
//─██░░░░░░░░░░░░██───────██░░██────────────────██░░██─────────────██░░██───────██░░░░░░░░████─
//─████████████████───────██████────────────────██████─────────────██████───────████████████───
//─────────────────────────────────────────────────────────────────────────────────────────────

#ifndef _GxEPD2_750c_Z08_H_
#define _GxEPD2_750c_Z08_H_

#include "../GxEPD2_EPD.h"

class GxEPD2_750c_Z08 : public GxEPD2_EPD
{
  public:
    // attributes
    static const uint16_t WIDTH = 800;
    static const uint16_t WIDTH_VISIBLE = WIDTH;
    static const uint16_t HEIGHT = 480;
    static const GxEPD2::Panel panel = GxEPD2::GDEW075Z08;
    static const bool hasColor = true;
    static const bool hasPartialUpdate = true;
    static const bool usePartialUpdateWindow = true; // needs be false, controller issue
    static const bool hasFastPartialUpdate = false;
    static const uint16_t power_on_time = 150; // ms, e.g. 79414us
    static const uint16_t power_off_time = 30; // ms, e.g. 39140us
    static const uint16_t full_refresh_time = 18000; // ms, e.g. 16788187us
    static const uint16_t partial_refresh_time = 25000; // ms, e.g. 16788187us
    // constructor
    GxEPD2_750c_Z08(int16_t cs, int16_t dc, int16_t rst, int16_t busy);
    // methods (virtual)
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    void clearScreen(uint8_t value = 0xFF); // init controller memory and screen (default white)
    void clearScreen(uint8_t black_value, uint8_t color_value); // init controller memory and screen
    void writeScreenBuffer(uint8_t value = 0xFF); // init controller memory (default white)
    void writeScreenBuffer(uint8_t black_value, uint8_t color_value); // init controller memory
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    void writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImagePartFresh(uint8_t command,const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImageRedFix(uint8_t command,const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    //void writeImagejushua(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImagePrevious(const uint8_t* black, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImageNew(const uint8_t* black, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImagePartPrevious(const uint8_t* black, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImagePartNew(const uint8_t* black, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                           int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write sprite of native data to controller memory, without screen refresh; x and w should be multiple of 8
    void writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    void drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write sprite of native data to controller memory, with screen refresh; x and w should be multiple of 8
    void drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void refresh(bool partial_update_mode = false); // screen refresh from controller memory to full screen
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h); // screen refresh from controller memory, partial screen
    void refresh_bw(int16_t x, int16_t y, int16_t w, int16_t h); // screen refresh from controller memory, partial screen, bw differential
    void powerOff();
    
    void hibernate(); // turns powerOff() and sets controller to deep sleep for minimum power use, ONLY if wakeable by RST (rst >= 0)
  private:
    void _writeScreenBuffer(uint8_t value);
    void _writeImage(uint8_t command, const uint8_t* bitmap, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm);
    void _writeImagePartFresh(uint8_t command, const uint8_t* bitmap, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm);
    void _writeImageRedFix(uint8_t command, const uint8_t* bitmap, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm);
    void _writeImagePart(uint8_t command, const uint8_t* bitmap, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                         int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm);
    void _setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void _PowerOn();
    void _PowerOff();
    void _InitDisplay();
    void _Init_Full();
    void _Init_Part();
    //void _Init_Part2();
    void _Update_Full();
    void _Update_Part();
    
  private:
    static const unsigned char lut_20_LUTC_partial[];
    static const unsigned char lut_21_LUTWW_partial[];
    static const unsigned char lut_22_LUTKW_partial[];
    static const unsigned char lut_23_LUTWK_partial[];
    static const unsigned char lut_24_LUTKK_partial[];
    static const unsigned char lut_25_LUTBD_partial[];
};

#endif
