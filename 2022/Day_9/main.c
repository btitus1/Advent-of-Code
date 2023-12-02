#include <stdio.h>
#include <stdlib.h>

#define MAX_MOVES 2100
#define BOARD_SIZE_Y 5000
#define BOARD_SIZE_X 5000
#define OFFSET_X ((BOARD_SIZE_X)/2)
#define OFFSET_Y ((BOARD_SIZE_Y)/2)

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    DIAG_UP_LEFT,
    DIAG_UP_RIGHT,
    DIAG_D_LEFT,
    DIAG_D_RIGHT,
} Direction;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Direction dir;
    int amount;
} Move;

int points_touch(Point *head,Point *tail);
void process_moves(Move *moves, int len);
void move_direction(Move *move, Point *point);
Move* find_move_dir(Point *head, Point *tail);
int sum_board(const int *input_board);

Point *head;
Point *tail;
Point *points[10];
int board[BOARD_SIZE_Y][BOARD_SIZE_X];
int board2[BOARD_SIZE_Y][BOARD_SIZE_X];

int main(void) {

    FILE *fp;
    char c;
    int amt;
    int len = 0;
    fp = fopen("datafile.txt","r");

    head = malloc(sizeof(Point));
    tail = malloc(sizeof(Point));
    points[0] = head;
    points[1] = tail;

    // Part 2
    for(int i = 2; i < 10;i++) {
        Point *pt = malloc(sizeof(Point));
        points[i] = pt;
        points[i]->x = 0;
        points[i]->y = 0;
    }

    Move move_seq[MAX_MOVES];

    while(fscanf(fp,"%c %d\n",&c, &amt) != EOF) {
        move_seq[len].amount = amt;
        switch (c)
        {
            case 'U':
                move_seq[len].dir = UP;
                break;
            case 'D':
                move_seq[len].dir = DOWN;
                break;
            case 'L':
                move_seq[len].dir = LEFT;
                break;
            case 'R':
                move_seq[len].dir = RIGHT;
            default:
                break;
        }
        len++;
    }

    fclose(fp);

    head->x = 0;
    head->y = 0;
    tail->x = 0;
    tail->y = 0;
    board[OFFSET_Y][OFFSET_X] = 1;
    board2[OFFSET_Y][OFFSET_X] = 1;

    process_moves(move_seq,len);
   //  print_board();
    int total_moves = sum_board((int*)board);

    printf("The tail visited %d unique spaces in part 1\n",total_moves);

    total_moves = sum_board((int*)board2);

    printf("The tail visited %d unique spaces in part 2\n",total_moves);

    return 0;
}

int points_touch(Point *h,Point *t) {
    return abs(h->x - t->x) <= 1 && abs(h->y - t->y) <= 1;
}

void process_moves(Move *moves, int len) {
    Move *move;
    for(int i = 0; i<len;i++) {
        for(int j=0;j<moves[i].amount;j++) {
            move_direction(&moves[i],head);
            if(!points_touch(head,tail)) {
                move = find_move_dir(head,tail);
                move_direction(move,tail);
                free(move);
                board[tail->y+OFFSET_Y][tail->x+OFFSET_X] = 1;
            }

            for(int k=1;k<9;k++) {
                if(!points_touch(points[k],points[k+1])) {
                    move = find_move_dir(points[k],points[k+1]);
                    move_direction(move,points[k+1]);
                    free(move);
                    if(k==8) {
                        board2[points[k+1]->y+OFFSET_Y][points[k+1]->x+OFFSET_X] = 1;
                    }

                }
            }

        }
    }

}

void move_direction(Move *move, Point *point) {
    switch (move->dir)
    {
        case UP:
            point->y -= 1;
            break;
        case DOWN:
            point->y += 1;
            break;
        case LEFT:
            point->x -= 1;
            break;
        case RIGHT:
            point->x += 1;
            break;
        case DIAG_D_LEFT:
            point->x -= 1;
            point->y += 1;
            break;
        case DIAG_D_RIGHT:
            point->x += 1;
            point->y += 1;
            break;
        case DIAG_UP_LEFT:
            point->x -= 1;
            point->y -= 1;
            break;
        case DIAG_UP_RIGHT:
            point->x += 1;
            point->y -= 1;
            break;
        default:
            break;
    }
}

Move* find_move_dir(Point *h, Point *t) {

    Move *new_move = (Move*)malloc(sizeof(Move));
    new_move->amount = 1;

    if(head->x == t->x) {
        // Non Diag move
        new_move->dir = h->y > t->y ? DOWN : UP;

    } else if (h->y == t->y) {
        // Non Diag movement
        new_move->dir = h->x > t->x ? RIGHT : LEFT;

    } else if(h->x > t->x && h->y > t->y) {
        // Diag movement
        new_move->dir = DIAG_D_RIGHT;
    } else if(h->x <t->x && h->y > t->y) {
        new_move->dir = DIAG_D_LEFT;
    } else if(h->x > t->x && h->y < t->y) {
        new_move->dir = DIAG_UP_RIGHT;
    } else {
        new_move->dir = DIAG_UP_LEFT;
    }

    return new_move;
}

int sum_board(const int *input_board) {
    int total_moves = 0;
    for(int i=0;i<BOARD_SIZE_Y;i++) {
        for(int j=0;j<BOARD_SIZE_X;j++) {
            total_moves += *((input_board+i*BOARD_SIZE_Y) +j);
        }
    }

    return total_moves;

}