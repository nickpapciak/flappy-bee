#include "display/render.h"

#include <stdlib.h>

int main() {
    // sets display control mode
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    draw_str(10, 10, "123456", BLUE);
    while(1);

    return 0;
}
