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
//    printf("TEST SUCCEEDED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
//  } else {
    printf("TEST FAILED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
  }
  return 0;
}

int testGreaterThan(int actual, int num, char* message) {
  if (actual > num) {
//    printf("TEST SUCCEEDED: testing %s. actual: %d. expected: %d.\n", message, actual, expected);
//  } else {
    printf("TEST FAILED: testing %s. expected %d to be greater than %d.\n", message, actual, num);
  }
  return 0;
}

int countTreasureCards(struct gameState *G) {
  int player = G->whoseTurn;
  int treasure = 0;
  for (int i = 0; i < G->deckCount[player]; i++) {
    if (G->deck[player][i] == copper || G->deck[player][i] == silver || G->deck[player][i] == gold) {
      treasure++;
    }
  }

  for (int j = 0; j < G->discardCount[player]; j++) {
    if (G->discard[player][j] == copper || G->discard[player][j] == silver || G->discard[player][j] == gold) {
      treasure++;
    }
  }
  return treasure;
}

int findNumCardsToReachTwoTreasureCards(struct gameState *G) {
  int player = G->whoseTurn;
  int foundCards = 0;
  int countedCards = 0;
  for (int i = G->deckCount[player] - 1; i > -1; i--) {
    if (G->deck[player][i] == copper || G->deck[player][i] == silver || G->deck[player][i] == gold) {
      foundCards++;
      countedCards++;
      if (foundCards == 2) {
        return countedCards;
      }
    } else {
      countedCards++;
    }
  }
  return -1;
}

int checkAdventurer(int choice1, int choice2, int choice3, struct gameState *G, int handPos, int* bonus) {
  const int numCardsToReachTwoTreasureCards = findNumCardsToReachTwoTreasureCards(G);
  struct gameState testG;
  memcpy(&testG, G, sizeof(struct gameState));
  if (numCardsToReachTwoTreasureCards == -1) {
    printf("before sending to Adventurer: original deck: %d, original discard: %d, original hand: %d\n", testG.deckCount[testG.whoseTurn], testG.discardCount[testG.whoseTurn], testG.handCount[testG.whoseTurn]);
  }
  int r = cardEffect(adventurer, choice1, choice2, choice3, &testG, handPos, bonus);
  const int treasureCards = countTreasureCards(G);
  int addedCards = 2;
  if (treasureCards < 2) {
    addedCards = treasureCards;
  }

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
//      for (int card = 0; card < G->discardCount[player]; card++) {
//        testAssert(testG.discard[player][card], G->discard[player][card], "other player's discard card is the same");
//      }
      testAssert(testG.handCount[player], G->handCount[player], "hand count for other player");
//      for (int card = 0; card < G->handCount[player]; card++) {
//        testAssert(testG.hand[player][card], G->hand[player][card], "other player's discard card is the same");
//      }
    } else {
      int cardsPlayedGreaterThan;
      if (numCardsToReachTwoTreasureCards == -1) {
        // This is the case in which the deck needed to be shuffled and there are fewer than two treasure cards.
        if (addedCards < 2) {
          printf("CASE 1 : LESS THAN 2 TREASURE");
          testAssert(testG.deckCount[player], 0, "deck should be empty since there are fewer than 2 treasure cards");
          testAssert(testG.discardCount[player], G->deckCount[player] + G->discardCount[player] - addedCards, "discard count should hold original deck + discard, minus any treasure cards");
        } else {
          printf("CASE 2 : HAD TO SHUFFLE");

//          This is the case in which the deck needs to be shuffled and there are more than 2 treasure cards.
          cardsPlayedGreaterThan = G->deckCount[player];
          int originalDeck = G->deckCount[player];
          int originalDiscard = G->discardCount[player];
//          printf("treasure cards: %d, addedCards: %d\n", treasureCards, addedCards);
//          printf("After shuffling: deck: %d,  discard: %d, hand: %d\n", testG.deckCount[player],
//                 testG.discardCount[player], testG.handCount[player]);
          int numToFindTwoCardsCalculatedFromDiscard = testG.discardCount[player] - originalDeck - addedCards;
          int numToFindTwoCardsCalculatedFromDeck = originalDiscard - originalDeck;
//          printf("numberMoved discard: %d, numberMoved deck: %d\n", numToFindTwoCardsCalculatedFromDiscard,
//                 numToFindTwoCardsCalculatedFromDeck);
          testAssert(numToFindTwoCardsCalculatedFromDeck, numToFindTwoCardsCalculatedFromDiscard,
                     "expect the number of cards moved to be the same");
        }
      } else {
//        printf("No need to shuffle\n");
        testAssert(testG.deckCount[player], G->deckCount[player] - numCardsToReachTwoTreasureCards, "deck count");
        testAssert(testG.discardCount[player], G->discardCount[player] + numCardsToReachTwoTreasureCards - addedCards, "discard count");
      }

      testAssert(testG.coins, G->coins, "coin count");
//      testAssert(testG.handCount[player], G->handCount[player] + addedCards - 1, "hand count");
//      testAssert(testG.playedCardCount, G->playedCardCount + 1, "played card count increased by 1");
//      testAssert(testG.playedCards[testG.playedCardCount - 1], adventurer, "correct card placed in played card pile");

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

  printf ("Testing Adventurer.\n");
  printf ("RANDOM TESTS.\n");

  for (int n = 0; n < 2000; n++) {
    for (int i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }

    int numPlayers = rand() % 3 + 2;
    initializeGame(numPlayers, k, rand(), &G);
    for (int j = 0; j < numPlayers; j++) {
      G.deckCount[j] = floor(Random() * MAX_DECK);
      G.discardCount[j] = floor(Random() * (MAX_DECK - G.deckCount[j]));
      G.handCount[j] = floor(Random() * MAX_HAND);
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
    checkAdventurer(choice1, choice2, choice3, &G, handPos, &bonus);
  }

  printf ("FINISHED TESTING ADVENTURER\n");

  return 0;
}
