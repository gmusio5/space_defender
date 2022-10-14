
#include "global.h"

main(){
    int i;
    obj spaceship, *enemy;
   __pid_t spaceship_pid, *enemy_pid;

    srand(time(NULL));    //Random numbers init

    initScreen();

    initColors();
    
    diff = menu(); //Inizialating M and other parameters from player difficulty choise

    initDifficulty(diff);

    enemy = (obj *) malloc (M * sizeof (obj));
    enemy_pid = (__pid_t *) malloc (M * sizeof (__pid_t));

    initPipe();

    printBorder(BORDER_DELAY);

    /* Creating spaceship process */
    spaceship.c = SPACESHIP;
    spaceship.position = createSpaceshipCoordinates();
    switch (spaceship_pid = fork()){
        case FAILURE:
            perror("An error occured during fork()");
            exit(1);
            break;

        case SUCCESS:
            close(filedes[READ]);
            spaceshipProducer(filedes[WRITE], spaceship);
            exit(0);
            break;       
    }        

    
    /* Creating M enemy processes */
    for (i = 0; i < M; i++){
        switch (enemy_pid[i] = fork()){
            case FAILURE:
                perror("An error occured during fork()");
                exit(1);
                break;

            case SUCCESS:
                close(filedes[READ]);
                enemy[i].id = i;
                enemy[i].c = ENEMY;
                enemy[i].position = createEnemyCoordinates(i);
                enemyProducer(filedes[WRITE], enemy[i]);
                exit(0);
                break;       
        }    

    }

    gameAreaConsumer(filedes[READ]);

    endgame(lives);

    free(enemy);
    free(enemy_pid);

    

    endwin();
    return 0;


}