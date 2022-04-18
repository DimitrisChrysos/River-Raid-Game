#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#include "state.h"
#include "interface.h"

#define SCREEN_WIDTH 450	// Πλάτος της οθόνης
#define SCREEN_HEIGHT 800	// Υψος της οθόνης

// Assets
Texture jet_img;
Texture helicopter_img;
Texture helicopter_img2;
Texture warship_img;
Texture warship_img2;
Sound game_over_snd;
Sound music;



// Αρχικοποιεί το interface του παιχνιδιού
void interface_init()  {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);
    InitAudioDevice();

    // Φόρτωση εικόνων και ήχων
	jet_img = LoadTextureFromImage(LoadImage("assets/jet_img.png"));
    helicopter_img = LoadTextureFromImage(LoadImage("assets/helicopter_img.png"));
    helicopter_img2 = LoadTextureFromImage(LoadImage("assets/helicopter_img2.png"));
    warship_img = LoadTextureFromImage(LoadImage("assets/warship_img.png"));
    warship_img2 = LoadTextureFromImage(LoadImage("assets/warship_img2.png"));

    game_over_snd = LoadSound("assets/game_over.mp3");
    music = LoadSound("assets/soundtrack.mp3");
}

// Κλείνει το interface του παιχνιδιού
void interface_close()  {
    CloseAudioDevice();
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
bool playing_music = false;
bool music_was_paused = false;

void interface_draw_frame(State state)  {
    
    StateInfo info = state_info(state);

    // Ηχος παιχνιδίου;
    if (playing_music == false && music_was_paused == false)  {
		PlaySound(music);
        playing_music = true;
    }

    if (playing_music == false && music_was_paused == true)  {
        ResumeSound(music);
        playing_music = true;
    }

    if (IsSoundPlaying(music) == false && info->playing == true)  {
        PlaySound(music);
    }

    if (info->playing == false)  {
        StopSound(music);
        playing_music = false;
    }

    if (info->paused == true)  {
        PauseSound(music);
        playing_music = false;
        music_was_paused = true;
    }
    
    // Σχεδίαση παιχνιδιού
    
    // state -> screen:    αφαιρώ τα offsets
    // screen -> state:     προσθέτω τα offsets
    int y_offset = - (SCREEN_HEIGHT - 100);
    Camera2D camera = { 0 };
    camera.target = (Vector2){ (SCREEN_WIDTH - 35)/2, info->jet->rect.y};
    camera.offset = (Vector2){ (SCREEN_WIDTH - 35)/2, y_offset+700 };
    camera.zoom = 1.0f;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);  
    
    
    // Σχεδιάζουμε το μπλε background
    DrawRectangle(info->jet->rect.x - SCREEN_WIDTH, info->jet->rect.y, 2*SCREEN_WIDTH, 2*SCREEN_HEIGHT, BLUE); 

    // Σχεδιάζουμε το jet
    DrawTexture(jet_img, info->jet->rect.x, info->jet->rect.y - y_offset, WHITE);
    
    // Σχεδιάζουμε το missile
    if (info->missile != NULL)  {
        
        Rectangle missile_rect = info->missile->rect;
        missile_rect.y -= y_offset;
        DrawRectangleRec(missile_rect, WHITE);
    }

    // state y offset
    int state_y_offset = -info->jet->rect.y;
    

    // Σχεδιάζουμε τα objects
    List objects1 = state_objects(state, -state_y_offset + SCREEN_HEIGHT, - state_y_offset - 2*SCREEN_HEIGHT);
    for (ListNode node = list_first(objects1);
        node != LIST_EOF;
        node = list_next(objects1, node))  {
            
        Object obj = list_node_value(objects1, node);

        if (obj->type == TERAIN)  {
            Object temp_obj = create_object(TERAIN, obj->rect.x, 
            obj->rect.y - y_offset, obj->rect.width, obj->rect.height);
            DrawRectangleRec(temp_obj->rect, DARKGREEN);
        }
        if (obj->type == HELICOPTER)  {
            if (obj->forward == true)  {
                DrawTexture(helicopter_img, obj->rect.x, obj->rect.y - y_offset, YELLOW);
            }
            if (obj->forward == false)  {
                DrawTexture(helicopter_img2, obj->rect.x, obj->rect.y - y_offset, YELLOW);
            }
        }
        if (obj->type == WARSHIP)  {
            if (obj->forward == true)  {
                DrawTexture(warship_img, obj->rect.x, obj->rect.y - y_offset, RED);
            }
            else if (obj->forward == false)  {
                DrawTexture(warship_img2, obj->rect.x, obj->rect.y - y_offset, RED);
            }
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


    // Αν το παιχνίδι είναι paused, σχεδιάζομαι το μήνυμα για να ξαναρχίσει
    if (info->paused == true) {
		DrawText(
			"PRESS [P] TO CONTINUE",
			  GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO CONTINUE", 20) / 2 + 40,
			 info->jet->rect.y - y_offset - SCREEN_HEIGHT/2, 20, GRAY
		);
	}


    // Αν το παιχνίδι έχει τελειώσει, σχεδιάζομαι το μήνυμα για να ξαναρχίσει
	if (info->playing == false) {
		DrawText(
			"PRESS [ENTER] TO PLAY AGAIN",
			  GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
			 info->jet->rect.y - y_offset - SCREEN_HEIGHT/2, 20, GRAY
		);
	}
    

    


	// Ηχος, αν είμαστε στο frame που συνέβη το game_over
	if (!info->playing == false)
		PlaySound(game_over_snd);

    EndDrawing();
}