#include "seabattle.h"

void ship_generate_Player(eFieldInfo *, int *);
void ship_generateAI(eFieldInfo *, int *);
int get_target_pos(unsigned short *, int, int);
int kill_check(unsigned short, eFieldInfo *, eFieldInfo *);
int shot_analyze(unsigned short, eFieldInfo *, eFieldInfo *);
void clearData(eFieldInfo *, eFieldInfo *, eFieldInfo *, eFieldInfo *);
