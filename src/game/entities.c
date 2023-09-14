#include "entities.h" 

#include "../images/buzz.h"
#include "../images/background.h"
#include "../images/toppipe.h"
#include "../images/bottompipe.h"
#include "../displaycnt/render.h"


Player default_player() {
    Player p;
    
    p.width = BUZZ_WIDTH; 
    p.height = BUZZ_HEIGHT; 
    
    p.phys.pos_x = (WIDTH - p.width) << 3; 
    p.phys.pos_y = 80 << 4;
    p.phys.vel_x = 0;
    p.phys.vel_y = 0;
    p.phys.acc_y = 3; 
    p.phys.num_safety_pixels = 4;

    p.phys.max_vel = 50;
    p.phys.jump_vel = -60;
    
    p.anim.cycle.delta = 0;
    p.anim.cycle.time_between_frames = 6;
    p.anim.cycle.num_frames = 3;
    
    p.anim.hover_anim = 0;
    p.anim.hover_dir = 1;
    p.anim.hover_amt = 10;

    p.type = BUZZ;

    return p;
}

Pipe default_pipe() {
    Pipe pipe;
    pipe.pos_x = (WIDTH - 1) << 4;
    pipe.top = 50; 
    pipe.bottom = 100;
    pipe.width = TOPPIPE_WIDTH;
    pipe.speed = 20;
    return pipe;
}

void draw_next_player_frame(Player *p) {
    // renders using subpixels (fixed point arithmetic)
    render_image(
        p->phys.pos_y >> 4, 
        p->phys.pos_x >> 4, 
        p->width, 
        p->height, 
        buzz_frames[incr_frame_cycle(&p->anim.cycle)]
    );
}

void undraw_player(Player *p) {
    unrender_image(
        p->phys.pos_y >> 4, 
        p->phys.pos_x >> 4, 
        p->width, 
        p->height, 
        background
    );
}


void hover_player(Player *p) {

    p->anim.hover_anim += p->anim.hover_dir;

    if (p->anim.hover_anim >= p->anim.hover_amt) {
        p->anim.hover_dir = -1;
        return;
    }

    if (p->anim.hover_anim <= -p->anim.hover_amt) {
        p->anim.hover_dir = 1;
        return;
    }

    p->phys.pos_y += (p->anim.hover_dir << 2);
}

void jump(Player *p){
    p->phys.vel_y = p->phys.jump_vel;
}

void keep_in_bounds(Player *p){
    if (p->phys.pos_y <= -p->height << 4) {
        p->phys.pos_y = -p->height << 4;
    }
    if (p->phys.pos_y >= 115 << 4) {
        jump(p);
    }
}

void update_physics(Player *p) {
    p->phys.pos_y += p->phys.vel_y;
    p->phys.vel_y += p->phys.acc_y;

    if (p->phys.vel_y >= p->phys.max_vel) {
        p->phys.vel_y = p->phys.max_vel;
    }
}


void draw_pipe(Pipe *pipe) {
    render_cutoff_image(pipe->top - TOPPIPE_HEIGHT, pipe->pos_x >> 4, TOPPIPE_WIDTH, TOPPIPE_HEIGHT, toppipe);
    render_cutoff_image(pipe->bottom, pipe->pos_x >> 4, BOTTOMPIPE_WIDTH, HEIGHT - pipe->bottom - 8, bottompipe);
}

void undraw_pipe(Pipe *pipe) {
    unrender_cutoff_image(pipe->top - TOPPIPE_HEIGHT, pipe->pos_x >> 4, TOPPIPE_WIDTH, TOPPIPE_HEIGHT, background);
    unrender_cutoff_image(pipe->bottom, pipe->pos_x >> 4, BOTTOMPIPE_WIDTH, HEIGHT -  pipe->bottom - 8, background);
}

static int __qran_seed = 42;
static int qran(void) {
  __qran_seed = 1664525 * __qran_seed + 1013904223;
  return (__qran_seed >> 16) & 0x7FFF;
}

static int randint(int min, int max) { return (qran() * (max - min) >> 15) + min;}


void randomize_pipe(Pipe *pipe) {
    pipe->pos_x = (WIDTH - 1) << 4;
    pipe->top = randint(20, 60);
    pipe->bottom = pipe->top + randint(55, 85);
}

int is_collision(Player *p, Pipe *pipe) {
  return ((((p->phys.pos_x >> 4) + p->width >= (pipe->pos_x >> 4)) && 
    ((p->phys.pos_x >> 4) <= pipe->width + (pipe->pos_x >> 4)))) && 
    ((((p->phys.pos_y >> 4) <= pipe->top - p->phys.num_safety_pixels)) 
    || ((p->phys.pos_y >> 4) + p->height >= pipe->bottom + p->phys.num_safety_pixels));
}

void start_game(Player *p, Pipe *pipe1, Pipe *pipe2) {
    *p = default_player();
    *pipe1 = default_pipe();
    *pipe2 = default_pipe();
    pipe2->pos_x = (WIDTH - p->width) << 3; 
    pipe2->top = 0;
    pipe2->bottom = 160;
}

int update_score(uint16_t score, Pipe *pipe) {
    if (pipe->pos_x >> 4 <= -pipe->width){
        score++;
        randomize_pipe(pipe);
        char scorestr[32];
        sprintf(scorestr, "%d", score);
        render_str(4, 4, scorestr, -1);
    }
    return score;

}