#include "global.h"


/* Init screen colors used for graphics */
void initColors(){
    start_color();
    use_default_colors();
    assume_default_colors( -1, -1);

    /* Background colors */
    init_pair (COLOR_EMPTY, -1, -1);
    init_pair (BG_RED, -1, COLOR_RED);
    init_pair (BG_WHITE, -1, COLOR_WHITE);
    init_pair (BG_YELLOW, -1, COLOR_YELLOW);
    init_pair (BG_GREEN, -1, COLOR_GREEN);
    init_pair (BG_BLUE, -1, COLOR_BLUE);

    /* Background colors used for graphic damages */
    init_pair (BG_DAMAGE_1, -1, COLOR_WHITE);
    init_pair (BG_DAMAGE_2, -1, COLOR_RED);
    init_pair (BG_DAMAGE_3, -1, COLOR_RED);

    /* Selection menu */
    init_pair (SELECTED_OPTION, COLOR_WHITE, COLOR_BLUE);

    /* Foreground colors */
    init_pair (FG_YELLOW, COLOR_YELLOW, -1);
    init_pair (FG_RED, COLOR_RED, -1);
    init_pair (FG_GREEN, COLOR_GREEN, -1);
    init_pair (FG_WHITE, COLOR_WHITE, -1);
    init_pair (FG_BLUE, COLOR_BLUE, -1);
    
}

/* Init video functionalities */
void initScreen(){
    system("resize -s 45 165");
    initscr();  //inizializza schermo di output
    noecho();
    curs_set(0);   //nasconde cursore    
    keypad(stdscr, 1);   //imposta modalita tastiera
}



/* Prints a logo and uses a delay to create a small animation*/
/******************************************************************************/
void printLogo (int cols, char logo[][cols + 1], int color, int delay){
    int x, y;
    int i, j;
    int counter = 0;
    int rows = LOGO_ROWS;

    erase();

    /* Background print  - row by row */
    for (i = 0; i < rows; i++){
        attron(A_BOLD);
        attron (COLOR_PAIR (color));
        mvprintw ((maxY * 0.3) + i, (maxX - (cols-1)) / 2, "%s\n", logo[i]);
        attroff (COLOR_PAIR (color));
        attroff(A_BOLD);

        y = (maxY * 0.3) + i - 1; 
        j = 0;

        /* Foreground print - cell by cell */
        while (j < (cols)){
            if (logo[i][j] == '|'){
                while (((logo[i][j+1] == ' ') || (logo[i][j+1] == '_')) && ((counter % 2 == 0))){
                    x = ((maxX - (cols)) / 2) + j;

                    attron (COLOR_PAIR (BG_WHITE));
                    move (y, x + 2);
                    printw (" ");                           
                    attroff (COLOR_PAIR (BG_WHITE));
                    

                    if (delay > 0){
                        usleep (delay);
                        refresh(); 
                    }
                    j++;
                    
                }
                counter++;
            }
            j++;
        }
    }
}
/******************************************************************************/


/* Prints and deletes entities starting with the char matrix of entities  */
/******************************************************************************/
void printEntity(int cols, int matrix[][cols], coordinates entityPosition){
    int i, j;
    int rows;
    
    rows = cols;    //since all the entities are square shaped
    
    for (i = 0; i < rows; i++){
        for (j = 0; j < cols; j++){
            move (entityPosition.y, entityPosition.x);
            attron (COLOR_PAIR (matrix[i][j]));
            printw (" ");
            attroff (COLOR_PAIR (matrix[i][j]));
            entityPosition.x++;
        }
        entityPosition.y++;
        entityPosition.x -= cols;
    }
    refresh();
}

void deleteEntity(int rows, int cols, coordinates entityPosition){
    int i, j;

    for (i = 0; i < rows; i++){
        for (j = 0; j < cols; j++){
            move (entityPosition.y, entityPosition.x);
            attron (COLOR_PAIR (COLOR_EMPTY));
            printw (" ");
            attroff (COLOR_PAIR (COLOR_EMPTY));
            entityPosition.x++;
        }
        entityPosition.y++;
        entityPosition.x -= cols;
    }
    refresh();
}
/******************************************************************************/




/* Prints an individual character, used for MISSILES and BOMS */
void printChar(coordinates pos, char entityType, int color){
    attron (A_BOLD);
    attron (COLOR_PAIR (color));
                
    mvaddch (pos.y, pos.x, entityType);

    attroff (COLOR_PAIR (color));
    attroff (A_BOLD);

}

/* Prints a visual border for the player to play into */
void printBorder(int delay){
    int i = 0;

    attron (COLOR_PAIR (FG_BLUE));
    attron(A_BOLD);

    /* Horizontal lines */
    for (i = SCREEN_OFFSET; i < maxX - SCREEN_OFFSET; i++){
        mvaddch (SCREEN_OFFSET, i+1, '_');
        mvaddch (maxY - SCREEN_OFFSET + 1, i, '_'); 
        if (delay > NO_DELAY){
            usleep(delay);
            refresh();
        }
            
        
    }

    mvaddch (SCREEN_OFFSET, i, ' ');

    /* Vertical lines */
    for (i = SCREEN_OFFSET; i < maxY - SCREEN_OFFSET + 1; i++){
        mvaddch (i+1, SCREEN_OFFSET, '|');
        mvaddch (i+1, maxX - SCREEN_OFFSET, '|');
       if (delay > NO_DELAY){
            usleep(delay);
            refresh();
       }
    }
    attroff(A_BOLD);
    attroff (COLOR_PAIR (FG_BLUE));
    
    if (delay > NO_DELAY)
        sleep(1);

}




/* Detects overlapping entities */
bool checkCollision(coordinates firstEntityPosition, coordinates secondEntityPosition, int rows, int cols){

    return (firstEntityPosition.x > secondEntityPosition.x && firstEntityPosition.x < secondEntityPosition.x + cols) && 
        (firstEntityPosition.y > secondEntityPosition.y && firstEntityPosition.y < secondEntityPosition.y + rows);
}