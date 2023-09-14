#include "displaycnt/render.h"
#include "game/entities.h"
#include "game/state.h"

#include "images/startbackground.h"
#include "images/background.h"

#include <stdlib.h>

int main() {
    // sets display control mode
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    // Save current and previous state of button input.
    uint32_t previous_buttons = BUTTONS;
    uint32_t current_buttons = BUTTONS;

    render_background(startbackground);

    State state = START;

    Player bee = default_player();
    Pipe pipe1 = default_pipe();
    Pipe pipe2 = default_pipe();

    start_game(&bee, &pipe1, &pipe2);

    uint16_t score = 0;

    while(1) {
        wait_vblank();
        previous_buttons = current_buttons;
        current_buttons = BUTTONS;
        

        switch (state) {
            // Start menu
            case START: 
                undraw_player(&bee);    
                render_str(56, 56, "Press A to Begin", -1);
                
                // x on keyboard
                if (KEY_DOWN(BUTTON_A, current_buttons)){
                    jump(&bee);
                    state = PLAYING;
                    render_background(background);
                }

                hover_player(&bee);
                draw_next_player_frame(&bee);

                break;
        
        // core gameplay loop
        case PLAYING: 

            undraw_player(&bee);
            undraw_pipe(&pipe1);
            undraw_pipe(&pipe2);

            update_physics(&bee);

            // x on keyboard
            if (KEY_TAPPED(BUTTON_A, current_buttons, previous_buttons)){
                jump(&bee);
            }

            keep_in_bounds(&bee);

            if ((is_collision(&bee, &pipe1)) || (is_collision(&bee, &pipe2))){
                bee.phys.acc_y = -1;
                bee.phys.vel_y = 0;
                state = ENDING;
                render_background(background);
            }

            pipe1.pos_x-= pipe1.speed;
            pipe2.pos_x-= pipe2.speed;

            score = update_score(score, &pipe1);
            score = update_score(score, &pipe2);

            draw_next_player_frame(&bee);
            draw_pipe(&pipe1);
            draw_pipe(&pipe2);

            break;
        
        // case for the floating up ending animation
        case ENDING: 
            undraw_player(&bee);
            undraw_pipe(&pipe1);
            undraw_pipe(&pipe2);
            
            update_physics(&bee);

            if (bee.phys.pos_y <= -bee.height << 4) {
                state = END;
                bee = default_player();
            }

            draw_next_player_frame(&bee);
            break;  

        // case for the score menu after losing
        case END: 
            undraw_player(&bee);

            if (KEY_DOWN(BUTTON_A, current_buttons)){
                state = PLAYING;
                render_background(background);
                start_game(&bee, &pipe1, &pipe2);
                score = 0;
                jump(&bee);
                break;
            } 

            if (KEY_DOWN(BUTTON_B, current_buttons)) {
                state = START;
                render_background(startbackground);
                start_game(&bee, &pipe1, &pipe2);
                score = 0;
                break;
            }

            render_str(50, 62, "RIP. Try Again?", -1);
            render_str(60, 64, "Yes ", -1);
            render_str(60, 96, "(A)", SKYBLUE);

            render_str(60, 120, " No ", -1);
            render_str(60, 152, "(B)", SKYBLUE);
            
            char scorestr[32];
            sprintf(scorestr, "Score was: %d", score);
            render_str(100, 68, scorestr, -1);

            hover_player(&bee);
            draw_next_player_frame(&bee);
        
            break;
        }
    }
    
    return 0;
}
