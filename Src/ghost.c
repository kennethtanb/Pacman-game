#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "ghost.h"
#include "map.h"
#include "pacman_obj.h"

/* global variables*/
// [ NOTE ]
// if you change the map .txt to your own design.
// You have to modify cage_grid_{x,y} to corressponding value also.
// Or you can do some change while loading map (reading .txt file)
// Make the start position metadata stored with map.txt.
const int cage_grid_x=22, cage_grid_y=11;

/* shared variables. */
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern const int block_width,  block_height;
extern int count;
/* Internal variables */
static const int fix_draw_pixel_offset_x = -3;
static const int fix_draw_pixel_offset_y = -3;
static const int draw_region = 30;
static Directions direc;
// [ NOTE - speed again ]
// Again, you see this notaficationd. If you still want to implement something 
// fancy with speed, objData->moveCD and GAME_TICK, you can first start on 
// working on animation of ghosts and pacman. // Once you finished the animation 
// part, you will have more understanding on whole mechanism.
static const int basic_speed = 2;

Ghost* ghost_create(int flag) {

	// NOTODO
	Ghost* ghost = (Ghost*)malloc(sizeof(Ghost));
	if (!ghost)
		return NULL;

	ghost->typeFlag = flag;
	ghost->objData.Size.x = block_width;
	ghost->objData.Size.y = block_height;

	ghost->objData.nextTryMove = NONE;
	ghost->speed = basic_speed;
	ghost->status = BLOCKED;

	ghost->flee_sprite = load_bitmap("Assets/ghost_flee.png");
	ghost->dead_sprite = load_bitmap("Assets/ghost_dead.png");

	switch (ghost->typeFlag) {
	case 0:
		ghost->objData.Coord.x = cage_grid_x;
		ghost->objData.Coord.y = cage_grid_y;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		ghost->move_script = &ghost_red_move_script;
		break;
    case 1:
        ghost->objData.Coord.x = cage_grid_x;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_pink.png");
        ghost->move_script = &ghost_red_move_script;
        break;
    case 2:
        ghost->objData.Coord.x = cage_grid_x;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_orange.png");
        ghost->move_script = &ghost_red_move_script;
        break;
    case 3:
        ghost->objData.Coord.x = cage_grid_x;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_blue.png");
        ghost->move_script = &ghost_red_move_script;
        break;
	default:
		ghost->objData.Coord.x = cage_grid_x;
		ghost->objData.Coord.y = cage_grid_y;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		ghost->move_script = &ghost_red_move_script;
		break;
	}
	return ghost;
}
void ghost_destory(Ghost* ghost) {

    al_destroy_bitmap(ghost->move_sprite);
    al_destroy_bitmap(ghost->flee_sprite);
    al_destroy_bitmap(ghost->dead_sprite);
     
    free(ghost);

}
void ghost_draw(Ghost* ghost) {
	// getDrawArea return the drawing RecArea defined by objData and GAME_TICK_CD
	RecArea drawArea = getDrawArea(ghost->objData, GAME_TICK_CD);

	//Draw default image

    

	int bitmap_x_offset = 0;
	// [TODO] below is for animation usage, change the sprite you want to use.
    switch(ghost->status){
        case FLEE:
            al_draw_scaled_bitmap(ghost->flee_sprite, 0, 0,
                16, 16,
                drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                draw_region, draw_region, 0
            );
            if (count<14){
                if (ghost->objData.moveCD>48){
                    al_draw_scaled_bitmap(ghost->flee_sprite, 0, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
                }
                else if (ghost->objData.moveCD>32 && ghost->objData.moveCD<48){
                    al_draw_scaled_bitmap(ghost->flee_sprite, 16, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
                }
            }
            else if (count==14 || count == 16 || count == 18){
                if (ghost->objData.moveCD>16 && ghost->objData.moveCD<32){
                    al_draw_scaled_bitmap(ghost->flee_sprite, 32, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
                }
                else {
                    al_draw_scaled_bitmap(ghost->flee_sprite, 48, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
                }
            }

            break;
        case GO_IN:
            al_draw_scaled_bitmap(ghost->dead_sprite, 0, 0,
                16, 16,
                drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                draw_region, draw_region, 0
            );

            switch(ghost->objData.facing)
            {
            case RIGHT:
                    al_draw_scaled_bitmap(ghost->dead_sprite, 0, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                break;
            case LEFT:
                    al_draw_scaled_bitmap(ghost->dead_sprite, 16, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                break;
            case UP:
                    al_draw_scaled_bitmap(ghost->dead_sprite, 32, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                break;
            case DOWN:
                    al_draw_scaled_bitmap(ghost->dead_sprite, 48, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                break;
            default:
                break;
            }
            printf("berhasil\n");
            break;
        default:
            al_draw_scaled_bitmap(ghost->move_sprite, 0, 0,
                16, 16,
                drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                draw_region, draw_region, 0
            );
            switch(ghost->objData.facing)
            {
            case RIGHT:
                if (ghost->objData.moveCD>32){
                    al_draw_scaled_bitmap(ghost->move_sprite, 16, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                }
                
                else {
                    al_draw_scaled_bitmap(ghost->move_sprite, 0, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                }
                break;
            case LEFT:
                if (ghost->objData.moveCD>32){
                    al_draw_scaled_bitmap(ghost->move_sprite, 48, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                }
                
                else {
                    al_draw_scaled_bitmap(ghost->move_sprite, 32, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                }
                break;
            case UP:
                if (ghost->objData.moveCD>32){
                    al_draw_scaled_bitmap(ghost->move_sprite, 80, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                }
                
                else {
                    al_draw_scaled_bitmap(ghost->move_sprite, 64, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                }
                break;
            case DOWN:
                if (ghost->objData.moveCD>32){
                    al_draw_scaled_bitmap(ghost->move_sprite, 112, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                }
                
                else {
                    al_draw_scaled_bitmap(ghost->move_sprite, 96, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                }
                break;
            
            default:
                break;
            }
            break;
    }
}
void ghost_NextMove(Ghost* ghost, Directions next) {
	ghost->objData.nextTryMove = next;
}
void printGhostStatus(GhostStatus S) {

	switch(S){
	
	case BLOCKED: // stay inside the ghost room
		game_log("BLOCKED");
		break;
	case GO_OUT: // going out the ghost room
		game_log("GO_OUT");
		break;
	case FREEDOM: // free at the map
		game_log("FREEDOM");
		break;
	case GO_IN:
		game_log("GO_IN");
		break;
	case FLEE:
		game_log("FLEE");
		break;
	default:
		game_log("status error");
		break;
	}
}
bool ghost_movable(Ghost* ghost, Map* M, Directions targetDirec, bool room) {
	// [HACKATHON 2-3]
	// TODO: Determine if the current direction is movable.
	// Basically, this is a ghost version of `pacman_movable`.
	// So if you have finished (and you should), you can just "copy and paste"
	// and do some small alternation.
    
    if (ghost->status == GO_IN) room = false;
    else if (!is_room_block(M, ghost->objData.Coord.x, ghost->objData.Coord.y)) room = true;//tambah ini jadi bener

    switch (targetDirec)
        {
        case UP:
            if (is_wall_block(M, ghost->objData.Coord.x, ghost->objData.Coord.y - 1) || (room&&is_room_block(M, ghost->objData.Coord.x, ghost->objData.Coord.y - 1)))
                return false;
            break;
        case DOWN:
            if (is_wall_block(M, ghost->objData.Coord.x, ghost->objData.Coord.y + 1) || (room&&is_room_block(M, ghost->objData.Coord.x, ghost->objData.Coord.y + 1)))
                return false;
            break;
        case LEFT:
            if (is_wall_block(M, ghost->objData.Coord.x - 1, ghost->objData.Coord.y) || (room&&is_room_block(M, ghost->objData.Coord.x - 1, ghost->objData.Coord.y)))
                return false;
            break;
        case RIGHT:
            if (is_wall_block(M, ghost->objData.Coord.x + 1, ghost->objData.Coord.y) || (room&&is_room_block(M, ghost->objData.Coord.x + 1, ghost->objData.Coord.y)))
                return false;
            break;
        default:
            // for none UP, DOWN, LEFT, RIGHT direction u should return false.
            break;
        }
            if (is_wall_block(M, ghost->objData.Coord.x, ghost->objData.Coord.y) || (room&& is_room_block(M, ghost->objData.Coord.x, ghost->objData.Coord.y)))
                return false;


            return true;
            
}

void ghost_toggle_FLEE(Ghost* ghost, bool setFLEE) {
	// [TODO]
	// TODO: Here is reserved for power bean implementation.
	// The concept is "When pacman eats the power bean, only
	// ghosts who are in state FREEDOM will change to state FLEE.
	// For those who are not (BLOCK, GO_IN, etc.), they won't change state."
	// This implementation is based on the classic PACMAN game.
	// You are allowed to do your own implementation of power bean system.
	
    if(setFLEE){
        if(ghost->status == FREEDOM){
            ghost->status = FLEE;
            ghost->speed = 1;
        }
        return;
    }else{
        if(ghost->status == FLEE){
            ghost->status = FREEDOM;
            ghost->speed = 2;
        }
    }
}

void ghost_collided(Ghost* ghost) {
	if (ghost->status == FLEE) {
		ghost->status = GO_IN;
		ghost->speed = 6;
	}
}

void ghost_move_script_GO_IN(Ghost* ghost, Map* M) {
	// Description
	// `shortest_path_direc` is a function that returns the direction of shortest path.
	// Check `map.c` for its detail usage.
	// For GO_IN state.
	ghost->objData.nextTryMove = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, cage_grid_x, cage_grid_y);
}
void ghost_move_script_GO_OUT(Ghost* ghost, Map* M) {
	// Description
	// Here we always assume the room of ghosts opens upward.
	// And used a greedy method to drag ghosts out of room.
	// You should modify here if you have different implementation/design of room.
	if(M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') 
		ghost_NextMove(ghost, UP);
	else
		ghost->status = FREEDOM;
}
void ghost_move_script_FLEE(Ghost* ghost, Map* M, const Pacman * const pacman) {
	// [TODO]
	Directions shortestDirection = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pacman->objData.Coord.x, pacman->objData.Coord.y);
    /*
    Directions waytogo;
    static Directions proba[4]; // possible movement
    int cnt = 0;
    for (Directions i = 1; i <= 4; i++)
        if (ghost_movable(ghost,M,i,false)) proba[cnt++] = i;
    waytogo = proba[rand()%4];
    if (waytogo==shortestDirection || waytogo == 0) waytogo = proba[rand()%4];
    ghost_NextMove(ghost, waytogo);
    printf("waytogo %d  premove%d\n",waytogo,shortestDirection);
    */
    switch (shortestDirection){
        case UP:
            while (direc==UP || direc==0) direc = rand()%4 +1;
            ghost->objData.nextTryMove = direc;
            break;
        case DOWN:
            while (direc==DOWN || direc==0) direc = rand()%4 +1;
            ghost->objData.nextTryMove = direc;
            break;
        case LEFT:
            while (direc==LEFT || direc==0) direc = rand()%4 +1;
            ghost->objData.nextTryMove = direc;
            break;
        case RIGHT:
            while (direc==RIGHT || direc==0) direc = rand()%4 +1;
            ghost->objData.nextTryMove = direc;
            break;
        default:
            break;
    }
     
	// Description:
	// The concept here is to simulate ghosts running away from pacman while pacman is having power bean ability.
	// To achieve this, think in this way. We first get the direction to shortest path to pacman, call it K (K is either UP, DOWN, RIGHT or LEFT).
	// Then we choose other available direction rather than direction K.
	// In this way, ghost will escape from pacman.

}

