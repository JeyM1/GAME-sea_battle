#include "seabattle.h"


bool gameover = false;

const char draw_icons[EFIELDINFO_END] =
{
    ' ',        //EMPTY
    '*',        //SHOT
    'X',        //HIT
    '#',        //KILL
    'O',        //SHIP
};

int player1_ship_count;
int player2_ship_count;
int player = PLAYER_1; //player's turn (can be different)
int net_player;  //defines SERVER or CLIENT playing; DONT CHANGE IT; SERVER = PLAYER_1, CLIENT = PLAYER_2

eFieldInfo player1_data[FIELD_SIZE*FIELD_SIZE] = {EMPTY};   //плохо, по возможности перенести в локал.
eFieldInfo player2_data[FIELD_SIZE*FIELD_SIZE] = {EMPTY};

FILE *logs;

SOCKET sHandleServer;               // socket file descriptor
SOCKET newSHandle;                  // socket descriptor from accept
SOCKET sHandleClient;

//-----------------------------------------------------------------------------
int main(void){
    unsigned short shot_pos = 0;
    unsigned short shoted_temp = 0;

    //eFieldInfo player1_data[FIELD_SIZE*FIELD_SIZE] = {EMPTY};
    eFieldInfo player1_shot_data[FIELD_SIZE*FIELD_SIZE] = {EMPTY};
    //eFieldInfo player2_data[FIELD_SIZE*FIELD_SIZE] = {EMPTY};
    eFieldInfo player2_shot_data[FIELD_SIZE*FIELD_SIZE] = {EMPTY};
    eFieldInfo *temp;
    eFieldInfo *shot_temp;

    eGameState game_state = INIT;
    eMenuState menu_state = MAIN_MENU;

    //-------------------------------------------------
    //introducing();
    //slowprint(100, "version 1.0");
    /*Sleep(1000);
    system("cls");*/
    /*if((logs = fopen(LOG_FILE, "w")) == NULL){
        char choice;
        fprintf(stderr, "Error in opening %s file. Continue? (y/n)\n", LOG_FILE);
        if((choice = getchar()) == 'n')
            exit(EXIT_FAILURE);
    }*/
    while(!gameover){           //меню, установка кораблей, выбор игры
        switch(game_state){
            case INIT: //init game data
                {
                    if(sHandleClient) closesocket(sHandleClient);
                    if(sHandleServer) closesocket(sHandleServer);
                    if(newSHandle) closesocket(newSHandle);
                    player1_ship_count = 0;
                    player2_ship_count = 0;
                    player = PLAYER_1;

                    clearData(player1_data, player2_data, player1_shot_data, player2_shot_data);
                    main_UI(&menu_state, &game_state, NULL);
                    break;
                }
            case DRAW: //drawing and entering data
                {
                    system("cls");
                    if(net_player == SERVER){
                        printf("====================================\n               Player 1\n====================================\n");
                        temp = player1_data;
                        shot_temp = player1_shot_data;
                    }else{
                        printf("====================================\n               Player 2\n====================================\n");
                        temp = player2_data;
                        shot_temp = player2_shot_data;
                    }
                    drawfield(temp, shot_temp, shot_pos);

                    main_UI(&menu_state, &game_state, &shot_pos);

                    if(player == PLAYER_1 && net_player == SERVER){
                        if(get_target_pos(&shot_pos, FIELD_SIZE-1, FIELD_SIZE)){
                            if(send(newSHandle, (char*)&shot_pos, sizeof(shot_pos), MSG_DONTROUTE) == SOCKET_ERROR){
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
                    }else if(player == PLAYER_2 && net_player == CLIENT){
                        if(get_target_pos(&shot_pos, FIELD_SIZE-1, FIELD_SIZE)){
                            if(send(sHandleClient, (char*)&shot_pos, sizeof(shot_pos), MSG_DONTROUTE) == SOCKET_ERROR){
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
                    }else{
                        printf("Waiting for opponents turn..\n");
                        if(recv((net_player == SERVER ? newSHandle : sHandleClient), (char*)&shoted_temp,
                                 sizeof(shoted_temp), 0) != SOCKET_ERROR){
                            game_state = PROCESSING;
                            break;
                        }else{
                            getSocketError();
                            exit(1);
                        }
                        printf("Not received anything\n");
                        break;
                    }
                    /*if(get_target_pos(&shot_pos, FIELD_SIZE-1, FIELD_SIZE) == 1){  //координаты прицела
                        game_state = PROCESSING;
                    }
                    break;*/
                }
            case PROCESSING: //processing data
                {
                    //ingame overlay
                    if((shot_pos >> 8) == FIELD_SIZE){
                        if(shot_pos%2 == 0)
                            if(choice_yes_no("Back to menu?\n")){
                                game_state = INIT;
                                menu_state = MAIN_MENU;
                            }else game_state = DRAW;
                        else if(shot_pos%2 == 1){
                            show_controls();
                            game_state = DRAW;
                        }
                        break;
                    }
                    temp = (net_player == SERVER) ? player2_data : player1_data;
                    //if((player == PLAYER_1 && net_player == SERVER) || (player == PLAYER_2 && net_player == CLIENT))
                    printf("Analyzing %s on %s\n", temp == player1_data ? "player1_data" : "player2_data",
                            shot_temp == player1_shot_data ? "player1_shot_data" : "player2_shot_data");
                    system("pause");
                    //need to analyze SAME data, fix it
                        if(shot_analyze(shoted_temp, temp, shot_temp) == 0){       //procesing who is shooting? if hit -
                            player = ~player;
                            /*slowprint(10, "Player ");
                            printf("%c", player == PLAYER_1 ? '1' : '2');
                            slowprint(10, " your turn!");*/
                        }

                    if(player1_ship_count == 0 || player2_ship_count == 0){
                        game_state = EXIT;
                        break;
                    }
                    game_state = DRAW;
                    break;
                }
            case EXIT:
                {
                    //system("cls");
                    printf("Congratulations! Player %d won!\n", player == PLAYER_1 ? 1 : 2);
                    Sleep(2000);
                    if(choice_yes_no("Back to menu?\n")){
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
