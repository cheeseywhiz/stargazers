/*
 * fonts.h
 *
 *  Created on: Apr 10, 2022
 *      Author: tomasfe
 */

#ifndef INC_FONTS_H_
#define INC_FONTS_H_


typedef struct {
	uint16_t bitmapOffset;     ///< Pointer into GFXfont->bitmap
	uint8_t  width;            ///< Bitmap dimensions in pixels
    uint8_t  height;           ///< Bitmap dimensions in pixels
	uint8_t  xAdvance;         ///< Distance to advance cursor (x axis)
	uint8_t   xOffset;          ///< X dist from cursor pos to UL corner
    uint8_t   yOffset;          ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
	uint8_t  *bitmap;      ///< Glyph bitmaps, concatenated
	GFXglyph *glyph;       ///< Glyph array
	uint8_t   first;       ///< ASCII extents (first char)
    uint8_t   last;        ///< ASCII extents (last char)
	uint8_t   yAdvance;    ///< Newline distance (y axis)
} GFXfont;

extern GFXfont *gfxFont;
extern const GFXfont mono9x7;
extern const GFXfont mono9x7bold;
extern const GFXfont mono12x7;
extern const GFXfont mono12x7bold;
extern const GFXfont mono18x7;
extern const GFXfont mono18x7bold;




#endif /* INC_FONTS_H_ */
