#include "seabattle.h"

extern const char draw_icons[EFIELDINFO_END];

/*----------------------------------------------------------------------[<]-
 Function: introducing()
 Synopsis: Function for drawing that cute intro on the beggining :)
 Returns:  void
 -----------------------------------------------------------------------[>]-*/
void introducing() {
	enum
	{
		INTRO_ROW = 5, INTRO_COL = 72
	};
	const char intro_text[INTRO_ROW][INTRO_COL] = {
		{ " $$$$   $$$$$    $$$$     $$$$$    $$$$   $$$$$$  $$$$$$  $$      $$$$$\n" },
		{ "$$      $$      $$  $$    $$  $$  $$  $$    $$      $$    $$      $$   \n" },
		{ " $$$$   $$$$    $$$$$$    $$$$$   $$$$$$    $$      $$    $$      $$$$ \n" },
		{ "    $$  $$      $$  $$    $$  $$  $$  $$    $$      $$    $$      $$   \n" },
		{ " $$$$   $$$$$   $$  $$    $$$$$   $$  $$    $$      $$    $$$$$$  $$$$$\n" }};
	printf("\n\n\n\n\n\n\n\n\n\n");
	for (int row = 0; row < INTRO_ROW; row++) {
		for (int i = 0; i < INTRO_COL; i++) {
			printf("%c", intro_text[row][i]);
			Sleep(2);
		}
	}
	printf("\n\n\n        ");
}

/*----------------------------------------------------------------------[<]-
 Function: draw_ship_setup_field()
 Synopsis: Function for drawing player's field in ship_generate_Player()
 Returns:  void
 -----------------------------------------------------------------------[>]-*/
void draw_ship_setup_field( eFieldInfo* ap_data, unsigned short pos, int ship_size, int direction ) {
	unsigned char x = pos;
	unsigned char y = pos >> 8;
	printf("  ABCDEFGHIJ\n");
	printf(" *----------*\n");
	for (int i = 0; i < FIELD_SIZE; i++) {
		printf(" |");
		for (int j = 0; j < FIELD_SIZE; j++) {
			if (direction == 0 && i == y && j == x) {
				for (; ship_size > 0; ship_size--) {
					printf("%c", draw_icons[SHIP]);
					j++; //if dir == 0
				}
				j--;
			}
			else if (direction == 1 && i == y && j == x && ship_size > 0) {
				ship_size--;
				y++;
				printf("%c", draw_icons[SHIP]);
			}
			else
				printf("%c", draw_icons[ap_data[i * FIELD_SIZE + j]]);
		}
		printf("|\n");
	}
	printf(" *----------*\n");
}

/*----------------------------------------------------------------------[<]-
 Function: drawfield()
 Synopsis: Function for drawing player's field in game
 Returns:  void
 -----------------------------------------------------------------------[>]-*/
void drawfield( eFieldInfo* field, eFieldInfo* shot_field, unsigned short target_pos ) {
	unsigned char target_x = 0;
	unsigned char target_y = 0;

	target_x = target_pos;
	target_y = target_pos >> 8;

	printf("  ABCDEFGHIJ            ABCDEFGHIJ\n");
	printf(" *----------*          *----------*\n");
	for (int i = 0; i < FIELD_SIZE; i++) {
		printf(" |");
		for (int j = 0; j < FIELD_SIZE; ++j) {               //field data
			printf("%c", draw_icons[field[i * FIELD_SIZE + j]]);
		}
		printf("|");

		for (int j = 12; j < 22; ++j)
			printf(" ");

		printf("|");
		for (int j = 0; j < FIELD_SIZE; ++j) { //shot field data
			if (target_y == i && target_x == j)
				printf("%c", TARGET_ICON);
			else
				printf("%c", draw_icons[shot_field[i * FIELD_SIZE + j]]);
		}
		printf("|");
		putchar('\n');
	}
	printf(" *----------*          *----------*\n");
}

/*----------------------------------------------------------------------[<]-
 Function: slowprint(int, char*)
 Synopsis: Function for making smooth effect of printing in console
 Returns:  void
 -----------------------------------------------------------------------[>]-*/
void slowprint( int speed, char* text ) {
	for (int i = 0; i < strlen(text); i++) {
		printf("%c", text[i]);
		Sleep(speed);
	}
	Sleep(1000);
}