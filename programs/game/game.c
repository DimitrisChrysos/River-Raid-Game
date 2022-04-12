#include <stdio.h>
#include "raylib.h"

#include "interface.h"
#include "state.h"

State state;

//for the saving of the starting state
static int x=0;
State first_state;
StateInfo first_info;

void update_and_draw() {
    struct key_state keys = {
        .left = IsKeyDown(KEY_LEFT),
        .right = IsKeyDown(KEY_RIGHT),
        .up = IsKeyDown(KEY_UP),
        .down = IsKeyDown(KEY_DOWN),
        .space = IsKeyDown(KEY_SPACE),
        .enter = IsKeyDown(KEY_ENTER),
        .n = IsKeyDown(KEY_N),
        .p = IsKeyPressed(KEY_P)
    };
    StateInfo info = state_info(state);

    //saving the starting state
    if (x == 0)  {
        first_state = state;
        first_info = info;
        x = 1;
    }


    //for every occasion
    if (info->playing == false)  {
        if (keys.enter == true)  {
            state = first_state;
            info = first_info;
            state_update(state, &keys);
            interface_draw_frame(state);
            return;
        }
        else if (keys.enter == false)  {
            interface_draw_frame(state);
            return;
        }
    }

    if (info->paused == false)  {
        state_update(state, &keys);
        interface_draw_frame(state);
    }
    else if (info->paused == true)  {
        if (keys.p == true)  {
            state_update(state, &keys);
            interface_draw_frame(state);
        }
        else if (keys.n == true)  {
            state_update(state, &keys);
            interface_draw_frame(state);
        }
        else if (keys.p == false)  {
            interface_draw_frame(state);
        }
    }
}

int main() {
	state = state_create();
	interface_init();


    

	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);

	interface_close();

	return 0;
}



