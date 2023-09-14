#ifndef ENTITIES_H
#define ENTITIES_H

#include "state.h"
#include <stdint.h>


// type of bee the player can bee
enum player_type {
    BUZZ,
    MAYA, 
};

// the state of the player animation
struct player_animation {
    Cycle cycle;

    int16_t hover_anim; 
    int16_t hover_dir;
    int16_t hover_amt;
};

struct player_physics {
    int16_t pos_x; 
    int16_t pos_y; 
    int16_t vel_x; 
    int16_t vel_y; 
    uint16_t acc_y; 

    // number of pixels that the sprite can collide with and still not count as a hit
    uint16_t num_safety_pixels; 

    uint16_t max_vel; 
    uint16_t jump_vel;
};

// the player
typedef struct player {
    uint16_t width;
    uint16_t height;

    struct player_physics phys;
    struct player_animation anim;
    enum player_type type;
} Player;

typedef struct pipe {
  int16_t pos_x;
  uint16_t top;
  uint16_t bottom;
  uint16_t width;
  uint16_t speed;
} Pipe;


Pipe default_pipe();
/*
  Creates a player with relevant defaults attatched to it
*/
Player default_player();

/*
  Draws a Player and updates its animation
*/
void draw_next_player_frame(Player *p);

/*
  Undraws a player
*/
void undraw_player(Player *p);

/*
  Updates a player hovering
*/
void hover_player(Player *p);

/*
  Jumps a player up
*/
void jump(Player *p);

/*
  Keeps a player in bounds
*/
void keep_in_bounds(Player *p);


/*
  Updates a player's physics
*/
void update_physics(Player *p);

/*
  Draws a pipe
*/
void draw_pipe(Pipe *pipe);

/*
  Undraws a pipe
*/
void undraw_pipe(Pipe *pipe);

/*
  Randomizes a pipe's top and bottom
*/
void randomize_pipe(Pipe *pipe);

/*
  Determines if there is a collision between a player and a pipe
*/
int is_collision(Player *p, Pipe *pipe);

/*
  Starts the game with defaults
*/
void start_game(Player *p, Pipe *pipe1, Pipe *pipe2);

/*
  Updates score
*/
int update_score(uint16_t score, Pipe *pipe);
#endif