#include "game.h"

/* PRODUCERS */
/*******************************************************************/
void *spaceshipProducer (void *args){
    int i;
    char choice;
    obj missile[MISSILES_NUM];
    pthread_t missile_tid[MISSILES_NUM];
    
    /* Spaceship specs */
    obj spaceship = *((obj*)args);
    spaceship.tid = pthread_self();
    
    /* First visualization */
    pthread_mutex_lock(&mtx_write);
    input = spaceship;
    pthread_mutex_unlock(&mtx_read);
    sleep(2);

    while (true){
        switch (choice = getchar()){
            case UP:
                if (spaceship.position.y > SCREEN_OFFSET + 1)
                    spaceship.position.y -= 1;
                break;

            case DOWN:
                if (spaceship.position.y + SPACESHIP_ROWS < maxY - SCREEN_OFFSET)
                    spaceship.position.y += 1;
                break;
            
            case SPACE:
                system ("aplay ./../fileAudio/beam.wav 2> ./../fileAudio/beam.txt &");
                
                /* Creating M missiles threads */
                for (i = 0; i < MISSILES_NUM; i++){
                    missile[i].id = i;
                    missile[i].c = MISSILE;
                    missile[i].position = spaceship.position;

                    if(pthread_create(&missile_tid[i], NULL, &missileProducer , (void*)&missile[i])){
                        endwin();
                        exit;
                    }
                }
                break; 
        }

        /*critic section*/
        pthread_mutex_lock(&mtx_write);
        input = spaceship;
        pthread_mutex_unlock(&mtx_read);
    }
}

void *missileProducer(void * args){

    /* Missile specs */
    obj missile = *((obj*) args);
    missile.position.x += SPACESHIP_COLS + 1;
    missile.tid = pthread_self();

    if (!missile.id)
        missile.position.y += SPACESHIP_ROWS / 2;
    else
        missile.position.y += (SPACESHIP_ROWS / 2) + 1;

    /* Main loop movement */
    while (true) {
        missile.position.x += MISSILE_X_MOVEMENT;
        
        if(!missile.id)
            missile.position.y -= MISSILE_Y_MOVEMENT;   //Upper missile
        else 
            missile.position.y += MISSILE_Y_MOVEMENT;   //Lower missile

        /*critic section*/
        pthread_mutex_lock(&mtx_write);
        input = missile;
        pthread_mutex_unlock(&mtx_read);
        usleep(10000);
    }
}

void *enemyProducer(void * args){
    int i;
    int maxYMovement = (maxY/2) - ((M/2) + 1) * (ENEMY_ROWS + 1) - 1;
    int stepsDone = 0;
    bool up = true;
    
    obj enemy = *((obj*) args);
    obj bomb;
    pthread_t bomb_tid;

    enemy.tid = pthread_self();
    bomb.c = BOMB;

    sleep(2);

    /* Main loop movement */
    while (true){
        enemy.position.x -= STEP;   // step to the left  

        /* Vertical steps */
        if (up){
            enemy.position.y --;
            stepsDone++;
        } else{
            enemy.position.y ++;
            stepsDone--;
        }
        
        /* Reverses the y motion if max movements are done in a direction */
        if (stepsDone == maxYMovement || stepsDone == -maxYMovement)
            up = !up;   
        
        /* Bombs generation frequency */      
        if ((i++ % speedBombsGeneration)){
            if (enemy.id == (rand() % M)){

                /* Creating bomb thread */
                bomb.position = enemy.position;
                if(pthread_create(&bomb_tid, NULL, &bombProducer, (void*)(&bomb))){
                    endwin();
                    exit;
                }
            }
        }

        /*critic section*/
        pthread_mutex_lock(&mtx_write);
        input = enemy;
        pthread_mutex_unlock(&mtx_read);
        usleep(speedEnemies);

    }
}

void *bombProducer(void *args){

    obj bomb = *((obj*) args);

    /* Setting first info */
    bomb.tid = pthread_self();
    bomb.position.x -= 1;
    bomb.position.y += ENEMY_ROWS / 2;
    
    while (true) {
        bomb.position.x -= BOMB_X_MOVEMENT;  // Horizontal movement - to the left

        /*critic section*/
        pthread_mutex_lock(&mtx_write);
        input = bomb;
        pthread_mutex_unlock(&mtx_read);
        usleep(speedBombs);
    }
}
/*******************************************************************/



/* CONSUMER */
/*******************************************************************/
void gameAreaConsumer(){ 
    int i;
    int killedEnemies = 0, enemyLives[M];
    obj spaceShip, enemy[M]; //Backup vars to keep track of the entities previous position
    
    /* Initialization of the backup var for the enemies position */
    for (i = 0; i < M; i++)
        enemyLives[i] = ENEMY_LIVES;
    
    lives = PLAYER_LIVES;
    score = 0;
    
    /* HUD inizialization effect*/
    attron (A_BOLD);
    attron (COLOR_PAIR (FG_BLUE));
    mvprintw(SCREEN_OFFSET - 1, SCREEN_OFFSET + 1, "PLAYER<~>");
    attroff (COLOR_PAIR (FG_BLUE));
    attron (COLOR_PAIR (FG_WHITE));
    mvprintw(SCREEN_OFFSET - 1, SCREEN_OFFSET + 20, "LIVES ~");   // Player lives
    mvprintw(SCREEN_OFFSET - 1, SCREEN_OFFSET + 30, "SCORE ~");   // Score achieved 
    mvprintw(SCREEN_OFFSET - 1, SCREEN_OFFSET + 60, "KILLS ~");   // Number of destroyed enemies
    attroff (COLOR_PAIR (FG_WHITE));
    attroff (A_BOLD);
    refresh();
    sleep(2);
    
    
    system ("aplay ./../fileAudio/background.wav 2> ./../fileAudio/background.txt &"); /* in-game background music */

    /* Keep running the game until the spaceship is destroyed or all the enemies are destroyed */
    while ((lives > 0) && (killedEnemies != M)){
        
        /* HUD */
        attron (A_BOLD);
        attron (COLOR_PAIR (FG_BLUE));
        mvprintw(SCREEN_OFFSET - 1, SCREEN_OFFSET + 1, "PLAYER<1>");
        attroff (COLOR_PAIR (FG_BLUE));

        attron (COLOR_PAIR (FG_WHITE));
        mvprintw(SCREEN_OFFSET - 1, SCREEN_OFFSET + 20, "LIVES %1d ", lives);   // Player lives
        mvprintw(SCREEN_OFFSET - 1, SCREEN_OFFSET + 30, "SCORE %1ld", score);   // Score achieved 
        mvprintw(SCREEN_OFFSET - 1, SCREEN_OFFSET + 60, "KILLS %1d/%d", killedEnemies, M);   // Number of destroyed enemies
        attroff (COLOR_PAIR (FG_WHITE));
        attroff (A_BOLD);

        pthread_mutex_lock(&mtx_read);

        /* Recognizes the incoming entity by the type */
        switch (input.c){   
              
            case SPACESHIP:

                // Due to the spaceship graphic it's not necessary to delete it

                spaceShip = input;
                printEntity(SPACESHIP_COLS, graphicsSpaceShip, spaceShip.position); //Printing new position
                break;
            

            case MISSILE:

                /* Deleting previous missile position */
                if (!input.id)
                    mvaddch (input.position.y + MISSILE_Y_MOVEMENT, input.position.x - MISSILE_X_MOVEMENT, ' ');    // Upper missile
                else
                    mvaddch (input.position.y - MISSILE_Y_MOVEMENT, input.position.x - MISSILE_X_MOVEMENT, ' ');    // Lower missile

                /* If the missile reaches the border */
                if ((input.position.x > maxX - SCREEN_OFFSET - 1) || (input.position.y < SCREEN_OFFSET + 1) || (input.position.y > maxY - SCREEN_OFFSET))
                    pthread_cancel(input.tid);
                else{
                    printChar(input.position, input.c, FG_YELLOW);

                    /* Checking for collisions with every alive enemy */
                    for (i = 0; i < M; i++){
                        if (checkCollision(input.position, enemy[i].position, ENEMY_ROWS, ENEMY_COLS) && enemyLives[enemy[i].id] > 0){
                            mvaddch (input.position.y, input.position.x, ' ');       
                            system ("aplay ./../fileAudio/explosion.wav 2> ./../fileAudio/explosion.txt &");         
                            pthread_cancel(input.tid);  //kills missile process 

                            deleteEntity(ENEMY_ROWS, ENEMY_COLS, enemy[i].position);  //Deletes the enemy
                            enemyLives[enemy[i].id]--;

                            switch (enemyLives[enemy[i].id]){
                                
                                case 0:
                                    deleteEntity(ENEMY_ROWS, ENEMY_COLS, enemy[i].position);  //Deletes the enemy
                                    enemy[i].position.x = -1;
                                    pthread_cancel(enemy[i].tid);  //kills a level2 enemy
                                    killedEnemies++;
                                    score += ADVANCED_POINT;
                                    break;
                                case 1:
                                    score += INTERMEDIATE_POINT;
                                    break;
                                case 2:
                                    score += BASIC_POINT;
                                    break;

                            }
                                                   
                        }
        
                    }
                }
                
                break;
                    

            case BOMB:
                mvaddch (input.position.y, input.position.x + BOMB_X_MOVEMENT, ' '); //Deleting previous printed bomb on the screen

                /* If the missile reaches the border */
                if (input.position.x < SCREEN_OFFSET + 1)
                    pthread_cancel(input.tid);
                else{
                    /* Checking for collisions with the spaceship */
                    if (checkCollision(input.position, spaceShip.position, SPACESHIP_ROWS-1, SPACESHIP_COLS)){
                        lives--;
                        
                        /* spaceship destruction effects */
                        if (lives == 2)
                            init_pair (BG_DAMAGE_1, -1, -1);
                        if (lives == 1)
                            init_pair (BG_DAMAGE_2, -1, -1);

                        system ("aplay ./../fileAudio/spaceShip_impact.wav 2> ./../fileAudio/spaceShip_impact.txt &");
                        pthread_cancel(input.tid);
                    }
                    else
                        printChar(input.position, input.c, FG_WHITE);  //printing bomb in the new position
                }

                break;

            
            case ENEMY:
                deleteEntity(ENEMY_ROWS, ENEMY_COLS, enemy[input.id].position); //Deleting previous printed enemy on the screen

                /* If the missile reaches the border */
                if (input.position.x < SCREEN_OFFSET + 1)
                    lives = 0;
                else{
                    enemy[input.id] = input;  // new coordinates

                    /* Different enemy graphic depending on their lives */
                    switch (enemyLives[input.id]){
                        case 3:
                            printEntity(ENEMY_COLS, graphicsEnemyLvl1, input.position);
                            break;

                        case 2:
                        case 1:
                            printEntity(ENEMY_COLS, graphicsEnemyLvl2, input.position);
                            break;
                    }
                }
                break;
        }

        pthread_mutex_unlock(&mtx_write);
        refresh();
    }
    
    /* Make sure to kill all the remaining entities */
    for (i = 0; i < M; i++)
        pthread_cancel(enemy[i].tid);
    pthread_cancel(spaceShip.tid);

    system ("pkill aplay"); //Stops the background music

    if (lives == 0){
        /* Game over */
        printEntity(SPACESHIP_COLS, graphicsSpaceShipDestroyed, spaceShip.position);
        system ("aplay ./../fileAudio/spaceShip_impact.wav 2> ./../fileAudio/spaceShip_impact.txt &");
        sleep(1);
        system ("aplay ./../fileAudio/gameOver.wav 2> ./../fileAudio/gameOver.txt &");
    }

    sleep(1);

}
/*******************************************************************/





