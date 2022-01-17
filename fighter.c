#include "defs.h"


/*
  Function:  initFighter
  Purpose:   Intialize FighterType with specified parameters
       in:   int for strength, armour, and health
       out:  Initialized FighterType
*/
void initFighter(FighterType **fighter, int s, int a, int h){
    (*fighter) = malloc(sizeof(FighterType));
    (*fighter)->strength = s;
    (*fighter)->armour = a;
    (*fighter)->health = h;
}



/*
  Function:  initPirates
  Purpose:   Intialize a deque with NUM_PIRATES pirates
       out:  A deque filled with NUM_PIRATES initialized pirates
*/
void initPirates(DequeType *deq){

    //Iterates for NUM_PIRATES
    for(int i = 0; i < NUM_PIRATES; i++){
        //Randomizes strength attribute until it is between 5 and 9 inclusively
        int str = randm(10);
        while(str < 5){
            str = randm(10);
        }
        //Randomizes armour attribute until it is between 5 and 9 inclusively
        int arm = randm(5);
        while(arm < 1){
            arm = randm(5);
        }
        //Initalizes hp to PIRATE_HEALTH
        int hp = PIRATE_HEALTH;

        //Creates, Initalizes, and adds a new fighter with the randomized stats to the deque.
        FighterType *fighter;
        initFighter(&fighter, str, arm, hp);
        addLast(deq, fighter);
    }
}



/*
  Function:  printFighter
  Purpose:   Prints the stats of a specific fighter, used for TESTING purposes only.
       in:   FighterType.
       out:  printed stats of a fighter.
*/
void printFighter(FighterType *fighter){
    printf("\nPrinting Fighter: s:%2d ,  a:%2d ,  h:%2d\n", fighter->strength, fighter->armour, fighter->health);
}