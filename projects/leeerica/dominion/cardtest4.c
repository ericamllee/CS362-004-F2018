/*
 * cardtest4.c
 *

 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest4: cardtest4.c dominion.o rngs.o
 *      gcc -o cardtest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTCARD "minion"

int testAssert(int actual, int expected, char* message) {
  if (actual == expected) {
    printf("TEST SUCCEEDED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  } else {
    printf("TEST FAILED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  }
  return 0;
}

int main() {

  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int seed = 1000;
  int numPlayers = 4;
  int thisPlayer = 0;
  struct gameState G, testG;
  int k[10] = {minion, adventurer, embargo, village, mine, cutpurse,
               sea_hag, tribute, smithy, council_room};

  int returnValue = 0;

  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

  // ----------- TEST 1: Choice 1 and choice 2 both 0 --------------
  printf("TEST 1: Choice 1 and choice 2 both 0\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  choice1 = 0;
  choice2 = 0;

  returnValue = cardEffect(minion, choice1, choice2, choice3, &testG, handpos, &bonus);

  testAssert(returnValue, -1, "you need to choose choice1 or choice2 here");

  // ----------- TEST 2: Choice 1 and choice 2 both 1 --------------
  printf("TEST 2: Choice 1 and choice 2 both 1\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  choice1 = 1;
  choice2 = 1;

  returnValue = cardEffect(minion, choice1, choice2, choice3, &testG, handpos, &bonus);

  // If both are choices are chosen, only the first choice should be used.
  testAssert(testG.coins, G.coins + 2, "coin count");
  testAssert(testG.handCount[thisPlayer], G.handCount[thisPlayer] - 1, "hand count");


  // ----------- TEST 3: Choice 1 --------------
  printf("TEST 3: Choice 1\n");

  choice1 = 1;
  choice2 = 0;

  testG.hand[thisPlayer][0] = minion;
  testG.hand[thisPlayer][1] = copper;
  testG.hand[thisPlayer][2] = duchy;

  G.hand[thisPlayer][0] = minion;
  G.hand[thisPlayer][1] = copper;
  G.hand[thisPlayer][2] = duchy;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(minion, choice1, choice2, choice3, &testG, handpos, &bonus);

  testAssert(testG.playedCardCount, G.playedCardCount + 1, "card in playedCard array");
  testAssert(testG.deckCount[thisPlayer], G.deckCount[thisPlayer], "deck count");
  testAssert(testG.coins, G.coins + 2, "coin count");
  testAssert(testG.handCount[thisPlayer], G.handCount[thisPlayer] - 1, "hand count");
  testAssert(testG.playedCards[testG.playedCardCount - 1], minion, "correct card placed in played card pile");
  testAssert(testG.numActions, G.numActions  + 1, "1 new action");

  // ----------- TEST 4: choice 1 - other players unchanged --------------
  printf("TEST 4: choice 1 - other players unchanged\n");

  choice1 = 1;
  choice2 = 0;
  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(minion, choice1, choice2, choice3, &testG, handpos, &bonus);

  assert(returnValue == 0);
  for (int i = 0; i < G.numPlayers; i++) {
    if (i != thisPlayer) {
      printf("testing player %d's hand\n", i);
      testAssert(testG.deckCount[i], G.deckCount[i], "deck count");
      testAssert(testG.handCount[i], G.handCount[i], "hand count");
    }
  }

  // ----------- TEST 5: Choice 2's effects on this player --------------
  printf("TEST 5: Choice 2's effects on this player\n");

  choice1 = 0;
  choice2 = 1;

  testG.hand[thisPlayer][0] = minion;
  testG.hand[thisPlayer][1] = copper;
  testG.hand[thisPlayer][2] = duchy;

  G.hand[thisPlayer][0] = minion;
  G.hand[thisPlayer][1] = copper;
  G.hand[thisPlayer][2] = duchy;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(minion, choice1, choice2, choice3, &testG, handpos, &bonus);

  testAssert(testG.numActions, G.numActions + 1, "1 new action");
  testAssert(testG.playedCardCount, G.handCount[thisPlayer], "all cards played");
  testAssert(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - 4, "deck count");
  testAssert(testG.coins, G.coins, "coin count");
  testAssert(testG.handCount[thisPlayer], 4, "hand count");
  testAssert(testG.playedCards[0], minion, "correct card placed in played card pile");

  // ----------- TEST 6: Choice 2's effects on other players --------------
  printf("TEST 6: Choice 2's effects on other players\n");

  choice1 = 0;
  choice2 = 1;

  // set up the hands.
  for (int i = 0; i < G.numPlayers; i++) {
    testG.hand[i][0] = minion;
    testG.hand[i][1] = copper;
    testG.hand[i][2] = duchy;
    testG.hand[i][3] = minion;

    testG.handCount[i] = 4;

    G.hand[i][0] = minion;
    G.hand[i][1] = copper;
    G.hand[i][2] = duchy;
    G.hand[i][3] = minion;

    G.handCount[i] = 4;
  }

  testG.hand[1][4] = minion;
  testG.hand[1][5] = copper;
  testG.hand[1][6] = duchy;

  testG.handCount[1] = 7;

  G.hand[1][4] = minion;
  G.hand[1][5] = copper;
  G.hand[1][6] = duchy;

  G.handCount[1] = 7;


  testG.hand[3][4] = minion;
  testG.hand[3][5] = copper;
  testG.hand[3][6] = duchy;

  testG.handCount[3] = 7;

  G.hand[3][4] = minion;
  G.hand[3][5] = copper;
  G.hand[3][6] = duchy;

  G.handCount[3] = 7;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(minion, choice1, choice2, choice3, &testG, handpos, &bonus);

  for (int i = 0; i < G.numPlayers; i++) {
    if (i != thisPlayer) {
      printf("testing player %d's hand, which originally had %d cards.\n", i, G.handCount[i]);

      if (G.handCount[i] < 5) {
        testAssert(testG.deckCount[i], G.deckCount[i], "deck count");
        testAssert(testG.handCount[i], G.handCount[i], "hand count");
      } else {
        testAssert(testG.deckCount[i], G.deckCount[i] - 4, "deck count");
        testAssert(testG.handCount[i], 4, "hand count");
      }
      printf("\n");
    }
  }

  // ----------- TEST 7: kingdom and victory cards unchanged --------------
  printf("TEST 7: kingdom and victory cards unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(minion, choice1, choice2, choice3, &testG, handpos, &bonus);

  testAssert(testG.supplyCount[estate], G.supplyCount[estate], "estate unchanged");
  testAssert(testG.supplyCount[duchy], G.supplyCount[duchy], "duchy unchanged");
  testAssert(testG.supplyCount[province], G.supplyCount[province], "province unchanged");
  testAssert(testG.supplyCount[gardens], G.supplyCount[gardens], "gardens unchanged");
  testAssert(testG.supplyCount[curse], G.supplyCount[curse], "curse unchanged");
  testAssert(testG.supplyCount[smithy], G.supplyCount[smithy], "smithy unchanged");
  testAssert(testG.supplyCount[adventurer], G.supplyCount[adventurer], "adventurer unchanged");
  testAssert(testG.supplyCount[embargo], G.supplyCount[embargo], "embargo unchanged");
  testAssert(testG.supplyCount[village], G.supplyCount[village], "village unchanged");
  testAssert(testG.supplyCount[minion], G.supplyCount[minion], "minion unchanged");
  testAssert(testG.supplyCount[mine], G.supplyCount[mine], "mine unchanged");
  testAssert(testG.supplyCount[cutpurse], G.supplyCount[cutpurse], "cutpurse unchanged");
  testAssert(testG.supplyCount[sea_hag], G.supplyCount[sea_hag], "sea_hag unchanged");
  testAssert(testG.supplyCount[tribute], G.supplyCount[tribute], "tribute unchanged");
  testAssert(testG.supplyCount[council_room], G.supplyCount[council_room], "council_room unchanged");

  printf("\n >>>>> Testing complete %s <<<<<\n\n", TESTCARD);

  return 0;
}


