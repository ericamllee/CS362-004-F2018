/*
 * unittest2.c
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest2: unittest2.c dominion.o rngs.o
 *      gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTFUNCTION "supplyCount"

int testAssert(int actual, int expected, char* message) {
  if (actual == expected) {
    printf("TEST SUCCEEDED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  } else {
    printf("TEST FAILED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  }
  return 0;
}

int main() {
  int seed = 1000;
  int numPlayers = 2;
  int otherPlayer = 1;
  int thisPlayer = 0;
  struct gameState G, testG;
  int k[10] = {smithy, adventurer, embargo, village, minion, mine, cutpurse,
               sea_hag, tribute, council_room};
  int smithies = 5;

  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);

  // ----------- TEST 1: correct supply count for smithy --------------
  printf("TEST 1: correct supply count for smithy \n");

  G.supplyCount[smithy] = smithies;
  testG.supplyCount[smithy] = smithies;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  int returnValue = supplyCount(smithy, &testG);

  testAssert(returnValue, smithies, "supplyCount returns the correct number");

  // ----------- TEST 2: out of bounds card --------------
  printf("TEST 2: out of bounds card \n");

  returnValue = supplyCount(-1, &testG);

  testAssert(returnValue, -1, "supplyCount returns -1 for an invalid card");


  // ----------- TEST 3: kingdom and victory cards unchanged --------------
  printf("TEST 3: kingdom and victory cards unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  supplyCount(smithy, &testG);

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

  // ----------- TEST 4: other player unchanged --------------
  printf("TEST 4: other player unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  supplyCount(smithy, &testG);

  testAssert(testG.deckCount[otherPlayer], G.deckCount[otherPlayer], "deck count unchanged for other player");
  testAssert(testG.discardCount[otherPlayer], G.discardCount[otherPlayer], "discard count unchanged for other player");
  testAssert(testG.handCount[otherPlayer], G.handCount[otherPlayer], "hand count for other player");

  // ----------- TEST 4: this player unchanged --------------
  printf("TEST 4: this player unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  supplyCount(smithy, &testG);

  testAssert(testG.deckCount[thisPlayer], G.deckCount[thisPlayer], "deck count unchanged for this player");
  testAssert(testG.discardCount[thisPlayer], G.discardCount[thisPlayer], "discard count unchanged for this player");
  testAssert(testG.handCount[thisPlayer], G.handCount[thisPlayer], "hand count for this player");

  printf("\n >>>>> Testing complete %s <<<<<\n\n", TESTFUNCTION);

  return 0;
}
