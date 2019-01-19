#include "seabattle.h"

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

int main_UI(eMenuState *menu_state){
    bool UI_end = false;  //изменить, когда начать игру
    while(!UI_end){
    switch(*menu_state){
        case MAIN_MENU:
            if((*menu_state = main_menu()) == 0){
                /*выбор типа игры, игроков и тд*/
                UI_end = true;
            }

            break;
        case SETTINGS:
            printf("Ooops, its empty for now :c\n");
            *menu_state = MAIN_MENU;
        case CONTROLS:
            system("cls");
            printf("=======CONTROLS=======\n");
            printf("Key Up - Move Up\n");
            printf("Key Down - Move Down\n");
            printf("Key Left - Move Left\n");
            printf("Key Right - Move Right\n");
            printf("Enter - Shoot\n");
            printf("Space - Change ship direction\n\n");
            system("pause");
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
        }
    }
    return 0;
}

int main_menu(){
    int menu_pos = 0;
    do{
        system("cls");
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
        printf("%s", string);
        printf(" %c Yes\n", pos%2 == 0 ? TARGET_ICON : ' ');
        printf(" %c No\n", abs(pos%2) == 1 ? TARGET_ICON : ' ');
    }while(!get_menu_pos(&pos, 1));
    return pos%2 == 0 ? true : false;
}
