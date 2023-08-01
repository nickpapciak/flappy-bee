#include "render.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
uint32_t frame_count = 0;

/*
  Wait until the start of the next VBlank phase to prevent tearing
*/
void wait_vblank(void) {
  while(SCANLINECOUNTER > 160);
  while (SCANLINECOUNTER < 160);
  frame_count++;
}

void render_pixel(int row, int col, uint16_t color) {
  videoBuffer[OFFSET(row, col, WIDTH)] = color;
}

void render_rectangle(int row, int col, int width, int height, volatile uint16_t color) {
  // starts at the beginning of each row of the image
  for (int i = 0; i < height; i++) {
    DMA[DMA_CHANNEL_3].cnt = 0;
    DMA[DMA_CHANNEL_3].src = &color;
    // copies over from the beginning of each row to the end
    DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(row + i, col, WIDTH);
    DMA[DMA_CHANNEL_3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
  }
}

void render_image(int row, int col, int width, int height, const uint16_t *image) {
  // starts at the beginning of each row of the image
  for (int i = 0; i < height; i++) {
    DMA[DMA_CHANNEL_3].cnt = 0;
    DMA[DMA_CHANNEL_3].src = image + i*width;
    // copies over from the beginning of each row to the end
    DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(row + i, col, WIDTH);
    DMA[DMA_CHANNEL_3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
  }
}

void unrender_image(int row, int col, int width, int height, const uint16_t *image) {
  // starts at the beginning of each row of the background
  for (int i = 0; i < height; i++) {
    DMA[DMA_CHANNEL_3].cnt = 0;
    DMA[DMA_CHANNEL_3].src = image + OFFSET(row + i, col, WIDTH);
    DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(row + i, col, WIDTH);
    DMA[DMA_CHANNEL_3].cnt = width | DMA_ON;
  }
}

void render_background(const uint16_t *image) {
    DMA[DMA_CHANNEL_3].cnt = 0;
    DMA[DMA_CHANNEL_3].src = image;
    DMA[DMA_CHANNEL_3].dst = videoBuffer;
    DMA[DMA_CHANNEL_3].cnt = WIDTH*HEIGHT | DMA_ON;
}

/*
  Draws an image with bounds checking to allow it to partially draw on the left and right edges
*/
void render_cutoff_image(int row, int col, int width, int height, const uint16_t *image) {
  int width_to_right = MIN(width, WIDTH - col);
  int width_to_left = MAX(0, -col);

  // starts at the beginning of each row of the image
  for (int i = 0; i < height; i++) {
    DMA[DMA_CHANNEL_3].cnt = 0;
    DMA[DMA_CHANNEL_3].src = image + i*width + width_to_left;
    // copies over from the beginning of each row to the end
    DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(row + i, col + width_to_left, WIDTH);
    DMA[DMA_CHANNEL_3].cnt = (width_to_right - width_to_left) | DMA_SOURCE_FIXED | DMA_ON;
  } 
}

void draw_char(int row, int col, char ch, uint16_t color) {
    // for height
    for (int i = 0; i < font.height; i++) {
        for (int j = 0; j < font.width; j++) {
            // selecting values from bitmap
            if ((1<<(8*sizeof(char) - j - 1)) & font.data[(ch*font.height) + i]) {
                render_pixel(row + i, col + j, color);
            }
        }
    }
}

void draw_str(int row, int col, char *str, uint16_t color) {
  while (*str) {
    draw_char(row, col, *str++, color);
    col += font.width;
  }
}