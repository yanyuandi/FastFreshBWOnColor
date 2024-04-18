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

#include "GxEPD2_750c_Z08.h"

GxEPD2_750c_Z08::GxEPD2_750c_Z08(int16_t cs, int16_t dc, int16_t rst, int16_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 20000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
}

void GxEPD2_750c_Z08::clearScreen(uint8_t value)
{
  clearScreen(value, 0xFF);
}

void GxEPD2_750c_Z08::clearScreen(uint8_t black_value, uint8_t color_value)
{
  _initial_write = false; // initial full screen buffer clean done
  _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x10);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(black_value);
  }
  _endTransfer();
  _writeCommand(0x13);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(~color_value);
  }
  _endTransfer();
  _Update_Part();
  
  _writeCommand(0x92); // partial out
  //Serial.println("清除屏幕");
}

void GxEPD2_750c_Z08::writeScreenBuffer(uint8_t value)
{
  writeScreenBuffer(value, 0xFF);
}

void GxEPD2_750c_Z08::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
  _initial_write = false; // initial full screen buffer clean done
  _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x10);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(black_value);
  }
  _endTransfer();
  _writeCommand(0x13);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(~color_value);
  }
  _endTransfer();
  _writeCommand(0x92); // partial out
  //Serial.println("写入屏幕缓存区");
}

void GxEPD2_750c_Z08::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, NULL, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_Z08::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
   //if (_using_partial_mode) _Init_Part2();
  //if (_using_partial_mode) writeScreenBuffer(); // initial full screen buffer clean
  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  uint16_t wb = (w + 7) / 8; // width bytes, bitmaps are padded
  x -= x % 8; // byte boundary
  w = wb * 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;
  _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x10);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data = 0xFF;
      if (black)
      {
        // use wb, h of bitmap for index!
        uint16_t idx = mirror_y ? j + dx / 8 + uint16_t((h - 1 - (i + dy))) * wb : j + dx / 8 + uint16_t(i + dy) * wb;
        if (pgm)
        {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
          data = pgm_read_byte(&black[idx]);
#else
          data = black[idx];
#endif
        }
        else
        {
          data = black[idx];
        }
        if (invert) data = ~data;
      }
      _transfer(data);
    }
  }
  _endTransfer();
  _writeCommand(0x13);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data = 0xFF;
      if (color)
      {
        // use wb, h of bitmap for index!
        uint16_t idx = mirror_y ? j + dx / 8 + uint16_t((h - 1 - (i + dy))) * wb : j + dx / 8 + uint16_t(i + dy) * wb;
        if (pgm)
        {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
          data = pgm_read_byte(&color[idx]);
#else
          data = color[idx];
#endif
        }
        else
        {
          data = color[idx];
        }
        if (invert) data = ~data;
      }
      _transfer(~data);
    }
  }
  _endTransfer();
  _writeCommand(0x92); // partial out
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  //Serial.println("写入图像数据-writeimage");
}

void GxEPD2_750c_Z08::_writeImage(uint8_t command, const uint8_t* bitmap, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //if (_using_partial_mode) _Init_Part2();

  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  int16_t wb = (w + 7) / 8; // width bytes, bitmaps are padded
  x -= x % 8; // byte boundary
  w = wb * 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;
  if (!_using_partial_mode) _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(command);
  _startTransfer();
  
  bool isBlack = (command == 0x10); // 判断是否是黑色数据，command 为 0x10 表示黑色数据，为 0x13 表示彩色数据
  
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data = 0xFF;
      if (bitmap)
      {
        int16_t idx = mirror_y ? j + dx / 8 + ((h - 1 - (i + dy))) * wb : j + dx / 8 + (i + dy) * wb;
        if (pgm)
        {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
          data = pgm_read_byte(&bitmap[idx]);
#else
          data = bitmap[idx];
#endif
        }
        else
        {
          data = bitmap[idx];
        }
        if (isBlack) {
          // 处理黑色数据
          if (invert) data = ~data;
        } else {
          // 处理彩色数据
          if (!invert) data = ~data;
        }
      }
      _transfer(data);
    }
  }
  _endTransfer();
  _writeCommand(0x92); // partial out
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32

  //_initial_write = true;
  //Serial.println("写入图像数据");
}

void GxEPD2_750c_Z08::writeImagePartFresh(uint8_t command,const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImagePartFresh(command, bitmap, x, y, w, h, invert, mirror_y, pgm);
}
void GxEPD2_750c_Z08::writeImageRedFix(uint8_t command,const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImageRedFix(command, bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_Z08::_writeImagePartFresh(uint8_t command, const uint8_t* bitmap, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //if (_using_partial_mode) _Init_Part2();

  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  uint16_t wb = (w + 7) / 8; // width bytes, bitmaps are padded
  x -= x % 8; // byte boundary
  w = wb * 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;
  if (!_using_partial_mode) _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(command);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data;
      // use wb, h of bitmap for index!
      uint16_t idx = mirror_y ? j + dx / 8 + uint16_t((h - 1 - (i + dy))) * wb : j + dx / 8 + uint16_t(i + dy) * wb;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&bitmap[idx]);
#else
        data = bitmap[idx];
#endif
      }
      else
      {
        data = bitmap[idx];
      }
      if (invert) data = ~data;
      _transfer(data);
    }
  }
  _endTransfer();
  _writeCommand(0x92); // partial out
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  //Serial.println("写入图像数据-writeImagePartFresh");
}

void GxEPD2_750c_Z08::_writeImageRedFix(uint8_t command, const uint8_t* bitmap, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    //if (_using_partial_mode) _Init_Part2();

    if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
    delay(1); // yield() to avoid WDT on ESP8266 and ESP32
    uint16_t wb = (w + 7) / 8; // width bytes, bitmaps are padded
    x -= x % 8; // byte boundary
    w = wb * 8; // byte boundary
    int16_t x1 = x < 0 ? 0 : x; // limit
    int16_t y1 = y < 0 ? 0 : y; // limit
    int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
    int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
    int16_t dx = x1 - x;
    int16_t dy = y1 - y;
    w1 -= dx;
    h1 -= dy;
    if ((w1 <= 0) || (h1 <= 0)) return;
    _Init_Part();
    _writeCommand(0x91); // partial in
    _setPartialRamArea(x1, y1, w1, h1);
    _writeCommand(command);
    _startTransfer();
    
    // 将bitmap数组中的所有元素赋值为0x00
    memset((void*)bitmap, 0x00, wb * h);

    for (int16_t i = 0; i < h1; i++)
    {
        for (int16_t j = 0; j < w1 / 8; j++)
        {
            uint8_t data;
            // use wb, h of bitmap for index!
            uint16_t idx = mirror_y ? j + dx / 8 + uint16_t((h - 1 - (i + dy))) * wb : j + dx / 8 + uint16_t(i + dy) * wb;
            if (pgm)
            {
    #if defined(__AVR) || defined(ESP8266) || defined(ESP32)
                data = pgm_read_byte(&bitmap[idx]);
    #else
                data = bitmap[idx];
    #endif
            }
            else
            {
                data = bitmap[idx];
            }
            if (invert) data = ~data;
            _transfer(data);
        }
    }
    _endTransfer();
    _writeCommand(0x92); // partial out
    delay(1); // yield() to avoid WDT on ESP8266 and ESP32
    //Serial.println("写入图像数据-writeImageRedFix");
}


void GxEPD2_750c_Z08::writeImagePrevious(const uint8_t* black, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImage(0x10, black, x, y, w, h, invert, mirror_y, pgm);
   //Serial.println("写入之前图像数据");
}

void GxEPD2_750c_Z08::writeImageNew(const uint8_t* black, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImage(0x13, black, x, y, w, h, invert, mirror_y, pgm);
   //Serial.println("写入新的图像数据");
}

void GxEPD2_750c_Z08::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                     int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, NULL, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_Z08::writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                     int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImagePart(0x10, black, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  _writeImagePart(0x13, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_Z08::_writeImagePart(uint8_t command, const uint8_t* bitmap, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                      int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  uint16_t wb = (w + 7) / 8; // width bytes, bitmaps are padded
  x -= x % 8; // byte boundary
  w = wb * 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;
  if (!_using_partial_mode) _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(command);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data;
      // use wb, h of bitmap for index!
      uint16_t idx = mirror_y ? j + dx / 8 + uint16_t((h - 1 - (i + dy))) * wb : j + dx / 8 + uint16_t(i + dy) * wb;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&bitmap[idx]);
#else
        data = bitmap[idx];
#endif
      }
      else
      {
        data = bitmap[idx];
      }
      if (invert) data = ~data;
      _transfer(data);
    }
  }
  _endTransfer();
  _writeCommand(0x92); // partial out
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  //Serial.println("写入部分图像数据");
}
void GxEPD2_750c_Z08::writeImagePartPrevious(const uint8_t* black, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                             int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.println("写入之前部分图像数据");
  _writeImagePart(0x10, black, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_Z08::writeImagePartNew(const uint8_t* black, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.println("写入新的部分图像数据");
  _writeImagePart(0x13, black, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_Z08::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_750c_Z08::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_750c_Z08::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_750c_Z08::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_750c_Z08::drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_750c_Z08::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_750c_Z08::refresh(bool partial_update_mode)
{
  if (partial_update_mode) refresh(0, 0, WIDTH, HEIGHT);
  else _Update_Full();
  //Serial.println("refresh选择");
}

void GxEPD2_750c_Z08::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  // intersection with screen
  int16_t w1 = x < 0 ? w + x : w; // reduce
  int16_t h1 = y < 0 ? h + y : h; // reduce
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  w1 = x1 + w1 < int16_t(WIDTH) ? w1 : int16_t(WIDTH) - x1; // limit
  h1 = y1 + h1 < int16_t(HEIGHT) ? h1 : int16_t(HEIGHT) - y1; // limit
  if ((w1 <= 0) || (h1 <= 0)) return; 
  // make x1, w1 multiple of 8
  w1 += x1 % 8;
  if (w1 % 8 > 0) w1 += 8 - w1 % 8;
  x1 -= x1 % 8;
  _Init_Part();
  //if (usePartialUpdateWindow) _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _Update_Part();
  //if (usePartialUpdateWindow) _writeCommand(0x92); // partial out
  //Serial.println("refresh刷新");

}

void GxEPD2_750c_Z08::powerOff()
{
  _PowerOff();
}

void GxEPD2_750c_Z08::hibernate()
{
  _PowerOff();
  if (_rst >= 0)
  {
    _writeCommand(0x07); // deep sleep
    _writeData(0xA5);    // check code
    _hibernating = true;
  }
}

void GxEPD2_750c_Z08::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = (x + w - 1) | 0x0007; // byte boundary inclusive (last byte)
  uint16_t ye = y + h - 1;
  x &= 0xFFF8; // byte boundary
  xe |= 0x0007; // byte boundary
  _writeCommand(0x90); // partial window
  _writeData(x / 256);
  _writeData(x % 256);
  _writeData(xe / 256);
  _writeData(xe % 256);
  _writeData(y / 256);
  _writeData(y % 256);
  _writeData(ye / 256);
  _writeData(ye % 256);
  _writeData(0x01);
   //Serial.println("设置部分更新区域");
}

void GxEPD2_750c_Z08::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    //_waitWhileBusy("_PowerOn", power_on_time);
    _waitWhileBusy("电源开启时间", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_750c_Z08::_PowerOff()
{
  _writeCommand(0x02); // power off
  //_waitWhileBusy("_PowerOff", power_off_time);
  _waitWhileBusy("电源关闭时间", power_off_time);
  _power_is_on = false;
}

void GxEPD2_750c_Z08::_InitDisplay()
{
  if (_hibernating) _reset();
  _writeCommand(0x01); // POWER SETTING
  _writeData (0x07);
  _writeData (0x07); // VGH=20V,VGL=-20V
  _writeData (0x3f); // VDH=15V
  _writeData (0x3f); // VDL=-15V
  _writeCommand(0x00); //PANEL SETTING
  _writeData(0x0f); //KW: 3f, KWR: 2F, BWROTP: 0f, BWOTP: 1f
  _writeCommand(0x61); //tres
  _writeData (WIDTH / 256); //source 800
  _writeData (WIDTH % 256);
  _writeData (HEIGHT / 256); //gate 480
  _writeData (HEIGHT % 256);
  _writeCommand(0x15);
  _writeData(0x00);
  _writeCommand(0x50); //VCOM AND DATA INTERVAL SETTING
  _writeData(0x11);
  _writeData(0x07);
  _writeCommand(0x60); //TCON SETTING
  _writeData(0x22);
  //Serial.println("屏幕初始化");
}

void GxEPD2_750c_Z08::_Init_Full()
{
   //Serial.println("全刷初始化");
  _InitDisplay();
  _PowerOn();
}
#define T1 30 // charge balance pre-phase
#define T2  5 // optional extension
#define T3 30 // color change phase (b/w)
#define T4  5 // optional extension for one color
const unsigned char GxEPD2_750c_Z08::lut_20_LUTC_partial[42] PROGMEM =
{
  0x00, T1, T2, T3, T4, 1, 
  //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
};

const unsigned char GxEPD2_750c_Z08::lut_21_LUTWW_partial[42] PROGMEM =
{ // 10 w
  0x00, T1, T2, T3, T4, 1, 
  //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
};

const unsigned char GxEPD2_750c_Z08::lut_22_LUTKW_partial[42] PROGMEM =
{ // 10 w  
  0x5A, T1, T2, T3, T4, 1, 
  //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
};

const unsigned char GxEPD2_750c_Z08::lut_23_LUTWK_partial[42] PROGMEM =
{ // 01 b
  0x84, T1, T2, T3, T4, 1, 
  //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
};

const unsigned char GxEPD2_750c_Z08::lut_24_LUTKK_partial[42] PROGMEM =
{ // 01 b
  0x00, T1, T2, T3, T4, 1, 
  //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
};

const unsigned char GxEPD2_750c_Z08::lut_25_LUTBD_partial[42] PROGMEM =
{
  0x00, T1, T2, T3, T4, 1, 
  //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
};
void GxEPD2_750c_Z08::_Init_Part()
{
  //Serial.println("局刷初始化");
  _InitDisplay();
  _PowerOn();
}

void GxEPD2_750c_Z08::_Update_Full()
{
 
  _writeCommand(0x12); //display refresh
  //_waitWhileBusy("_Update_Full", full_refresh_time);
  _waitWhileBusy("全部刷新时间", full_refresh_time);
}

void GxEPD2_750c_Z08::_Update_Part()
{
  _writeCommand(0x12); //display refresh
  //_waitWhileBusy("_Update_Part", partial_refresh_time);
  _waitWhileBusy("局部刷新时间", partial_refresh_time);
}


void GxEPD2_750c_Z08::refresh_bw(int16_t x, int16_t y, int16_t w, int16_t h)
{
  //Serial.println("refresh_bw局刷黑白");
  int16_t w1 = x < 0 ? w + x : w; // reduce
  int16_t h1 = y < 0 ? h + y : h; // reduce
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  w1 = x1 + w1 < int16_t(WIDTH) ? w1 : int16_t(WIDTH) - x1; // limit
  h1 = y1 + h1 < int16_t(HEIGHT) ? h1 : int16_t(HEIGHT) - y1; // limit
  if ((w1 <= 0) || (h1 <= 0)) return; 
  // make x1, w1 multiple of 8
  w1 += x1 % 8;
  if (w1 % 8 > 0) w1 += 8 - w1 % 8;
  x1 -= x1 % 8; 
  
  _Init_Part();

  _writeCommand(0x20);
  _writeDataPGM(lut_20_LUTC_partial, sizeof(lut_20_LUTC_partial), 42);
  _writeCommand(0x21);
  _writeDataPGM(lut_21_LUTWW_partial, sizeof(lut_21_LUTWW_partial), 42);
  _writeCommand(0x22);
  _writeDataPGM(lut_22_LUTKW_partial, sizeof(lut_22_LUTKW_partial), 42);
  _writeCommand(0x23);
  _writeDataPGM(lut_23_LUTWK_partial, sizeof(lut_23_LUTWK_partial), 42);
  _writeCommand(0x24);
  _writeDataPGM(lut_24_LUTKK_partial, sizeof(lut_24_LUTKK_partial), 42);
  _writeCommand(0x25);
  _writeDataPGM(lut_25_LUTBD_partial, sizeof(lut_25_LUTBD_partial), 42);

  _writeCommand(0x00); //panel setting
  _writeData(0x3F); // partial update LUT from registers

 _writeCommand(0x01); 
 _writeData(0x07); 
 _writeData(0x07); 
 _writeData(0x3f); 
 _writeData(0x3f); 

 _writeCommand(0x61);
 _writeData(0x03);
 _writeData(0x20);  
 _writeData(0x01);
 _writeData(0xE0);

 _writeCommand(0x15);
 _writeData(0x00);

 _writeCommand(0x60);
 _writeData(0x22);

 _writeCommand(0x82);
 _writeData(0x31);

 _writeCommand(0X50);  //VCOM AND DATA INTERVAL SETTING
 _writeData(0x39);
 _writeData(0x07);

  _PowerOn();
  _using_partial_mode = true;
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _Update_Part();
  _writeCommand(0x92); // partial out
 
}


//void GxEPD2_750c_Z08::_Init_Part2()
//{
//
//
//
//  _writeCommand(0x00); //panel setting
//  _writeData(0x3F); // partial update LUT from registers
//
//  _writeCommand(0x01); 
//  _writeData(0x07); 
//  _writeData(0x07); 
//  _writeData(0x3f); 
//  _writeData(0x3f); 
//
//  _writeCommand(0x04); 
//  delay(100);
//
//  //Enhanced display drive(Add 0x06 command)
// // _writeCommand(0x06);  //Booster Soft Start
// // _writeData(0x17);
// // _writeData(0x17);
// // _writeData(0x28);
// // _writeData(0x17);
// // 
// // _writeCommand(0xE0);
// // _writeData(0x02);
////
// // _writeCommand(0xE5);
// // _writeData(0xFF);
//
//
//
//  _writeCommand(0x61);
//  _writeData(0x03);
//  _writeData(0x20);  
//  _writeData(0x01);
//  _writeData(0xE0);
//
//  _writeCommand(0x15);
//  _writeData(0x00);
//
//  _writeCommand(0x60);
//  _writeData(0x22);
//
//  _writeCommand(0x82);
//  _writeData(0x31);
//
//  _writeCommand(0X50);  //VCOM AND DATA INTERVAL SETTING
//  _writeData(0x39);
//  _writeData(0x07);
//
//  _writeCommand(0x20);
//  _writeDataPGM(lut_20_LUTC_partial, sizeof(lut_20_LUTC_partial), 42);
//  _writeCommand(0x21);
//  _writeDataPGM(lut_21_LUTWW_partial, sizeof(lut_21_LUTWW_partial), 42);
//  _writeCommand(0x22);
//  _writeDataPGM(lut_22_LUTKW_partial, sizeof(lut_22_LUTKW_partial), 42);
//  _writeCommand(0x23);
//  _writeDataPGM(lut_23_LUTWK_partial, sizeof(lut_23_LUTWK_partial), 42);
//  _writeCommand(0x24);
//  _writeDataPGM(lut_24_LUTKK_partial, sizeof(lut_24_LUTKK_partial), 42);
//  _writeCommand(0x25);
//  _writeDataPGM(lut_25_LUTBD_partial, sizeof(lut_25_LUTBD_partial), 42);
//
//  _PowerOn();
//  _using_partial_mode = true;
//  
//}