#include <stdio.h>
#include <stdlib.h>
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

static Object create_object(ObjectType type, float x, float y, float width, float height) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	return obj;
}

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state)  {
    

    StateInfo info = state_info(state);

    // state -> screen:    αφαιρώ τα offsets
    // screen -> state:     προσθέτω τα offsets
    int x_offset = (SCREEN_WIDTH - 35)/2 - 225;
    int y_offset = - (SCREEN_HEIGHT - 100);
    Camera2D camera = { 0 };
    camera.target = (Vector2){ (SCREEN_WIDTH - 35)/2, info->jet->rect.y};
    camera.offset = (Vector2){ (SCREEN_WIDTH - 35)/2, y_offset+700 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    camera.target = (Vector2){ (SCREEN_WIDTH - 35)/2, info->jet->rect.y };

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);
    

    

    // Σχεδιάζουμε το jet
    DrawCircle(info->jet->rect.x - x_offset, info->jet->rect.y - y_offset, 16, RED);
    
    //state y offset
    int state_y_offset = -info->jet->rect.y;
    
    // Σχεδιάζουμε τα objects
    List objects1 = state_objects(state, 0-state_y_offset + SCREEN_HEIGHT  , - 2*SCREEN_HEIGHT - state_y_offset);
    for (ListNode node = list_first(objects1);
        node != LIST_EOF;
        node = list_next(objects1, node))  {
            
        Object obj = list_node_value(objects1, node);
        //printf(" x cord = %f ||| y cord = %f\n", obj->rect.x, obj->rect.y);
        //DrawCircle(obj->rect.x - x_offset, obj->rect.y - y_offset, 32, PURPLE);

        if (obj->type == TERAIN)  {
            Object temp_obj = create_object(TERAIN, obj->rect.x, 
            obj->rect.y - y_offset, obj->rect.width, obj->rect.height);
            DrawRectangleRec(temp_obj->rect, GREEN);
        }
        if (obj->type == HELICOPTER)  {
            DrawCircle(obj->rect.x - x_offset, obj->rect.y - y_offset, 16, PURPLE);
        }
        if (obj->type == WARSHIP)  {
            DrawCircle(obj->rect.x - x_offset, obj->rect.y - y_offset, 32, YELLOW);
        }
        if (obj->type == BRIDGE)  {
            Object temp_obj = create_object(BRIDGE, obj->rect.x, 
            obj->rect.y - y_offset, obj->rect.width, obj->rect.height);
            DrawRectangleRec(temp_obj->rect, RED);
        }
    }
    //TERAIN, HELICOPTER, WARSHIP, JET, MISSLE, BRIDGE



    // Σχεδιάζουμε το σκορ και το FPS counter
	DrawText(TextFormat("%04i", info->score), 340, info->jet->rect.y - y_offset + -670, 40, GRAY);
	DrawFPS(30, info->jet->rect.y - y_offset + -655);

    EndDrawing();
}