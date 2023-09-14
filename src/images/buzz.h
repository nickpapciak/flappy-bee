/*
 * Exported with nin10kit v1.8
 * Invocation command was nin10kit --mode=3 --resize=16x16 --transparent=87C4CC buzz buzz.gif 
 * Time-stamp: Saturday 07/15/2023, 19:14:48
 * 
 * Image Information
 * -----------------
 * buzz.gif (frame 0) 16@16
 * buzz.gif (frame 1) 16@16
 * buzz.gif (frame 2) 16@16
 * Transparent color: (135, 196, 204)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef BUZZ_H
#define BUZZ_H

#define BUZZ_TRANSPARENT 0x6710

extern const unsigned short buzz0[256];

extern const unsigned short buzz1[256];

extern const unsigned short buzz2[256];

extern const unsigned short* buzz_frames[3];
#define BUZZ_FRAMES 3

#define BUZZ_SIZE 512
#define BUZZ_LENGTH 256
#define BUZZ_WIDTH 16
#define BUZZ_HEIGHT 16

#endif

