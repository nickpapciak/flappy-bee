#include "render.h"

// video buffer
volatile unsigned short *video_buffer = (volatile unsigned short *) 0x6000000;

// number of frames elapsed
uint32_t frame_count = 0;

/*
  Wait until the start of the next VBlank phase to prevent tearing
*/
void wait_vblank(void) {
  while(SCANLINE > 160);
  while (SCANLINE < 160);
  frame_count++;
}

void render_pixel(int row, int col, uint16_t color) {
  video_buffer[OFFSET(row, col)] = color;
}

void render_rectangle(int row, int col, int width, int height, volatile uint16_t color) {
  // starts at the beginning of each row of the image
  for (int i = 0; i < height; i++) {
    DMA[DMA_CHNL_3].cnt = 0;
    DMA[DMA_CHNL_3].src = &color;
    // copies over from the beginning of each row to the end
    DMA[DMA_CHNL_3].dst = video_buffer + OFFSET(row + i, col);
    DMA[DMA_CHNL_3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
  }
}

void render_image(int row, int col, int width, int height, const uint16_t *image) {
  // starts at the beginning of each row of the image
  for (int i = 0; i < height; i++) {
    DMA[DMA_CHNL_3].cnt = 0;
    DMA[DMA_CHNL_3].src = image + i*width;
    // copies over from the beginning of each row to the end
    DMA[DMA_CHNL_3].dst = video_buffer + OFFSET(row + i, col);
    DMA[DMA_CHNL_3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
  }
}

void unrender_image(int row, int col, int width, int height, const uint16_t *image) {
  // starts at the beginning of each row of the background
  for (int i = 0; i < height; i++) {
    DMA[DMA_CHNL_3].cnt = 0;
    DMA[DMA_CHNL_3].src = image + OFFSET(row + i, col);
    DMA[DMA_CHNL_3].dst = video_buffer + OFFSET(row + i, col);
    DMA[DMA_CHNL_3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
  }
}

/*
  Draws an image with bounds checking to allow it to partially draw on the left and right edges
*/
void unrender_cutoff_image(int row, int col, int width, int height, const uint16_t *image) {
  unrender_image(MAX(0, row), MAX(0, col), MIN(WIDTH - col, MIN(width, width + col)), MIN(HEIGHT - row, MIN(height, height + row)), image);
}


void render_background(const uint16_t *image) {
    DMA[DMA_CHNL_3].cnt = 0;
    DMA[DMA_CHNL_3].src = image;
    DMA[DMA_CHNL_3].dst = video_buffer;
    DMA[DMA_CHNL_3].cnt = WIDTH*HEIGHT | DMA_ON;
}

/*
  Draws an image with bounds checking to allow it to partially draw on the left and right edges
*/
void render_cutoff_image(int row, int col, int width, int height, const uint16_t *image) {
  int width_to_right = MIN(width, WIDTH - col);
  int width_to_left = MAX(0, -col);

  // starts at the beginning of each row of the image
  for (int i = 0; i < height; i++) {
    DMA[DMA_CHNL_3].cnt = 0;
    DMA[DMA_CHNL_3].src = image + i*width + width_to_left;
    // copies over from the beginning of each row to the end
    DMA[DMA_CHNL_3].dst = video_buffer + OFFSET(row + i, col + width_to_left);
    DMA[DMA_CHNL_3].cnt = (width_to_right - width_to_left) | DMA_SOURCE_FIXED | DMA_ON;
  } 
}

void render_char(int row, int col, char ch, uint16_t color) {
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

void render_str(int row, int col, char *str, uint16_t color) {
  while (*str) {
    render_char(row, col, *str++, color);
    col += font.width;
  }
}
