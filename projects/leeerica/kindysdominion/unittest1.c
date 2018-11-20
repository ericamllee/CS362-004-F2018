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

#define TESTFUNCTION "updateCoins"

int testAssert(int actual, int expected, char* message) {
  if (actual == expected) {
    printf("TEST SUCCEEDED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  } else {
    printf("TEST FAILED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  }
  return 0;
}

int main() {
  int bonus = 0;
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;
  int otherPlayer = 1;
  struct gameState G, testG;
  int k[10] = {smithy, adventurer, embargo, village, minion, mine, cutpurse,
               sea_hag, tribute, council_room};

  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);

  // ----------- TEST 1: Correct number for copper --------------
  printf("TEST 1: Correct number for copper \n");

  G.hand[thisPlayer][0] = copper;
  G.handCount[thisPlayer] = 1;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testAssert(testG.coins, G.coins, "coins are equal initially");

  int returnValue = updateCoins(thisPlayer, &testG, bonus);

  testAssert(testG.coins, 1, "copper is worth 1");
  testAssert(returnValue, 0, "return value is 0");

  // ----------- TEST 2: Correct number for silver --------------
  printf("TEST 2: Correct number for silver \n");

  G.hand[thisPlayer][0] = silver;
  G.handCount[thisPlayer] = 1;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testAssert(testG.coins, G.coins, "coins are equal initially");

  returnValue = updateCoins(thisPlayer, &testG, bonus);

  testAssert(testG.coins, 2, "silver is worth 2");
  testAssert(returnValue, 0, "return value is 0");

  // ----------- TEST 3: Correct number for gold --------------
  printf("TEST 3: Correct number for gold \n");

  G.hand[thisPlayer][0] = gold;
  G.handCount[thisPlayer] = 1;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testAssert(testG.coins, G.coins, "coins are equal initially");

  returnValue = updateCoins(thisPlayer, &testG, bonus);

  testAssert(testG.coins, 3, "gold is worth 3");
  testAssert(returnValue, 0, "return value is 0");

  // ----------- TEST 4: Bonus increments coin count --------------
  printf("TEST 4: Bonus increments coin count \n");

  G.hand[thisPlayer][0] = village;
  G.handCount[thisPlayer] = 1;

  bonus = 5;
  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testAssert(testG.coins, G.coins, "coins are equal initially");

  returnValue = updateCoins(thisPlayer, &testG, bonus);

  testAssert(testG.coins, 5, "bonus added 5");
  testAssert(returnValue, 0, "return value is 0");

  // ----------- TEST 5: Coins can't be negative  --------------
  printf("TEST 5: Coins can't be negative \n");

  G.hand[thisPlayer][0] = village;
  G.handCount[thisPlayer] = 1;

  bonus = -1;
  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testAssert(testG.coins, G.coins, "coins are equal initially");

  returnValue = updateCoins(thisPlayer, &testG, bonus);

  testAssert(testG.coins, 0, "coin count can't be less than 1");
  testAssert(returnValue, 0, "return value is 0");

  bonus = 0;

  // ----------- TEST 6: All coins add up. --------------
  printf("TEST 6: All coins add up. \n");

  G.hand[thisPlayer][0] = copper;
  G.hand[thisPlayer][1] = silver;
  G.hand[thisPlayer][2] = gold;

  G.handCount[thisPlayer] = 3;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testAssert(testG.coins, G.coins, "coins are equal initially");

  returnValue = updateCoins(thisPlayer, &testG, bonus);

  int coins = 0;
  //add coins for each Treasure card in player's hand
  for (int i = 0; i < testG.handCount[thisPlayer]; i++)
  {
    if (testG.hand[thisPlayer][i] == copper)
    {
      coins += 1;
    }
    else if (testG.hand[thisPlayer][i] == silver)
    {
      coins += 2;
    }
    else if (testG.hand[thisPlayer][i] == gold)
    {
      coins += 3;
    }
  }

  testAssert(testG.coins, 6, "all coins should be worth 6");
  testAssert(returnValue, 0, "return value is 0");

  // ----------- TEST 6: Only the given player gets coins --------------
  printf("TEST 6: Only the given player gets coins. \n");

  G.hand[thisPlayer][0] = copper;
  G.hand[otherPlayer][0] = silver;
  G.handCount[thisPlayer] = 1;
  G.handCount[otherPlayer] = 1;
  G.whoseTurn = thisPlayer;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testAssert(testG.coins, G.coins, "coins are equal initially");

  returnValue = updateCoins(thisPlayer, &testG, bonus);

  testAssert(testG.coins, 1, "only the active player gets coins");
  testAssert(returnValue, 0, "return value is 0");

  // ----------- TEST 7: Inactive player cannot get coins --------------
  printf("TEST 7: Inactive player cannot get coins.\n");

  G.hand[thisPlayer][0] = copper;
  G.hand[otherPlayer][0] = silver;
  G.handCount[thisPlayer] = 1;
  G.handCount[otherPlayer] = 1;
  G.whoseTurn = thisPlayer;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testAssert(testG.coins, G.coins, "coins are equal initially");

  returnValue = updateCoins(otherPlayer, &testG, bonus);

  testAssert(testG.coins, G.coins, "coin count is unchanged");
  testAssert(returnValue, -1, "return value");


  // ----------- TEST 8: reset coin count --------------
  printf("TEST 8: reset coin count \n");

  G.hand[thisPlayer][0] = copper;
  G.handCount[thisPlayer] = 1;

  testG.hand[thisPlayer][0] = copper;
  testG.handCount[thisPlayer] = 1;

  G.coins = 6;
  bonus = 0;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testAssert(testG.coins, G.coins, "coins are equal initially");

  returnValue = updateCoins(thisPlayer, &testG, bonus);
  testAssert(testG.coins, 1, "coin count is reset");
  testAssert(returnValue, 0, "return value");

  // ----------- TEST 9: out of bounds player --------------
  printf("TEST 9: out of bounds player \n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testAssert(testG.coins, G.coins, "coins are equal initially");

  returnValue = updateCoins(-1, &testG, bonus);
  testAssert(testG.coins, G.coins, "coins shouldn't change due to invalid player");
  testAssert(returnValue, -1, "return value");

  // ----------- TEST 7: kingdom and victory cards unchanged --------------
  printf("TEST 7: kingdom and victory cards unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  updateCoins(thisPlayer, &testG, bonus);

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

  // ----------- TEST 8: other player unchanged --------------
  printf("TEST 8: other player unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  updateCoins(thisPlayer, &testG, bonus);

  testAssert(testG.deckCount[otherPlayer], G.deckCount[otherPlayer], "deck count unchanged for other player");
  testAssert(testG.discardCount[otherPlayer], G.discardCount[otherPlayer], "discard count unchanged for other player");
  testAssert(testG.handCount[otherPlayer], G.handCount[otherPlayer], "hand count for other player");

  printf("\n >>>>> Testing complete %s <<<<<\n\n", TESTFUNCTION);

  return 0;
}


