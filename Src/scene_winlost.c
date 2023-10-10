// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_winlost.h"
#include "shared.h"
#include "utility.h"
#include "scene_menu_object.h"
#include "scene_menu.h"
#include "scene_game.h"

static ALLEGRO_SAMPLE_ID winBGM;
static ALLEGRO_SAMPLE_ID lostBGM;

extern bool winFlag;

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static void init() {
    if(winFlag){
        stop_bgm(winBGM);
        winBGM = play_bgm(winMusic, music_volume);
    }
    else{
        stop_bgm(lostBGM);
        lostBGM = play_bgm(lostMusic, music_volume);
    }
}

static void draw(void){
	al_clear_to_color(al_map_rgb(0, 0, 0));
    
    if (winFlag)
        al_draw_text(
            winlostFont,
            al_map_rgb(50, 255, 50),
            400, 150,
            ALLEGRO_ALIGN_CENTER,
            "YOU WIN"
        );
    else
        al_draw_text(
            winlostFont,
            al_map_rgb(255, 0, 0),
            400, 150,
            ALLEGRO_ALIGN_CENTER,
            "YOU LOST"
         );
    
    al_draw_text(
        menuFont,
        al_map_rgb(255, 255, 255),
        400, 600,
        ALLEGRO_ALIGN_CENTER,
        "PRESS R TO RESTART"
    );
    
    al_draw_text(
        menuFont,
        al_map_rgb(255, 255, 255),
        400, 650,
        ALLEGRO_ALIGN_CENTER,
        "PRESS ESC TO RETURN TO MAIN MENU"
    );
}

static void on_key_down(int keycode) {
    switch (keycode) {
        case ALLEGRO_KEY_R:
            game_change_scene(scene_main_create());
            break;
        case ALLEGRO_KEY_ESCAPE:
            game_change_scene(scene_menu_create());
            break;
        default:
            break;
    }
}

static void destroy() {
    if(winFlag)
        stop_bgm(winBGM);
    else
        stop_bgm(lostBGM);

}


// The only function that is shared across files.
Scene scene_winlost_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
    scene.initialize = &init;
	scene.name = "winlost";
	scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_key_down = &on_key_down;
	game_log("WinLost scene created");
	return scene;
}

