// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
// #include "scene_menu.h"

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* settingsMusic = NULL;
ALLEGRO_SAMPLE* gameMusic = NULL;
ALLEGRO_SAMPLE* winMusic = NULL;
ALLEGRO_SAMPLE* lostMusic = NULL;
ALLEGRO_SAMPLE* PBMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_SAMPLE* pb_sound = NULL;
ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_FONT* settingsFont = NULL;
ALLEGRO_FONT* winlostFont = NULL;
int fontSize = 30;
float music_volume = 0.75;
float effect_volume = 0.75;
bool gameDone = false;

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
	
*/
void shared_init(void) {
	
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
    settingsFont = load_font("Assets/Minecraft.ttf",60);
    winlostFont = load_font("Assets/Minecraft.ttf", 100);
	themeMusic = load_audio("Assets/Music/original_theme.ogg");
    settingsMusic = load_audio("Assets/Music/settings_theme.ogg");
    gameMusic = load_audio("Assets/Music/game_theme.ogg");
    winMusic = load_audio("Assets/Music/win_theme.ogg");
    lostMusic = load_audio("Assets/Music/lost_theme.ogg");
    PBMusic = load_audio("Assets/Music/powerbean_theme.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/pacman-chomp1.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/pacman_death.ogg");
    pb_sound = load_audio("Assets/Music/pb_sound.ogg");
}

void shared_destroy(void) {
	al_destroy_font(menuFont);
    al_destroy_font(winlostFont);
    al_destroy_font(settingsFont);
	al_destroy_sample(themeMusic);
    al_destroy_sample(settingsMusic);
    al_destroy_sample(gameMusic);
    al_destroy_sample(winMusic);
    al_destroy_sample(lostMusic);
    al_destroy_sample(PBMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
    al_destroy_sample(pb_sound);
}
