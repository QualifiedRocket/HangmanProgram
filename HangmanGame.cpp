// Hangman in C++
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <string>
#include <random>
#include <chrono>

using myClock = std::chrono::high_resolution_clock;

/*
- Implement x under vowels - Done
- Create pretty game box - Done
- Create hangman icon - Done
- Create options menu - Done
- Create single user input function - Done
- Remove global variables - Done
- Get vowel marks once - Done
- Get random word on game start - Done
- Get random word from text file - Done
- Validate words from file when pulled -
- Add words to file in settings -
- Figure out phrases -
- Have computer guess player's word -

- Make this a collection of games
*/

// Player Settings
const std::string playerWordsFile = "HangmanProgram/CommonNouns.txt";
const std::string restrictedWordsFile = "HangmanProgram/RestrictedWords.txt";
bool settingShowVowelMarks = false;
int settingMaxGuesses = 6;
int settingMinWordLength;
int settingMaxWordLength;

// Global variables
char userInputChar;
int userInputInt;

// Display
void printArtHorizontalLine()
{
  for (int i=0; i<60; i++) std::cout << '/';
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
    case 6:
      hangmanPicture[4][13] = '\\';
    case 5:
      hangmanPicture[4][11] = '/';
    case 4:
      hangmanPicture[3][13] = '\\';
    case 3:
      hangmanPicture[3][11] = '/';
    case 2:
      hangmanPicture[3][12] = '|';
    case 1:
      hangmanPicture[2][12] = 'O';
    // Print hangman picture
    default:
      for (int row=0; row<numRows; row++)
      {
        for (int col=0; col<numCols; col++)
        {
          std::cout << hangmanPicture[row][col];
        }
        std::cout << '\n';
      }
  }
}

// Setup

bool getMinMaxWordLength(int& smallestWordInFile, int& largestWordInFile)
{
  std::ifstream readFile;
  readFile.open(playerWordsFile);
  if (!readFile.is_open())
  {
    std::cout << "Unable to open " << playerWordsFile << std::endl;
    return false;
  }
  else
  {
    std::string lineWord;
    while (getline(readFile, lineWord))
    {
      if (lineWord.length()-1 < smallestWordInFile)
        smallestWordInFile = lineWord.length()-1;
      else if (lineWord.length()-1 > largestWordInFile)
        largestWordInFile = lineWord.length()-1;
    }
    settingMinWordLength = smallestWordInFile;
    settingMaxWordLength = largestWordInFile;
  }
  readFile.clear();
  readFile.close();
  return true;
}

bool restrictWordsFile()
{
  std::ifstream readFile;
  std::ofstream writeFile;
  readFile.open(playerWordsFile);
  writeFile.open(restrictedWordsFile);
  if (!readFile.is_open())
  {
    std::cout << "Unable to open " << playerWordsFile << std::endl;
    return false;
  }
  else if (!writeFile.is_open())
  {
    std::cout << "Unable to open " << restrictedWordsFile << std::endl;
    readFile.close();
    return false;
  }
  else
  {
    std::string lineWord;
    while (getline(readFile, lineWord))
    {
      if (lineWord.length()-1 < settingMinWordLength || lineWord.length()-1 > settingMaxWordLength)
        continue;
      else
        writeFile << lineWord << std::endl;
    }
  }
  readFile.clear();
  writeFile.clear();
  readFile.close();
  writeFile.close();
  return true;
}
int getRandomWord(myClock::time_point beginning, const int wordsMax)
{
  // Get seed from system clock
  myClock::duration difference = myClock::now() - beginning;
  unsigned seed = difference.count();
  //std::cout << "Random seed: " << seed << '\n';

  // Use seed to get random word
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(1,wordsMax);
  return distribution(generator);
}
bool getWordFromFile(myClock::time_point beginning, std::string& chosenWord)
{
  std::ifstream wordsFile;
  wordsFile.open(restrictedWordsFile);
  if (!wordsFile.is_open())
  {
    std::cout << "Unable to open words file" << std::endl;
    return false;
  }
  else
  {
    // Get number of lines in file
    int lineNum = 0;
    std::string lineWord;
    while (getline(wordsFile, lineWord))
    {
      lineNum++;
      /*
      std::cout << "Line " << lineNum << ": " << lineWord << '\n';
      std::cout << "good:\t " << wordsFile.good() << '\n';
      std::cout << "bad:\t " << wordsFile.bad() << '\n';
      std::cout << "fail:\t " << wordsFile.fail() << '\n';
      std::cout << "eof:\t " << wordsFile.eof() << '\n';
      */
    }
    /*
    std::cout << "There are " << lineNum << " lines in HangmanWords\n";
    std::cout << "good:\t " << wordsFile.good() << '\n';
    std::cout << "bad:\t " << wordsFile.bad() << '\n';
    std::cout << "fail:\t " << wordsFile.fail() << '\n';
    std::cout << "eof:\t " << wordsFile.eof() << '\n';
    */
    wordsFile.clear();

    const int wordsMax = lineNum;

    bool gettingWord = true;
    while (gettingWord)
    {
      int wordNumber = getRandomWord(beginning, wordsMax);

      // Retrieve word from line number in file
      lineNum = 0;
      wordsFile.seekg(0, std::ios::beg);
      while (getline(wordsFile, lineWord))
      {
        lineNum++;
        if (lineNum == wordNumber)
        {
          //std::cout << "Chosen line " << lineNum << " with word: " << lineWord << std::endl;
          break;
        }
      }
      for (char& c : lineWord)
        c = toupper(c);
      //std::cout << "Chosen line " << lineNum << " with word: " << lineWord << '\n';
      chosenWord = lineWord;
      gettingWord = false;
    }
    wordsFile.clear();
    wordsFile.close();
    if (remove(restrictedWordsFile.c_str()) != 0)
      std::cout << "Error: Could not remove restricted words file" << std::endl;
  }
  return true;
}
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

// Printing Game Letters
void printGuessedLetters(int guessNumber, char guessedLetters[])
{
  std::cout << "Guesses so far: ";
  for (int i=1; i<guessNumber; i++)
  {
    std::cout << guessedLetters[i] << ' ';
  }
  std::cout << '\n';
}
void printRevealedWord(char revealedWord[], int mysteryWordLength)
{
  std::cout << "Revealed word:\t";
  for (int i=0; i<mysteryWordLength; i++) std::cout << revealedWord[i] << ' ';
  std::cout << '\n';
}
void printVowelMarks(char vowelMarks[], int mysteryWordLength)
{
  if (settingShowVowelMarks)
  {
    std::cout << "\t\t";
    for (int i=0; i<mysteryWordLength; i++)
      std::cout << vowelMarks[i] << ' ';
  }
  std::cout << "\n\n";
}
void printWrongGuesses(int numberOf)
{
  std::cout << "You have guessed wrong " << numberOf << " times.\n";
}
void printVictoryMessage()
{
  std::cout << "Congratulations! You won!";
}
void printDefeatMessage(char mysteryWord[], int mysteryWordLength)
{
  std::cout << "I'm sorry, you lost.\n\n";
  std::cout << "The answer is: ";
  for (int i=0; i<mysteryWordLength; i++)
    std::cout << mysteryWord[i];
}

// Character Handling and Checking
int convertCharToInt(char input)
{
  return input - 48;
}
bool checkIfUserInputIsOneChar(std::string input)
{
  if (input.length() != 1)
    return false;
  else
    return true;
}
bool getUserInput(char outputType)
{
  if (outputType == 'c')
    std::cout << "Please enter a letter: ";
  else if (outputType == 'i')
    std::cout << "Please enter a number: ";
  std::string input;
  char inputChar;
  int inputInt;
  getline(std::cin, input);
  if (outputType == 'c' && checkIfUserInputIsOneChar(input))
  {
    inputChar = input[0];
    if (isalpha(inputChar))
    {
      inputChar = toupper(inputChar);
      userInputChar = inputChar;
      return true;
    }
  }
  else if (outputType == 'i')
  {
    bool isANumber = true;
    for (char c : input)
    {
      if (!isdigit(c))
      {
        isANumber = false;
        break;
      }
    }
    if (isANumber)
    {
      userInputInt = stoi(input);
      return true;
    }
  }
  std::cout << "Please enter a valid option";
  return false;
}
bool inputWasNotAlreadyGuessed(int guessNumber, char guessedLetters[])
{
  for (int i=1; i<=guessNumber; i++)
  {
    if (userInputChar == guessedLetters[i])
    {
      std::cout << "You already guessed that letter!";
      return false;
    }
  }
  return true;
}
void addGuessToGuessedLetters(int& guessNumber, char guessedLetters[])
{
  guessedLetters[guessNumber] = userInputChar;
  guessNumber++;
}
bool guessIsCorrect(char mysteryWord[], int mysteryWordLength)
{
  for (int i=0; i < mysteryWordLength; i++)
  {
    if (mysteryWord[i] == userInputChar)
    {
      std::cout << "Correct!";
      return true;
    }
  }
  std::cout << "Incorrect!";
  return false;
}
void revealLetters(char mysteryWord[], int mysteryWordLength, char revealedWord[])
{
  for (int i=0; i < mysteryWordLength; i++)
  {
    if (mysteryWord[i] == userInputChar) revealedWord[i] = userInputChar;
  }
}
bool playerHasWon(char mysteryWord[], int mysteryWordLength, char revealedWord[], bool& gameRunning)
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
  std::cout << "Error with playerHasWon\n";
  gameRunning = false;
  return false;
}
bool playerHasLost(int wrongGuesses)
{
  if (wrongGuesses == settingMaxGuesses) return true;
  else return false;
}

// Menu
void printOptionsMenu()
{
  std::cout << "Options Menu\n";
  std::cout << "Please select an option:\n";
  std::cout << "\t1. Show vowel marks\t[";
  if (settingShowVowelMarks) std::cout << "o]\n";
  else std::cout << " ]\n";
  std::cout << "\t2. Smallest word size: " << settingMinWordLength << '\n';
  std::cout << "\t3. Largest word size: " << settingMaxWordLength << '\n';
  std::cout << "\t4. Return to main menu\n";
}
void gameOptionsMenu(int smallestWordInFile, int largestWordInFile)
{
  bool inGameOptionsMenu = true;
  while (inGameOptionsMenu)
  {
    printOptionsMenu();
    if (getUserInput('i'))
    {
      switch (userInputInt)
      {
        case 1:
          settingShowVowelMarks = !settingShowVowelMarks;
          break;
        case 2:
          std::cout << "Enter a new smallest word size\n";
          if (getUserInput('i'))
          {
            if (userInputInt < smallestWordInFile)
              std::cout << "Word size cannot be less than " << smallestWordInFile;
            else if (userInputInt <= settingMaxWordLength)
              settingMinWordLength = userInputInt;
            else
              std::cout << "Smallest word size must be smaller than largest word size";
          }
          break;
        case 3:
          std::cout << "Enter a new largest word size\n";
          if (getUserInput('i'))
          {
            if (userInputInt > largestWordInFile)
              std::cout << "Word size cannot be greater than " << largestWordInFile;
            else if (userInputInt >= settingMinWordLength)
              settingMaxWordLength = userInputInt;
            else
              std::cout << "Largest word size must be larger than smallest word size";
          }
          break;
        case 4:
          inGameOptionsMenu = false;
          break;
        default:
          std::cout << "Please enter a valid option";
      }
    }
    std::cout << "\n\n\n";
  }
}
void printMainMenu()
{
  std::cout << "Main Menu\n";
  std::cout << "Please Select an Option:\n";
  std::cout << "\t1. Play Game\n";
  std::cout << "\t2. Options\n";
  std::cout << "\t3. Exit\n";
}

// Game
void runGame(myClock::time_point beginning)
{
  bool gameRunning = true;
  // Setup

  if (!restrictWordsFile())
  {
    std::cout << "Error: Failure generating restrictedWordsFile" << std::endl;
    gameRunning = false;
  }

  // Get mystery word
  std::string chosenWord;
  if (!getWordFromFile(beginning, chosenWord))
  {
    std::cout << "Error: Failure obtaining word from file!" << std::endl;
    gameRunning = false;
  }

  const int mysteryWordLength = chosenWord.length()-1;
  char mysteryWord[mysteryWordLength];
  for (int i=0; i<=mysteryWordLength; i++)
    mysteryWord[i] = chosenWord[i];

  // Revealed word
  char revealedWord[mysteryWordLength];
  for (int i=0; i<=mysteryWordLength; i++)
  {
    if (mysteryWord[i] == ' ') revealedWord[i] = ' ';
    else revealedWord[i] = '_';
  }

  // Guessed letters
  const int maxGuessedLetters = 27;
  char guessedLetters[maxGuessedLetters] = {' '};
  for (int i=1; i<maxGuessedLetters; i++)
    guessedLetters[i] = '0';

  // Vowels
  char vowelMarks[mysteryWordLength];
  getVowelMarks(mysteryWord, mysteryWordLength, vowelMarks);

  int guessNumber = 1;
  int wrongGuesses = 0;
  std::cout << "\n\n";

  while (gameRunning)
  {
    //std::cout << "Word is: " << mysteryWord << '\n';
    printHangman(wrongGuesses);
    printWrongGuesses(wrongGuesses);
    std::cout << '\n';
    printRevealedWord(revealedWord, mysteryWordLength);
    printVowelMarks(vowelMarks, mysteryWordLength);
    printGuessedLetters(guessNumber, guessedLetters);
    if (playerHasWon(mysteryWord, mysteryWordLength, revealedWord, gameRunning))
    {
      std::cout << '\n';
      printVictoryMessage();
      gameRunning = false;
    }
    else if (playerHasLost(wrongGuesses))
    {
      std::cout << '\n';
      printDefeatMessage(mysteryWord, mysteryWordLength);
      gameRunning = false;
    }
    else
    {
      std::cout << '\n';
      printArtHorizontalLine();
      std::cout << "\n\n";
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
    std::cout << '\n' << std::endl;
  }
}
void gameMainMenu(myClock::time_point beginning)
{
  std::cout << "\nWelcome to Hangman!\n\n";
  int smallestWordInFile = 50;
  int largestWordInFile = 1;
  bool inGameMainMenu = true;
  if (!getMinMaxWordLength(smallestWordInFile, largestWordInFile))
    inGameMainMenu = false;
  while (inGameMainMenu)
  {
    printMainMenu();
    if (getUserInput('i'))
    {
      switch (userInputInt)
      {
        case 1:
          runGame(beginning);
          break;
        case 2:
          std::cout << "\n\n\n";
          gameOptionsMenu(smallestWordInFile, largestWordInFile);
          break;
        case 3:
          std::cout << "Thanks for playing!\n\n";
          inGameMainMenu = false;
          break;
        default:
          std::cout << "Please enter a valid option\n";
      }
    }
    std::cout << "\n\n\n";
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
