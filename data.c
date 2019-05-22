#include "seabattle.h"

extern FILE *logs;
extern int player;
extern int player1_ship_count;
extern int player2_ship_count;

//-----------------------------------------------------------------------------
/*----------------------------------------------------------------------[<]-
 Function: ship_generate_Player(eFieldInfo*, int*)
 Synopsis: Function for generating field by player.
 Returns: void
 -----------------------------------------------------------------------[>]-*/
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
            //place check
            switch(dir)
            {
            //x-direction
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
            //y-direction
            case 1:
                for(int i=0; i<ship_size; i++){
                    if(ap_data[(y+i)*FIELD_SIZE + x]               == SHIP                                                 ||
                       (ap_data[(y+i)*FIELD_SIZE + (x+1)]          == SHIP && x!=FIELD_SIZE-1)                             ||
                       (ap_data[(y+i)*FIELD_SIZE + (x-1)]          == SHIP && x!=0)                                        ||
                       (ap_data[(y+ship_size)*FIELD_SIZE + x]      == SHIP && y!=FIELD_SIZE-ship_size)                     ||
                       (ap_data[(y+ship_size)*FIELD_SIZE + (x+1)]  == SHIP && x!=FIELD_SIZE-1 && y!=FIELD_SIZE-ship_size)  ||
                       (ap_data[(y+ship_size)*FIELD_SIZE + (x-1)]  == SHIP && x!=0 && y!=FIELD_SIZE-ship_size)             ||
                       (ap_data[(y-1)*FIELD_SIZE + x]              == SHIP && y!=0)                                        ||
                       (ap_data[(y-1)*FIELD_SIZE + (x+1)]          == SHIP && x!=FIELD_SIZE-1 && y!=0)                     ||
                       (ap_data[(y-1)*FIELD_SIZE + (x-1)]          == SHIP && x!=0 && y!=0)){
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
/*----------------------------------------------------------------------[<]-
 Function: ship_generateAI(eFieldInfo*, int*)
 Synopsis: Function for generating field by algorithm.
 Returns: void
 -----------------------------------------------------------------------[>]-*/
void ship_generateAI(eFieldInfo *ap_data, int *player_ship_count){
    srand(time(NULL));
    int ship_size = 4;
    int ship_count = 0;
    int x, y;
    int dir; //direction for growing
    bool isSet = false;
    while(!isSet){
        x = rand()%FIELD_SIZE;
        y = rand()%FIELD_SIZE;

        int temp_x = x;
        int temp_y = y;

        bool setting_possible = true;

        //dir generation
        dir = rand()%4;

        //checking setting_possible
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

        //setting ship if possible
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

            //checking ships count
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
/*----------------------------------------------------------------------[<]-
 Function: get_target_pos(unsigned short *, int, int)
 Synopsis: Function for moving players crosshair.
 Returns: 0 if moved chair, 1 if pressed shot key,
          2 if pressed destination key
 -----------------------------------------------------------------------[>]-*/
int get_target_pos(unsigned short *pp_global, int max_dest_x, int max_dest_y){

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
/*----------------------------------------------------------------------[<]-
 Function: shot_analyze(unsigned short, eFieldInfo*, eFieldInfo*)
 Synopsis: Function for analyzing player's 'shot'.
 Returns: 1 if turn not finished, 0 if finished
 -----------------------------------------------------------------------[>]-*/
int shot_analyze(unsigned short position_global, eFieldInfo *field, eFieldInfo *shot_field){
    unsigned char position_x = position_global;
    unsigned char position_y = position_global >> 8;
    int ship_size;
    //checking was that place already shoted
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
            printf("%s killed %d-deck ship!\n", (player == PLAYER_1 ? "Player 1" : "Player 2"), ship_size);
            Sleep(1000);
            return 1;
        }
        printf("%s hit!\n", (player == PLAYER_1 ? "Player 1" : "Player 2"));
        Sleep(1000);
        return 1;
    }
    printf("%s miss!\n", (player == PLAYER_1 ? "Player 1" : "Player 2"));
    shot_field[position_y*FIELD_SIZE + position_x] = SHOT;
    Sleep(1000);
    return 0;
}
//-----------------------------------------------------------------------------
/*----------------------------------------------------------------------[<]-
 Function: kill_check(unsigned short, eFieldInfo*, eFieldInfo*)
 Synopsis: Function for checking full 'kill' of ship.
 Returns: 0 if player hit, ship_size if 'kill'
 -----------------------------------------------------------------------[>]-*/
int kill_check(unsigned short position_global, eFieldInfo *field, eFieldInfo *shot_field){
    unsigned char y = position_global >> 8;
    unsigned char y_up = y;
    unsigned char y_down = y;
    unsigned char x = position_global;
    unsigned char x_right = x;
    unsigned char x_left = x;

    int dir = 0;
    int ship_size = 1;

    while(shot_field[y_up*FIELD_SIZE + x] == HIT){        //up
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
        if(x_left != 0 && shot_field[y*FIELD_SIZE + (x_left-1)] == HIT){
            x_left--;
            ship_size++;
            dir = 1;
        }else if(field[y*FIELD_SIZE + (x_left-1)] == EMPTY || x == 0 || x_left == 0){
            break;
        }else if(field[y*FIELD_SIZE + (x_left-1)] == SHIP)
            return 0;
    }
    while(shot_field[y*FIELD_SIZE + x_right] == HIT){        //right
        if(shot_field[y*FIELD_SIZE + (x_right+1)] == HIT){
            x_right++;
            ship_size++;
            dir = 1;
        }else if(field[y*FIELD_SIZE + (x_right+1)] == EMPTY || x == 9 || x_right == 9){
            break;
        }else if(field[y*FIELD_SIZE + (x_right+1)] == SHIP)
            return 0;
    }
    //setting field mask
    if(dir == 0){ //taking on y
        for(int i=0; i<ship_size; i++){
            field[(y_up + i)*FIELD_SIZE + x] = KILL;
            shot_field[(y_up + i)*FIELD_SIZE + x] = KILL;
            //MASK
            if(x == 0 && y_up != 0 && y_down != 9){
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //up
                shot_field[(y_up - 1)*FIELD_SIZE + x+1] = SHOT;        //right up
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //down
                shot_field[(y_down + 1)*FIELD_SIZE + x+1] = SHOT;      //right down
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //right
            }else if(x == 9 && y_up != 0 && y_down != 9){
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //up
                shot_field[(y_up - 1)*FIELD_SIZE + x-1] = SHOT;        //left up
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //down
                shot_field[(y_down + 1)*FIELD_SIZE + x-1] = SHOT;      //left down
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //left
            }else if(y_up == 0 && x != 0 && x != 9){
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //left
                shot_field[(y_down + 1)*FIELD_SIZE + x+1] = SHOT;      //right down
                shot_field[(y_down + 1)*FIELD_SIZE + x-1] = SHOT;      //left down
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //right
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //left
            }else if(y_down == 9 && x != 0 && x != 9){
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //up
                shot_field[(y_up - 1)*FIELD_SIZE + x+1] = SHOT;        //right up
                shot_field[(y_up - 1)*FIELD_SIZE + x-1] = SHOT;        //left up
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //right
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //left
            }else if(y_up == 0 && x == 0){
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //down
                shot_field[(y_down + 1)*FIELD_SIZE + x+1] = SHOT;      //right down
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //right
            }else if(y_up == 0 && x == 9){
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //down
                shot_field[(y_down + 1)*FIELD_SIZE + x-1] = SHOT;      //left down
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //left
            }else if(y_down == 9 && x == 0){
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //up
                shot_field[(y_up - 1)*FIELD_SIZE + x+1] = SHOT;        //right up
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //right
            }else if(y_down == 9 && x == 9){
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //up
                shot_field[(y_up - 1)*FIELD_SIZE + x-1] = SHOT;        //left up
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //left
            }else{
                shot_field[(y_up - 1)*FIELD_SIZE + x] = SHOT;          //up
                shot_field[(y_up - 1)*FIELD_SIZE + x+1] = SHOT;        //right up
                shot_field[(y_up - 1)*FIELD_SIZE + x-1] = SHOT;        //left up
                shot_field[(y_down + 1)*FIELD_SIZE + x] = SHOT;        //down
                shot_field[(y_down + 1)*FIELD_SIZE + x+1] = SHOT;      //right down
                shot_field[(y_down + 1)*FIELD_SIZE + x-1] = SHOT;      //left down
                shot_field[(y_up + i)*FIELD_SIZE + x+1] = SHOT;        //right
                shot_field[(y_up + i)*FIELD_SIZE + x-1] = SHOT;        //left
            }
        }
    }
    if(dir == 1){ //taking on x
        for(int i=0; i<ship_size; i++){
            field[y*FIELD_SIZE + (x_left+i)] = KILL;
            shot_field[y*FIELD_SIZE + (x_left+i)] = KILL;
            //MASK
            if(y == 0 && x_left != 0 && x_right != 9){
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //left
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //right
                shot_field[(y+1)*FIELD_SIZE + x_left-1] = SHOT;        //left down
                shot_field[(y+1)*FIELD_SIZE + x_right+1] = SHOT;       //right down
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //down
            }else if(y == 9 && x_left != 0 && x_right != 9){
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //down
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //right
                shot_field[(y-1)*FIELD_SIZE + x_left-1] = SHOT;        //left up
                shot_field[(y-1)*FIELD_SIZE + x_right+1] = SHOT;       //right up
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //up
            }else if(x_left == 0 && y != 0 && y != 9){
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //right
                shot_field[(y+1)*FIELD_SIZE + x_right+1] = SHOT;       //right down
                shot_field[(y-1)*FIELD_SIZE + x_right+1] = SHOT;       //right up
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //down
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //up
            }else if(x_right == 9 && y != 0 && y != 9){
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //left
                shot_field[(y+1)*FIELD_SIZE + x_left-1] = SHOT;        //left down
                shot_field[(y-1)*FIELD_SIZE + x_left-1] = SHOT;        //left up
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //down
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //up
            }else if(y == 0 && x_left == 0){
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //right
                shot_field[(y+1)*FIELD_SIZE + x_right+1] = SHOT;       //right down
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //down
            }else if(y == 0 && x_right == 9){
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //left
                shot_field[(y+1)*FIELD_SIZE + x_left-1] = SHOT;        //left down
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //left
            }else if(y == 9 && x_left == 0){
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //right
                shot_field[(y-1)*FIELD_SIZE + x_right+1] = SHOT;       //right up
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //up
            }else if(y == 9 && x_right == 9){
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //left
                shot_field[(y-1)*FIELD_SIZE + x_left-1] = SHOT;        //left up
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //up
            }else{
                shot_field[y*FIELD_SIZE + x_left-1] = SHOT;            //left
                shot_field[y*FIELD_SIZE + x_right+1] = SHOT;           //right
                shot_field[(y+1)*FIELD_SIZE + x_left-1] = SHOT;        //left down
                shot_field[(y+1)*FIELD_SIZE + x_right+1] = SHOT;       //right down
                shot_field[(y-1)*FIELD_SIZE + x_left-1] = SHOT;        //left up
                shot_field[(y-1)*FIELD_SIZE + x_right+1] = SHOT;       //right up
                shot_field[(y+1)*FIELD_SIZE + x_left + i] = SHOT;      //down
                shot_field[(y-1)*FIELD_SIZE + x_left + i] = SHOT;      //up
            }
        }
    }
    return ship_size;
}
//-----------------------------------------------------------------------------
/*----------------------------------------------------------------------[<]-
 Function: clearData(eFieldInfo*, eFieldInfo*, eFieldInfo*, eFieldInfo*)
 Synopsis: Function for clearing all player's arrays.
 Returns: void
 -----------------------------------------------------------------------[>]-*/
void clearData(eFieldInfo *field1, eFieldInfo *field2, eFieldInfo *shot_field1, eFieldInfo *shot_field2){
    for(int i=0; i<FIELD_SIZE*FIELD_SIZE; i++){
        field1[i] = EMPTY;
        field2[i] = EMPTY;
        shot_field1[i] = EMPTY;
        shot_field2[i] = EMPTY;
    }
}
//-----------------------------------------------------------------------------
