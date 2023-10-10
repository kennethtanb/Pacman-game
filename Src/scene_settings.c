// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include "shared.h"
#include "utility.h"
#include "scene_menu_object.h"
#include "scene_menu.h"

static ALLEGRO_SAMPLE_ID settingsBGM;
static ALLEGRO_SAMPLE_ID pacmanSound;
static Button btnBack;
static Button btnMute;
static Button btnUnmute;
static int volumeBarW;
static int volumeBarH;
static float volume_cache;
static float sfx_cache;
static bool muteFlag = false;
static ALLEGRO_BITMAP* volumebars[6];
static int volumeselect = 3;
static int sfxselect = 3;
static int rowselect = 0;


// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static void init() {
    char str[300];
    btnBack = button_create(730, 20, 50, 50, "Assets/back.png", "Assets/back2.png");
    btnMute = button_create(440, 140, 50, 50, "Assets/sound_off.png", "Assets/sound_off1.png");
    btnUnmute = button_create(500, 140, 50, 50, "Assets/sound_on.png", "Assets/sound_on1.png");
    for(int i=0; i<6; i++){
        sprintf(str,"Assets/volume_bar%d.png",i);
        volumebars[i] = load_bitmap(str);
    }
    volumeBarW = al_get_bitmap_width(volumebars[5]);
    volumeBarH = al_get_bitmap_height(volumebars[5]);
    stop_bgm(settingsBGM);
    settingsBGM = play_bgm(settingsMusic, music_volume);

}

static void draw(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
    
    
    al_draw_text(
        settingsFont,
        al_map_rgb(255, 255, 255),
        400, 40,
        ALLEGRO_ALIGN_CENTER,
        "SETTINGS"
    );
    
    al_draw_text(
        menuFont,
        al_map_rgb(255, 255, 255),
        200, 160,
        ALLEGRO_ALIGN_CENTER,
        "Music Volume: "
    );
    
    al_draw_text(
        menuFont,
        al_map_rgb(255, 255, 255),
        195, 260,
        ALLEGRO_ALIGN_CENTER,
        "Effect Volume: "
    );
    
    if(rowselect == 0)
        al_draw_text(
            menuFont,
            al_map_rgb(255, 255, 255),
            50, 160,
            ALLEGRO_ALIGN_CENTER,
            ">"
        );
    if(rowselect == 1)
        al_draw_text(
            menuFont,
            al_map_rgb(255, 255, 255),
            50, 260,
            ALLEGRO_ALIGN_CENTER,
            ">"
        );
    if (muteFlag){
        al_draw_text(
            menuFont,
            al_map_rgb(255, 255, 255),
            360, 160,
            ALLEGRO_ALIGN_CENTER,
            "MUTED"
        );
        
        al_draw_text(
            menuFont,
            al_map_rgb(255, 255, 255),
            360, 260,
            ALLEGRO_ALIGN_CENTER,
            "MUTED"
        );
    }
    else{
        al_draw_scaled_bitmap(
            volumebars[volumeselect],
            0, 0,
            volumeBarW-2, volumeBarH,
            200+volumeBarW*0.7, 190-volumeBarH*0.7,
            volumeBarW*0.7, volumeBarH*0.7,
            0
        );
        al_draw_scaled_bitmap(
            volumebars[sfxselect],
            0, 0,
            volumeBarW-2, volumeBarH,
            200+volumeBarW*0.7, 290-volumeBarH*0.7,
            volumeBarW*0.7, volumeBarH*0.7,
            0
        );
    }
    drawButton(btnBack);
    drawButton(btnMute);
    drawButton(btnUnmute);

}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
    //    [HACKATHON 3-7]
    //    TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
    //    Uncomment and fill the code below
    btnBack.hovered = buttonHover(btnBack, mouse_x, mouse_y);
    btnMute.hovered = buttonHover(btnMute, mouse_x, mouse_y);
    btnUnmute.hovered = buttonHover(btnUnmute, mouse_x, mouse_y);
}

static void on_mouse_down() {
    if (btnBack.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        game_change_scene(scene_menu_create());
    if (btnMute.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !muteFlag){
        muteFlag = true;
        stop_bgm(settingsBGM);
        volume_cache = music_volume;
        sfx_cache = effect_volume;
        music_volume = 0;
        effect_volume = 0;

        printf("Muted\n");
    }
    if (btnUnmute.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && muteFlag){
        muteFlag = false;
        stop_bgm(settingsBGM);
        music_volume = volume_cache;
        effect_volume = sfx_cache;
        settingsBGM = play_bgm(settingsMusic, music_volume);
        pacmanSound = play_audio(PACMAN_MOVESOUND, effect_volume);
        printf("Unmuted\n");
    }
        
        //customMap = true;
}

static void on_key_down(int keycode) {
    switch (keycode) {
        case ALLEGRO_KEY_RIGHT:
            if(music_volume<1.25 && !muteFlag && rowselect == 0){
                if(volumeselect<5) volumeselect++;
                music_volume += 0.25;
                printf("up %f",music_volume);
                stop_bgm(settingsBGM);
                settingsBGM = play_bgm(settingsMusic, music_volume);
            };
            if(effect_volume<1.25 && !muteFlag && rowselect == 1){
                if(sfxselect<5) sfxselect++;
                effect_volume += 0.25;
                printf("up %f",effect_volume);
                stop_bgm(pacmanSound);
                pacmanSound = play_audio(PACMAN_MOVESOUND, effect_volume);

            };
            break;
        case ALLEGRO_KEY_LEFT:
            if (music_volume>0 && !muteFlag && rowselect == 0){
                if(volumeselect>0) volumeselect--;
                music_volume -= 0.25;
                printf("down %f",music_volume);
                stop_bgm(settingsBGM);
                settingsBGM = play_bgm(settingsMusic, music_volume);
            }
            if(effect_volume>0 && !muteFlag && rowselect == 1){
                if(sfxselect>0) sfxselect--;
                effect_volume -= 0.25;
                printf("up %f",effect_volume);
                stop_bgm(pacmanSound);
                pacmanSound = play_audio(PACMAN_MOVESOUND, effect_volume);
            };
            break;
        case ALLEGRO_KEY_UP:
            if(rowselect>0) rowselect--;
            break;
        case ALLEGRO_KEY_DOWN:
            if(rowselect<1) rowselect++;
            break;
        default:
            break;
    }
}

static void destroy() {
    stop_bgm(settingsBGM);
    al_destroy_bitmap(btnBack.default_img);
    al_destroy_bitmap(btnBack.hovered_img);
    al_destroy_bitmap(btnUnmute.default_img);
    al_destroy_bitmap(btnUnmute.hovered_img);
    al_destroy_bitmap(btnMute.default_img);
    al_destroy_bitmap(btnMute.hovered_img);
    for(int i=0;i<6;i++){
        al_destroy_bitmap(volumebars[i]);
    }
}


// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
    scene.initialize = &init;
	scene.name = "Settings";
	scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_key_down = &on_key_down;
    scene.on_mouse_move = &on_mouse_move;
    scene.on_mouse_down = &on_mouse_down;
	game_log("Settings scene created");
	return scene;
}

