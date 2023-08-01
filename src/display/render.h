#ifndef GBA_H
#define GBA_H

#include "font.h"
#include <stdint.h>

#define OFFSET(row, col, width) ((col)+(width)*(row))

// Display control pointers
#define REG_DISPCNT (*(volatile unsigned short *) 0x4000000)
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31, 31, 31)
#define RED COLOR(31, 0, 0)
#define GREEN COLOR(0, 31, 0)
#define BLUE COLOR(0, 0, 31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK COLOR(0, 0, 0)
#define GRAY COLOR(5, 5, 5)
#define SKYBLUE COLOR(135, 196, 204)

// The size of the GBA Screen
#define WIDTH 240
#define HEIGHT 160

// initialized in gba.c
extern volatile unsigned short *videoBuffer;

// ---------------------------------------------------------------------------
//                       BUTTON INPUT
// ---------------------------------------------------------------------------
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
#define KEY_JUST_PRESSED(key, buttons, oldbuttons) (KEY_DOWN(key, buttons) & ~KEY_DOWN(key, oldbuttons))

// ---------------------------------------------------------------------------
//                       DMA
// ---------------------------------------------------------------------------
typedef struct
{
  const volatile void *src;
  const volatile void *dst;
  uint32_t                  cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

// ---------------------------------------------------------------------------
//                       VBLANK
// ---------------------------------------------------------------------------
#define SCANLINECOUNTER (*(volatile unsigned short *) 0x4000006)

extern uint32_t frame_count;

/*
 * Runs a blocking loop until the start of next VBlank.
 */
void wait_vblank(void);

// ---------------------------------------------------------------------------
//                       MISC
// ---------------------------------------------------------------------------
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/*
 * Generates a pseudo-random number between min and max.
 *
 * @param  min bottom end of range (inclusive).
 * @param  max top end of range (exclusive).
 * @return random number in the given range.
 */
int randint(int min, int max);

// ---------------------------------------------------------------------------
//                       DRAWING
// ---------------------------------------------------------------------------
void render_pixel(int row, int col, uint16_t color);
void render_rectangle(int row, int col, int width, int height, volatile uint16_t color);
void render_background(const uint16_t *image);
void render_image(int row, int col, int width, int height, const uint16_t *image);
void unrender_image(int row, int col, int width, int height, const uint16_t *image);
void fillScreen(volatile uint16_t color);
void draw_char(int row, int col, char ch, uint16_t color);
void draw_str(int row, int col, char *str, uint16_t color);
void drawCenteredString(int row, int col, int width, int height, char *str, uint16_t color);
void render_cutoff_image(int row, int col, int width, int height, const uint16_t *image);

extern Font font;

#endif
