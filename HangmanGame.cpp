// Hangman in C++
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

/*
- Implement x under vowels - Done
- Create pretty game box - Done
- Create hangman icon - Done
- Create options menu - Done
- Create single user input function - Done
- Remove global variables - Done
-
- Get random word on game start

- Make this a collection of games
*/

// Player Settings
bool settingShowVowelMarks = false;

// Global variables
char userInput;

// Display
void printArtHorizontalLine()
{
  for (int i=0; i<60; i++) cout << '/';
}
void printGameBox() //Empty
{

}
void printHangman(int wrongGuesses) //Empty
{
  const int numRows = 10;
  const int numCols = 20;
  char hangmanPicture[numRows][numCols]
  {
    {"     ________      "}, // row 0
    {"     ||//   |      "},
    {"     ||/    1      "},
    {"     ||    213     "},
    {"     ||    4 5     "}, // row 4
    {"   __||__________  "},
    {"  / /||\\        /| "},
    {" /____________ / | "},
    {"|             | /  "},
    {"|_____________|/   "} // row 9
    /*
      Head: row 2 col 12
      Body: row 3 col 12
      LArm: row 3 col 11
      RArm: row 3 col 13
      LLeg: row 4 col 11
      RLeg: row 4 col 13
    */
  };

  // Modify hangman picture
  switch (wrongGuesses)
  {
    case 5:
      hangmanPicture[4][13] = '\\';
    case 4:
      hangmanPicture[4][11] = '/';
    case 3:
      hangmanPicture[3][13] = '\\';
    case 2:
      hangmanPicture[3][11] = '/';
    case 1:
      hangmanPicture[2][12] = 'O';
      hangmanPicture[3][12] = '|';
    // Print hangman picture
    default:
      for (int row=0; row<numRows; row++)
      {
        for (int col=0; col<numCols; col++)
        {
          cout << hangmanPicture[row][col];
        }
        cout << '\n';
      }
  }
}

// Printing Game Letters
void printGuessedLetters(int guessNumber, char guessedLetters[])
{
  cout << "Guesses so far: ";
  for (int i=1; i<guessNumber; i++)
  {
    cout << guessedLetters[i] << ' ';
  }
  cout << '\n';
}
void printRevealedWord(char revealedWord[], int mysteryWordLength)
{
  cout << "Revealed word:\t";
  for (int i=0; i<mysteryWordLength; i++) cout << revealedWord[i] << ' ';
  cout << '\n';
}
void printRevealedWordVowelMarks(char mysteryWord[], int mysteryWordLength, const char (& vowels)[5])
{
  if (settingShowVowelMarks)
  {
    bool isAVowel = false;
    cout << "\t\t";

    for (int i=0; i<mysteryWordLength; i++)
    {
      for (char d : vowels)
      {
        if (mysteryWord[i] == d)
        {
          isAVowel = true;
          cout << 'x';
          break;
        }
      }
      if (!isAVowel) cout << '-';
      cout << ' ';
      isAVowel = false;
    }
  }
  cout << "\n\n";
}
void printWrongGuesses(int numberOf)
{
  cout << "You have guessed wrong " << numberOf << " times.\n";
}
void printVictoryMessage()
{
  cout << "Congratulations! You won!";
}
void printDefeatMessage(char mysteryWord[])
{
  cout << "I'm sorry, you lost.";
  cout << "\n\nThe answer is: " << mysteryWord;
}
void printGameText()
{

}

// Character Handling and Checking
int convertCharToInt(char input)
{
  return input - 48;
}
bool checkIfUserInputIsOneChar(string input)
{
  if (input.length() != 1)
    return false;
  else
    return true;
}
bool getUserInput(string outputType)
{
  if (outputType == "char")
    cout << "Please enter a letter: ";
  else if (outputType == "int")
    cout << "Please enter a number: ";
  string input;
  char inputChar;
  getline (cin, input);
  if (checkIfUserInputIsOneChar(input))
  {
    stringstream (input) >> inputChar;
    if (outputType == "char")
    {
      if (isalpha(inputChar))
      {
        if (islower(inputChar))
        {
          inputChar = toupper(inputChar);
        }
        userInput = inputChar;
        return true;
      }
    }
    else if (outputType == "int")
    {
      if (isdigit(inputChar))
      {
        userInput = convertCharToInt(inputChar);
        return true;
      }
    }
  }
  cout << "Please enter a valid option";
  return false;
}
/*void resetGame(int wrongGuesses, int guessNumber)
{

}
*/

bool inputWasNotAlreadyGuessed(int guessNumber, char guessedLetters[])
{
  for (int i=1; i<=guessNumber; i++)
  {
    if (userInput == guessedLetters[i])
    {
      cout << "You already guessed that letter!";
      return false;
    }
  }
  return true;
}
void addGuessToGuessedLetters(int guessNumber, char guessedLetters[])
{
  guessedLetters[guessNumber] = userInput;
}
bool guessIsCorrect(char mysteryWord[], int mysteryWordLength)
{
  for (int i=0; i < mysteryWordLength; i++)
  {
    if (mysteryWord[i] == userInput)
    {
      cout << "Correct!";
      return true;
    }
  }
  cout << "Incorrect!";
  return false;
}
void revealLetters(char mysteryWord[], int mysteryWordLength, char revealedWord[])
{
  for (int i=0; i < mysteryWordLength; i++)
  {
    if (mysteryWord[i] == userInput) revealedWord[i] = userInput;
  }
}
bool playerHasWon(char mysteryWord[], int mysteryWordLength, char revealedWord[])
{
  int revealedLetters = 0;
  for (int i=0; i < mysteryWordLength; i++)
  {
    if (revealedWord[i] == '_')
    {
      revealedLetters = 0;
      return false;
    }
    revealedLetters++;
  }
  if (revealedLetters == mysteryWordLength)
  {
    return true;
  }
  cout << "Error with playerHasWon\n";
  return false;
}
bool playerHasLost(int wrongGuesses)
{
  if (wrongGuesses == 5) return true;
  else return false;
}

// Menu
void printOptionsMenu()
{
  cout << "Options Menu\n";
  cout << "Please select an option:\n";
  cout << "\t1. Show vowel marks\t[";
  if (settingShowVowelMarks) cout << "o]\n";
  else cout << " ]\n";
  cout << "\t2. Return to main menu\n";
}
void gameOptionsMenu()
{
  bool inGameOptionsMenu = true;
  while (inGameOptionsMenu)
  {
    printOptionsMenu();
    if (getUserInput("int"))
    {
      switch (userInput)
      {
        case 1:
          settingShowVowelMarks = !settingShowVowelMarks;
          break;
        case 2:
          inGameOptionsMenu = false;
          break;
        default:
          cout << "Please enter a valid option";
      }
    }
    cout << "\n\n\n";
  }
}
void printMainMenu()
{
  cout << "Main Menu\n";
  cout << "Please Select an Option:\n";
  cout << "\t1. Play Game\n";
  cout << "\t2. Options\n";
  cout << "\t3. Exit\n";
}

// Game
void runGame()
{
  // Guessed letters
  const int maxGuessedLetters = 27;
  char guessedLetters[maxGuessedLetters] = {' '};
  for (int i=1; i<maxGuessedLetters; i++) guessedLetters[i] = '0';

  // Mystery Word
  char mysteryWord[] = {"SNOOPLES AND STUFF"};
  const int mysteryWordLength = strlen(mysteryWord);

  // Revealed word
  char revealedWord[mysteryWordLength];
  for (int i=0; i < mysteryWordLength; i++)
  {
    if (mysteryWord[i] == ' ') revealedWord[i] = ' ';
    else revealedWord[i] = '_';
  }

  // Vowels
  char revealedWordVowelMarks[mysteryWordLength];
  char vowels[5] = {'A','E','I','O','U'};

  int guessNumber = 1;
  int wrongGuesses = 0;
  cout << "\n\n";




  bool gameRunning = true;
  while (gameRunning)
  {
    printHangman(wrongGuesses);
    printWrongGuesses(wrongGuesses);
    cout << '\n';
    printRevealedWord(revealedWord, mysteryWordLength);
    printRevealedWordVowelMarks(mysteryWord, mysteryWordLength, vowels);
    printGuessedLetters(guessNumber, guessedLetters);
    if (playerHasWon(mysteryWord, mysteryWordLength, revealedWord))
    {
      cout << '\n';
      printVictoryMessage();
      gameRunning = false;
    }
    else if (playerHasLost(wrongGuesses))
    {
      cout << '\n';
      printDefeatMessage(mysteryWord);
      gameRunning = false;
    }
    else
    {
      cout << '\n';
      printArtHorizontalLine();
      cout << "\n\n";
      if (getUserInput("char"))
      {
        if (inputWasNotAlreadyGuessed(guessNumber, guessedLetters))
        {
          addGuessToGuessedLetters(guessNumber, guessedLetters);
          guessNumber++;
          if (guessIsCorrect(mysteryWord, mysteryWordLength))
            revealLetters(mysteryWord, mysteryWordLength, revealedWord);
          else wrongGuesses++;
        }
      }
    }
    cout << '\n';
  }
}
void gameMainMenu()
{
  bool inGameMainMenu = true;
  while (inGameMainMenu)
  {
    printMainMenu();
    if (getUserInput("int"))
    {
      switch (userInput)
      {
        case 1:
          runGame();
          break;
        case 2:
          cout << "\n\n\n";
          gameOptionsMenu();
          break;
        case 3:
          inGameMainMenu = false;
          break;
        default:
          cout << "Please enter a valid option\n";
      }
    }
    cout << "\n\n\n";
  }
}

int main()
{
  cout << "\nWelcome to Hangman!\n\n";
  bool programRunning = true;
  while (programRunning)
  {
    gameMainMenu();
    programRunning = false;
  }
  cout << "Thanks for playing!\n\n";
  return 0;
}
