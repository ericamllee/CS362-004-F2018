/*
 * cardtest2.c
 *
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest2: cardtest2.c dominion.o rngs.o
 *      gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTCARD "adventurer"

int testAssert(int actual, int expected, char* message) {
  if (actual == expected) {
    printf("TEST SUCCEEDED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  } else {
    printf("TEST FAILED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  }
  return 0;
}

int main() {
  int numCards = 2;

  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;
  int otherPlayer = 1;
  struct gameState G, testG;
  int k[10] = {adventurer, village, embargo, minion, mine, cutpurse,
               sea_hag, tribute, smithy, council_room};

  int returnValue = 0;

  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

  // ----------- TEST 1: need to draw lots of cards --------------
  printf("TEST 1: need to draw lots of cards\n");

  // set up deck
  testG.deck[thisPlayer][0] = mine;
  testG.deck[thisPlayer][1] = silver;
  testG.deck[thisPlayer][2] = mine;
  testG.deck[thisPlayer][3] = mine;
  testG.deck[thisPlayer][4] = mine;
  testG.deck[thisPlayer][5] = silver;

  testG.deckCount[thisPlayer] = 6;

  G.deck[thisPlayer][0] = mine;
  G.deck[thisPlayer][1] = silver;
  G.deck[thisPlayer][2] = mine;
  G.deck[thisPlayer][3] = mine;
  G.deck[thisPlayer][4] = mine;
  G.deck[thisPlayer][5] = silver;

  G.deckCount[thisPlayer] = 6;

  // set up hands.
  testG.hand[thisPlayer][0] = adventurer;
  testG.hand[thisPlayer][1] = adventurer;

  testG.handCount[thisPlayer] = 2;

  G.hand[thisPlayer][0] = adventurer;
  G.hand[thisPlayer][1] = adventurer;

  G.handCount[thisPlayer] = 2;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  int numCardsToReachTwoTreasureCards = 5;
  testAssert(testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCards - 1, "drew two new treasure cards and put adventurer card in played cards pile");
  testAssert(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsToReachTwoTreasureCards, "drew 5 cards to reveal treasure cards");
  testAssert(testG.playedCardCount, G.playedCardCount + numCardsToReachTwoTreasureCards - numCards + 1, "other cards are in the played cards array");

  testAssert(returnValue, 0, "return value");

  // ----------- TEST 2: draw minimum number of cards --------------

  printf("TEST 2: draw minimum number of cards\n");

  // set up deck
  testG.deck[thisPlayer][0] = silver;
  testG.deck[thisPlayer][1] = silver;

  testG.deckCount[thisPlayer] = 2;

  G.deck[thisPlayer][0] = silver;
  G.deck[thisPlayer][1] = silver;

  G.deckCount[thisPlayer] = 2;

  // set up hands.
  testG.hand[thisPlayer][0] = adventurer;

  testG.handCount[thisPlayer] = 1;

  G.hand[thisPlayer][0] = adventurer;

  G.handCount[thisPlayer] = 1;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  numCardsToReachTwoTreasureCards = 2;
  testAssert(testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCards - 1, "drew two new treasure cards and put adventurer card in played cards pile");
  testAssert(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsToReachTwoTreasureCards, "drew 2 cards to reveal treasure cards");
  testAssert(testG.playedCardCount, G.playedCardCount + numCardsToReachTwoTreasureCards - numCards + 1, "adventurer card is in the played cards array");

  testAssert(returnValue, 0, "return value");

  // ----------- TEST 3: other players unchanged --------------
  printf("TEST 3: other players unchanged\n");

  // set up deck
  testG.deck[thisPlayer][0] = silver;
  testG.deck[thisPlayer][1] = silver;

  testG.deckCount[thisPlayer] = 2;

  G.deck[thisPlayer][0] = silver;
  G.deck[thisPlayer][1] = silver;

  G.deckCount[thisPlayer] = 2;

  // set up hands
  testG.hand[thisPlayer][0] = adventurer;

  testG.handCount[thisPlayer] = 1;

  G.hand[thisPlayer][0] = adventurer;

  G.handCount[thisPlayer] = 1;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

  assert(returnValue == 0);
  testAssert(testG.deckCount[otherPlayer], G.deckCount[otherPlayer], "deck count unchanged for other player");
  testAssert(testG.discardCount[otherPlayer], G.discardCount[otherPlayer], "discard count unchanged for other player");
  testAssert(testG.handCount[otherPlayer], G.handCount[otherPlayer], "hand count for other player");

  // ----------- TEST 4: not enough coins --------------

  printf("TEST 4: not enough coins\n");

  // set up deck
  testG.deck[thisPlayer][0] = silver;

  testG.deckCount[thisPlayer] = 1;

  G.deck[thisPlayer][0] = silver;

  G.deckCount[thisPlayer] = 1;

  // set up hands
  testG.hand[thisPlayer][0] = adventurer;

  testG.handCount[thisPlayer] = 1;

  G.hand[thisPlayer][0] = adventurer;

  G.handCount[thisPlayer] = 1;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  numCardsToReachTwoTreasureCards = 1;
  testAssert(testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCards - 1, "drew one new treasure cards and put adventurer card in played cards pile");
  testAssert(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsToReachTwoTreasureCards, "drew 1 card to reveal treasure cards");
  testAssert(testG.playedCardCount, G.playedCardCount + numCardsToReachTwoTreasureCards - numCards + 1, "adventurer card is in the played cards array");

  testAssert(returnValue, 0, "return value");

  // ----------- TEST 5: handpos is out of bounds --------------

  printf("TEST 5: handpos is out of bounds\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(adventurer, choice1, choice2, choice3, &testG, -1, &bonus);

  testAssert(returnValue, -1, "return value should be negative since the handpos is out of bounds");

  // ----------- TEST 6: card at handpos doesn't match adventurer --------------

  printf("TEST 6: card at handpos doesn't match adventurer\n");

  G.hand[thisPlayer][0] = testG.hand[thisPlayer][0] = village;
  G.hand[thisPlayer][0] = testG.hand[thisPlayer][0] = adventurer;
  G.handCount[thisPlayer] = testG.handCount[thisPlayer] = 2;

  handpos = 0;

  testAssert(testG.hand[thisPlayer][handpos], adventurer, "handPos doesn't match adventurer card");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  testAssert(returnValue, -1, "return value should be negative because handPos doesn't match adventurer card");

  // ----------- TEST 7: adventurer card in hand --------------

  printf("TEST 7: adventurer card in hand \n");

  G.hand[thisPlayer][0] = testG.hand[thisPlayer][0] = village;
  G.handCount[thisPlayer] = testG.handCount[thisPlayer] = 1;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  int found = 0;
  for (int i = 0; i < G.handCount[thisPlayer]; i++) {
    if (G.hand[thisPlayer][i] == adventurer) {
      found = 1;
      printf("adventurer card can be played");
    }
  }

  if (!found) {
    testAssert(returnValue, -1, "return value should be negative because adventurer card is not in the player's hand");
  }

  // ----------- TEST 7: kingdom and victory cards unchanged --------------
  printf("TEST 7: kingdom and victory cards unchanged\n");

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  returnValue = cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

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
