// ===== includes =====
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <csignal>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ===== constants =====
constexpr int BOARD_SIZE = 9;
constexpr int MAX_SAVES = 1000;

// ===== global state =====
bool g_running = true;

// ===== utility funtions =====
void clearScreen();
void alphabetize(string strings[], int numStrings);

// ===== persistence =====
void loadDirectory(string saves[], int &count);
void saveDirectory(const string saves[], int count);
bool loadGame(const string &filename, int board[BOARD_SIZE][BOARD_SIZE]);
bool saveGame(const string &filename, int board[BOARD_SIZE][BOARD_SIZE]);
bool addSave(string saves[], int &count, const string &filename);
bool removeSave(string saves[], int &count, const string &filename);

// ===== test =====
void loadTestGame(int board[BOARD_SIZE][BOARD_SIZE], int id);

// ===== game logic =====
void printBoard(const int board[BOARD_SIZE][BOARD_SIZE]);
bool isLegal(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int value);
bool solveGame(int board[BOARD_SIZE][BOARD_SIZE]);
void resetGame(int board[BOARD_SIZE][BOARD_SIZE]);

// ===== command handling =====
bool handleCommand(const string &command, int board[BOARD_SIZE][BOARD_SIZE], string saves[], int &numSaves, stringstream &console, bool &running);
bool handleSetCommand(const string &command, int board[BOARD_SIZE][BOARD_SIZE], stringstream &console);

// ===== signal handling =====
void signalHandler(int signal);

// ===== main =====
int main()
{
  signal(SIGINT, signalHandler);

  int (*board)[BOARD_SIZE] = new int[BOARD_SIZE][BOARD_SIZE];

  int initialBoard[BOARD_SIZE][BOARD_SIZE] = {
      {-1, 0, 0, -8, 0, 0, -6, -5, 0},
      {0, 0, 0, -9, -1, 0, 0, -2, 0},
      {0, -8, 0, 0, -5, 0, -7, 0, -9},
      {0, 0, 0, 0, 0, 0, 0, -9, 0},
      {0, -5, -3, 0, -4, 0, -1, -7, 0},
      {0, -4, 0, 0, 0, 0, 0, 0, 0},
      {-5, 0, -2, 0, -9, 0, 0, -3, 0},
      {0, -9, 0, 0, -7, -5, 0, 0, 0},
      {0, -7, -6, 0, 0, -2, 0, 0, -5}};

  memcpy(board, initialBoard, sizeof(initialBoard));

  string *savedGames = new string[MAX_SAVES];
  int numSaves = 0;

  stringstream console;
  bool running = true;

  loadDirectory(savedGames, numSaves);
  loadGame("autosave.txt", board);

  while (running && g_running)
  {
    clearScreen();
    printBoard(board);

    if (!console.str().empty())
    {
      cout << console.str() << "\n\n";
      console.str("");
      console.clear();
    }

    cout << "Enter a command: ";
    string command;
    getline(cin, command);

    if (!handleCommand(command, board, savedGames, numSaves, console, running))
    {
      console << "Unknown command.";
    }
  }

  saveGame("autosave.txt", board);
  saveDirectory(savedGames, numSaves);

  delete[] board;
  delete[] savedGames;

  return 0;
}

void clearScreen()
{
#ifdef _WIN32
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD cellCount;
  DWORD count;
  COORD homeCoords = {0, 0};

  if (hConsole == INVALID_HANDLE_VALUE)
  {
    return;
  }

  if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
  {
    return;
  }

  cellCount = csbi.dwSize.X * csbi.dwSize.Y;

  FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);

  FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);

  SetConsoleCursorPosition(hConsole, homeCoords);
#else
  cout << "\033[2J\033[H";
#endif
}
void alphabetize(string strings[], int numStrings)
{
  for (int i = 0; i < numStrings; i++)
  {
    int swapIndex = i;
    for (int j = i; j < numStrings; j++)
    {
      if (strings[j] < strings[swapIndex])
      {
        swapIndex = j;
      }
    }

    string temp = strings[swapIndex];
    strings[swapIndex] = strings[i];
    strings[i] = temp;
  }
}
void loadDirectory(string saves[], int &count)
{
  ifstream fin("directory.txt");
  if (!fin)
  {
    return;
  }

  fin >> count;
  for (int i = 0; i < count; i++)
  {
    fin >> saves[i];
  }
}
void saveDirectory(const string saves[], int count)
{
  ofstream fout("directory.txt");
  fout << count << '\n';
  for (int i = 0; i < count; i++)
  {
    fout << saves[i] << '\n';
  }
}
bool saveGame(const string &filename, int board[BOARD_SIZE][BOARD_SIZE])
{
  ofstream fout(filename);
  if (!fout)
  {
    return false;
  }

  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      fout << board[i][j] << " ";
    }
    fout << '\n';
  }
  return true;
}
bool loadGame(const string &filename, int board[BOARD_SIZE][BOARD_SIZE])
{
  ifstream fin(filename);
  if (!fin)
  {
    return false;
  }

  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      fin >> board[i][j];
    }
  }

  return true;
}
bool addSave(string saves[], int &count, const string &filename)
{
  for (int i = 0; i < count; i++)
  {
    if (saves[i] == filename)
    {
      return false;
    }
  }

  saves[count++] = filename;
  alphabetize(saves, count);
  return true;
}
bool removeSave(string saves[], int &count, const string &filename)
{
  for (int i = 0; i < count; i++)
  {
    if (saves[i] == filename)
    {
      for (int j = i; j < count - 1; j++)
      {
        saves[j] = saves[j + 1];
      }
      count--;
      return true;
    }
  }
  return false;
}
void loadTestGame(int board[BOARD_SIZE][BOARD_SIZE], int id)
{
  static int tests[][BOARD_SIZE][BOARD_SIZE] = {
      {{-1, 0, 0, -8, 0, 0, -6, -5, 0},
       {0, 0, 0, -9, -1, 0, 0, -2, 0},
       {0, -8, 0, 0, -5, 0, -7, 0, -9},
       {0, 0, 0, 0, 0, 0, 0, -9, 0},
       {0, -5, -3, 0, -4, 0, -1, -7, 0},
       {0, -4, 0, 0, 0, 0, 0, 0, 0},
       {-5, 0, -2, 0, -9, 0, 0, -3, 0},
       {0, -9, 0, 0, -7, -5, 0, 0, 0},
       {0, -7, -6, 0, 0, -2, 0, 0, -5}}};

  memcpy(board, tests[id], sizeof(tests[id]));
}
void printBoard(const int board[BOARD_SIZE][BOARD_SIZE])
{
  cout << "\033[38;2;150;150;150m";

  cout << "\n    1 2 3   4 5 6   7 8 9\n\n";

  for (int i = 0; i < 9; i++)
  {
    cout << "\033[38;2;150;150;150m";

    if (i % 3 == 0 && i != 0)
    {
      cout << "    ------+-------+------\n";
    }

    cout << char('A' + i) << "   ";

    for (int j = 0; j < 9; j++)
    {
      if (j % 3 == 0 && j != 0)
      {
        cout << "\033[38;2;150;150;150m" << "| ";
      }

      if (board[i][j] < 0)
      {
        cout << "\033[38;2;0;255;0m";
      }
      else
      {
        cout << "\033[0m";
      }

      if (board[i][j] != 0)
      {
        cout << abs(board[i][j]) << " ";
      }
      else
      {
        cout << "  ";
      }
    }

    cout << '\n';
  }

  cout << "\033[0m" << '\n';
}
bool isLegal(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int value)
{
  if (value < 1 || value > 9)
  {
    return false;
  }

  // Check row
  for (int j = 0; j < 9; j++)
  {
    if (abs(board[row][j]) == value)
    {
      return false;
    }
  }

  // Check column
  for (int i = 0; i < 9; i++)
  {
    if (abs(board[i][col]) == value)
    {
      return false;
    }
  }

  // Check subsquare 3x3
  int startRow = (row / 3) * 3;
  int startCol = (col / 3) * 3;

  for (int i = startRow; i < startRow + 3; i++)
  {
    for (int j = startCol; j < startCol + 3; j++)
    {
      if (abs(board[i][j]) == value)
      {
        return false;
      }
    }
  }

  return true;
}
bool solveGame(int board[BOARD_SIZE][BOARD_SIZE])
{
  for (int row = 0; row < BOARD_SIZE; row++)
  {
    for (int col = 0; col < BOARD_SIZE; col++)
    {
      if (board[row][col] == 0)
      {
        for (int val = 1; val <= 9; val++)
        {
          if (isLegal(board, row, col, val))
          {
            board[row][col] = val;

            if (solveGame(board))
            {
              return true;
            }
            board[row][col] = 0; // backtrack
          }
        }
        return false;
      }
    }
  }
  return true;
}
void resetGame(int board[BOARD_SIZE][BOARD_SIZE])
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      if (board[i][j] > 0)
      {
        board[i][j] = 0;
      }
    }
  }
}
bool handleCommand(const string &command, int board[BOARD_SIZE][BOARD_SIZE], string saves[], int &numSaves, stringstream &console, bool &running)
{
  if (command == "exit")
  {
    running = false;
    console << "Game saved. Goodbye!";
    return true;
  }

  if (command.rfind("set", 0) == 0)
  {
    return handleSetCommand(command, board, console);
  }

  if (command.rfind("load", 0) == 0)
  {
    string filename = command.size() > 4 ? command.substr(5) : "default.txt";
    loadGame(filename, board);
    return true;
  }

  if (command.rfind("save", 0) == 0)
  {
    string filename = command.size() > 4 ? command.substr(5) : "default.txt";
    saveGame(filename, board);
    addSave(saves, numSaves, filename);
    return true;
  }

  if (command == "list saves")
  {
    console << "Saved Games:";
    if (numSaves > 0)
    {
      console << '\n';
      for (int i = 0; i < numSaves; i++)
      {
        console << "  " << saves[i];
        if (i < numSaves - 1)
        {
          console << '\n';
        }
      }
    }
    return true;
  }

  if (command.rfind("delete", 0) == 0)
  {
    string filename = command.substr(7);
    removeSave(saves, numSaves, filename);
    remove(filename.c_str());
    return true;
  }

  if (command == "test")
  {
    loadTestGame(board, 0);
    console << "Test game loaded.";
    return true;
  }

  if (command == "solve")
  {
    resetGame(board);

    if (solveGame(board))
    {
      console << "Game solved successfully.";
    }
    else
    {
      console << "No solution found.";
    }
    return true;
  }

  if (command.rfind("hint", 0) == 0)
  {
    // Expected format: hint A1
    if (command.size() != 7 || command[4] != ' ')
    {
      console << "Usage: hint A1";
      return true;
    }

    char rowChar = toupper(command[5]);
    char colChar = command[6];

    if (rowChar < 'A' || rowChar > 'I' || colChar < '1' || colChar > '9')
    {
      console << "Invalid position.";
      return true;
    }

    int row = rowChar - 'A';
    int col = colChar - '1';

    if (board[row][col] < 0)
    {
      console << "This cell is fixed.";
      return true;
    }

    if (board[row][col] > 0)
    {
      console << "Cell already filled.";
      return true;
    }

    int temp[BOARD_SIZE][BOARD_SIZE];
    memcpy(temp, board, sizeof(temp));

    resetGame(temp);

    if (!solveGame(temp))
    {
      console << "No solution available.";
      return true;
    }

    console << "Hint for " << rowChar << colChar << ": " << temp[row][col];

    return true;
  }

  return false;
}
bool handleSetCommand(const string &command, int board[BOARD_SIZE][BOARD_SIZE], stringstream &console)
{
  // Expected format: set A1 5
  if (command.size() < 7)
  {
    return false;
  }

  if (command.substr(0, 3) != "set" || command[3] != ' ')
  {
    return false;
  }

  int pos = 4;

  char rowChar = toupper(command[pos++]);
  char colChar = command[pos++];

  if (pos >= command.size() || command[pos++] != ' ')
  {
    return false;
  }

  char valChar = command[pos];

  if (rowChar < 'A' || rowChar > 'I')
  {
    return false;
  }

  if (colChar < '1' || colChar > '9')
  {
    return false;
  }

  if (valChar < '1' || valChar > '9')
  {
    return false;
  }

  int row = rowChar - 'A';
  int col = colChar - '1';
  int value = valChar - '0';

  if (board[row][col] < 0)
  {
    console << "This cell is fixed.";
    return true;
  }

  if (board[row][col] > 0)
  {
    console << "Cell already occupied.";
    return true;
  }

  if (!isLegal(board, row, col, value))
  {
    console << "Illegal move.";
    return true;
  }

  board[row][col] = value;
  console << "Move applied successfully.";
  return true;
}
void signalHandler(int signal)
{
  if (signal == SIGINT)
  {
    g_running = false;
  }
}
