#ifndef STATE_H
#define STATE_H

#include <stdint.h>

typedef enum state {
  START,
  PLAYER_SELECTION, 
  PLAYING,
  ENDING,
  END, 
} State;


typedef struct cycle {
  uint8_t delta;
  uint8_t time_between_frames;
  uint8_t num_frames;

} Cycle;

/*
  Creates cyclic addition behavior, returns the current frame of the cyclic behavior
*/
uint8_t incr_frame_cycle(Cycle *c);

#endif