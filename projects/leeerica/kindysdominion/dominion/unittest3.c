/*
 * unittest3.c
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest3: unittest3.c dominion.o rngs.o
 *      gcc -o unittest3 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTFUNCTION "handCard"

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
  int thisPlayer = 0;
  int otherPlayer = 1;
  struct gameState G, testG;
  int k[10] = {smithy, adventurer, embargo, village, minion, mine, cutpurse,
               sea_hag, tribute, council_room};

  int handPos = 0;
  int card = copper;
  int otherCard = silver;

  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);
  printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);

  // ----------- TEST 1: returns correct card --------------
  printf("TEST 1: returns correct card at 0th position.\n");

  G.hand[thisPlayer][handPos] = card;
  G.handCount[thisPlayer] = 1;

  testG.hand[thisPlayer][handPos] = card;
  testG.handCount[thisPlayer] = 1;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  int returnValue = handCard(handPos, &testG);

  testAssert(returnValue, card, "returns the correct card");

  // ----------- TEST 2: out of bounds handPos --------------
  printf("TEST 2: out of bounds card \n");

  returnValue = handCard(-1, &testG);

  testAssert(returnValue, -1, "returns -1 if the handPos is out of bounds");

  // ----------- TEST 3: returns current player's card --------------
  printf("TEST 3: returns current player's card.\n");

  G.hand[thisPlayer][handPos] = card;
  G.handCount[thisPlayer] = 1;

  testG.hand[thisPlayer][handPos] = card;
  testG.handCount[thisPlayer] = 1;

  G.hand[otherPlayer][handPos] = otherCard;
  G.handCount[otherPlayer] = 1;

  testG.hand[otherPlayer][handPos] = otherCard;
  testG.handCount[otherPlayer] = 1;

  G.whoseTurn = thisPlayer;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  returnValue = handCard(handPos, &testG);

  testAssert(returnValue, card, "returns active player's card");

  // ----------- TEST 4: returns correct card in other position --------------
  printf("TEST 3: returns correct card in other position\n");

  handPos = 1;
  G.hand[thisPlayer][0] = card;
  G.hand[thisPlayer][handPos] = otherCard;
  G.handCount[thisPlayer] = 2;

  testG.hand[thisPlayer][0] = card;
  testG.hand[thisPlayer][handPos] = otherCard;
  testG.handCount[thisPlayer] = 2;

  G.whoseTurn = thisPlayer;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  returnValue = handCard(handPos, &testG);

  testAssert(returnValue, otherCard, "returns correct card in other positions");


  // ----------- TEST 5: kingdom and victory cards unchanged --------------
  printf("TEST 5: kingdom and victory cards unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  handPos = 0;
  handCard(handPos, &testG);

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

  // ----------- TEST 6: other player unchanged --------------
  printf("TEST 6: other player unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  handCard(handPos, &testG);

  testAssert(testG.deckCount[otherPlayer], G.deckCount[otherPlayer], "deck count unchanged for other player");
  testAssert(testG.discardCount[otherPlayer], G.discardCount[otherPlayer], "discard count unchanged for other player");
  testAssert(testG.handCount[otherPlayer], G.handCount[otherPlayer], "hand count for other player");

  // ----------- TEST 7: this player unchanged --------------
  printf("TEST 7: this player unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  supplyCount(smithy, &testG);

  testAssert(testG.deckCount[thisPlayer], G.deckCount[thisPlayer], "deck count unchanged for this player");
  testAssert(testG.discardCount[thisPlayer], G.discardCount[thisPlayer], "discard count unchanged for this player");
  testAssert(testG.handCount[thisPlayer], G.handCount[thisPlayer], "hand count for this player");

  return 0;
}
