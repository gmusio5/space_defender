#include "menu.h"

/* Used by the player to set the game environment and start the game */
int menu(){
    
    bool firstTime = true;

    int i, actual = 1;
    int difficultyChoice;
    
    char selector;
    char *difficulty[DIFFICULTY_LEVELS + 2] = {"          ","   EASY   ", "  MEDIUM  ", "   HARD   ", "          "};    // blank cells to avoid overlapping when resizing
    
    system ("aplay ./../fileAudio/intro.wav 2> ./../fileAudio/intro.txt &");  /* Beggining music */
    
    
    while(true){
        getmaxyx (stdscr, maxY, maxX);

        /* Minimum dimensions for the screen */
        while ((maxX < SCREEN_X) || (maxY < SCREEN_Y)){
            erase();
            mvprintw (maxY / 2, maxX * 0.1, "<------   In order to fully enjoy the game extend the window until this message dissapears!   ------>");
            refresh();
            getmaxyx (stdscr, maxY, maxX);
            firstTime = true;
        }

        erase();
        if (firstTime){
            /* Full animation */
            printLogo (SPACE_DEFENDER_OFFSET, logoSpaceDefender, FG_BLUE, LOGO_DELAY);
            printBorder(BORDER_DELAY);
        }  
        else{
            /* No animation - It doesn't overwrite the content when resizing */
            printLogo (SPACE_DEFENDER_OFFSET, logoSpaceDefender, FG_BLUE, NO_DELAY);
            printBorder(NO_DELAY);
        }

        

        firstTime = false;

        attron (A_BOLD);
        attron (COLOR_PAIR (FG_WHITE));
        mvprintw(maxY * 0.7, maxX * 0.40, "Press [SPACE] to start...");
        mvprintw(maxY * 0.94, maxX * 0.03, "Press [e] to EXIT");
        attroff (COLOR_PAIR (FG_WHITE));

        /* Difficulty choice menu */
        for (i = 0; i < DIFFICULTY_LEVELS+2; i++){
            mvprintw(maxY * 0.75 + i,  maxX * 0.45 -1, " ");    // blank space to prevent overlapping when resizing
            if (i == actual){   
                attron (COLOR_PAIR (SELECTED_OPTION));
                mvprintw(maxY * 0.75 + i,  maxX * 0.45, "%s", difficulty[i]);
                attroff (COLOR_PAIR (SELECTED_OPTION));
                difficultyChoice = i;   // chosen difficulty level
            }      
            else{
                attron (COLOR_PAIR (FG_WHITE));
                mvprintw(maxY * 0.75 + i,  maxX * 0.45, "%s", difficulty[i]);
                attroff (COLOR_PAIR (FG_WHITE));
            }
            mvprintw(maxY * 0.75 + i,  maxX * 0.45 + strlen(difficulty[0]), " ");   // blank space to prevent overlapping when resizing
        }
        attroff (A_BOLD); 
        refresh();
        
    
        switch (selector = getchar()){
            case DOWN:
                system ("aplay ./../fileAudio/button.wav 2> ./../fileAudio/button.txt &");
                switch (actual){
                    case EASY:
                    case MEDIUM:
                        actual ++;
                        break;
                    
                    case HARD:
                        actual = EASY;
                        break;
                }
                break;

            case UP:
                system ("aplay ./../fileAudio/button.wav 2> ./../fileAudio/button.txt &");
                switch (actual){
                    case EASY:
                        actual = HARD;
                        break;

                    case MEDIUM:
                    case HARD:
                        actual --;
                        break;
                }
                break;

            case EXIT:
            case SPACE:
                erase();
                getmaxyx (stdscr, maxY, maxX);
                system ("pkill aplay"); //stops the music

                if (selector == EXIT)
                    difficultyChoice = 0;
                
                return difficultyChoice;
                break;
            
        }
    }
}

/*endgame routine based on remaining lives of the player >0 WIN otherwise GAME OVER*/
void endgame(){
    bool endLoop = true;

    if (lives > 0){
        /* Victory */
        printLogo (VICTORY_OFFSET, logoVictory, FG_BLUE, LOGO_DELAY); 
        system ("aplay ./../fileAudio/victory.wav 2> ./../fileAudio/victory.txt &");
    }
    else{
        /* Game Over */
        printLogo (GAME_OVER_OFFSET, logoGameOver, FG_BLUE, LOGO_DELAY);
    }
    
    sleep(1);

    /* Wait for the user input to restart or close the game */
    attron (A_BOLD);
    mvprintw(maxY * 0.8, maxX * 0.45, "SCORE: %ld", score);
    mvprintw(maxY * 0.94, maxX * 0.03, "Press [e] to EXIT");
    attroff (A_BOLD);
    refresh();

    /* Wait for the user input to restart or close the game */
    while(endLoop){
        if(getchar() == EXIT){
                endLoop = false;
        }
    }

    return;


}
