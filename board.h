
#ifndef BOARD_H
#define BOARD_H
#include "./tc.h"
// direction enum
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;
typedef struct 
{
    int x;
    int y;
    char value;
} Point;

typedef struct
{
    int height;
    Point head;
    Point data[1000];
    char value;
    Direction old_direction;
}Snake;
typedef struct 
{
    int row, col;
    Point curr_food;
    Point data[100][100];
}Board;

Board* board_new(int row, int col);
void board_free(Board *board);
void board_print(Board *board);
void board_set(Board *board, int row, int col, int x, int y);
void snake_init(Snake *snake, int height, Point *data);
void set_snake(Board *board, Snake *snake);
void draw_loop(Board *board, Snake *snake);
void move_snake(Snake *snake, Direction direction, Board *board);
void refresh_board(Board *board, Snake *snake);
Point add_food(Board *board);
void update_food(Board *board,Snake *snake);
void update_snake(Snake *s);
int check_lose(Snake *s);
void check_win(Snake *snake);

void clear_board(Board *board, Snake *s);
void lose_screen(Snake *s){
    tc_clear_screen();
    tc_move_cursor(10,10);
    printf("You lose!\n");
    tc_move_cursor(10,12);
    printf("Press r to continue...");
}
void clear_board(Board *board, Snake *s){
    for(int i = 0; i < board->row; i++){
        for(int j = 0; j < board->col; j++){
            board->data[i][j].value = ' ';
        }
    }
    s->height =2;
    s->head.x = 1;
    s->head.y = 1;
    s->head.value = '>';
    Point data[2] = {{0,0,'>'},{1,1,'>'}};
    snake_init(s, 2, data);
    set_snake(board, s);
    board->curr_food = add_food(board);
    refresh_board(board, s);
}
void refresh_board(Board *board, Snake *s){
        int a= check_lose(s);
        if(a != 1){
            tc_clear_screen();
            tc_move_cursor(0,0);
            board_print(board);
        }else{
            lose_screen(s);
        }
}
Board * board_new(int row, int col){
    // Board *board = (Board*)malloc(sizeof(Board));
    Board *board = (Board*)calloc(1, sizeof(Board));
    board->row = row;
    board->col = col;
    // init by value by '.'
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            board->data[i][j].x = i;
            board->data[i][j].y = j;
            board->data[i][j].value = ' ';
        }
    }
    board->curr_food=add_food(board);
    return board;
}
void board_free(Board *board)
{
    free(board);
}
void menu(){
    printf("Press esc to exit\n");
    printf("Press r to restart\n");
    printf("Press p to pause\n");
}
void board_print(Board *board){
    tc_clear_screen();
    tc_move_cursor(0,0);
    int i, j;
    menu();
    printf("=");
    for(i = 0; i < board->col; i++){
        printf(" = ");
    }
    printf("=");
    printf("\n");
    for(i = 0; i < board->row; i++){
        printf("=");
        for(j = 0; j < board->col; j++){
            if(board->data[i][j].value ==' ')
                printf(" %s%c%s ",TC_B_WHT, board->data[i][j].value,TC_NRM);
            else if(board->data[i][j].value == '*') 
                printf(" %s%c%s ",TC_B_GRN, board->data[i][j].value,TC_NRM);
            else printf(" %s%c%s ",TC_RED,board->data[i][j].value,TC_NRM);
        }
        printf("=\n"); 
    }
    printf("=");
    for(i = 0; i < board->col; i++){
        printf(" = ");
    }
    printf("=\n");    
}
void board_set(Board *board, int row, int col, int x, int y){
    board->data[row][col].x = x;
    board->data[row][col].y = y;
}
void snake_init(Snake *snake, int height, Point *data){
    snake->height = height;
    for(int i = 0; i < height; i++){
        snake->data[i].x = data[i].x;
        snake->data[i].y = data[i].y;
        snake->data[i].value = data[i].value;
    }
}
void set_snake(Board *board, Snake *snake){
    // `clear board
    for(int i = 0; i < board->row; i++){
        for(int j = 0; j < board->col; j++){
            board->data[i][j].value =' ';
        }
    }
    board->data[board->curr_food.x][board->curr_food.y].value = '*';
    for(int i = 1; i < snake->height; i++){
        if (snake->data[i].x == snake->head.x && snake->data[i].y == snake->head.y)
            board->data[snake->data[i].x][snake->data[i].y].value = snake->head.value;
        else board->data[snake->data[i].x][snake->data[i].y].value = '-';
       }
}
void draw_loop(Board *board,Snake * snake){
    int c;
    Direction direction;
    while (1) {
        c = kbget();
        
        if ( c == KEY_ESCAPE ) {
            return;
        }else if ( c == 'w' ) {
            move_snake(snake, (Direction)0, board);
            update_food(board,snake);
            refresh_board(board,snake);
        } else if ( c == 'd' ) {
            move_snake(snake,(Direction) 3,board);
            update_food(board,snake);
            refresh_board(board,snake);
        } else if ( c == 'a' ) {
            move_snake(snake,(Direction) 2,board);
            update_food(board,snake);
            refresh_board(board,snake);
        } else if ( c == 's' ) {
            move_snake(snake,(Direction) 1,board);
            update_food(board,snake);
            refresh_board(board,snake);
        }else if (c == 'r') 
        {
         clear_board(board,snake);   
        }
        else
        {
            putchar(c);
        }
    }
}
void move_snake(Snake *s, Direction direction,Board *board)
{
    Point p = s->head;
    switch (direction) {
        case UP:
                p.value = '^';
                if(p.x == 0)
                    p.x = board->row - 1;
                else p.x--;
                s->old_direction = UP;
            break;
        case DOWN:
                p.value = 'v';
                if(p.x == board->row - 1)
                    p.x = 0;
                else p.x++;
                s->old_direction = DOWN;
            break;
        case LEFT:
                p.value = '<';
                if(p.y == 0) p.y = board->col - 1;
                else p.y--;
                s->old_direction = LEFT;
            break;
        case RIGHT:
                p.value = '>';
                if(p.y == board->col - 1)
                    p.y = 0;
                else p.y++;
                s->old_direction = RIGHT;
            break;
        default:
            break;
    }
    for (int i = 0; i < s->height-1; i++)
    {
        s->data[i].x = s->data[i+1].x;
        s->data[i].y = s->data[i+1].y;
        s->data[i].value = s->data[i+1].value;
    }
    s->head.x = p.x;
    s->head.y = p.y;
    s->head.value = p.value;
    // printf("{\n %d %d}",s->head.x,s->head.y);
    s->data[s->height-1].x = p.x;
    s->data[s->height-1].y = p.y;
    s->data[s->height-1].value = p.value;
    set_snake(board,s);
}
Point add_food(Board *board){
    int x,y;
    Point p;
    do{
        x = rand() % board->row;
        y = rand() % board->col;
    }while(board->data[x][y].value != ' '
         && board->data[x][y].value != '@'
         && board->data[x][y].value != '#');
         p.x = x;p.y = y;p.value = '*';
    return p;
}
void update_food(Board *board,Snake *snake){
    if(snake->data[snake->height -1].x == board->curr_food.x && snake->data[snake->height -1].y == board->curr_food.y){
        
        board->curr_food = add_food(board);
        update_snake(snake);
        refresh_board(board,snake);

    }
}
void update_snake(Snake *s){
    s->height++;
    // add to snake data array in first position
    Point tmp = s->data[0];
    for(int i = 0; i < s->height; i++){
        printf("{%d %d}",s->data[i].x,s->data[i].y);
     }
    // shift all elements to the left
    for(int i = s->height-1; i >= 0; i--){
         s->data[i] =s->data[i-1];
    }
    
    s->data[0].x = tmp.x-1;
    s->data[0].y = tmp.y;
    s->data[0].value = '#';
    printf("\n");
    for(int i = 0; i < s->height; i++){
        printf("{%d %d}",s->data[i].x,s->data[i].y);
     }
    //  exit(0);
    
}
int check_lose(Snake *s){
    for(int i = 0; i < s->height-1; i++){
        if(s->head.x == s->data[i].x && s->head.y == s->data[i].y){
            return 1;
        }
    }
    return 0;
}
#endif // BOARD_H
