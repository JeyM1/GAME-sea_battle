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
int player = PLAYER_1;

FILE *logs;



//-----------------------------------------------------------------------------
int main(void){
    eGameState game_state = INIT;
    unsigned short shot_pos = 0;

    eFieldInfo player1_data[FIELD_SIZE*FIELD_SIZE] = {EMPTY};
    eFieldInfo player1_shot_data[FIELD_SIZE*FIELD_SIZE] = {EMPTY};
    eFieldInfo player2_data[FIELD_SIZE*FIELD_SIZE] = {EMPTY};
    eFieldInfo player2_shot_data[FIELD_SIZE*FIELD_SIZE] = {EMPTY};
    eFieldInfo *temp;
    eFieldInfo *shot_temp;

    eMenuState menu_state = MAIN_MENU;

    //-------------------------------------------------
    //introducing();
    //slowprint(100, "version 0.2");
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
                    main_UI(&menu_state);
                    ship_generate_Player(player1_data, &player1_ship_count);
                    //ship_generateAI(player1_data, &player1_ship_count); //if player1mode set to AI
                    //Sleep(1000); //srand(time!!!)
                    ship_generateAI(player2_data, &player2_ship_count); //if player2mode set to AI
                    game_state = DRAW;
                    //system("pause");
                    break;
                }
            case DRAW: //drawing and entering data
                {
                    system("cls");
                    if(player == PLAYER_1){
                        printf("====================================\n               Player 1\n====================================\n");
                        temp = player1_data;
                        shot_temp = player1_shot_data;
                    }else{
                        printf("====================================\n               Player 2\n====================================\n");
                        temp = player2_data;
                        shot_temp = player2_shot_data;
                    }
                    drawfield(temp, shot_temp, shot_pos);

                    if(get_target_pos(&shot_pos, FIELD_SIZE-1, FIELD_SIZE-1) == 1){  //координаты прицела
                        game_state = PROCESSING;
                    }
                    break;
                }
            case PROCESSING: //processing data
                {
                    temp = (player == PLAYER_1) ? player2_data : player1_data;
                    if(shot_analyze(shot_pos, temp, shot_temp) == 0){       //procesing who is shooting? if hit -
                        player = ~player;
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
                    char choice;
                    //system("cls");
                    printf("Congratulations! Player %d won!\n", player == PLAYER_1 ? 1 : 2);
                    printf("Exit game? (y/n)\n");
                    if((choice = getchar()) == 'n'){
                        game_state = INIT;
                        clearData(player1_data, player2_data, player1_shot_data, player2_shot_data);
                        break;
                    }
                    printf("Goodbye! :) \n");
                    gameover = true;
                    break;
                }
        }
    }
    //MessageBox(NULL, "End of program", "END", MB_OK | MB_ICONEXCLAMATION);
    return 0;
}
