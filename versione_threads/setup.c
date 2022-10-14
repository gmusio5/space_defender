#include "global.h"

/* Mutex inizialization */
void initMutex(){
    pthread_mutex_init(&mtx_write, NULL);
    pthread_mutex_init(&mtx_read, NULL);
}



/* Setting in-game difficulty based on user choice*/
void initDifficulty(difficulty d){
    switch (d){
        case EASY:
            M = 5;
            speedBombs = 90000;
            speedEnemies = 450000;
            speedBombsGeneration = M;
            break;
       case MEDIUM:
            M = 7;
            speedBombs = 40000;
            speedEnemies = 300000;
            speedBombsGeneration = M;
            break;
        case HARD:
            M = 9;
            speedBombs = 22000;
            speedEnemies = 150000;
            speedBombsGeneration = M;
            break;
    }
}

/* Calculating enemy position upon chosen deployment */
coordinates createEnemyCoordinates(int enemyId){
    int i; 
    coordinates pos; 
 
    /* Arrow deployment */ 
    for (i = 0; i < M; i++) { 
        if (i == 0){ 
            pos.x = maxX - (ENEMY_COLS * ((M - 1) / 2)) - SCREEN_OFFSET - 10; 
            pos.y = maxY/2; 
        } 
        else if (i % 2 == 0)
            pos.y += (ENEMY_ROWS + 1) * i; 
        
        else if(i % 2 != 0){ 
            pos.y -= (ENEMY_ROWS+1) * i; 
            pos.x += ENEMY_COLS + 1; 
        } 

        if(i == enemyId)
            return pos;
    } 

    return pos;
}

/* Generating spaceship initial coordinates */
coordinates createSpaceshipCoordinates(){
    coordinates pos;

    pos.x = SCREEN_OFFSET+1;
    pos.y = maxY / 2;

    return pos;
}



