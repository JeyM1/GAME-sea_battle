#include "seabattle.h"


extern int player1_ship_count;
extern int player2_ship_count;
extern eFieldInfo player1_data[FIELD_SIZE*FIELD_SIZE];
extern eFieldInfo player2_data[FIELD_SIZE*FIELD_SIZE];

int get_menu_pos(int *p_menu_pos, int step){
    int key = 0;
    key = getch();
    switch(key)
    {
        case ARROW_KEY_PRESSED:
        {
            switch(getch())
            {
                case KEY_DOWN:
                {
                   (*p_menu_pos)++;
                    break;
                    //return 0;
                }
                case KEY_UP:
                {
                    if(*p_menu_pos >0)
                        (*p_menu_pos)--;
                    else
                        *p_menu_pos+=step;
                    break;
                    //return 0;
                }
            }
            break;
        }
        case KEY_ENTER:
            return 1;
    }
    return 0;
}

void show_controls(){
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

void main_UI(eMenuState *menu_state, eGameState *game_state, unsigned short *shot_pos){
    switch(*menu_state){
        case MAIN_MENU:
            if((*menu_state = main_menu()) == 0){   /*=Play Game, поменять по возможности (оно изначально =0, возможны баги*/
                *menu_state = GAME_SETTINGS;
            }
            break;
        case SETTINGS:
            printf("Ooops, its empty for now :c\n");
            *menu_state = MAIN_MENU;
            break;
        case CONTROLS:
            show_controls();
            *menu_state = MAIN_MENU;
            break;
        case GAME_EXIT:
            if(choice_yes_no("Are you really want to exit?\n")){
                printf("Goodbye :)\n");
                exit(EXIT_SUCCESS);
            }else{
                *menu_state = MAIN_MENU;
            }
            break;
        case M_MENU_END:
            printf("Oh, its an Easter Egg!\n");
            break;
        case GAME_SETTINGS:
            //changing game settings here;
            if(set_game_settings()){
                *menu_state = IN_GAME;
                *game_state = DRAW;
            }else{
                fprintf(stderr, "Something went wrong. Exiting..\n");
                exit(EXIT_FAILURE);
            }

            break;
        case IN_GAME:
            {
                int x = *shot_pos;
                int y = *shot_pos >> 8;
                printf("\n %c Back to menu     %c Show Controls\n\n   Combatlog: ", (y == FIELD_SIZE && x%2 == 0) ? TARGET_ICON : ' ',
                                                                    (y == FIELD_SIZE && x%2 == 1) ? TARGET_ICON : ' ');   //in-game UI, 'Show Controls', 'Back to Menu'
            }
        }
}

int main_menu(){
    int menu_pos = 0;
    do{
        system("cls");
        printf("======Main Menu======\n");
        printf(" %c Play Game\n", menu_pos%M_MENU_END == 0 ? TARGET_ICON : ' ');
        printf(" %c Settings\n", menu_pos%M_MENU_END == SETTINGS ? TARGET_ICON : ' ');
        printf(" %c Controls\n", menu_pos%M_MENU_END == CONTROLS ? TARGET_ICON : ' ');
        printf(" %c Exit\n", menu_pos%M_MENU_END == GAME_EXIT ? TARGET_ICON : ' ');
    }while(!get_menu_pos(&menu_pos, M_MENU_END-1));
    return menu_pos%M_MENU_END;
}

bool choice_yes_no(char *string){
    int pos = 0;
    do{
        system("cls");
        printf("%s", string); //slow_print?
        printf(" %c Yes\n", pos%2 == 0 ? TARGET_ICON : ' ');
        printf(" %c No\n", abs(pos%2) == 1 ? TARGET_ICON : ' ');
    }while(!get_menu_pos(&pos, 1));
    return pos%2 == 0 ? true : false;
}

int set_game_settings(){
    int pos = 0;
    int player_game_type[2] = {0,0};
    bool game_ready = false;
    while(!game_ready){
        system("cls");
        printf("   Player       Field Generation\n");
        printf(" %c Player 1        %s\n", pos%3 == 0 ? TARGET_ICON : ' ', player_game_type[0] == 0 ? "By Player" : "By AI");
        printf(" %c Player 2        %s\n", pos%3 == 1 ? TARGET_ICON : ' ', player_game_type[1] == 0 ? "By Player" : "By AI");
        printf(" %c End\n", pos%3 == 2 ? TARGET_ICON : ' ');
        if(get_menu_pos(&pos, 1))
        switch(pos%3)
        {
        case 0:
            player_game_type[0] = !player_game_type[0];
            break;
        case 1:
            player_game_type[1] = !player_game_type[1];
            break;
        case 2:
            if(player_game_type[0] == 0){
                system("cls");
                slowprint(10, "Player 1, generate your field!     \n");
            }
            player_game_type[0] == 0 ? ship_generate_Player(player1_data, &player1_ship_count) : ship_generateAI(player1_data, &player1_ship_count);
            if(player_game_type[0] == 1 && player_game_type[1] == 1)
                Sleep(1000); //srand(time)!
            else if(player_game_type[1] == 0){
                system("cls");
                slowprint(10, "Player 2, generate your field!    \n");
            }
            player_game_type[1] == 0 ? ship_generate_Player(player2_data, &player2_ship_count) : ship_generateAI(player2_data, &player2_ship_count);
            game_ready = true;
        }
    }
    return 1;
}
