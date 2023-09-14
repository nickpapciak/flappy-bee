#ifndef RENDER_H
#define RENDER_H

#include "font.h"
#include <stdint.h>

// Display control pointers
#define REG_DISPCNT (*(volatile unsigned short *) 0x4000000)
#define MODE3 3
#define BG2_ENABLE (1<<10)

// colors
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define SKYBLUE COLOR(4, 18, 24)

// GBA Screen Size
#define WIDTH 240
#define HEIGHT 160

// offset
#define OFFSET(row, col) ((col)+(WIDTH)*(row))

// video buffer
extern volatile unsigned short *video_buffer;

// buttons
#define BUTTON_A        (1<<0)
#define BUTTON_B        (1<<1)
#define BUTTON_SELECT   (1<<2)
#define BUTTON_START    (1<<3)
#define BUTTON_RIGHT    (1<<4)
#define BUTTON_LEFT     (1<<5)
#define BUTTON_UP       (1<<6)
#define BUTTON_DOWN     (1<<7)
#define BUTTON_R        (1<<8)
#define BUTTON_L        (1<<9)

#define BUTTONS (*(volatile uint32_t *) 0x4000130)
#define KEY_DOWN(key, buttons) (~(buttons) & (key))
#define KEY_TAPPED(key, buttons, oldbuttons) (KEY_DOWN(key, buttons) & ~KEY_DOWN(key, oldbuttons))

typedef struct{
  const volatile void *src;
  const volatile void *dst;
  uint32_t cnt;
} DMA_CONTROL;

// DMA control defines
#define DMA ((volatile DMA_CONTROL *) 0x040000B0)
// dma channel 3
#define DMA_CHNL_3 3
// fixed dma source pointer
#define DMA_SOURCE_FIXED (2 << 23)
// turns DMA on
#define DMA_ON (1 << 31)

// Scanline and vblank managing
#define SCANLINE (*(volatile unsigned short *) 0x4000006)
extern uint32_t frame_count;
void wait_vblank(void);

// min/max
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// render images
void render_pixel(int row, int col, uint16_t color);
void render_rectangle(int row, int col, int width, int height, volatile uint16_t color);
void render_background(const uint16_t *image);
void render_image(int row, int col, int width, int height, const uint16_t *image);
void unrender_image(int row, int col, int width, int height, const uint16_t *image);
void render_char(int row, int col, char ch, uint16_t color);
void render_str(int row, int col, char *str, uint16_t color);
void render_cutoff_image(int row, int col, int width, int height, const uint16_t *image);
void unrender_cutoff_image(int row, int col, int width, int height, const uint16_t *image);

// font
extern Font font;

#endif
