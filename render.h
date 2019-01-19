#include "seabattle.h"
#define INTRO_ROW 5
#define INTRO_COL 72

void introducing();
void draw_ship_setup_field(eFieldInfo *, unsigned short, int, int);
void drawfield(eFieldInfo *, eFieldInfo *, unsigned short);
void slowprint(int speed, char *text);
