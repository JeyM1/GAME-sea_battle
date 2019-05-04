//#include "data.h"
#include "seabattle.h"

extern FILE *logs;
extern int player;
extern int player1_ship_count;
extern int player2_ship_count;

//-----------------------------------------------------------------------------
void ship_generate_Player(eFieldInfo *ap_data, int *player_ship_count){
    int ship_size = 4;
    int dir = 0;    //0 for x, 1 for y
    unsigned short pos = 0;
    unsigned char x, y;
    int temp;
    bool isSet = false;
    while(!isSet){
        bool setting_possible = true;
        system("cls");
        printf("==============\n  Ship setup\n==============\n");
        draw_ship_setup_field(ap_data, pos, ship_size, dir);
        if(dir == 0)
            temp = get_target_pos(&pos, FIELD_SIZE-ship_size, FIELD_SIZE-1);
        else if(dir == 1)
            temp = get_target_pos(&pos, (FIELD_SIZE - 1), (FIELD_SIZE - ship_size));
        x = pos;
        y = pos >> 8;
        if(temp == 1){
            //проверка
            switch(dir)
            {
            case 0:
                for(int i=0; i<ship_size; i++){
                    if(ap_data[y*FIELD_SIZE + x+i]                  == SHIP                            ||
                       (ap_data[y*FIELD_SIZE + (x+ship_size)]       == SHIP && x!=FIELD_SIZE-ship_size)||
                       (ap_data[y*FIELD_SIZE + (x-1)]               == SHIP && x!=0)                   ||
                       ap_data[(y+1)*FIELD_SIZE + x+i]              == SHIP                            ||
                       (ap_data[(y+1)*FIELD_SIZE + (x+ship_size)]   == SHIP && x!=FIELD_SIZE-ship_size)||
                       (ap_data[(y+1)*FIELD_SIZE + (x-1)]           == SHIP && x!=0)                   ||
                       ap_data[(y-1)*FIELD_SIZE + x+i]              == SHIP                            ||
                       (ap_data[(y-1)*FIELD_SIZE + (x+ship_size)]   == SHIP && x!=FIELD_SIZE-ship_size)||
                       (ap_data[(y-1)*FIELD_SIZE + (x-1)]           == SHIP && x!=0)){
                        setting_possible = false;
                        break;
                       }
                }
                break;
            case 1:
                for(int i=0; i<ship_size; i++){
                        //make as with x
                    if(ap_data[(y+i)*FIELD_SIZE + x]               == SHIP                    ||
                       (ap_data[(y+i)*FIELD_SIZE + (x+1)]          == SHIP && x!=FIELD_SIZE-1)||
                       (ap_data[(y+i)*FIELD_SIZE + (x-1)]          == SHIP && x!=0)           ||
                       ap_data[(y+ship_size)*FIELD_SIZE + x]       == SHIP                    ||
                       (ap_data[(y+ship_size)*FIELD_SIZE + (x+1)]  == SHIP && x!=FIELD_SIZE-1)||
                       (ap_data[(y+ship_size)*FIELD_SIZE + (x-1)]  == SHIP && x!=0)           ||
                       ap_data[(y-1)*FIELD_SIZE + x]               == SHIP                    ||
                       (ap_data[(y-1)*FIELD_SIZE + (x+1)]          == SHIP && x!=FIELD_SIZE-1)||
                       (ap_data[(y-1)*FIELD_SIZE + (x-1)]          == SHIP && x!=0)){
                        setting_possible = false;
                        break;
                       }
                }
            }
            if(setting_possible){
                switch(dir)
                {
                case 0:
                    for(int i=0; i<ship_size; i++)
                        ap_data[y*FIELD_SIZE + x+i] = SHIP;
                    break;
                case 1:
                    for(int i=0; i<ship_size; i++)
                        ap_data[(y+i)*FIELD_SIZE + x] = SHIP;
                    break;
                }
                (*player_ship_count)++;
                switch(*player_ship_count)
                {
                case 1:
                    ship_size--;
                    break;
                case 3:
                    ship_size--;
                    break;
                case 6:
                    ship_size--;
                    break;
                case 10:
                    isSet = true;
                    break;
                }
            }else{
                printf("DEBUG: dir=%d\n", dir);
                printf("Could not place ship there! Maybe there are another ship(s) around this one\n");
                system("pause");
            }

        }else if(temp == 2){        //switching dir
            while(x>FIELD_SIZE-ship_size && dir == 1){
                x--;
            }
            while(y>FIELD_SIZE-ship_size && dir == 0){
                y--;
            }
            dir == 0 ? dir++ : dir--;
            pos = (y << 8) | x;
        }
    }
}
//-----------------------------------------------------------------------------
void ship_generateAI(eFieldInfo *ap_data, int *player_ship_count){
    srand(time(NULL));
    int ship_size = 4;
    int ship_count = 0;
    int x, y;
    int dir; //направление для наращивания
    bool isSet = false;
    while(!isSet){
        x = rand()%FIELD_SIZE;
        y = rand()%FIELD_SIZE;

        int temp_x = x;
        int temp_y = y;

        bool setting_possible = true;

        //dir generation
        dir = rand()%4;

        //проверка setting_possible
        for(int i=0; i<ship_size; i++){
            if(x<0 || y<0 || x>=FIELD_SIZE || y>=FIELD_SIZE){
                setting_possible = false;
                break;
            }
            if(ap_data[y*FIELD_SIZE + x]         == SHIP ||
               ap_data[y*FIELD_SIZE + (x+1)]     == SHIP ||
               ap_data[y*FIELD_SIZE + (x-1)]     == SHIP ||
               ap_data[(y+1)*FIELD_SIZE + x]     == SHIP ||
               ap_data[(y+1)*FIELD_SIZE + (x+1)] == SHIP ||
               ap_data[(y+1)*FIELD_SIZE + (x-1)] == SHIP ||
               ap_data[(y-1)*FIELD_SIZE + x]     == SHIP ||
               ap_data[(y-1)*FIELD_SIZE + (x+1)] == SHIP ||
               ap_data[(y-1)*FIELD_SIZE + (x-1)] == SHIP){
                setting_possible = false;
                break;
               }
               switch(dir){
                case 0:
                    y++;
                break;
                case 1:
                    x++;
                break;
                case 2:
                    y--;
                break;
                case 3:
                    x--;
                break;
               }

        }

        //если есть возможность, то ставим корабль
        if(setting_possible){
            x = temp_x;
            y = temp_y;

            for(int i=0; i<ship_size; i++){
                switch(dir){
                case 0:
                    ap_data[y*FIELD_SIZE + x] = SHIP;
                    y++;
                break;
                case 1:
                    ap_data[y*FIELD_SIZE + x] = SHIP;
                    x++;
                break;
                case 2:
                    ap_data[y*FIELD_SIZE + x] = SHIP;
                    y--;
                break;
                case 3:
                    ap_data[y*FIELD_SIZE + x] = SHIP;
                    x--;
                break;
                }
            }

            //проверка на к-во кораблей
            switch(ship_size){
            case 4:
                ship_count++;
                ship_size--;
            break;
            case 3:
                if(ship_count == 2){
                    ship_count++;
                    ship_size--;
                    break;
                }
                ship_count++;
            break;
            case 2:
                if(ship_count == 5){
                    ship_count++;
                    ship_size--;
                    break;
                }
                ship_count++;
            break;
            case 1:
                if(ship_count == 9){
                    isSet = true;
                    break;
                }
                ship_count++;
            break;
            }
        }

    }
    *player_ship_count = ship_count+1;
}
//-----------------------------------------------------------------------------
int get_target_pos(unsigned short *pp_global, int max_dest_x, int max_dest_y){       //unsigned char *pp_x, unsigned char *pp_y){
    unsigned char p_x,p_y;
    p_x = *pp_global;
    p_y = *pp_global >> 8;

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
                    if(p_y < max_dest_y)
                        (p_y)++;
                    break;
                    //return 0;
                }
                case KEY_UP:
                {
                    if(p_y > 0)
                        (p_y)--;
                    break;
                }
                case KEY_LEFT:
                {
                    if(p_x > 0)
                        (p_x)--;
                    break;
                }
                case KEY_RIGHT:
                {
                    if(p_x < max_dest_x)
                        (p_x)++;
                    break;
                }
            }
            break;
        }
        case KEY_ENTER:
            return 1;
        case KEY_SWITCH_DIR:
            return 2;
    }
    *pp_global = (p_y << 8) | (p_x);
    return 0;
}
//-----------------------------------------------------------------------------
int shot_analyze(unsigned short position_global, eFieldInfo *field, eFieldInfo *shot_field){
    unsigned char position_x = position_global;
    unsigned char position_y = position_global >> 8;
    int ship_size;
    //проверка был ли туда уже выстрел??
    if(shot_field[position_y*FIELD_SIZE + position_x] == HIT || shot_field[position_y*FIELD_SIZE + position_x] == SHOT || shot_field[position_y*FIELD_SIZE + position_x] == KILL){
        printf("You had already shot here!\n");
        Sleep(1000);
        return 1;
    }
    if(field[position_y*FIELD_SIZE + position_x] != EMPTY && field[position_y*FIELD_SIZE + position_x] != SHOT){
        field[position_y*FIELD_SIZE + position_x] = HIT;
        shot_field[position_y*FIELD_SIZE + position_x] = HIT;
        if((ship_size=kill_check(position_global, field, shot_field)) >= 1){
            if(player == PLAYER_1)
                player2_ship_count--;
            else
                player1_ship_count--;
            printf("You killed %d-deck ship!\n", ship_size);
            Sleep(1000);
            return 1;
        }
        printf("Hit!\n");
        Sleep(1000);
        return 1;
    }
    printf("Miss!\n");
    shot_field[position_y*FIELD_SIZE + position_x] = SHOT;
    Sleep(1000);
    return 0;
}
//-----------------------------------------------------------------------------
int kill_check(unsigned short position_global, eFieldInfo *field, eFieldInfo *shot_field){
    unsigned char y = position_global >> 8;
    unsigned char y_up = y;
    unsigned char y_down = y;
    unsigned char x = position_global;
    unsigned char x_right = x;
    unsigned char x_left = x;

    int dir = 0;
    int ship_size = 1;
    int error_check = 0;

    while(shot_field[y_up*FIELD_SIZE + x] == HIT){        //up
        if(error_check > 100){
            fprintf(logs, "Crashed in %s. Checking was going UP.\n", __func__);
            MessageBoxA(NULL, "Sorry, something went wrong. Please, send me \"logs.txt\" file (it was created in the same directory with \"exe\" file). Thank you for supporting!", "Error", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
        error_check++;
        if(shot_field[(y_up-1)*FIELD_SIZE + x] == HIT){
            y_up--;
            ship_size++;
            dir = 0;
        }else if(field[(y_up-1)*FIELD_SIZE + x] == EMPTY || y == 0 || y_up == 0){
            break;
        }else if(field[(y_up-1)*FIELD_SIZE + x] == SHIP)
            return 0;
    }
    while(shot_field[y_down*FIELD_SIZE + x] == HIT){        //down
        if(error_check > 100){
            fprintf(logs, "Crashed in %s. Checking was going DOWN.\n", __func__);
            MessageBoxA(NULL, "Sorry, something went wrong. Please, send me \"logs.txt\" file (it was created in the same directory with \"exe\" file). Thank you for supporting!", "Error", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
        error_check++;
        if(shot_field[(y_down+1)*FIELD_SIZE + x] == HIT){
            y_down++;
            ship_size++;
            dir = 0;
        }else if(field[(y_down+1)*FIELD_SIZE + x] == EMPTY || y == 9 || y_down == 9){
            break;
        }else if(field[(y_down+1)*FIELD_SIZE + x] == SHIP)
            return 0;
    }
    while(shot_field[y*FIELD_SIZE + x_left] == HIT){        //left
        if(error_check > 100){
            fprintf(logs, "Crashed in %s. Checking was going DOWN.\n", __func__);
            MessageBoxA(NULL, "Sorry, something went wrong. Please, send me \"logs.txt\" file (it was created in the same directory with \"exe\" file). Thank you for supporting!", "Error", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
        error_check++;
        if(shot_field[y*FIELD_SIZE + (x_left-1)] == HIT){
            x_left--;
            ship_size++;
            dir = 1;
        }else if(field[y*FIELD_SIZE + (x_left-1)] == EMPTY || x == 0 || x_left == 0){
            break;
        }else if(field[y*FIELD_SIZE + (x_left-1)] == SHIP)
            return 0;
    }
    while(shot_field[y*FIELD_SIZE + x_right] == HIT){        //right
        if(error_check > 100){
            fprintf(logs, "Crashed in %s. Checking was going DOWN.\n", __func__);
            MessageBoxA(NULL, "Sorry, something went wrong. Please, send me \"logs.txt\" file (it was created in the same directory with \"exe\" file). Thank you for supporting!", "Error", MB_OK | MB_ICONERROR);
            exit(EXIT_FAILURE);
        }
        error_check++;
        if(shot_field[y*FIELD_SIZE + (x_right+1)] == HIT){
            x_right++;
            ship_size++;
            dir = 1;
        }else if(field[y*FIELD_SIZE + (x_right+1)] == EMPTY || x == 9 || x_right == 9){
            break;
        }else if(field[y*FIELD_SIZE + (x_right+1)] == SHIP)
            return 0;
    }
    if(dir == 0){ //берем по y
        /*printf("DEBUG: x_left = %d, x = %d. Coming on Y\n", x_left, x);
        getchar();*/
        for(int i=0; i<ship_size; i++){
            field[(y_up + i)*FIELD_SIZE + x] = KILL;
            shot_field[(y_up + i)*FIELD_SIZE + x] = KILL;
            //MASK
            if(x == 0 && y_up != 0 && y_down != 9){
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //сверху
                shot_field[(y_up - 1)*FIELD_SIZE + x+1] = SHOT;        //справа сверху
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //снизу
                shot_field[(y_down + 1)*FIELD_SIZE + x+1] = SHOT;      //снизу справа
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //справа
            }else if(x == 9 && y_up != 0 && y_down != 9){
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //сверху
                shot_field[(y_up - 1)*FIELD_SIZE + x-1] = SHOT;        //слева сверху
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //снизу
                shot_field[(y_down + 1)*FIELD_SIZE + x-1] = SHOT;      //снизу слева
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //слева
            }else if(y_up == 0 && x != 0 && x != 9){
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //снизу
                shot_field[(y_down + 1)*FIELD_SIZE + x+1] = SHOT;      //снизу справа
                shot_field[(y_down + 1)*FIELD_SIZE + x-1] = SHOT;      //снизу слева
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //справа
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //слева
            }else if(y_down == 9 && x != 0 && x != 9){
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //сверху
                shot_field[(y_up - 1)*FIELD_SIZE + x+1] = SHOT;        //справа сверху
                shot_field[(y_up - 1)*FIELD_SIZE + x-1] = SHOT;        //слева сверху
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //справа
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //слева
            }else if(y_up == 0 && x == 0){
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //снизу
                shot_field[(y_down + 1)*FIELD_SIZE + x+1] = SHOT;      //снизу справа
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //справа
            }else if(y_up == 0 && x == 9){
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //снизу
                shot_field[(y_down + 1)*FIELD_SIZE + x-1] = SHOT;      //снизу слева
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //слева
            }else if(y_down == 9 && x == 0){
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //сверху
                shot_field[(y_up - 1)*FIELD_SIZE + x+1] = SHOT;        //справа сверху
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //справа
            }else if(y_down == 9 && x == 9){
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //сверху
                shot_field[(y_up - 1)*FIELD_SIZE + x-1] = SHOT;        //слева сверху
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //слева
            }else{
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //сверху
                shot_field[(y_up - 1)*FIELD_SIZE + x+1] = SHOT;        //справа сверху
                shot_field[(y_up - 1)*FIELD_SIZE + x-1] = SHOT;        //слева сверху
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //снизу
                shot_field[(y_down + 1)*FIELD_SIZE + x+1] = SHOT;      //снизу справа
                shot_field[(y_down + 1)*FIELD_SIZE + x-1] = SHOT;      //снизу слева
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //справа
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //слева
            }
        }
    }
    if(dir == 1){ //берем по x
        /*printf("DEBUG: y_up = %d, y = %d. Coming towards x\n", y_up, y);
        getchar();*/
        for(int i=0; i<ship_size; i++){
            field[y*FIELD_SIZE + (x_left+i)] = KILL;
            shot_field[y*FIELD_SIZE + (x_left+i)] = KILL;
            //MASK
            if(y == 0 && x_left != 0 && x_right != 9){
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //слева
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //справа
                shot_field[(y+1)*FIELD_SIZE + x_left-1] = SHOT;        //снизу слева
                shot_field[(y+1)*FIELD_SIZE + x_right+1] = SHOT;       //снизу справа
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //снизу
            }else if(y == 9 && x_left != 0 && x_right != 9){
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //слева
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //справа
                shot_field[(y-1)*FIELD_SIZE + x_left-1] = SHOT;        //сверху слева
                shot_field[(y-1)*FIELD_SIZE + x_right+1] = SHOT;       //сверху справа
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //сверху
            }else if(x_left == 0 && y != 0 && y != 9){
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //справа
                shot_field[(y+1)*FIELD_SIZE + x_right+1] = SHOT;       //снизу справа
                shot_field[(y-1)*FIELD_SIZE + x_right+1] = SHOT;       //сверху справа
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //снизу
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //сверху
            }else if(x_right == 9 && y != 0 && y != 9){
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //слева
                shot_field[(y+1)*FIELD_SIZE + x_left-1] = SHOT;        //снизу слева
                shot_field[(y-1)*FIELD_SIZE + x_left-1] = SHOT;        //сверху слева
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //снизу
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //сверху
            }else if(y == 0 && x_left == 0){
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //справа
                shot_field[(y+1)*FIELD_SIZE + x_right+1] = SHOT;       //снизу справа
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //снизу
            }else if(y == 0 && x_right == 9){
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //слева
                shot_field[(y+1)*FIELD_SIZE + x_left-1] = SHOT;        //снизу слева
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //снизу
            }else if(y == 9 && x_left == 0){
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //справа
                shot_field[(y-1)*FIELD_SIZE + x_right+1] = SHOT;       //сверху справа
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //сверху
            }else if(y == 9 && x_right == 9){
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //слева
                shot_field[(y-1)*FIELD_SIZE + x_left-1] = SHOT;        //сверху слева
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //сверху
            }else{
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //слева
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //справа
                shot_field[(y+1)*FIELD_SIZE + x_left-1] = SHOT;        //снизу слева
                shot_field[(y+1)*FIELD_SIZE + x_right+1] = SHOT;       //снизу справа
                shot_field[(y-1)*FIELD_SIZE + x_left-1] = SHOT;        //сверху слева
                shot_field[(y-1)*FIELD_SIZE + x_right+1] = SHOT;       //сверху справа
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //снизу
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //сверху
            }
        }
    }
    /*printf("DEBUG: ship_size = %d\n",ship_size);
    getchar();*/
    return ship_size;
}
//-----------------------------------------------------------------------------
void clearData(eFieldInfo *field1, eFieldInfo *field2, eFieldInfo *shot_field1, eFieldInfo *shot_field2){
    for(int i=0; i<FIELD_SIZE*FIELD_SIZE; i++){
        field1[i] = EMPTY;
        field2[i] = EMPTY;
        shot_field1[i] = EMPTY;
        shot_field2[i] = EMPTY;
    }
}
//-----------------------------------------------------------------------------
