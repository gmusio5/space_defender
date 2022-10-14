
#include "global.h"

main(){
    int i;
    obj spaceship, *enemy;
    pthread_t spaceship_tid, *enemy_tid;

    srand(time(NULL));    //Random numbers init

    initScreen();

    initColors();
    
    diff = menu(); //Inizialating M and other parameters from player difficulty choise
    
    initDifficulty(diff);

    enemy = (obj*) malloc(M * sizeof(obj));
    enemy_tid = (pthread_t *) malloc (M *sizeof(pthread_t));

    initMutex();

    printBorder(BORDER_DELAY);

    /* Creating spaceship thread */
    spaceship.position = createSpaceshipCoordinates();
    spaceship.c = SPACESHIP;
    if(pthread_create(&spaceship_tid, NULL, &spaceshipProducer, (void*)(&spaceship))){
            endwin();
            exit;
    }


    /* Creating M enemy threads */
    for (i = 0; i < M; i++){
        enemy[i].id = i;
        enemy[i].c = ENEMY;
        enemy[i].position = createEnemyCoordinates(i);
        if(pthread_create(&enemy_tid[i], NULL, &enemyProducer , (void*)&enemy[i])){
            endwin();
            exit;
        }
    }
    
    gameAreaConsumer();

    endgame(lives);

    pthread_mutex_destroy(&mtx_read);
    pthread_mutex_destroy(&mtx_write);

    free(enemy);
    free(enemy_tid);

    endwin();
    return 0;


}