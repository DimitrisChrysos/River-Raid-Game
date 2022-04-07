#include <stdio.h>
#include "raylib.h"

#include "state.h"
#include "interface.h"

#define SCREEN_WIDTH 450	// Πλάτος της οθόνης
#define SCREEN_HEIGHT 800	// Υψος της οθόνης


// Αρχικοποιεί το interface του παιχνιδιού
void interface_init()  {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);
    //InitAudioDevice();
}

// Κλείνει το interface του παιχνιδιού
void interface_close()  {
    //CloseAudioDevice();
	CloseWindow();
}

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state)  {
    
    StateInfo info = state_info(state);

    // state -> screen:    αφαιρώ τα offsets
    // screen -> state:     προσθέτω τα offsets
    int x_offset = info->jet->rect.x - 225;
    int y_offset = - (SCREEN_HEIGHT - 100);
    Camera2D camera = { 0 };
    camera.target = (Vector2){ 225, info->jet->rect.y};
    camera.offset = (Vector2){ x_offset+225, y_offset+700 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    camera.target = (Vector2){ info->jet->rect.x, info->jet->rect.y };

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);
    


    DrawCircle(info->jet->rect.x - x_offset, info->jet->rect.y - y_offset, 16, RED);

    DrawCircle(400, 200, 16, YELLOW);
    
    List objects = state_objects(state, 0 , SCREEN_HEIGHT );
    for (ListNode node = list_first(objects);
        node != LIST_EOF;
        node = list_next(objects, node))  {
            
        Object obj = list_node_value(objects, node);

        DrawCircle(obj->rect.x - x_offset, obj->rect.y - y_offset, 32, PURPLE);
    }
    DrawCircle(225, 200, 48, GREEN);

    EndDrawing();
}