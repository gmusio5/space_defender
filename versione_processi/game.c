#include "game.h"

/* PRODUCERS */
/*******************************************************************/
void spaceshipProducer (int pipeout, obj spaceship){
    int i;
    char choice;
    obj missile[MISSILES_NUM];
    __pid_t missile_pid[MISSILES_NUM];

    /* Spaceship specs */
    spaceship.pid = getpid();
    
    /* First visualization */
    write (pipeout, &spaceship, sizeof(obj)); //writing on pipe first position
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
                
                /* Creating missile processes */
                for (i = 0; i < MISSILES_NUM; i++){
                    switch (missile_pid[i] = fork()){
                        case FAILURE:
                            perror("An error occured during fork()");
                            _exit(1);
                            break;

                        case SUCCESS:
                            missile[i].id = i;
                            missile[i].c = MISSILE;
                            missile[i].position = spaceship.position;
                            missileProducer(pipeout, missile[i]);
                            exit(0);
                            break;       
                    }    

                }
                break; 
            
        }
        write (pipeout, &spaceship, sizeof (spaceship));    ////writing on pipe new position
    }
}

void missileProducer(int pipeout, obj missile){

    /* Missile specs */
    missile.position.x += SPACESHIP_COLS + 1;
    missile.pid = getpid();

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

        write (pipeout, &missile, sizeof(missile));
        usleep(10000);
    }
}

void enemyProducer(int pipeout, obj enemy){
    int i;
    int maxYMovement = (maxY/2) - ((M/2) + 1) * (ENEMY_ROWS + 1) - 1;
    int stepsDone = 0;
    bool up = true;

    obj bomb;
   __pid_t bomb_pid;

    enemy.pid = getpid();
    bomb.c = BOMB;

    /* First visualization */
    write (pipeout, &enemy, sizeof(obj));
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

                /* Creating spaceship process */
                switch (bomb_pid = fork()){
                    case FAILURE:
                        perror("An error occured during fork()");
                        exit(1);
                        break;

                    case SUCCESS:
                        bomb.position = enemy.position;
                        bombProducer(pipeout, bomb);
                        exit(0);
                        break;       
                }
            } 
        }

        write (pipeout, &enemy, sizeof(obj));
        usleep(speedEnemies);

    }
}

void bombProducer(int pipeout, obj bomb){

    /* Setting first info */
    bomb.pid = getpid();
    bomb.position.x -= 1;
    bomb.position.y += ENEMY_ROWS / 2;
    
    while (true) {
        bomb.position.x -= BOMB_X_MOVEMENT;  // Horizontal movement - to the left

        write (pipeout, &bomb, sizeof(bomb));
        usleep(speedBombs);
    }
}
/*******************************************************************/



/* CONSUMER */
/*******************************************************************/
void gameAreaConsumer(int pipein){ 
    int i;
    int killedEnemies = 0, enemyLives[M];
    obj input, spaceShip, enemy[M]; //Backup vars to keep track of the entities previous position
    
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

        read (pipein, &input, sizeof(obj)); // Reading from the pipe

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
                    kill(input.pid, 1);
                else{
                    printChar(input.position, input.c, FG_YELLOW);

                    /* Checking for collisions with every alive enemy */
                    for (i = 0; i < M; i++){
                        if (checkCollision(input.position, enemy[i].position, ENEMY_ROWS, ENEMY_COLS) && enemyLives[enemy[i].id] > 0){
                            mvaddch (input.position.y, input.position.x, ' ');       
                            system ("aplay ./../fileAudio/explosion.wav 2> ./../fileAudio/explosion.txt &");         
                            kill(input.pid, 1);  //kills missile process 

                            deleteEntity(ENEMY_ROWS, ENEMY_COLS, enemy[i].position);  //Deletes the enemy
                            enemyLives[enemy[i].id]--;

                            switch (enemyLives[enemy[i].id]){
                                
                                case 0:
                                    deleteEntity(ENEMY_ROWS, ENEMY_COLS, enemy[i].position);  //Deletes the enemy
                                    enemy[i].position.x = -1;
                                    kill(enemy[i].pid, 1);  //kills a level2 enemy
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
                    kill(input.pid, 1);
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
                        kill (input.pid, 1);
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
        
        refresh();
    }
    
    /* Make sure to kill all the remaining entities */
    for (i = 0; i < M; i++)
            kill(enemy[i].pid, 1);
    kill(spaceShip.pid, 1);

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





