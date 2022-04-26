/*
 * dataPix.c
 *
 *  Created on: Apr 10, 2022
 *      Author: tomasfe
 */


#include "dataPix.h"


#define MADCTL_MY  0x80 ///< Bottom to top
#define MADCTL_MX  0x40 ///< Right to left
#define MADCTL_MV  0x20 ///< Reverse Mode
#define MADCTL_ML  0x10 ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00 ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08 ///< Blue-Green-Red pixel order
#define MADCTL_MH  0x04 ///< LCD refresh right to left


static const uint8_t
  initb[] = {
      //14,
    HX8357B_SETPOWER, 3,
      0x44, 0x41, 0x06,
    HX8357B_SETVCOM, 2,
      0x40, 0x10,
    HX8357B_SETPWRNORMAL, 2,
      0x05, 0x12,
    HX8357B_SET_PANEL_DRIVING, 5,
      0x14, 0x3b, 0x00, 0x02, 0x11,
    HX8357B_SETDISPLAYFRAME, 1,
      0x0c,                      // 6.8mhz
    HX8357B_SETPANELRELATED, 1,
      0x01,                      // BGR
    0xEA, 3,                     // seq_undefined1, 3 args
      0x03, 0x00, 0x00,
    0xEB, 4,                     // undef2, 4 args
      0x40, 0x54, 0x26, 0xdb,
    HX8357B_SETGAMMA, 12,
      0x00, 0x15, 0x00, 0x22, 0x00, 0x08, 0x77, 0x26, 0x66, 0x22, 0x04, 0x00,
    HX8357_MADCTL, 1,
      0xC0,
    HX8357_COLMOD, 1,
      0x55,
    HX8357_PASET, 4,
      0x00, 0x00, 0x01, 0xDF,
    HX8357_CASET, 4,
      0x00, 0x00, 0x01, 0x3F,
    HX8357B_SETDISPMODE, 1,
      0x00,                      // CPU (DBI) and internal oscillation ??
    HX8357_SLPOUT, 0x80 + 120/5, // Exit sleep, then delay 120 ms
    HX8357_DISPON, 0x80 +  10/5, // Main screen turn on, delay 10 ms
    0                            // END OF COMMAND LIST
  }, initd[] = {
      //16,
    HX8357_SWRESET, 0x80 + 100/5, // Soft reset, then delay 10 ms
    HX8357D_SETC, 3,
      0xFF, 0x83, 0x57,
    0xFF, 0x80 + 500/5,          // No command, just delay 300 ms
    HX8357_SETRGB, 4,
      0x80, 0x00, 0x06, 0x06,    // 0x80 enables SDO pin (0x00 disables)
    HX8357D_SETCOM, 1,
      0x25,                      // -1.52V
    HX8357_SETOSC, 1,
      0x68,                      // Normal mode 70Hz, Idle mode 55 Hz
    HX8357_SETPANEL, 1,
      0x05,                      // BGR, Gate direction swapped
    HX8357_SETPWR1, 6,
      0x00,                      // Not deep standby
      0x15,                      // BT
      0x1C,                      // VSPR
      0x1C,                      // VSNR
      0x83,                      // AP
      0xAA,                      // FS
    HX8357D_SETSTBA, 6,
      0x50,                      // OPON normal
      0x50,                      // OPON idle
      0x01,                      // STBA
      0x3C,                      // STBA
      0x1E,                      // STBA
      0x08,                      // GEN
    HX8357D_SETCYC, 7,
      0x02,                      // NW 0x02
      0x40,                      // RTN
      0x00,                      // DIV
      0x2A,                      // DUM
      0x2A,                      // DUM
      0x0D,                      // GDON
      0x78,                      // GDOFF
    HX8357D_SETGAMMA, 34,
      0x02, 0x0A, 0x11, 0x1d, 0x23, 0x35, 0x41, 0x4b, 0x4b,
      0x42, 0x3A, 0x27, 0x1B, 0x08, 0x09, 0x03, 0x02, 0x0A,
      0x11, 0x1d, 0x23, 0x35, 0x41, 0x4b, 0x4b, 0x42, 0x3A,
      0x27, 0x1B, 0x08, 0x09, 0x03, 0x00, 0x01,
    HX8357_COLMOD, 1,
      0x55,                      // 16 bit
    HX8357_MADCTL, 1,
      0xC0,
    HX8357_TEON, 1,
      0x00,                      // TW off
    HX8357_TEARLINE, 2,
      0x00, 0x02,
    HX8357_SLPOUT, 0x80 + 150/5, // Exit Sleep, then delay 150 ms
    HX8357_DISPON, 0x80 +  50/5, // Main screen turn on, delay 50 ms
    0,                           // END OF COMMAND LIST
  };


void reset(){
    HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_SET);
}

void writeCmd(uint8_t cmd){
  HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&ST7735_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void writeCmd16(uint16_t cmd){
  HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&ST7735_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void writeData(uint8_t* buff, size_t buff_size){
  HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&ST7735_SPI_PORT, buff, buff_size, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void writeData16(uint16_t* buff, size_t buff_size){
  uint8_t msbuff = (*buff) >> 8;
  uint8_t lsbuff = (*buff) & 0x00FF;
  HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&ST7735_SPI_PORT, &msbuff, 1, HAL_MAX_DELAY);
  HAL_SPI_Transmit(&ST7735_SPI_PORT, &lsbuff, 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void pix_initialize(){
	reset();

    //select();

    uint8_t *addr = initd;

    uint8_t cmd, x, numArgs;
    while((cmd = *addr) > 0){
        ++addr;
        if(cmd != 0xFF){
        	writeCmd(cmd);
        }
        x = (*addr);
        ++addr;
        numArgs = x & 0x7F;
        if(x & 0x80){
            HAL_Delay(5*numArgs);
        }
        else{
            while(numArgs--){
                writeData((uint8_t*)addr, 1);
                ++addr;
            }
        }
    }

    setRotation(0);
}// initialize

void setRotation(uint8_t m){
    uint8_t rotation = m & 3;

    switch(rotation) {
    case 0:
      m       = MADCTL_MX | MADCTL_MY | MADCTL_RGB;
      break;
    case 1:
      m       = MADCTL_MV | MADCTL_MY | MADCTL_RGB;
      break;
    case 2:
      m       = MADCTL_RGB;
      break;
    case 3:
      m       = MADCTL_MX | MADCTL_MV | MADCTL_RGB;
      break;
    }

    writeCmd(HX8357_MADCTL);
    writeData(&m, 1);

}// setRotation

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h){
    uint16_t x = x0 + w - 1;
    uint16_t y = y0 + h - 1;
    writeCmd(HX8357_CASET);
    writeData16(&x0, 2);
    writeData16(&x, 2);
    writeCmd(HX8357_PASET);
    writeData16(&y0, 2);
    writeData16(&y, 2);
    writeCmd(HX8357_RAMWR);
}

void drawPixel(uint16_t x0, uint16_t y0, uint16_t color){
    setAddrWindow(x0, y0, 1, 1);
    writeData16(&color, 2);
}

// Return color
uint16_t findColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t maxRed, uint8_t maxGreen, uint8_t maxBlue){
    // Red
	double r = red;
	double maxr = maxRed;
    double fracRed = r / maxr;
    if(fracRed > 1){
    	fracRed = 1;
    }
    int screenRed = 31 * fracRed;
    uint16_t returnColor = ((uint16_t) screenRed) << 6;

    // Green
    double g = green;
    double maxg = maxGreen;
    double fracGreen = g / maxg;
    if(fracGreen > 1){
    	fracGreen = 1;
    }
    int screenGreen = 63 * fracGreen;
    returnColor = (((uint16_t)screenGreen) + returnColor) << 5;

    // Blue
    double b = blue;
    double maxb = maxBlue;
    double fracBlue = b / maxb;
    if(fracBlue > 1){
    	fracBlue = 1;
    }
    int screenBlue = 31 * fracBlue;
    returnColor = ((uint16_t)screenBlue) + returnColor;

    return returnColor;
}

// Specialized Drawing
void fillScreen(uint16_t color){
  printRect(0, 0, 320, 480, color);
}

void printRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color){
  if(x0 > x1){
    uint16_t temp = x0;
    x0 = x1;
    x1 = temp;
  }
  if(y0 > y1){
    uint16_t temp = y0;
    y0 = y1;
    y1 = temp;
  }
  // x0, y0 is smaller than x1, y1
  int w = x1 - x0;
  int h = y1 - y0;
  uint8_t msbuff = color >> 8;
  uint8_t lsbuff = color & 0x00FF;
  setAddrWindow(x0, y0, w, h);
  HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
  for(int i = 0; i < w; ++i){
    for(int j = 0; j < h; ++j){
      HAL_SPI_Transmit(&ST7735_SPI_PORT, &msbuff, 1, HAL_MAX_DELAY);
      HAL_SPI_Transmit(&ST7735_SPI_PORT, &lsbuff, 1, HAL_MAX_DELAY);
    }
  }
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);

}


void printChar(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size){
	y -= 240;
	c -= (uint8_t) gfxFont->first;
	GFXglyph *glyph = &(((GFXglyph *) gfxFont->glyph)[c]);
	uint8_t *bitmap = (uint8_t *) gfxFont->bitmap;

	uint16_t bo = glyph->bitmapOffset;
	uint8_t w = glyph->width;
	uint8_t h = glyph->height;
	uint8_t xo = glyph->xOffset;
	uint8_t yo = glyph->yOffset;

	uint8_t xx;
	uint8_t yy;
	uint8_t bits = 0;
	uint8_t bit = 0;

	uint16_t xo16;
	uint16_t yo16;

	for(yy = 0; yy < h; yy++){
		for(xx = 0; xx < w; xx++){
			if(!(bit++ & 7)){
				bits = (bitmap[bo++]);
			}
			if(bits & 0x80){
				if(size == 1){
					drawPixel(x+xo+xx, y+yo+yy, color);
				}
				else{
					printRect(x+(xo16+xx)*size, y+(yo16+yy)*size, x+(xo16+xx)*size + size, y+(yo16+yy)*size + size, color);
				}
			}
			bits <<= 1;
		} // for xx
	} // for yy
}

void printWord(uint16_t x, uint16_t y, char *str, uint16_t color, uint16_t bg, uint16_t sizeStr){
	int i = 0;
	while(*str != '\0'){
		printChar(x + i*15, y, *str, color, bg, 1);
		++i;
		++str;
	} // for i
}

void buildDisplay(int scroll){
	uint16_t textColor = HX8357_WHITE;
	uint16_t lineColor = HX8357_YELLOW;
	uint16_t squareColor = HX8357_WHITE;
	uint16_t bgColor = HX8357_BLACK;
	gfxFont = &mono9x7;
  // Background
  fillScreen(bgColor);
  for(int i = 0; i < 100; ++i){
	  drawPixel(rand()%310, rand()%410, HX8357_WHITE);
  }
  // Lines (horizontal)
  printRect(0, 70, 320, 72, lineColor);
  printRect(0, 140, 320, 142, lineColor);
  printRect(0, 210, 320, 212, lineColor);
  printRect(0, 280, 320, 282, lineColor);
  printRect(0, 350, 320, 352, lineColor);
  printRect(0, 420, 320, 422, lineColor);
  // Lines (vertical at bottom)
  printRect(90, 420, 92, 480, lineColor);
  printRect(205, 420, 203, 480, lineColor);

  uint16_t xWordOffset = 50;
  // Print Planet List
  if (scroll == 0) {
	  printWord(xWordOffset, 20, "1: Mercury", textColor, bgColor, 1);
	  printWord(xWordOffset, 90, "2: Venus", textColor, bgColor, 1);
	  printWord(xWordOffset, 160, "3: Moon", textColor, bgColor, 1);
	  printWord(xWordOffset, 230, "4: Mars", textColor, bgColor, 1);
	  printWord(xWordOffset, 300, "5: Jupiter", textColor, bgColor, 1);
	  printWord(xWordOffset, 370, "6: Saturn", textColor, bgColor, 1);
  }
  else if (scroll == 1) {
	  printWord(xWordOffset, 20, "2: Venus", textColor, bgColor, 1);
	  printWord(xWordOffset, 90, "3: Moon", textColor, bgColor, 1);
	  printWord(xWordOffset, 160, "4: Mars", textColor, bgColor, 1);
	  printWord(xWordOffset, 230, "5: Jupiter", textColor, bgColor, 1);
	  printWord(xWordOffset, 300, "6: Saturn", textColor, bgColor, 1);
	  printWord(xWordOffset, 370, "7: Uranus", textColor, bgColor, 1);
  }
  else if (scroll == 2) {
  	  printWord(xWordOffset, 20, "3: Moon", textColor, bgColor, 1);
  	  printWord(xWordOffset, 90, "4: Mars", textColor, bgColor, 1);
  	  printWord(xWordOffset, 160, "5: Jupiter", textColor, bgColor, 1);
  	  printWord(xWordOffset, 230, "6: Saturn", textColor, bgColor, 1);
  	  printWord(xWordOffset, 300, "7: Uranus", textColor, bgColor, 1);
  	  printWord(xWordOffset, 370, "8: Neptune", textColor, bgColor, 1);
  }
  else {
	  printWord(xWordOffset, 20, "4: Mars", textColor, bgColor, 1);
	  printWord(xWordOffset, 90, "5: Jupiter", textColor, bgColor, 1);
	  printWord(xWordOffset, 160, "6: Saturn", textColor, bgColor, 1);
	  printWord(xWordOffset, 230, "7: Uranus", textColor, bgColor, 1);
	  printWord(xWordOffset, 300, "8: Neptune", textColor, bgColor, 1);
	  printWord(xWordOffset, 370, "9: Pluto", textColor, bgColor, 1);
  }

  char *up = "A:UP";
  char *down = "B:DOWN";
  char *enter = "C:PHOTO";
  printWord(10, 450, up, textColor, bgColor, 1);
  printWord(102, 450, down, textColor, bgColor, 1);
  printWord(215, 450, enter, textColor, bgColor, 1);
}

void loadingScreen(char *star){
	gfxFont = &mono9x7;
	fillScreen(HX8357_WHITE);
	printWord(50, 200, "Loading: ", HX8357_BLACK, HX8357_WHITE, 1);
	printWord(50, 250, star, HX8357_BLACK, HX8357_WHITE, 1);
}
