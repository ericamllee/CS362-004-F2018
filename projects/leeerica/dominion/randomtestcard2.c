#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

int testAssert(int actual, int expected, char* message) {
  if (actual != expected) {
    printf("TEST FAILED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  }
  return 0;
}

int checkVillage(int choice1, int choice2, int choice3, struct gameState *G, int handPos, int* bonus) {
  int r;
  struct gameState testG;
  memcpy(&testG, G, sizeof(struct gameState));

  r = cardEffect(village, choice1, choice2, choice3, &testG, handPos, bonus);

  testAssert(r, 0, "return value");
  // Check that the supply count of cards is unchanged.
  testAssert(testG.supplyCount[estate], G->supplyCount[estate], "estate unchanged");
  testAssert(testG.supplyCount[duchy], G->supplyCount[duchy], "duchy unchanged");
  testAssert(testG.supplyCount[province], G->supplyCount[province], "province unchanged");
  testAssert(testG.supplyCount[gardens], G->supplyCount[gardens], "gardens unchanged");
  testAssert(testG.supplyCount[curse], G->supplyCount[curse], "curse unchanged");
  testAssert(testG.supplyCount[smithy], G->supplyCount[smithy], "smithy unchanged");
  testAssert(testG.supplyCount[adventurer], G->supplyCount[adventurer], "adventurer unchanged");
  testAssert(testG.supplyCount[embargo], G->supplyCount[embargo], "embargo unchanged");
  testAssert(testG.supplyCount[village], G->supplyCount[village], "village unchanged");
  testAssert(testG.supplyCount[minion], G->supplyCount[minion], "minion unchanged");
  testAssert(testG.supplyCount[mine], G->supplyCount[mine], "mine unchanged");
  testAssert(testG.supplyCount[cutpurse], G->supplyCount[cutpurse], "cutpurse unchanged");
  testAssert(testG.supplyCount[sea_hag], G->supplyCount[sea_hag], "sea_hag unchanged");
  testAssert(testG.supplyCount[tribute], G->supplyCount[tribute], "tribute unchanged");
  testAssert(testG.supplyCount[council_room], G->supplyCount[council_room], "council_room unchanged");

//  Check that the other players hand counts are unchanged.
  for (int player = 0; player < G->numPlayers; player ++) {
    if (player != G->whoseTurn) {
      testAssert(testG.deckCount[player], G->deckCount[player], "deck count unchanged for other player");
      for (int card = 0; card < G->deckCount[player]; card++) {
        testAssert(testG.deck[player][card], G->deck[player][card], "other player's deck card is the same");
      }
      testAssert(testG.discardCount[player], G->discardCount[player], "discard count unchanged for other player");
      for (int card = 0; card < G->discardCount[player]; card++) {
        testAssert(testG.discard[player][card], G->discard[player][card], "other player's discard card is the same");
      }
      testAssert(testG.handCount[player], G->handCount[player], "hand count for other player");
      for (int card = 0; card < G->handCount[player]; card++) {
        testAssert(testG.hand[player][card], G->hand[player][card], "other player's card in hand is the same");
      }
    } else {
//    Check that the current player has two additional actions.
      testAssert(testG.numActions, G->numActions  + 2, "2 new actions");

      testAssert(testG.coins, G->coins, "coin count");
      testAssert(testG.handCount[player], G->handCount[player], "hand count");
      testAssert(testG.playedCardCount, G->playedCardCount + 1, "played card count increased by 1");
      int expectedDeck = G->deckCount[player] <= 0 ? G->discardCount[player] - 1 : G->deckCount[player] - 1;
      testAssert(testG.deckCount[player], expectedDeck, "deck count");
      testAssert(testG.playedCards[testG.playedCardCount - 1], village, "correct card placed in played card pile");
      testAssert(testG.handCount[player] >= 0, 1, "no negative hand count");
      testAssert(testG.discardCount[player] >= 0, 1, "no negative discard count");
      testAssert(testG.deckCount[player] >= 0, 1, "no negative deck count");
    }
  }

  return 0;
}

int main () {
  time_t t;
  srand((unsigned) time(&t));
  SelectStream(2);
  PutSeed(3);

  int k[10] = {smithy, adventurer, embargo, village, minion, mine, cutpurse,
               sea_hag, tribute, council_room};
  struct gameState G;

  printf ("Testing VILLAGE.\n");
  printf ("RANDOM TESTS.\n");

  for (int n = 0; n < 2000; n++) {
    for (int i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }

    int numPlayers = rand() % 3 + 2;
    initializeGame(numPlayers, k, rand(), &G);
    for (int j = 0; j < numPlayers; j++) {
      // ensure that all numbers added together are < 500;
      do {
        G.deckCount[j] = floor(Random() * (MAX_DECK));
        G.discardCount[j] = floor(Random() * MAX_DECK);
        G.handCount[j] = floor(Random() * MAX_HAND);
      } while (G.deckCount[j] + G.discardCount[j] + G.handCount[j] >= 500);
    }
    G.whoseTurn = rand() % numPlayers;
    int choice1 = rand();
    int choice2 = rand();
    int choice3 = rand();
    int bonus = rand();
    int random = rand();
    int handPos;
    if (G.handCount[G.whoseTurn] == 0) {
      handPos = 0;
    } else {
      handPos = random % G.handCount[G.whoseTurn];
    }
    checkVillage(choice1, choice2, choice3, &G, handPos, &bonus);
  }

  printf ("FINISHED TESTING VILLAGE\n");

  return 0;
}
