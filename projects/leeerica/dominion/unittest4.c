/*
 * unittest1.c
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTFUNCTION "fullDeckCount"

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
  int card = copper;
  int otherCard = silver;
  int returnValue = 0;

  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);

  // ----------- TEST 1: Correct number for coins in hand. --------------
  printf("TEST 1: Correct number for coins in hand.\n");

  G.hand[thisPlayer][0] = card;
  G.hand[thisPlayer][1] = otherCard;
  G.hand[thisPlayer][2] = otherCard;
  G.handCount[thisPlayer] = 3;

  testG.hand[thisPlayer][0] = card;
  G.hand[thisPlayer][1] = otherCard;
  G.hand[thisPlayer][2] = otherCard;
  testG.handCount[thisPlayer] = 3;

  G.deckCount[thisPlayer] = 0;
  G.discardCount[thisPlayer] = 0;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  returnValue = fullDeckCount(thisPlayer, card, &testG);
  testAssert(returnValue, 1, "there was 1 card in hand");

  // ----------- TEST 2: Correct number for cards in deck --------------
  printf("TEST 2: Correct number for cards in deck \n");

  G.deck[thisPlayer][0] = card;
  G.deck[thisPlayer][1] = otherCard;
  G.deck[thisPlayer][2] = otherCard;
  G.deckCount[thisPlayer] = 3;

  testG.deck[thisPlayer][0] = card;
  G.deck[thisPlayer][1] = otherCard;
  G.deck[thisPlayer][2] = otherCard;
  testG.deckCount[thisPlayer] = 3;

  G.handCount[thisPlayer] = 0;
  G.discardCount[thisPlayer] = 0;
  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  returnValue = fullDeckCount(thisPlayer, card, &testG);
  testAssert(returnValue, 1, "there was 1 card in deck");

  // ----------- TEST 3: Correct number for cards in discards --------------
  printf("TEST 2: Correct number for cards in discards \n");

  G.discard[thisPlayer][0] = card;
  G.discard[thisPlayer][1] = otherCard;
  G.discard[thisPlayer][2] = otherCard;
  G.discardCount[thisPlayer] = 3;

  testG.discard[thisPlayer][0] = card;
  G.discard[thisPlayer][1] = otherCard;
  G.discard[thisPlayer][2] = otherCard;
  testG.discardCount[thisPlayer] = 3;

  G.handCount[thisPlayer] = 0;
  G.deckCount[thisPlayer] = 0;
  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  returnValue = fullDeckCount(thisPlayer, card, &testG);
  testAssert(returnValue, 1, "there was 1 card in discards");

  // ----------- TEST 4: Correct number for all cards --------------
  printf("TEST 4: Correct number for all cards \n");

  G.discard[thisPlayer][0] = card;
  G.discard[thisPlayer][1] = otherCard;
  G.discard[thisPlayer][2] = otherCard;
  G.discardCount[thisPlayer] = 3;

  testG.discard[thisPlayer][0] = card;
  G.discard[thisPlayer][1] = card;
  G.discard[thisPlayer][2] = otherCard;
  testG.discardCount[thisPlayer] = 3;


  G.deck[thisPlayer][0] = card;
  G.deck[thisPlayer][1] = otherCard;
  G.deck[thisPlayer][2] = otherCard;
  G.deckCount[thisPlayer] = 3;

  testG.deck[thisPlayer][0] = card;
  G.deck[thisPlayer][1] = otherCard;
  G.deck[thisPlayer][2] = otherCard;
  testG.deckCount[thisPlayer] = 3;

  G.hand[thisPlayer][0] = card;
  G.hand[thisPlayer][1] = otherCard;
  G.hand[thisPlayer][2] = otherCard;
  G.handCount[thisPlayer] = 3;

  testG.hand[thisPlayer][0] = card;
  G.hand[thisPlayer][1] = otherCard;
  G.hand[thisPlayer][2] = otherCard;
  testG.handCount[thisPlayer] = 3;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  returnValue = fullDeckCount(thisPlayer, card, &testG);
  testAssert(returnValue, 4, "correct number for all cards");

  // ----------- TEST 5: Invalid card --------------
  printf("TEST 5: invalid card \n");


  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  returnValue = fullDeckCount(thisPlayer, -1, &testG);
  testAssert(returnValue, -1, "invalid card");

  // ----------- TEST 6: other player unchanged --------------
  printf("TEST 6: other player unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  fullDeckCount(thisPlayer, card, &testG);

  testAssert(testG.deckCount[otherPlayer], G.deckCount[otherPlayer], "deck count unchanged for other player");
  testAssert(testG.discardCount[otherPlayer], G.discardCount[otherPlayer], "discard count unchanged for other player");
  testAssert(testG.handCount[otherPlayer], G.handCount[otherPlayer], "hand count for other player");

  // ----------- TEST 7: this player unchanged --------------
  printf("TEST 7: this player unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  fullDeckCount(thisPlayer, card, &testG);

  testAssert(testG.deckCount[thisPlayer], G.deckCount[thisPlayer], "deck count unchanged for this player");
  testAssert(testG.discardCount[thisPlayer], G.discardCount[thisPlayer], "discard count unchanged for this player");
  testAssert(testG.handCount[thisPlayer], G.handCount[thisPlayer], "hand count for this player");

  // ----------- TEST 8: kingdom and victory cards unchanged --------------
  printf("TEST 8: kingdom and victory cards unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  fullDeckCount(thisPlayer, smithy, &testG);

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

//  // ----------- TEST 9: Invalid player --------------
//  printf("TEST 9: invalid player \n");
//
//
//  // copy the game state to a test case
//  memcpy(&testG, &G, sizeof(struct gameState));
//
//  returnValue = fullDeckCount(-1, card, &testG);
//  testAssert(returnValue, 3, "invalid player");


  printf("\n >>>>> Testing complete %s <<<<<\n\n", TESTFUNCTION);

  return 0;
}


