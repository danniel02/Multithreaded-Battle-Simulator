#include "defs.h"



/*
  Function:  initRunSpec
  Purpose:   Initializes a RunSpecType Structure with the specified parameters.
       in:   A deque, a tortoise fighter, hare fighter, stats.
       out:  An initialized runSpecsType struct.
*/
void initRunSpec(RunSpecsType **runSpecs, DequeType *deque, FighterType *tort, FighterType *hare, StatsType *stats){
    (*runSpecs) = malloc(sizeof(RunSpecsType));
    (*runSpecs)->pirates = deque;
    (*runSpecs)->tort = tort;
    (*runSpecs)->hare = hare;
    (*runSpecs)->stats = stats;
}


/*
  Function:  initStats
  Purpose:   Initializes an empty Stats struct.
       in:   stats struct
*/
void initStats(StatsType **stats){
    (*stats) = malloc(sizeof(StatsType));
    (*stats)->numFailure = 0;
    (*stats)->numPartSuccess = 0;
    (*stats)->numSuccess = 0;

}


/*
  Function:  initFightSpecs
  Purpose:   Initializes a fightSpecsType struct with the specified parameters.
       in:   A deque of pirates, a hero fighter, a mutex, and an enum Directiontype. 
       out:  An initialized FightSpecsType struct.
*/
void initFightSpecs(FightSpecsType **FightSpecs, DequeType *pirates, FighterType *hero, sem_t *mutex, DirectionType *dir){
    (*FightSpecs) = malloc(sizeof(FightSpecsType));
    (*FightSpecs)->pirates = pirates;
    (*FightSpecs)->hero = hero;
    (*FightSpecs)->dir = *dir;
    (*FightSpecs)->dqMutex = mutex;
}


/*
  Function:  cleanStats
  Purpose:   cleans the stats struct.
       in:   Stats struct.
*/
void cleanStats(StatsType *stats){
    free(stats);
}


/*
  Function:  cleanRunSpec
  Purpose:   Cleans and deallocates everything inside of a RunSpecsType struct.
       in:   A RunSpecsType struct.
*/
void cleanRunSpec(RunSpecsType *runSpecs){
    free(runSpecs->hare);
    free(runSpecs->tort);
    cleanupDeque(runSpecs->pirates);
    cleanStats(runSpecs->stats);
}


/*
  Function:  cleanFightSpecs
  Purpose:   Frees a fightSpecs struct
       in:   a FightSpecsType struct.
*/
void cleanFightSpecs(FightSpecsType *fightSpecs){
    free(fightSpecs);
}


/*
  Function:  computePercent
  Purpose:   Computes the percent of a given statsType and stores the information in a new StatsType
       in:   Source Statstype to convert and copy from.
       out:  new StatsType with percentage values
*/
void computePercent(StatsType *source, StatsType *new){
    //Get the total number
    int total = source->numFailure + source->numPartSuccess + source->numSuccess;
    //Divide each category by the total number and copy the percent to the new StatsType object
    new->numSuccess = source->numSuccess / total;
    new->numPartSuccess = source->numPartSuccess / total;
    new->numFailure = source->numFailure / total;
}


/*
  Function:  printTotalStats
  Purpose:   Prints to the screen information of every stat with their percentages of each scenario
       in:   StatTypes of the tort and hare with a sword, and without a sword
       out:  A table with percentages of stats
*/
void printTotalStats(StatsType *tort, StatsType *hare, StatsType *none){
    int width = 20;
    char* labels[] = {"Tortoise", "Hare", "None"};

    printLine(width);
    printf("| Hero with Sword | Success Percent | Partial Percent | Failure Percent |");      
    printLine(width);
    printf("| %10s      | %6d      | %6d      | %6d      |", labels[0], tort->numSuccess, tort->numPartSuccess, tort->numFailure);
    printLine(width);
    printf("| %10s      | %6d      | %6d      | %6d      |", labels[1], hare->numSuccess, hare->numPartSuccess, hare->numFailure);
    printLine(width);
    printf("| %10s      | %6d      | %6d      | %6d      |", labels[2], none->numSuccess, none->numPartSuccess, none->numFailure);
    printLine(width);
}


/*
  Function:  printLine
  Purpose:   Prints a line equal in length to the given parameter
       in:   int that represents the width of the line
       out:  A line with the width of the specified width
*/
void printLine(int width){
    printf("+");
    printf("%*s", width,  "- ");
    printf("+");
}

