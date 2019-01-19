#include "seabattle.h"

int get_menu_pos(int *p_menu_pos){
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
                   (*p_menu_pos)--;
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
    int temp;
    bool start = false;  //изменить, когда начать игру
    while(!start){
        switch(*menu_state){
        case MAIN_MENU:
            if((temp = main_menu()) == 0){
                printf("Hello!\n");
            } else if(temp == 1){
                *menu_state = GAME_EXIT;
            }

        break;
        case GAME_EXIT:
        {
            int pos = 0;
            do{
                system("cls");
                printf("Are you really want to exit?\n");
                printf(" %c Yes\n", pos%2 == 0 ? TARGET_ICON : ' ');
                printf(" %c No\n", abs(pos%2) == 1 ? TARGET_ICON : ' ');
            }while(!get_menu_pos(&pos));
            if(abs(pos) == 0){
                printf("Goodbye :)\n");
                exit(EXIT_SUCCESS);
            }else{
                *menu_state = MAIN_MENU;
            }
        }
        }
    }
    return 0;
}

int main_menu(){
    int menu_pos = 0;
    do{
        system("cls");
        printf(" %c Play Game\n", menu_pos%2 == 0 ? TARGET_ICON : ' ');
        printf(" %c Exit\n", abs(menu_pos%2) == 1 ? TARGET_ICON : ' ');
    }while(!get_menu_pos(&menu_pos));
    return abs(menu_pos%2);
}
