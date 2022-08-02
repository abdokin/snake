#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tc.h"
#include "./board.h"
#include <pthread.h>
void *PrintHello(void *threadid) {
   printf("Hello World! Thread ID, \n");
   while (1)
   {
        printf("Hello World! Thread ID, \n");
        sleep(1);
   }
   
}
struct arg_struct {
    Snake *arg1;
    Board *arg2;
};
void* make_snake_move(void *arg){
    struct arg_struct *args = (struct arg_struct *)arg;
    Snake *s = args->arg1;
    Board *board = args->arg2;
    
    while (1)
    {
        
        // sleep(1);
        // sleep 100ms
        usleep(100000);
        move_snake(s,s->old_direction,board);
        printf("%d\n",s->old_direction);
        // sleep(1);
        // exit(0);
        refresh_board(board,s);
    }
    pthread_exit(NULL);
    
}

int main(int argc, char *argv[]){
    pthread_t thread_id;
    struct arg_struct args;
    Board *board = board_new(10,10);
    srand(time(NULL));
    Snake s;
    s.height =4;
    s.old_direction = RIGHT;
    s.head = (Point){1,2,'>'};
    Point data[4] = {{0,0,'-'},{0,1,'-'},{1,1,'-'},{1,2,'>'}};
    snake_init(&s, s.height, data);
    set_snake(board, &s);
    args.arg1 = &s;
    args.arg2 = board;
    board_print(board);
    pthread_create(&thread_id, NULL, make_snake_move, &args);
    draw_loop(board, &s);

    return 0;

}