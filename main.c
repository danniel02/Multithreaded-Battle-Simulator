#include "defs.h"
#include <errno.h>



/*
  Function:  main
  Purpose:   Simulate multiple runs of a battle using multiple threads.
       in:   command line arguments; int specifying number of runs.
   return:   int 0 if all good.
*/
int main(int argc, char *argv[]){

  //Declaring variable of number of runs
  int numRuns;

  //Error check arguments, use 100 runs by default, use specified argument otherwise.
  if (argc == 1) {
    numRuns = NUM_RUNS;
  }else{
    char* p;
    errno = 0;
    long arg = strtol(argv[1], &p, 10);
    if (*p != '\0' || errno != 0) {
      printf("Invalid argument: Please try again.\n");
      return 1;
    }
    numRuns = arg;
  }

  //Use time for randomization
  srand( (unsigned)time ( NULL ) );
  
  //Simulating all runs // each iteration = 1 run
  for(int i = 0; i < numRuns; i++){

    //Declaring array of deques each representing a different scenario
    DequeType *pirates[NUM_SCENARIOS-1];

    //init deque for each scenario
    for(int j = 0; j < NUM_SCENARIOS; j++){
      DequeType *deq;
      initDeque(&deq);
      pirates[j] = deq;
    }

    //Filling first deque with 10 random pirates
    initPirates(pirates[0]);

    //Copying contents of first deque to other deques
    for(int j = 1; j < NUM_SCENARIOS; j++){
      deepCopy(pirates[0], pirates[j]);
    }
    StatsType *stats[NUM_SCENARIOS];
    RunSpecsType *runSpecs[NUM_SCENARIOS];

    //Initalizes tort, hare, and collection of runSpecsType
    for(int j = 0; j < NUM_SCENARIOS; j++){

      FighterType *tort;
      FighterType *hare;

      switch(j){

        //tort sword
        case 0:
          initFighter(&tort, TORT_STRENGTH + SWORD_STRENGTH, TORT_ARMOUR, HERO_HEALTH);
          initFighter(&hare, HARE_STRENGTH, HARE_ARMOUR, HERO_HEALTH);
          break;

        //hare sword
        case 1:
          initFighter(&tort, TORT_STRENGTH, TORT_ARMOUR, HERO_HEALTH);
          initFighter(&hare, HARE_STRENGTH + SWORD_STRENGTH, HARE_ARMOUR, HERO_HEALTH);
          break;

        //Default case is no sword
        default:
          initFighter(&tort, TORT_STRENGTH, TORT_ARMOUR, HERO_HEALTH);
          initFighter(&hare, HARE_STRENGTH, HARE_ARMOUR, HERO_HEALTH);  

      }
      initStats(&stats[j]);
      initRunSpec(&runSpecs[j], pirates[j], tort, hare, stats[j]);
    }

    //Creating a number of threads equal to the NUM_SCCENARIOS and running them with a RunSpecsType param
    pthread_t tid[NUM_SCENARIOS-1];  
    for(int j = 0; j < NUM_SCENARIOS; j++){
      pthread_create(&tid[j], NULL, scenario, runSpecs[j]);
    }

    //Waiting for thread to terminate
    for(int j = 0; j < NUM_SCENARIOS; j++){
      pthread_join(tid[j], NULL);
    }

    //Converting the stats to percentages
    for(int j = 0; j < NUM_SCENARIOS; j++){
      computePercent(stats[j], stats[j]);
    }
   
    //Printing the stats
    printTotalStats(stats[0], stats[1], stats[2]);

    //Cleans up and deallocates all allocated memory in the run
    for(int j = 0; j < NUM_SCENARIOS; j++){
      cleanRunSpec(runSpecs[j]);
    }
  }

  //End of program
  return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
  Function:  scenario
  Purpose:   simulate a scenario in which a tortoise and a hare battle
             on either ends of a pirate deque.
       in:   void * of a RunSpecsType Struct. 
   return:   int 0 if all good.
*/
void *scenario(void *arg){

  //Takes RunSpecsType Param and changes it form void * to RunSpecsType - Declares Useful variables
  RunSpecsType curSpecs = *((RunSpecsType *)arg);
  FightSpecsType *fightSpecs[1];
  sem_t lock;
  pthread_t tid[1];
  DirectionType f = FRONT;
  DirectionType b = BACK;
  void* result;
  ResultType outcome;

  //Init mutex
  if (sem_init(&lock, 0, 1) < 0) {
    printf("semaphore initialization error\n");
    exit(1);
  }

  //Init the two fight specs (tortoise at start, hare at end)
  initFightSpecs(&fightSpecs[0], curSpecs.pirates, curSpecs.tort, &lock, &f);
  initFightSpecs(&fightSpecs[1], curSpecs.pirates, curSpecs.hare, &lock, &b);

  //Creating two threads for the two fight specs
  pthread_create(&tid[0], NULL, fight, &fightSpecs[0]);
  pthread_create(&tid[1], NULL, fight, &fightSpecs[1]);
  
  //Waiting for thread to terminate and increment stats based on returned value of thread
  for(int j = 0; j < 2; j++){
    //wait for thread
    pthread_join(tid[j], &result);
    outcome += *((ResultType *)result);

    //increment failure stat
    if(outcome == 0){
      curSpecs.stats->numFailure++;

    //increment part success stat
    }else if(outcome == 1){
      curSpecs.stats->numPartSuccess++;

    //increment success stat
    }else{
      curSpecs.stats->numSuccess++;
    }
  }

  //Cleans up all allocated memory used in this scenario
  cleanFightSpecs(fightSpecs[0]);
  cleanFightSpecs(fightSpecs[1]);

  //Returns all good
  return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
  Function:  fight
  Purpose:   simulate a turn based fight between a hero (tortoise or hare) and
             pirates on deque (pun intended).
       in:   void * of a FightSpecsType Struct. 
   return:   enum ResultType based on if the hero killed all the pirates or died.
*/
void *fight(void *arg){

  //Takes void * FightSpecsType param and changes it to FightSpecsType // Declares pirate to fight
  FightSpecsType curFight = *((FightSpecsType *)arg);
  FighterType pirate;

  //Tortoise Fight - - - - 
  if(curFight.dir == FRONT){
    
    //Continue fighting as long as enemies remain
    while(curFight.pirates->tail != NULL){

      //Wait for mutex
      if (sem_wait(curFight.dqMutex) < 0) {
        printf("semaphore wait error\n");
        exit(1);
      }

      //Remove the First pirate as long as deque is not empty
      if(isEmpty(curFight.pirates) == 0){
        removeFirst(&pirate, curFight.pirates);
      }
      
      //Opens mutex again
      if (sem_post(curFight.dqMutex) < 0) {
        printf("semaphore post error\n");
        exit(1);
      }
      
      //Continue while current enemy is still alive or hero is still alive
      while(1){

        //Hero attacks

        //Attack does 0 damage
        if((curFight.hero->strength - pirate.armour) < 0){
        
        //Hero defeats pirate
        }else if((pirate.health - (curFight.hero->strength - pirate.armour)) < 0){
          pirate.health = 0;
        //Hero attacks pirate
        }else{
          pirate.health = pirate.health - (curFight.hero->strength - pirate.armour);
        }

        //check if pirate is defeated, if so, hero gets 3 hp up to 20 hp
        if(pirate.health == 0){
          if((curFight.hero->health + 3) > 20){
            curFight.hero->health = 20;
          }else{
            curFight.hero->health += 3;
          }break;
        }

        //Pirate Attacks

        //Temporary pirate attack value
        int attack = pirate.strength + randm(pirate.strength-2);

        //Pirate attack does nothing
        if((attack - curFight.hero->armour) < 0){

        //Pirate defeats hero
        }else if((curFight.hero->health - (attack - curFight.hero->armour)) < 0){
          curFight.hero->health = 0;
        //Pirate attacks hero
        }else{
          curFight.hero->health = curFight.hero->health - (attack - curFight.hero->armour);
        }

        //Check if hero is defeated
        if(curFight.hero->health == 0){
          break;
        }
      }

      //Sleep for program consistency
      usleep(1000);

      //If hero is defeated, return resultType FAILURE and terminate thread
      if(curFight.hero->health == 0){
        return (void *)FAILURE;
      }
    } 

  //Hare Fight - - - - 
  }else{
    
    //Continue fighting as long as enemies remain
    while(curFight.pirates->head != NULL){

      //Wait for mutex
      if (sem_wait(curFight.dqMutex) < 0) {
        printf("semaphore wait error\n");
        exit(1);
      }

      //Remove from END as long as deque is not empty
      if(isEmpty(curFight.pirates) == 0){
        removeLast(&pirate, curFight.pirates);
      }

      //Open mutex
      if (sem_post(curFight.dqMutex) < 0) {
        printf("semaphore post error\n");
        exit(1);
      }

      //While hare is alive or pirate is alive
      while(1){

        //Hero attacks

        //Attack does 0 damage
        if((curFight.hero->strength - pirate.armour) < 0){
        
        //Hero defeats pirate
        }else if((pirate.health - (curFight.hero->strength - pirate.armour)) < 0){
          pirate.health = 0;
        //Hero attacks pirate
        }else{
          pirate.health = pirate.health - (curFight.hero->strength - pirate.armour);
        }

        //check if pirate is defeated, if so, hero gets 3 hp up to 20 hp
        if(pirate.health == 0){
          if((curFight.hero->health + 3) > 20){
            curFight.hero->health = 20;
          }else{
            curFight.hero->health += 3;
          }break;
        }

        //Pirate Attacks

        //Temporary pirate attack value
        int attack = pirate.strength + randm(pirate.strength-2);

        //Pirate attack does nothing
        if((attack - curFight.hero->armour) < 0){

        //Pirate defeats hero
        }else if((curFight.hero->health - (attack - curFight.hero->armour)) < 0){
          curFight.hero->health = 0;
        //Pirate attacks hero
        }else{
          curFight.hero->health = curFight.hero->health - (attack - curFight.hero->armour);
        }

        //Check if hero is defeated
        if(curFight.hero->health == 0){
          break;
        }
      }

      //Sleep for program consistency
      usleep(1000);
      
      //Return enum resultType FAILURE if hero is defeated
      if(curFight.hero->health == 0){
        return (void *)FAILURE;
      }
    }
  }

  //If Code reaches here, hero survived
  return (void *)SUCCESS;
}



/*
  Function:  randm
  Purpose:   returns a pseudo randomly generated number, 
             in the range 0 to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1) 
*/
int randm(int max)
{
  double r = ( (double)rand() / ((double)(RAND_MAX)+(double)(1)) ); 

  return (int)(r * max);
}

