#include "state.h"

uint8_t incr_frame_cycle(Cycle *c) {
    c->delta++;
    if (c->delta >= c->num_frames*c->time_between_frames) {
        c->delta = 0;
    }
    return (c->delta)/(c->time_between_frames);
}