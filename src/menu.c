#include "seabattle.h"

extern int player1_ship_count;
extern int player2_ship_count;
extern eFieldInfo player1_data[FIELD_SIZE * FIELD_SIZE];
extern eFieldInfo player2_data[FIELD_SIZE * FIELD_SIZE];
extern int net_player;

extern SOCKET newSHandle;
extern SOCKET sHandleServer;
extern SOCKET sHandleClient;
extern int SERVER_PORT_NUM;

/*----------------------------------------------------------------------[<]-
 Function: get_menu_pos(int*, int)
 Synopsis: Function for moving player's crosshair in menus.
 Returns:  0 if crosshair moved, 1 if enter key pressed
 -----------------------------------------------------------------------[>]-*/
int get_menu_pos( int* p_menu_pos, int step ) {
	int key = getch();
	switch (key) {
	case ARROW_KEY_PRESSED: {
		switch (getch()) {
		case KEY_DOWN: {
			(*p_menu_pos)++;
			break;
		}
		case KEY_UP: {
			if (*p_menu_pos > 0)
				(*p_menu_pos)--;
			else
				*p_menu_pos += step;
			break;
		}
		}
		break;
	}
	case KEY_ENTER:
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------[<]-
 Function: show_controls()
 Synopsis: Function for displaying controls.
 Returns:  void
 -----------------------------------------------------------------------[>]-*/
void show_controls() {
	system("cls");
	printf("=======CONTROLS=======\n");
	printf("Key Up - Move Up\n");
	printf("Key Down - Move Down\n");
	printf("Key Left - Move Left\n");
	printf("Key Right - Move Right\n");
	printf("Enter - Shoot\n");
	printf("Space - Change ship direction\n\n");
	system("pause");
}

/*----------------------------------------------------------------------[<]-
 Function: main_UI(eMenuState*, eGameState*, unsigned short*)
 Synopsis: Function for coordinating main User Interface.
 Returns:  void
 -----------------------------------------------------------------------[>]-*/
void main_UI( eMenuState* menu_state, eGameState* game_state, const unsigned short* shot_pos ) {
	switch (*menu_state) {
	case MAIN_MENU:
		if ((*menu_state = main_menu()) == 0) {
			*menu_state = GAME_SETTINGS;
		}
		break;
	case OPTIONS:
		options();
		*menu_state = MAIN_MENU;
		break;
	case CONTROLS:
		show_controls();
		*menu_state = MAIN_MENU;
		break;
	case GAME_EXIT:
		if (choice_yes_no("Are you really want to exit?\n")) {
			printf("Goodbye :)\n");
			Sleep(1000);
			exit(EXIT_SUCCESS);
		}
		else {
			*menu_state = MAIN_MENU;
		}
		break;
	case M_MENU_END:
		printf("Oh, its an Easter Egg!\n");
		break;
	case GAME_SETTINGS:
		// changing game settings here;
		if (set_game_settings()) {
			if (net_player == SERVER) {
				set_field_settings(player1_data, &player1_ship_count);
				// sending data about P1 field
				if (send(newSHandle, (char*)player1_data, sizeof(player1_data), MSG_DONTROUTE) == SOCKET_ERROR) {
					getSocketError();
					closesocket(sHandleClient);
					closesocket(sHandleServer);
					closesocket(newSHandle);
					getch();
					exit(1);
				}
				printf("Waiting for other player to set-up his field..\n");
				if (recv(newSHandle, (char*)player2_data, sizeof(player2_data), 0) != SOCKET_ERROR) {
					player2_ship_count = player1_ship_count;
				}
				else {
					getSocketError();
					closesocket(sHandleClient);
					closesocket(sHandleServer);
					closesocket(newSHandle);
					getch();
					exit(1);
				}
			}
			else if (net_player == CLIENT) {
				set_field_settings(player2_data, &player2_ship_count);
				// same with P2
				if (send(sHandleClient, (char*)player2_data, sizeof(player2_data), MSG_DONTROUTE) == SOCKET_ERROR) {
					getSocketError();
					closesocket(sHandleClient);
					closesocket(sHandleServer);
					closesocket(newSHandle);
					getch();
					exit(1);
				}
				printf("Waiting for other player to set-up his field..");
				if (recv(sHandleClient, (char*)player1_data, sizeof(player1_data), 0) != SOCKET_ERROR) {
					player1_ship_count = player2_ship_count;
				}
				else {
					getSocketError();
					closesocket(sHandleClient);
					closesocket(sHandleServer);
					closesocket(newSHandle);
					getch();
					exit(1);
				}
			}
			*menu_state = IN_GAME;
			*game_state = DRAW;
		}
		else {
			*menu_state = MAIN_MENU;
		}

		break;
	case IN_GAME: {
		int x = *shot_pos;
		int y = *shot_pos >> 8;
		printf("\n %c Back to menu     %c Show Controls\n\n   Combatlog: ", (y == FIELD_SIZE && x % 2 == 0)
		                                                                    ? TARGET_ICON : ' ',
			(y == FIELD_SIZE && x % 2 == 1) ? TARGET_ICON : ' ');   // in-game UI, 'Show Controls', 'Back to Menu'
	}
	}
}

/*----------------------------------------------------------------------[<]-
 Function: main_menu()
 Synopsis: Function for displaying main menu.
 Returns:  menu number of block, that was choosed
 -----------------------------------------------------------------------[>]-*/
int main_menu() {
	int menu_pos = 0;
	do {
		system("cls");
		printf("======Main Menu======\n");
		printf(" %c Play Game\n", menu_pos % M_MENU_END == 0 ? TARGET_ICON : ' ');
		printf(" %c Options\n", menu_pos % M_MENU_END == OPTIONS ? TARGET_ICON : ' ');
		printf(" %c Controls\n", menu_pos % M_MENU_END == CONTROLS ? TARGET_ICON : ' ');
		printf(" %c Exit\n", menu_pos % M_MENU_END == GAME_EXIT ? TARGET_ICON : ' ');
	} while (!get_menu_pos(&menu_pos, M_MENU_END - 1));
	return menu_pos % M_MENU_END;
}

/*----------------------------------------------------------------------[<]-
 Function: choice_yes_no(char*)
 Synopsis: Function for printing question with simple Yes/No answers.
 Returns:  true if 'Yes' choosed, else false
 -----------------------------------------------------------------------[>]-*/
bool choice_yes_no( char* string ) {
	int pos = 0;
	do {
		system("cls");
		printf("%s", string);
		printf(" %c Yes\n", pos % 2 == 0 ? TARGET_ICON : ' ');
		printf(" %c No\n", abs(pos % 2) == 1 ? TARGET_ICON : ' ');
	} while (!get_menu_pos(&pos, 1));
	return pos % 2 == 0 ? true : false;
}

/*----------------------------------------------------------------------[<]-
 Function: set_game_settings()
 Synopsis: Function for configuring player's play type.
 Returns:  1 if players connected, 0 if 'Back'
 -----------------------------------------------------------------------[>]-*/
int set_game_settings() {
	char server_name[64];
	int status = 1;
	int pos = 0;
	bool mode_chosen = false;
	while (!mode_chosen) {
		system("cls");
		printf("======Game Settings======\n");
		printf(" %c Host game\n", pos % 4 == 0 ? TARGET_ICON : ' ');
		printf(" %c Connect to server\n", pos % 4 == 1 ? TARGET_ICON : ' ');
		printf(" %c Connect to Local host\n", pos % 4 == 2 ? TARGET_ICON : ' ');
		printf(" %c Back\n", pos % 4 == 3 ? TARGET_ICON : ' ');
		if (get_menu_pos(&pos, 3)) {
			switch (pos % 4) {
			case 0:  // init server
				status = init_server_socket();
				net_player = SERVER;
				break;
			case 1:
				printf("\nEnter IP address: ");
				fgets(server_name, sizeof(server_name), stdin);
				status = init_client_socket(server_name);
				net_player = CLIENT;
				break;
			case 2:
				status = init_client_socket("127.0.0.1");
				net_player = CLIENT;
				break;
			case 3:
				return 0;
			}
			mode_chosen = true;
		}
	}
	return status;
}

/*----------------------------------------------------------------------[<]-
 Function: set_field_settings()
 Synopsis: Function for configuring player's field filling (AI/Player).
 Returns:  1 if fields are set
 -----------------------------------------------------------------------[>]-*/
int set_field_settings( eFieldInfo* ap_data, int* player_ship_count ) {
	int pos = 0;
	// int player_game_type[2] = {0,0};
	bool game_ready = false;
	while (!game_ready) {
		system("cls");
		printf("   Generate field: \n");
		printf(" %c By Player\n", pos % 2 == 0 ? TARGET_ICON : ' ');
		printf(" %c By AI\n", pos % 2 == 1 ? TARGET_ICON : ' ');
		if (get_menu_pos(&pos, 1))
			switch (pos) {
			case 0:
				ship_generate_Player(ap_data, player_ship_count);
				game_ready = true;
				break;
			case 1:
				ship_generateAI(ap_data, player_ship_count);
				game_ready = true;
				break;
			}
	}
	return 1;
}

/*----------------------------------------------------------------------[<]-
 Function: options()
 Synopsis: Function for configuring game options
            (Curr. only for conf. port).
 Returns:  void
 -----------------------------------------------------------------------[>]-*/
void options() {
	int pos = 0;
	bool fOptionsSet = false;
	char buff[16];
	while (!fOptionsSet) {
		system("cls");
		printf("======Game Options======\n");
		printf(" %c Server Port:\t%d\n", pos % 2 == 0 ? TARGET_ICON : ' ', SERVER_PORT_NUM);
		printf(" %c Back\n", pos % 2 == 1 ? TARGET_ICON : ' ');
		if (get_menu_pos(&pos, 1))
			switch (pos % 2) {
			case 0:  //change game port
				printf("Enter server port: ");
				fgets(buff, 16, stdin);
				SERVER_PORT_NUM = atoi(buff);
				printf("Success! Port set to %d\n", SERVER_PORT_NUM);
				Sleep(1000);
			case 1:
				fOptionsSet = true;
				break;
			}
	}
}
