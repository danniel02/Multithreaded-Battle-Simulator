#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_RUNS       100
#define NUM_PIRATES     10
#define NUM_SCENARIOS    3

#define TORT_STRENGTH    5
#define TORT_ARMOUR      8
#define HARE_STRENGTH    8
#define HARE_ARMOUR      5
#define HERO_HEALTH     20
#define PIRATE_HEALTH   10
#define SWORD_STRENGTH   2

#define MAX_STR         64

typedef enum { SUCCESS, FAILURE, PART_SUCCESS, RES_UNKNOWN } ResultType;
typedef enum { FRONT, BACK } DirectionType;

typedef struct {	// Stats for one scenario, accumulated over all runs
  int numSuccess;
  int numPartSuccess;
  int numFailure;
} StatsType;

typedef struct {	// One fighter:  Tortoise, Hare, or Pirate
  int  strength;
  int  armour;
  int  health;
} FighterType;

typedef struct Node {
  FighterType  *data;
  struct Node  *next;
} NodeType;

typedef struct {	// Stores the fighting pirates for one scenario
  NodeType *head;
  NodeType *tail;
} DequeType;

typedef struct {	// Specs for one scenario of one run
  DequeType   *pirates;
  FighterType *tort;
  FighterType *hare;
  StatsType   *stats;
} RunSpecsType;

typedef struct {	// Specs for one fight of one scenario of one run
  FighterType  *hero;
  DequeType    *pirates;
  DirectionType dir;
  sem_t        *dqMutex;
} FightSpecsType;

//Functions in main.c
int randm(int);		// Pseudo-random number generator function
void *scenario(void *arg);
void *fight(void *arg);

//Functions in fighter.c
void initFighter(FighterType **fighter, int s, int a, int h);
void initPirates(DequeType *deq);
void printFighter(FighterType *fighter);

//Functions in deque.c
void initDeque(DequeType **deq);
void addLast(DequeType *deq, FighterType *fighter);
void addFirst(DequeType *deq, FighterType *fighter);
void removeFirst(FighterType *fighter, DequeType *deq);
void removeLast(FighterType *fighter, DequeType *deq);
void deepCopy(DequeType *source, DequeType *newDeque);
void printDeque(DequeType *deq);
void cleanupDeque(DequeType *deq);
int isEmpty(DequeType *deq);

//Functions in specs.c
void initRunSpec(RunSpecsType **runSpecs, DequeType *deque, FighterType *tort, FighterType *hare, StatsType *stats);
void initStats(StatsType **stats);
void cleanStats(StatsType *stats);
void cleanRunSpec(RunSpecsType *runSpecs);
void initFightSpecs(FightSpecsType **FightSpecs, DequeType *pirates, FighterType *hero, sem_t *mutex, DirectionType *dir);
void cleanFightSpecs(FightSpecsType *fightSpecs);
void printTotalStats(StatsType *tort, StatsType *hare, StatsType *none);
void printLine(int width);
void computePercent(StatsType *source, StatsType *new);


#endif

