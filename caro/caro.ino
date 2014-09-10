#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <avr/pgmspace.h>

// If your 32x32 matrix has the SINGLE HEADER input,
// use this pinout:
#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

static const int8_t PROGMEM sinetab[256] = {
    0,   2,   5,   8,  11,  15,  18,  21,
    24,  27,  30,  33,  36,  39,  42,  45,
    48,  51,  54,  56,  59,  62,  65,  67,
    70,  72,  75,  77,  80,  82,  85,  87,
    89,  91,  93,  96,  98, 100, 101, 103,
    105, 107, 108, 110, 111, 113, 114, 116,
    117, 118, 119, 120, 121, 122, 123, 123,
    124, 125, 125, 126, 126, 126, 126, 126,
    127, 126, 126, 126, 126, 126, 125, 125,
    124, 123, 123, 122, 121, 120, 119, 118,
    117, 116, 114, 113, 111, 110, 108, 107,
    105, 103, 101, 100,  98,  96,  93,  91,
    89,  87,  85,  82,  80,  77,  75,  72,
    70,  67,  65,  62,  59,  56,  54,  51,
    48,  45,  42,  39,  36,  33,  30,  27,
    24,  21,  18,  15,  11,   8,   5,   2,
    0,  -3,  -6,  -9, -12, -16, -19, -22,
    -25, -28, -31, -34, -37, -40, -43, -46,
    -49, -52, -55, -57, -60, -63, -66, -68,
    -71, -73, -76, -78, -81, -83, -86, -88,
    -90, -92, -94, -97, -99,-101,-102,-104,
    -106,-108,-109,-111,-112,-114,-115,-117,
    -118,-119,-120,-121,-122,-123,-124,-124,
    -125,-126,-126,-127,-127,-127,-127,-127,
    -128,-127,-127,-127,-127,-127,-126,-126,
    -125,-124,-124,-123,-122,-121,-120,-119,
    -118,-117,-115,-114,-112,-111,-109,-108,
    -106,-104,-102,-101, -99, -97, -94, -92,
    -90, -88, -86, -83, -81, -78, -76, -73,
    -71, -68, -66, -63, -60, -57, -55, -52,
    -49, -46, -43, -40, -37, -34, -31, -28,
    -25, -22, -19, -16, -12,  -9,  -6,  -3
};

const char msg1[] PROGMEM = "Te amo!!!";


void setup() {
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setTextSize(2);
}

long loopCount =  0;

//===============================================================================
//  Plasma
//===============================================================================

const float radius1  = 16.3;
const float radius2  = 23.0;
const float radius3  = 40.8;
const float radius4  = 44.2;
const float centerx1 = 16.1;
const float centerx2 = 11.6;
const float centerx3 = 23.4;
const float centerx4 =  4.1;
const float centery1 =  8.7;
const float centery2 =  6.5;
const float centery3 = 14.0;
const float centery4 = -2.9;

float angle1   =  0.0;
float angle2   =  0.0;
float angle3   =  0.0;
float angle4   =  0.0;

void plasma() {
    int           x1, x2, x3, x4, y1, y2, y3, y4, sx1, sx2, sx3, sx4;
    unsigned char x, y;
    long          value;

    sx1 = (int)(cos(angle1) * radius1 + centerx1);
    sx2 = (int)(cos(angle2) * radius2 + centerx2);
    sx3 = (int)(cos(angle3) * radius3 + centerx3);
    sx4 = (int)(cos(angle4) * radius4 + centerx4);
    y1  = (int)(sin(angle1) * radius1 + centery1);
    y2  = (int)(sin(angle2) * radius2 + centery2);
    y3  = (int)(sin(angle3) * radius3 + centery3);
    y4  = (int)(sin(angle4) * radius4 + centery4);

    long hueShift = loopCount * 2;

    for(y = 0; y < matrix.height(); y++) {
        x1 = sx1;
        x2 = sx2;
        x3 = sx3;
        x4 = sx4;
        for(x = 0; x < matrix.width(); x++) {
            value = hueShift
                + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x1 * x1 + y1 * y1) >> 2))
                + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x2 * x2 + y2 * y2) >> 2))
                + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x3 * x3 + y3 * y3) >> 3))
                + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x4 * x4 + y4 * y4) >> 3));
            matrix.drawPixel(x, y, matrix.ColorHSV(value * 3, 255, 255, true));
            x1--; x2--; x3--; x4--;
        }
        y1--; y2--; y3--; y4--;
    }

    angle1 += 0.03;
    angle2 -= 0.07;
    angle3 += 0.13;
    angle4 -= 0.15;
}

//===============================================================================
//  Black
//===============================================================================

void black() {
    unsigned char x, y;
    for (y = 0; y < matrix.height(); y++) {
        for (x = 0; x < matrix.width(); x++) {
            uint8_t z = 0;
            matrix.drawPixel(x, y, matrix.Color888(z,z,z, /*gamma correct*/true));
        }
    }
}

//===============================================================================
//  Whitedown
//===============================================================================

void whitedown(long wd_start, long n_loops, long times) {
    unsigned char x, y;

    long n = loopCount - wd_start;

    char now = 255 - (times + 1) * (n * (255 / n_loops));

    for (y = 0; y < matrix.height(); y++) {
        for (x = 0; x < matrix.width(); x++) {
            matrix.drawPixel(x, y, matrix.Color888(now,now,now, /*gamma correct*/true));
        }
    }
}

//===============================================================================
//  Text
//===============================================================================

void text(long start) {
    unsigned char x, y;
    //matrix.fillScreen(0);

    matrix.fillCircle(23,13,16, matrix.ColorHSV(30,255,20, /*gamma correct*/false));

    long n = loopCount - start;
    int textX = matrix.width() - n;

    matrix.setTextColor(matrix.Color888(255,255,255,false));
    matrix.setCursor(textX, 1);
    matrix.print(F2(msg1));
}

//===============================================================================
//  Waves
//===============================================================================
void single_wave() {
    int cx1 = 16;
    int cy1 = 16;
    unsigned char x, y;
    long waveSpeed = loopCount * 10;
    for (y = 0; y < matrix.height(); y++) {
        for (x = 0; x < matrix.width(); x++) {
            unsigned char xc1 = x - cx1;
            unsigned char yc1 = y - cy1;
            uint8_t r = (uint8_t)((xc1 * xc1 + yc1 * yc1) >> 2);
            long value = waveSpeed
                + (int8_t)pgm_read_byte(sinetab + r);
            uint8_t z = 0;
            matrix.drawPixel(x, y, matrix.ColorHSV(3 * 230,255,value, /*gamma correct*/false));
        }
    }
}

void waves(char move_center) {
    int cx1 = 20;
    int cy1 = 16;
    int cx2 = 3;
    int cy2 = 30;
    int cx3 = 20;
    int cy3 = 16;
    unsigned char x, y;
    uint8_t waveSpeed = (uint8_t)(loopCount * 5);
    for (y = 0; y < matrix.height(); y++) {
        long y_value = pgm_read_byte(sinetab + (uint8_t)((y - 16) * 3));
        for (x = 0; x < matrix.width(); x++) {
            uint8_t r1 = (uint8_t)(((x - cx1) * (x - cx1) + (y - cy1) * (y - cy1)));
            uint8_t r2 = (uint8_t)(((x - cx2) * (x - cx2) + (y - cy2) * (y - cy2)));
            uint8_t r3 = (uint8_t)(((x - cx3) * (x - cx3) + (y - cy3) * (y - cy3)));
            long value = 0
                + (int8_t)pgm_read_byte(sinetab + r1 + waveSpeed);
            + (int8_t)pgm_read_byte(sinetab + r2 + waveSpeed);
            + (int8_t)pgm_read_byte(sinetab + r3 + waveSpeed);
            value /= 3;
            value += y_value;
            uint8_t z = 0;
            if ((((x - 25) * (x - 25) + y * y) >> 2) >= 7)
                matrix.drawPixel(x, y, matrix.ColorHSV(3 * 230,255, value, /*gamma correct*/false));
        }
        if (move_center) {
            cx1 += 1;
            cy1 -= 1;
            cx2 += 3;
            cy2 += 1;
        }
    }
    matrix.fillCircle(25,0,7,matrix.ColorHSV(255,255,255,false));
}

enum counts {
    Count_plasma = 150,
    Count_black = Count_plasma + 3,
    Count_whitedown = Count_plasma + 20,
    Count_text = Count_whitedown + 150,
    Count_wave = Count_text + 50,
    Count_waves = Count_wave + 60,
    Count_sea = Count_waves + 25,
    Count_end,
};

void loop() {
    long reloop;
    long wd_start;
    loopCount++;
    if (loopCount < Count_plasma) {
        plasma();
    } else if (loopCount < Count_black) {
        black();
    } else if (loopCount < Count_whitedown) {
        if (loopCount == Count_black) {
            wd_start = loopCount;
        }
        whitedown(wd_start, Count_whitedown, 3);
    } else if (loopCount < Count_text) {
        if (loopCount == Count_whitedown) {
            wd_start = loopCount;
            matrix.fillScreen((1 << 16) - 1);
        }
        text(wd_start);
    } else if (loopCount < Count_wave) {
        single_wave();
    } else if (loopCount < Count_waves) {
        waves(0);
    } else if (loopCount < Count_sea) {
        waves(1);
    } else {
        loopCount = 0;
    }
}

