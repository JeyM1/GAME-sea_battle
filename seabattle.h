#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <math.h>

#define FIELD_SIZE 10

#define INTRO_ROW 5
#define INTRO_COL 72

#define ARROW_KEY_PRESSED 0xE0
#define KEY_ENTER         13
#define KEY_UP            72
#define KEY_DOWN          80
#define KEY_RIGHT         77
#define KEY_LEFT          75
#define KEY_SWITCH_DIR    100

#define TARGET_ICON '+'

#define PLAYER_1 0
#define PLAYER_2 ~PLAYER_1

#define LOG_FILE "logs.txt"

typedef enum {EMPTY=0, SHOT, HIT, KILL, SHIP, EFIELDINFO_END}eFieldInfo;
typedef enum {INIT=0, DRAW, PROCESSING, EXIT}eGameState;
typedef enum {MAIN_MENU=0, SETTINGS, CONTROLS, GAME_EXIT, GAME_SETTINGS}eMenuState;

//-------------------------------------------------------------------------------------------------
//-------------RENDERING FUNCTIONS-----------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void introducing();
void draw_ship_setup_field(eFieldInfo *, unsigned short, int, int);
void drawfield(eFieldInfo *, eFieldInfo *, unsigned short);
void slowprint(int speed, char *text);


//-------------------------------------------------------------------------------------------------
//-------------DATA-OPERATING FUNCTIONS------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void ship_generate_Player(eFieldInfo *, int *);
void ship_generateAI(eFieldInfo *, int *);
int get_target_pos(unsigned short *, int, int);
int kill_check(unsigned short, eFieldInfo *, eFieldInfo *);
int shot_analyze(unsigned short, eFieldInfo *, eFieldInfo *);
void clearData(eFieldInfo *, eFieldInfo *, eFieldInfo *, eFieldInfo *);


//-------------------------------------------------------------------------------------------------
//---------------------------------MENU------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

int main_UI(eMenuState *);
int main_menu(void);
int get_menu_pos(int *);
