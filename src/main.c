#include "seabattle.h"

bool gameover = false;

const char draw_icons[EFIELDINFO_END] =
	{
		' ',        // EMPTY
		'*',        // SHOT
		'X',        // HIT
		'#',        // KILL
		'O',        // SHIP
	};

int player1_ship_count;
int player2_ship_count;
int player = PLAYER_1; // player's turn (can be different)
int net_player;        // defines SERVER or CLIENT playing; DONT CHANGE IT; SERVER = PLAYER_1, CLIENT = PLAYER_2

eFieldInfo player1_data[FIELD_SIZE * FIELD_SIZE] = { EMPTY };
eFieldInfo player2_data[FIELD_SIZE * FIELD_SIZE] = { EMPTY };

FILE* logs;

SOCKET sHandleServer;               // socket file descriptor
SOCKET newSHandle;                  // socket descriptor from accept
SOCKET sHandleClient;
int SERVER_PORT_NUM = 26838;

int main( void ) {
	unsigned short shot_pos = 0;
	unsigned short shoted_temp = 0;

	eFieldInfo player1_shot_data[FIELD_SIZE * FIELD_SIZE] = { EMPTY };
	eFieldInfo player2_shot_data[FIELD_SIZE * FIELD_SIZE] = { EMPTY };
	eFieldInfo* temp;
	eFieldInfo* shot_temp;

	eGameState game_state = INIT;
	eMenuState menu_state = MAIN_MENU;

	introducing();
	slowprint(100, "version 2.0");
	while (!gameover) {           // main game loop
		switch (game_state) {
		case INIT:          // init game data
		{
			if (sHandleClient) {
				closesocket(sHandleClient);
				sHandleClient = 0;
			}
			if (sHandleServer) {
				closesocket(sHandleServer);
				sHandleServer = 0;
			}
			if (newSHandle) {
				closesocket(newSHandle);
				newSHandle = 0;
			}
			player1_ship_count = 0;
			player2_ship_count = 0;
			player = PLAYER_1;

			clearData(player1_data, player2_data, player1_shot_data, player2_shot_data);
			main_UI(&menu_state, &game_state, NULL);
			break;
		}
		case DRAW:          // drawing and entering data
		{
			system("cls");
			if (net_player == SERVER) {
				printf("====================================\n               Player 1\n====================================\n");
				temp = player1_data;
				shot_temp = player1_shot_data;
			}
			else {
				printf("====================================\n               Player 2\n====================================\n");
				temp = player2_data;
				shot_temp = player2_shot_data;
			}
			drawfield(temp, shot_temp, shot_pos);

			main_UI(&menu_state, &game_state, &shot_pos);
			// sending information about shot. Bad when coming to in-game menu!
			if (player == PLAYER_1 && net_player == SERVER) {
				if (get_target_pos(&shot_pos, FIELD_SIZE - 1, FIELD_SIZE)) {
					if ((shot_pos >> 8) == FIELD_SIZE) {
						game_state = PROCESSING;
						break;
					}
					if (send(newSHandle, (char*)&shot_pos, sizeof(shot_pos), MSG_DONTROUTE) == SOCKET_ERROR) {
						getSocketError();
						closesocket(sHandleClient);
						closesocket(sHandleServer);
						closesocket(newSHandle);
						getch();
						exit(1);
					}
					shoted_temp = shot_pos;
					game_state = PROCESSING;
					break;
				}
				break;
			}
			else if (player == PLAYER_2 && net_player == CLIENT) {
				if (get_target_pos(&shot_pos, FIELD_SIZE - 1, FIELD_SIZE)) {
					if ((shot_pos >> 8) == FIELD_SIZE) {
						game_state = PROCESSING;
						break;
					}
					if (send(sHandleClient, (char*)&shot_pos, sizeof(shot_pos), MSG_DONTROUTE) == SOCKET_ERROR) {
						getSocketError();
						closesocket(sHandleClient);
						closesocket(sHandleServer);
						closesocket(newSHandle);
						getch();
						exit(1);
					}
					shoted_temp = shot_pos;
					game_state = PROCESSING;
					break;
				}
				break;
			}
			else {
				printf("Waiting for opponents turn..\n   ");
				if (recv((net_player == SERVER ? newSHandle : sHandleClient), (char*)&shoted_temp,
					sizeof(shoted_temp), 0) != SOCKET_ERROR) {
					if (shoted_temp >> 8 == 255) {
						system("cls");
						printf("%s left the game\n", net_player == SERVER ? "CLIENT" : "HOST");
						system("pause");
						game_state = INIT;
						menu_state = MAIN_MENU;
						break;
					}
					game_state = PROCESSING;
					break;
				}
				else {
					getSocketError();
					exit(1);
				}
			}
		}
		case PROCESSING: // processing data
		{
			// in-game overlay
			if ((shot_pos >> 8) == FIELD_SIZE) {
				if (shot_pos % 2 == 0)
					if (choice_yes_no("Back to menu?\n")) {
						unsigned short status = (255 << 8) | 255;
						if (send((net_player == SERVER ? newSHandle
						                               : sHandleClient), (char*)&status, sizeof(status), MSG_DONTROUTE)
						    == SOCKET_ERROR) {
							getSocketError();
							closesocket(sHandleClient);
							closesocket(sHandleServer);
							closesocket(newSHandle);
							getch();
							exit(1);
						}
						game_state = INIT;
						menu_state = MAIN_MENU;
					}
					else
						game_state = DRAW;
				else if (shot_pos % 2 == 1) {
					show_controls();
					game_state = DRAW;
				}
				break;
			}
			temp = (player == PLAYER_1) ? player2_data : player1_data;
			shot_temp = (player == PLAYER_1) ? player1_shot_data : player2_shot_data;
			if (shot_analyze(shoted_temp, temp, shot_temp) == 0) {       // processing who is shooting? if hit -
				player = ~player;
			}

			if (player1_ship_count == 0 || player2_ship_count == 0) {
				game_state = EXIT;
				break;
			}
			game_state = DRAW;
			break;
		}
		case EXIT: {
			printf("Congratulations! Player %d won!\n", player == PLAYER_1 ? 1 : 2);
			system("pause");
			if (choice_yes_no("Back to menu?\n")) {
				game_state = INIT;
				menu_state = MAIN_MENU;
				break;
			}
			closesocket(sHandleClient);
			closesocket(sHandleServer);
			closesocket(newSHandle);
			printf("Goodbye! :) \n");
			gameover = true;
			break;
		}
		}
	}
	Sleep(1000);
	return 0;
}
