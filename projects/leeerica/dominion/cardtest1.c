/*
 * cardtest1.c
 *

 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTCARD "smithy"

int testAssert(int actual, int expected, char* message) {
  if (actual == expected) {
    printf("TEST SUCCEEDED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  } else {
    printf("TEST FAILED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  }
  return 0;
}

int main() {
  int newCards = 3;
  int discarded = 1;

  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;
  int otherPlayer = 1;
  struct gameState G, testG;
  int k[10] = {smithy, adventurer, embargo, village, minion, mine, cutpurse,
               sea_hag, tribute, council_room};

  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

  // ----------- TEST 1: choice1 = 1 = +2 cards --------------
  printf("TEST 1: 3 additional cards in hand\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  int returnValue = cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

  testAssert(testG.handCount[thisPlayer], G.handCount[thisPlayer]  + newCards - discarded, "hand count");
  testAssert(returnValue, 0, "return value");

  // ----------- TEST 2: Check Played Card Pile --------------
  printf("TEST 2: Played Card Pile increases by 1\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

  testAssert(testG.playedCardCount, G.playedCardCount + discarded, "card in playedCard array");

  // ----------- TEST 3: check deck count --------------
  printf("TEST 3: check deck count\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

  testAssert(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards, "deck count");


  // ----------- TEST 4: check coins --------------
  printf("TEST 4: check coins\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

  testAssert(testG.coins, G.coins, "coin count");

  // ----------- TEST 5: other player unchanged --------------
  printf("TEST 5: other player unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

  testAssert(testG.deckCount[otherPlayer], G.deckCount[otherPlayer], "deck count unchanged for other player");
  testAssert(testG.discardCount[otherPlayer], G.discardCount[otherPlayer], "discard count unchanged for other player");
  testAssert(testG.handCount[otherPlayer], G.handCount[otherPlayer], "hand count for other player");

  // ----------- TEST 6: kingdom and victory cards unchanged --------------
  printf("TEST 6: kingdom and victory cards unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

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


