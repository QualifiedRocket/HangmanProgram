// Hangman in C++
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <random>
#include <chrono>

using namespace std;
using myClock = chrono::high_resolution_clock;

/*
- Implement x under vowels - Done
- Create pretty game box - Done
- Create hangman icon - Done
- Create options menu - Done
- Create single user input function - Done
- Remove global variables - Done
- Get vowel marks once - Done
- Get random word on game start - Done
- Get random word from text file -

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
void printHangman(int wrongGuesses)
{
  const int numRows = 10;
  const int numCols = 20;
  char hangmanPicture[numRows][numCols]
  {
    {"     ________      "}, // row 0
    {"     ||//   |      "},
    {"     ||/           "},
    {"     ||            "},
    {"     ||            "}, // row 4
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

// Setup
void getVowelMarks(const char mysteryWord[], int mysteryWordLength, char vowelMarks[])
{
  if (settingShowVowelMarks)
  {
    const char vowels[5] = {'A','E','I','O','U'};
    bool isAVowel = false;
    for (int i=0; i<mysteryWordLength; i++)
    {
      for (char d : vowels)
      {
        if (mysteryWord[i] == d)
        {
          isAVowel = true;
          vowelMarks[i] = 'x';
          break;
        }
      }
      if (!isAVowel) vowelMarks[i] = ' ';
      isAVowel = false;
    }
  }
}
int getRandomWord(myClock::time_point beginning, const int wordsMax)
{
  // Get seed from system clock
  myClock::duration difference = myClock::now() - beginning;
  unsigned seed = difference.count();
  //cout << "Random seed: " << seed << '\n';

  // Use seed to get random word
  default_random_engine generator(seed);
  uniform_int_distribution<int> distribution(1,wordsMax);
  return distribution(generator);
}
void getWordFromFile(myClock::time_point beginning, string& chosenWord)
{
  ifstream wordsFile;
  wordsFile.open("HangmanProgram/HangmanWords.txt");
  if (!wordsFile.is_open())
  {
    cout << "Unable to open HangmanWords.txt\n";
  }
  else
  {
    // Get number of lines in file
    int lineNum = 0;
    string lineWord;
    while (getline(wordsFile, lineWord))
    {
      lineNum++;
      /*
      cout << "Line " << lineNum << ": " << lineWord << '\n';
      cout << "good:\t " << wordsFile.good() << '\n';
      cout << "bad:\t " << wordsFile.bad() << '\n';
      cout << "fail:\t " << wordsFile.fail() << '\n';
      cout << "eof:\t " << wordsFile.eof() << '\n';
      */
    }
    /*
    cout << "There are " << lineNum << " lines in HangmanWords\n";
    cout << "good:\t " << wordsFile.good() << '\n';
    cout << "bad:\t " << wordsFile.bad() << '\n';
    cout << "fail:\t " << wordsFile.fail() << '\n';
    cout << "eof:\t " << wordsFile.eof() << '\n';
    */

    wordsFile.clear();

    // Get random word number
    const int wordsMax = lineNum;
    int wordNumber = getRandomWord(beginning, wordsMax);

    // Retrieve word from line number in file
    lineNum = 0;
    wordsFile.seekg(0, ios::beg);
    while (getline(wordsFile, lineWord))
    {
      lineNum++;
      if (lineNum == wordNumber)
      {
        cout << "Chosen line " << lineNum << " with word: " << lineWord << '\n';
        chosenWord = lineWord;
        break;
      }
    }
    wordsFile.clear();
    wordsFile.close();
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
void printVowelMarks(char vowelMarks[], int mysteryWordLength)
{
  if (settingShowVowelMarks)
  {
    cout << "\t\t";
    for (int i=0; i<mysteryWordLength; i++)
      cout << vowelMarks[i] << ' ';
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
bool getUserInput(char outputType)
{
  if (outputType == 'c')
    cout << "Please enter a letter: ";
  else if (outputType == 'i')
    cout << "Please enter a number: ";
  string input;
  char inputChar;
  getline(cin, input);
  if (checkIfUserInputIsOneChar(input))
  {
    inputChar = input[0];
    if (outputType == 'c')
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
    else if (outputType == 'i')
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
void addGuessToGuessedLetters(int& guessNumber, char guessedLetters[])
{
  guessedLetters[guessNumber] = userInput;
  guessNumber++;
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
    if (getUserInput('i'))
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
void runGame(myClock::time_point beginning)
{
  bool gameRunning = true;
  // Setup
  // Guessed letters
  const int maxGuessedLetters = 27;
  char guessedLetters[maxGuessedLetters] = {' '};
  for (int i=1; i<maxGuessedLetters; i++)
    guessedLetters[i] = '0';

  // Mystery word
  string chosenWord;
  getWordFromFile(beginning, chosenWord);
  const int mysteryWordLength = chosenWord.length();
  char mysteryWord[mysteryWordLength];
  for (int i=0; i<=mysteryWordLength; i++)
    mysteryWord[i] = chosenWord[i];

  // Revealed word
  char revealedWord[mysteryWordLength];
  for (int i=0; i<mysteryWordLength; i++)
  {
    if (mysteryWord[i] == ' ') revealedWord[i] = ' ';
    else revealedWord[i] = '_';
  }

  // Vowels
  char vowelMarks[mysteryWordLength];
  getVowelMarks(mysteryWord, mysteryWordLength, vowelMarks);

  int guessNumber = 1;
  int wrongGuesses = 0;
  cout << "\n\n";

  while (gameRunning)
  {
    cout << "Word is: " << mysteryWord << '\n';
    printHangman(wrongGuesses);
    printWrongGuesses(wrongGuesses);
    cout << '\n';
    printRevealedWord(revealedWord, mysteryWordLength);
    printVowelMarks(vowelMarks, mysteryWordLength);
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
      if (getUserInput('c'))
      {
        if (inputWasNotAlreadyGuessed(guessNumber, guessedLetters))
        {
          addGuessToGuessedLetters(guessNumber, guessedLetters);
          if (guessIsCorrect(mysteryWord, mysteryWordLength))
            revealLetters(mysteryWord, mysteryWordLength, revealedWord);
          else wrongGuesses++;
        }
      }
    }
    cout << '\n';
  }
}
void gameMainMenu(myClock::time_point beginning)
{
  cout << "\nWelcome to Hangman!\n\n";
  bool inGameMainMenu = true;
  while (inGameMainMenu)
  {
    printMainMenu();
    if (getUserInput('i'))
    {
      switch (userInput)
      {
        case 1:
          runGame(beginning);
          break;
        case 2:
          cout << "\n\n\n";
          gameOptionsMenu();
          break;
        case 3:
          cout << "Thanks for playing!\n\n";
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
  myClock::time_point beginning = myClock::now();
  bool programRunning = true;
  while (programRunning)
  {
    gameMainMenu(beginning);
    programRunning = false;
  }
  return 0;
}
