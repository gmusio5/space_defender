#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <curses.h>
#include <stdbool.h>
#include <string.h>


/* General setup */
#define STEP 1  //single movement
#define BORDER_DELAY 5000
#define DIFFICULTY_LEVELS 3

/* KEYBOARD INTERACTION */
#define UP 65
#define DOWN 66
#define SPACE 32
#define ENTER 13
#define CONTINUE 'c'
#define EXIT 'e'

/* PIPE INTERACTION */
#define READ 0
#define WRITE 1

/* FORK RESULT */
#define FAILURE -1
#define SUCCESS 0


/* SPACESHIP */
#define SPACESHIP '#'
#define SPACESHIP_ROWS 6
#define SPACESHIP_COLS 6
#define PLAYER_LIVES 3


/* MISSILE */
#define MISSILE 'o'
#define MISSILES_NUM 2
#define MISSILE_X_MOVEMENT 2
#define MISSILE_Y_MOVEMENT 0.2

/* ENEMY */
#define ENEMY '*'
#define ENEMY_LIVES 3
#define ENEMY_ROWS 3
#define ENEMY_COLS 3

/* BOMB */
#define BOMB '<'
#define BOMB_X_MOVEMENT 2

/* POINTS */
#define BASIC_POINT 1000
#define INTERMEDIATE_POINT 2500
#define ADVANCED_POINT 4700

/* SCREEN PROPERTIES */
#define SCREEN_Y 45
#define SCREEN_X 165
#define SCREEN_OFFSET 3


/* COLORS DEFINITION */
/************************/
#define COLOR_EMPTY 0
#define BG_RED 1
#define BG_WHITE 2
#define BG_YELLOW 3
#define BG_GREEN 4
#define BG_BLUE 5

#define FG_YELLOW 6
#define FG_RED 7
#define FG_GREEN 8
#define FG_WHITE 9
#define FG_BLUE 10

#define BG_DAMAGE_1 11
#define BG_DAMAGE_2 12
#define BG_DAMAGE_3 13

#define SELECTED_OPTION 14
/************************/




/* LOGOS PROPERTIES */
/********************************************************************************/
#define LOGO_ROWS 7
#define LOGO_DIM_LETTER 9
#define LOGO_DELAY 4000
#define NO_DELAY 0

#define NUM_LETTERS_SPACE_DEFENDER 14
#define SPACE_DEFENDER_OFFSET (LOGO_DIM_LETTER * NUM_LETTERS_SPACE_DEFENDER)

#define NUM_LETTERS_GAME_OVER 9
#define GAME_OVER_OFFSET (LOGO_DIM_LETTER * NUM_LETTERS_GAME_OVER)

#define NUM_LETTERS_VICTORY 7
#define VICTORY_OFFSET (LOGO_DIM_LETTER * NUM_LETTERS_VICTORY)
/********************************************************************************/

typedef struct Coordinates{
    float x;
    float y;
}coordinates;

typedef struct Obj{
    char c;
    int id;
    __pid_t pid;
    coordinates position;
}obj;

typedef enum Difficulty{
    EASY = 1, MEDIUM = 2, HARD = 3
} difficulty;


difficulty diff;

int M; //enemy number

int filedes[2];

int maxX, maxY;


long score;
int lives;


long speedEnemies;
long speedBombs;
long speedBombsGeneration;



/*GAME.C -> all producers and consumer (gamearea()) */
/*******************************************************************/

//PRODUCERS

void spaceshipProducer (int pipeout, obj spaceship);

void missileProducer(int pipeout, obj missile);

void enemyProducer(int pipeout, obj enemy);

void bombProducer(int pipeout, obj bomb);

//CONSUMER

void gameAreaConsumer(int pipein);
/*******************************************************************/



/*SETUP.C -> all support funtions: screen inizializating, initial coordinates generation... */
/*******************************************************************/
void initPipe();

void initDifficulty(difficulty);

coordinates createEnemyCoordinates(int enemyId);

coordinates createSpaceshipCoordinates();
/*******************************************************************/




/* MENU.C: intro menu & end menu */
/*******************************************************************/
int menu();

void endgame();
/*******************************************************************/



/* GRAPHICS.C: printing entity functions and graphics collision control */
/****************************************************************************/
void initScreen();

void initColors();

void printEntity(int cols, int matrix[][cols], coordinates );

void deleteEntity(int rows, int cols, coordinates );

void printChar(coordinates pos, char entityType, int color);

void printLogo (int cols, char logo[][cols], int , int);

void printBorder(int delay);

bool checkCollision(coordinates , coordinates , int , int );
/*******************************************************************/