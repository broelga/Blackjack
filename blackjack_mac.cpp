
/* Blackjack

This game will be a simple* game where the computer (dealer) will deal two cards to the player (user). The player will have the 
choice to hit, stand, or split (if cards are identical and != A).

*Simple in appearance, not in execution..

Array will contain 52 numbers:
1, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, A * 4 each

numbers will be radnomly assigned to the array (shuffled) and dealt from i = 0, 1 and so on.

Impliment a method of scoring cards:

cards > 10:
J = 10,
Q = 10,
K = 10,
A = 11 || 1.

If A is dealt if player or dealer cards equal > 21, A = 1, else, A = 1.

Need to keep a running total of dealer and player cards, however, if A is drawn and > 21, it must be equal to 1.

Dealer cards must be kept from user, however, dealer's top card can be seen by player. 

Rules:
if dealer and player tie, it is a push and no one wins.
if player has score higher than dealer and is not bust, player wins.
If player and dealer both bust, the dealer wins.
if player is dealt a blackjack (10 card and ace) and the dealer does not, player wins.

Action:
Hit
Stand,
Double down,
Split,

Dealer will hit on 17

 */

#include <iostream>
#include <cstdlib> // Needed for array
#include <iomanip> // Needed for output formatting
#include <chrono>  // Needed for seeding random number generator
#include <random>  // Needed for random number generator
#include <thread>  // Needed for sleep_for()
using namespace std;

// Global constants
const int g_DECK_SIZE = 52;   // Row size of array
const int g_COL = 2;          // Col size of array
const int g_CARD_INDEX = 0;   // Position of cards in array
const int g_POINTS_INDEX = 1; // Position of card points in array

// Global bools for the runtime of the game.
bool running = true; // True while game is running
bool reset = false;  // Used to reset round counters of player and dealer

// Global bools (flags) to be used in the progam.
bool playerTurn = true;   // Flag for when it is the player's turn
bool dealerTurn = false;  // Flag for when player's turn is over
bool blackjack = false;   // Flag for blackjack
bool bust = false;        // Flag for bust
bool doubleDown = false;  // Flag for double down
bool hit = false;         // Flag for hit
bool stand = false;       // Flag for stand
bool canSplit = true;     // Flag for split availability
bool handIsSplit = false; // Flag showing hand has been split

// Function prototypes
void shuffleDeck(int[][g_COL]);                                                     // Function that randomly generates the deck
void cardCounterShuffle(const int, int &, int &, int &, int &, int &, int &, int &, // Counters used to keep track of the number of each card in deck while
                        int &, int &, int &, int &, int &, int &, int &);           // deck is generated
int cardPoints(const int);                                                          // Function that assigns the points for each card in deck
void dealCards(int[][g_COL], int[][g_COL], int[][g_COL], int[][g_COL], const int,   // Function that deals the initial cards to player and dealer
               unsigned &, unsigned &, unsigned, unsigned &, int &, int &, int &);
void dealOneCardPlayer(int[][g_COL], int[][g_COL], int[][g_COL], unsigned &, // Function that deals one card to player
                       unsigned &, unsigned &, int &, int &);
void dealOneCardDealer(int[][g_COL], int[][g_COL], unsigned &, unsigned &, int &); // Function that deals one card to dealer
void playerSplitHand(int[][g_COL], int[][g_COL], unsigned &, unsigned &, int &);   // Function that handles the split hand of the player
void splitOneHand(int[][g_COL], int[][g_COL], unsigned &, unsigned &);             // Splits hand of player into two separate hands with one card each
void showHandPlayer(const int[][g_COL], const unsigned);                           // Function that displays the hand of the player
void showHandSplit(const int[][g_COL], const unsigned);                            // Function to display the split hand of the player
void showHandDealer(const int[][g_COL], const unsigned);                           // Function that displays the hand of the dealer
void showDeck(const int[][g_COL]);                                                 // Function that displays the entire deck (only used for testing purposes)
int sumHand(const int[][g_COL], const int);                                        // Function that sums the hand that is used to call the function
void showSum(const int);                                                           // Function that displays the sum of the hand that is used to call the function
void checkScore(const int, const unsigned);                                        // Function that checks score of the hand used to call the function
void callBlackjack();                                                              // Function that displays blackjack to player
void callBust();                                                                   // Function that displays bust to player
void declareWinner(const int, const int, const int);                               // Function that declares the winner
void playAgain();                                                                  // Function that prompts the player to play again

//*****************************************************************************************************************************************************************
//              Split               *
//***********************************
// TODO: If hand is split, there needs to be a way to be able to hit/stand on each hand, rather than one option.
// TODO: Create additional menu to display further option if handIsSplit
// TODO: Right now if one hand bust, player loses. Create separate entities of playerHand and splitHand
// This will allow player to control each hand separately. Scoring should be unique as well.
//*****************************************************************************************************************************************************************

int main()
{
    // Begin program loop
    while (running)
    {
        // Variables for game
        const int HAND_SIZE = 10;                            // Max size of the hands
        int deck[g_DECK_SIZE][g_COL];                        // Array for card deck
        int playerHand[HAND_SIZE][g_COL];                    // Array for player's hand
        int dealerHand[HAND_SIZE][g_COL];                    // Array for dealer's hand
        int splitHand[HAND_SIZE][g_COL];                     // Array for player's split hand
        unsigned totalNumCardsDealt = 0, numPlayerCards = 0, // Keeps track of the number of cards for each array
            numPlayerCardsSplit = 0, numDealerCards = 0;
        int playerSum = 0, dealerSum = 0, playerSumSplit = 0; // Stores the sum of each hand's score
        unsigned choice;                                      // Used for player choice of options
        bool isValid;                                         // Used for input validation

        // Clears screen
        system("clear");

        // Deals cards to player and dealer
        dealCards(deck, playerHand, splitHand, dealerHand, HAND_SIZE, totalNumCardsDealt, numPlayerCards,
                  numPlayerCardsSplit, numDealerCards, playerSum, playerSumSplit, dealerSum);

        //***************************************
        //             Player's turn            *
        //***************************************
        // Loop player turn until stand, bust, or blackjack
        while (playerTurn)
        {
            // Player should also see dealer's one card.
            if (stand)
            {
                this_thread::sleep_for(chrono::milliseconds(600));
                playerTurn = false;
                dealerTurn = true;
            }
            else if (bust)
            {
                // Call bust and ends loop
                this_thread::sleep_for(chrono::milliseconds(600));
                callBust();
                playerTurn = false;
                dealerTurn = true;
            }
            else if (blackjack)
            {
                // Call blackjack and ends loop
                this_thread::sleep_for(chrono::milliseconds(600));
                callBlackjack();
                playerTurn = false;
                dealerTurn = true;
            }
            else if (canSplit && !handIsSplit)
            {
                // To be used after initial deal if split is available
                do
                {
                    // Prompt menu
                    // Function call: if (split) then this.
                    cout << "1. Hit\n"
                         << "2. Stand\n"
                         << "3. Double down\n"
                         << "4. Split\n\n"
                         << "Enter selection: " << flush;
                    cin >> choice;

                    switch (choice)
                    {
                    case 1:
                        // Clears screen, prompts game data
                        system("clear");
                        showHandDealer(dealerHand, numDealerCards);
                        showSum(dealerSum);
                        dealOneCardPlayer(deck, playerHand, splitHand, totalNumCardsDealt, numPlayerCards,
                                          numPlayerCardsSplit, playerSum, playerSumSplit);
                        isValid = true;
                        break;
                    case 2:
                        // Player stands and ends loop
                        stand = true;
                        isValid = true;
                        break;
                    case 3:
                        // Player chooses to double down (not functional yet)
                        doubleDown = true;
                        isValid = true;
                        break;
                    case 4:
                        // Clears screen, prompts game data
                        system("clear");
                        handIsSplit = true;
                        showHandDealer(dealerHand, numDealerCards);
                        showSum(dealerSum);
                        splitOneHand(playerHand, splitHand, numPlayerCards, numPlayerCardsSplit);
                        dealOneCardPlayer(deck, playerHand, splitHand, totalNumCardsDealt, numPlayerCards,
                                          numPlayerCardsSplit, playerSum, playerSumSplit);
                        // playerSplitHand(deck, splitHand, totalNumCardsDealt, numPlayerCardsSplit, playerSumSplit);
                        isValid = true;
                        break;
                    default: // Input validation
                        // Clears error status
                        if (!cin)
                            cin.clear();

                        // Clears screen, shows invalid choice, reprompts game data
                        system("clear");
                        cout << "invalid choice.\n";
                        cin.ignore(256, '\n');
                        showHandDealer(dealerHand, numDealerCards);
                        showSum(dealerSum);
                        showHandPlayer(playerHand, numPlayerCards);
                        showSum(playerSum);
                        // checkScore(playerSum, round);
                    }
                } while (!isValid);
            }
            else
            {
                do
                {
                    // Prompt menu
                    // To be used after initial deal if no split is available and subsequent deals
                    // Function call: else, then this. (Or can be if (!split))
                    cout << "1. Hit\n"
                         << "2. Stand\n"
                         << "3. Double down\n\n"
                         << "Enter selection: " << flush;
                    cin >> choice;

                    switch (choice)
                    {
                    case 1:
                        // Clears screen, prompts game data
                        system("clear");
                        showHandDealer(dealerHand, numDealerCards);
                        showSum(dealerSum);
                        dealOneCardPlayer(deck, playerHand, splitHand, totalNumCardsDealt, numPlayerCards,
                                          numPlayerCardsSplit, playerSum, playerSumSplit);
                        isValid = true;
                        break;
                    case 2:
                        // Player stands and ends loop
                        stand = true;
                        isValid = true;
                        break;
                    case 3:
                        // Player chooses to double down (not functional yet)
                        doubleDown = true;
                        isValid = true;
                        break;
                    default: // Input validation
                        // Clears error status
                        if (!cin)
                            cin.clear();

                        // Clears screen, shows invalid choice, reprompts game data
                        system("clear");
                        cout << "invalid choice.\n";
                        cin.ignore(256, '\n');
                        isValid = false;
                        showHandDealer(dealerHand, numDealerCards);
                        showSum(dealerSum);
                        showHandPlayer(playerHand, numPlayerCards);
                        showSum(playerSum);
                        // checkScore(playerSum, round);
                    }
                } while (!isValid);
            }
        }

        if (bust)
        {
            // Let the player see their results before moving on
            cout << "Press enter to contiue...";
            cin.ignore(100, '\n');
            cin.get();
        }
        else if (blackjack)
        {
            // Let the player see their results before moving on
            cout << "Press enter to contiue...";
            cin.get();
        }
        else // Clears buffer
            cin.ignore(100, '\n');

        //***************************************
        //             Dealer's turn            *
        //***************************************
        while (dealerTurn && !playerTurn)
        {
            // Clear screen
            system("clear");

            // Dealer is dealt a card and plays
            dealOneCardDealer(deck, dealerHand, totalNumCardsDealt, numDealerCards, dealerSum);

            // Depending on flag, dealer will choose its move
            if (hit)
            {
                unsigned round = 1;
                string dealerHits = "Dealer is hitting. . .";
                this_thread::sleep_for(chrono::milliseconds(50));

                for (int i = 0; i < dealerHits.length(); i++)
                {
                    if (i < (dealerHits.length() - 5))
                    {
                        this_thread::sleep_for(chrono::milliseconds(20));
                        cout << dealerHits[i] << flush;
                    }
                    else
                    {
                        this_thread::sleep_for(chrono::milliseconds(300));
                        cout << dealerHits[i] << flush;
                    }
                }

                this_thread::sleep_for(chrono::milliseconds(200));
                cout << endl;

                continue;
            }

            else if (stand)
            {
                // Stand is flagged, loop ends
                this_thread::sleep_for(chrono::milliseconds(600));
                dealerTurn = false;
            }

            else if (blackjack)
            {
                // Blackjack is declared, loop ends
                this_thread::sleep_for(chrono::milliseconds(600));
                callBlackjack();
                dealerTurn = false;
            }
            else if (bust)
            {
                // Dealer busts, loop ends.
                this_thread::sleep_for(chrono::milliseconds(600));
                callBust();
                dealerTurn = false;
            }
        }

        // Game is over and finals are displayed
        cout << "Press enter to see the results...";
        cin.get();
        system("clear");

        // Show player and dealer hands and scores
        showHandDealer(dealerHand, numDealerCards);
        showSum(dealerSum);
        showHandPlayer(playerHand, numPlayerCards);
        showSum(playerSum);
        
        if (handIsSplit)
        {
            showHandSplit(splitHand, numPlayerCardsSplit);
            showSum(playerSumSplit);
        }

        // Declare winner and prompt to play again
        declareWinner(playerSum, playerSumSplit, dealerSum);
        playAgain();
    }
}

//***************************
//  Function defintions     *
//***************************

// **************************************************************************
// Function that prompts the player to play again                           *
// **************************************************************************
void playAgain()
{
    // Local variables
    char choice;
    bool isValid;

    do
    {
        cout << "Play again? (Y/N): ";
        cin >> choice;
        cin.ignore(100, '\n');
        choice = toupper(choice);

        if (choice == 'Y' || choice == 'N')
        {
            if (choice == 'Y')
            {
                running = true,
                reset = true,
                playerTurn = true,
                dealerTurn = false,
                blackjack = false,
                bust = false,
                doubleDown = false,
                hit = false,
                stand = false,
                canSplit = false,
                handIsSplit = false;
                isValid = true;
            }
            else
                running = false;
            isValid = true;
        }
        else
            cout << "Invalid choice.\n";
    } while (!isValid);

    return;
}

// **************************************************************************
// Function that declares the winner                                        *
// **************************************************************************
void declareWinner(const int playerSum, const int playerSumSplit, const int dealerSum)
{
    if (dealerSum <= 21 && playerSum <= 21)
    {
        if (dealerSum > playerSum)
            cout << "You lose!";

        else if (playerSum > dealerSum)
            cout << "You win!";

        else if (dealerSum == playerSum)
            cout << "Push!";
    }
    else if (dealerSum > 21 && playerSum <= 21)
        cout << "You win!";

    else if (dealerSum <= 21 && playerSum > 21)
        cout << "You lose!";

    else
        cout << "You lose!";

    cout << endl;
}

// **************************************************************************
// Function that displays blackjack to player                               *
// **************************************************************************
void callBlackjack()
{
    // Function call: if (blackjack)
    cout << "BLACK JACK!\n"
         << flush;
}

// **************************************************************************
// Function that displays bust to player                                    *
// **************************************************************************
void callBust()
{
    // Function call: if (bust)
    cout << "BUST!\n"
         << flush;
}

// **************************************************************************
// Function that checks score of the hand used to call the function         *
// **************************************************************************
void checkScore(const int sum, const unsigned round)
{
    // Reset global bools at beginning of function
    blackjack = false, bust = false, doubleDown = false, hit = false, stand = false;

    // assign bool (if any) and return to main().
    // Dealer's play
    if (dealerTurn && !playerTurn)
    {
        if (round == 2 && sum == 21)
        {
            blackjack = true;
            return;
        }

        else if (sum <= 17)
        {
            hit = true;
            return;
        }

        else if (sum > 17 && sum <= 21)
        {
            stand = true;
            return;
        }

        else if (sum > 21)
        {
            bust = true;
            return;
        }
    }
    else
    {
        if (sum > 21)
        {
            bust = true;
            return;
        }
        else if (round == 2 && sum == 21)
        {
            blackjack = true;
            return;
        }
        else if (sum < 21)
            return;
    }
}

// **************************************************************************
// Function that displays the sum of the hand that is used to call the      *
// function                                                                 *
// **************************************************************************
void showSum(const int sum)
{
    cout << "Total: " << sum << "\n\n"
         << flush;
}

// **************************************************************************
// Function that sums the hand that is used to call the function            *
// **************************************************************************
int sumHand(const int hand[][g_COL], const int numCards)
{
    // Local variables
    int sum = 0;

    for (int i = 0; i < numCards; i++)
    {
        for (int j = 0; j < 1; j++)
            sum += hand[i][g_POINTS_INDEX];
    }
    return sum;
}

// **************************************************************************
// Function that deals one card to player                                   *
// **************************************************************************
void dealOneCardPlayer(int deck[][g_COL], int playerHand[][g_COL], int splitHand[][g_COL], unsigned &totalNumCardsDealt,
                       unsigned &numPlayerCards, unsigned &numPlayerCardsSplit, int &playerSum, int &playerSumSplit)
{
    // Local variables
    static unsigned round = 1;  // Keeps count of number of rounds or times the function has been called.
    int i = numPlayerCards;     // Stores the number of cards in the hand
    int j = totalNumCardsDealt; // Stores the total number of cards dealt (player and dealer)

    // If game has been reset, round restart at 1
    if (reset)
        round = 1;

    // ***************
    // Player's Hand
    // ***************
    // Deal one card, count cards dealt
    playerHand[i][g_CARD_INDEX] = deck[j][g_CARD_INDEX];     // Assigns card [j,0] to [i,0]
    playerHand[i][g_POINTS_INDEX] = deck[j][g_POINTS_INDEX]; // Assigns score of [j,1] to [i, 1]
    numPlayerCards++;
    totalNumCardsDealt++;

    // Player sum is computed before new card is checked for ace
    playerSum = sumHand(playerHand, numPlayerCards);

    // If there is an ace in the hand and card drawn busts, set the ace points = 1 (only one ace can be changed to score of 1)
    if (playerSum > 21)
    {
        unsigned numAces = 0;

        for (int i = 0; i < numPlayerCards; i++)
        {
            if (playerHand[i][g_CARD_INDEX] == 14 && numAces < 1)
            {
                playerHand[i][g_POINTS_INDEX] = 1;
                playerSum = sumHand(playerHand, numPlayerCards);
                numAces++;
            }
        }
    }

    // Display hand and total score of playerHand
    if (round > 1)
    {
        showHandPlayer(playerHand, numPlayerCards);
        showSum(playerSum);
        checkScore(playerSum, round);
    }

    // Deal one card to playerSplitHand deal with score and display
    if (handIsSplit)
    {
        playerSplitHand(deck, splitHand, totalNumCardsDealt, numPlayerCardsSplit, playerSumSplit);

        // Compute sum of cards in hand
        playerSumSplit = sumHand(splitHand, numPlayerCardsSplit);

        // Display hand and total score
        showHandSplit(splitHand, numPlayerCardsSplit);
        showSum(playerSumSplit);
        checkScore(playerSumSplit, round);
    }
    // Check first two cards and see if they are equal. If so, player is allowed to split
    if (round == 2)
    {
        if (playerHand[0][g_CARD_INDEX] == playerHand[1][g_CARD_INDEX])
            canSplit = true;
    }
    else if (round > 2)
        canSplit = false;

    round++;
}

// **************************************************************************
// Function that deals one card to dealer                                   *
// **************************************************************************
void dealOneCardDealer(int deck[][g_COL], int hand[][g_COL], unsigned &totalNumCardsDealt, unsigned &numDealerCards, int &dealerSum)
{
    // Local variables
    static unsigned round = 1;  // Keeps count of number of rounds or times the function has been called.
    int i = numDealerCards;     // Stores the number of cards in the hand
    int j = totalNumCardsDealt; // Stores the total number of cards dealt (player and dealer)

    // If game has been reset, round restart at 1 and reset is made false
    if (reset)
    {
        round = 1;
        reset = false;
    }

    // Deal one card, count cards dealt
    hand[i][g_CARD_INDEX] = deck[j][g_CARD_INDEX];     // Assigns card [j,0] to [i,0]
    hand[i][g_POINTS_INDEX] = deck[j][g_POINTS_INDEX]; // Assigns score of [j,1] to [i, 1]
    numDealerCards++;
    totalNumCardsDealt++;

    // Dealer sum is computed before new card is checked for ace
    dealerSum = sumHand(hand, numDealerCards);

    // If there is an ace in the hand and card drawn busts, set the ace points = 1 (only one ace can be changed to score of 1)
    if (dealerSum > 21)
    {
        unsigned numAces = 0;

        for (int i = 0; i < numDealerCards; i++)
        {
            if (hand[i][g_CARD_INDEX] == 14 && numAces < 1)
            {
                hand[i][g_POINTS_INDEX] = 1;
                dealerSum = sumHand(hand, numDealerCards);
                numAces++;
            }
        }
    }

    if (round > 1)
        checkScore(dealerSum, round);

    // Display hand and total score
    showHandDealer(hand, numDealerCards);
    showSum(dealerSum);

    round++;
}

// **************************************************************************
// Function that splits the hand, assign one card from one deck to the other*
// **************************************************************************
// Works
void splitOneHand(int hand[][g_COL], int splitHand[][g_COL], unsigned &numCardsHand, unsigned &numCardsSplit)
{
    splitHand[0][g_CARD_INDEX] = hand[1][g_CARD_INDEX]; // Assigns card [j,0] to [i,0]
    splitHand[0][g_POINTS_INDEX] = hand[1][g_POINTS_INDEX];
    numCardsHand--;
    numCardsSplit++;
}

// **************************************************************************
// Function that deals the initial cards to player and dealer               *
// **************************************************************************
void dealCards(int deck[][g_COL], int playerHand[][g_COL], int splitHand[][g_COL], int dealerHand[][g_COL], const int handSize,
               unsigned &totalNumCardsDealt, unsigned &numPlayerCards, unsigned numPlayerCardsSplit, unsigned &numDealerCards,
               int &playerSum, int &playerSumSplit, int &dealerSum)
{
    // Local variable
    int num = 0;

    shuffleDeck(deck);

    // Deals one card to player, then dealer, then player
    dealOneCardPlayer(deck, playerHand, splitHand, totalNumCardsDealt, numPlayerCards,
                      numPlayerCardsSplit, playerSum, playerSumSplit);
    dealOneCardDealer(deck, dealerHand, totalNumCardsDealt, numDealerCards, dealerSum);
    dealOneCardPlayer(deck, playerHand, splitHand, totalNumCardsDealt, numPlayerCards,
                      numPlayerCardsSplit, playerSum, playerSumSplit);
}

// **************************************************************************
// Function that handles the split hand of the player                       *
// **************************************************************************
void playerSplitHand(int deck[][g_COL], int hand[][g_COL], unsigned &totalNumCardsDealt,
                     unsigned &numPlayerCardsSplit, int &playerSumSplit)
{
    // Local variables
    static unsigned round = 1;   // Keeps count of number of rounds or times the function has been called.
    int i = numPlayerCardsSplit; // Stores the number of cards in the hand
    int j = totalNumCardsDealt;  // Stores the total number of cards dealt (player and dealer)

    if (reset)
        round = 1;

    // Deal one card, count cards dealt
    hand[i][g_CARD_INDEX] = deck[j][g_CARD_INDEX];
    hand[i][g_POINTS_INDEX] = deck[j][g_POINTS_INDEX];
    numPlayerCardsSplit++;
    totalNumCardsDealt++;
}

// **************************************************************************
// Function that displays the hand of the player                            *
// **************************************************************************
void showHandPlayer(const int playerHand[][g_COL], const unsigned numPlayerCards)
{
    // Local variables
    unsigned numDigits = 0, highest = 0; // Used for output formatting
    int cardWidth;                       // Used for output formatting
    string cardCap, cardWall = "|";      // Used for output formatting

    // Peak at cards in hand for output formatting
    // for (int i = 0; i < numCards; i++)
    // {
    //     for (int j = 0; j < 1; j++)
    //     {
    //         numDigits = hand[i][j];

    //         if (numDigits > highest)
    //             highest = numDigits;
    //     }
    // }

    // if (sizeof(highest) == 1)
    // {
    //     cardWidth = 2;
    //     cardCap = "____";
    // }
    // else
    // {
    //     cardWidth = 2;
    //     cardCap = "____";
    // }

    cout << "Player Cards:\n"
         << flush;

    if (handIsSplit)
        cout << "\nHand #1:\n"
             << flush;

    // for (int i = 0; i < numCards; i++)
    //     cout << cardCap << " ";
    // cout << endl;

    for (int i = 0; i < numPlayerCards; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            int card = playerHand[i][g_CARD_INDEX];

            if (card == 0)
                break;

            else
            {
                switch (card)
                {
                case 11:
                    cout << "J"
                         //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                         << flush;
                    break;
                case 12:
                    cout << "Q"
                         //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                         << flush;
                    break;
                case 13:
                    cout << "K"
                         //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                         << flush;
                    break;
                case 14:
                    cout << "A"
                         //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                         << flush;
                    break;
                default:
                    cout << playerHand[i][g_CARD_INDEX] /* << " " << hand[i][g_POINTS_INDEX] << "\n" */
                         << flush;
                }
            }
        }
        cout << setw(3) << flush;
    }
    cout << "\n\n";
}

void showHandSplit(const int splitHand[][g_COL], const unsigned numCards)
{
    // For split hand display
    if (handIsSplit)
    {
        cout << "Hand #2:\n"
             << flush;

        for (int i = 0; i < numCards; i++)
        {
            for (int j = 0; j < 1; j++)
            {
                int card = splitHand[i][g_CARD_INDEX];

                if (card == 0)
                    break;

                else
                {
                    switch (card)
                    {
                    case 11:
                        cout << "J"
                             //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                             << flush;
                        break;
                    case 12:
                        cout << "Q"
                             //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                             << flush;
                        break;
                    case 13:
                        cout << "K"
                             //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                             << flush;
                        break;
                    case 14:
                        cout << "A"
                             //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                             << flush;
                        break;
                    default:
                        cout << splitHand[i][g_CARD_INDEX] /* << " " << hand[i][g_POINTS_INDEX] << "\n" */
                             << flush;
                    }
                }
            }
            cout << setw(3) << flush;
        }
    }
    cout << "\n\n";
}

// **************************************************************************
// Function that displays the hand of the dealer                            *
// **************************************************************************
void showHandDealer(const int hand[][g_COL], const unsigned numCards)
{
    // Local variables
    unsigned numDigits = 0, highest = 0; // Used for output formatting
    int cardWidth;                       // Used for output formatting
    string cardCap, cardWall = "|";      // Used for output formatting

    // Peak at cards in hand for output formatting
    // for (int i = 0; i < numCards; i++)
    // {
    //     for (int j = 0; j < 1; j++)
    //     {
    //         numDigits = hand[i][j];

    //         if (numDigits > highest)
    //             highest = numDigits;
    //     }
    // }

    // if (sizeof(highest) == 1)
    // {
    //     cardWidth = 2;
    //     cardCap = "____";
    // }
    // else
    // {
    //     cardWidth = 2;
    //     cardCap = "____";
    // }

    cout << "Dealer cards: \n"
         << flush;

    // for (int i = 0; i < numCards; i++)
    //     cout << cardCap << " ";
    // cout << endl;

    for (int i = 0; i < numCards; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            int card = hand[i][g_CARD_INDEX];

            if (card == 0)
                return;

            else
            {
                switch (card)
                {
                case 11:
                    cout << "J"
                         //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                         << flush;
                    break;
                case 12:
                    cout << "Q"
                         //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                         << flush;
                    break;
                case 13:
                    cout << "K"
                         //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                         << flush;
                    break;
                case 14:
                    cout << "A"
                         //  << " " << hand[i][g_POINTS_INDEX] << "\n"
                         << flush;
                    break;
                default:
                    cout << hand[i][g_CARD_INDEX] /* << " " << hand[i][g_POINTS_INDEX] << "\n" */
                         << flush;
                }
            }
        }
        cout << setw(3) << flush;
    }
    cout << "\n\n";
}

// **************************************************************************
// Function that displays the entire deck (only used for testing purposes)  *
// **************************************************************************
void showDeck(const int deck[][g_COL])
{
    // Local variables
    int card; // Stores the card number in the deck array
    int counter = 0;

    for (int i = 0; i < g_DECK_SIZE; i++)
    {
        for (int j = 1; j < 2; j++)
        {
            card = deck[i][g_CARD_INDEX];

            // Cards over 10 are displayed as letters. Also displays the points of each card.
            switch (card)
            {
            case 11:
                cout << "J"
                     << " " << deck[i][g_POINTS_INDEX] << "\n"
                     << flush;
                break;
            case 12:
                cout << "Q"
                     << " " << deck[i][g_POINTS_INDEX] << "\n"
                     << flush;
                break;
            case 13:
                cout << "K"
                     << " " << deck[i][g_POINTS_INDEX] << "\n"
                     << flush;
                break;
            case 14:
                cout << "A"
                     << " " << deck[i][g_POINTS_INDEX] << "\n"
                     << flush;
                break;
            default:
                cout << deck[i][g_CARD_INDEX] << " " << deck[i][g_POINTS_INDEX] << "\n"
                     << flush;
            }
            counter++;
        }
    }
    // cout << "\n# of Cards\n";
    // cout << counter << "\n\n";
}

// **************************************************************************
// This function will generate the deck making sure there are 4 of each     *
// card in deck                                                             *
// **************************************************************************
void shuffleDeck(int deck[][g_COL])
{
    // Local variables
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();          // Create the seed for the random number generator.
    mt19937 generator(seed);                                                         // mt19937 is more random than srand
    int card1 = 0, card2 = 0, card3 = 0, card4 = 0, card5 = 0, card6 = 0, card7 = 0, // Counters
        card8 = 0, card9 = 0, card10 = 0, cardJ = 0, cardQ = 0, cardK = 0, cardA = 0;
    int card = 0, points = 0; // Stores the number and score of cards

    // Start assigning cards to deck in random order from 1 to 14
    for (int i = 0; i < g_DECK_SIZE; i++)
    {
        deck[i][g_CARD_INDEX] = (generator() % 14 + 1);

        // Intialize card with the number in the deck array for ease of passing
        card = deck[i][g_CARD_INDEX];

        // Check the card assigned and count the number of that card already in the deck. If there are already 4 of that card,
        // the iter will be decremented and the card replaced with a new card.
        cardCounterShuffle(card, card1, card2, card3, card4, card5, card6,
                           card7, card8, card9, card10, cardJ, cardQ, cardK, cardA);

        if (card1 > 4 || card2 > 4 || card3 > 4 || card4 > 4 || card5 > 4 || card6 > 4 || card7 > 4 ||
            card8 > 4 || card9 > 4 || card10 > 4 || cardJ > 4 || cardQ > 4 || cardK > 4 || cardA > 4)
        {
            if (card1 > 4)
            {
                if (card == 1)
                    i--;
            }
            else if (card2 > 4)
            {
                if (card == 2)
                    i--;
            }
            else if (card3 > 4)
            {
                if (card == 3)
                    i--;
            }
            else if (card4 > 4)
            {
                if (card == 4)
                    i--;
            }
            else if (card5 > 4)
            {
                if (card == 5)
                    i--;
            }
            else if (card6 > 4)
            {
                if (card == 6)
                    i--;
            }
            else if (card7 > 4)
            {
                if (card == 7)
                    i--;
            }
            else if (card8 > 4)
            {
                if (card == 8)
                    i--;
            }
            else if (card9 > 4)
            {
                if (card == 9)
                    i--;
            }
            else if (card10 > 4)
            {
                if (card == 10)
                    i--;
            }
            else if (cardJ > 4)
            {
                if (card == 11)
                    i--;
            }
            else if (cardQ > 4)
            {
                if (card == 12)
                    i--;
            }
            else if (cardK > 4)
            {
                if (card == 13)
                    i--;
            }
            else if (cardA > 4)
            {
                if (card == 14)
                    i--;
            }
        }
    }

    // Assign the points for each card in the deck
    for (int i = 0; i < g_DECK_SIZE; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            card = deck[i][g_CARD_INDEX];

            points = cardPoints(card);
            deck[i][g_POINTS_INDEX] = points;
        }
    }
}

// **************************************************************************
// Counters used to keep track of the number of each card in deck while     *
// deck is generated                                                        *
// **************************************************************************
void cardCounterShuffle(const int card, int &card1, int &card2, int &card3, int &card4,
                        int &card5, int &card6, int &card7, int &card8, int &card9,
                        int &card10, int &cardJ, int &cardQ, int &cardK, int &cardA)
{
    // Counters for cards in deck and returns to shuffleDeck().
    switch (card)
    {
    case 1:
        card1++;
        break;
    case 2:
        card2++;
        break;
    case 3:
        card3++;
        break;
    case 4:
        card4++;
        break;
    case 5:
        card5++;
        break;
    case 6:
        card6++;
        break;
    case 7:
        card7++;
        break;
    case 8:
        card8++;
        break;
    case 9:
        card9++;
        break;
    case 10:
        card10++;
        break;
    case 11:
        cardJ++;
        break;
    case 12:
        cardQ++;
        break;
    case 13:
        cardK++;
        break;
    case 14:
        cardA++;
        break;
    }
}

// **************************************************************************
// Function that assigns the points for each card in deck                   *
// **************************************************************************
int cardPoints(const int card)
{
    // Local variable
    int points = 0; // Stores the points the card is worth and returns it to the functioned that called it

    switch (card)
    {
    case 1:
        points = 1;
        break;
    case 2:
        points = 2;
        break;
    case 3:
        points = 3;
        break;
    case 4:
        points = 4;
        break;
    case 5:
        points = 5;
        break;
    case 6:
        points = 6;
        break;
    case 7:
        points = 7;
        break;
    case 8:
        points = 8;
        break;
    case 9:
        points = 9;
        break;
    case 10:
    case 11:
    case 12:
    case 13:
        points = 10;
        break;
    case 14:
        points = 11;
        break;
    }

    return points;
}