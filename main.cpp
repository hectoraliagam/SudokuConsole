#include <iostream>
#include <string>
#include <fstream>

using namespace std;

constexpr int SIZE = 9;

void clearScreen()
{
  cout << "\033[2J\033[H";
}

void printBoard(const int board[SIZE][SIZE]);
bool saveGame(const string &filename, int board[SIZE][SIZE]);
bool loadGame(const string &filename, int board[SIZE][SIZE]);
bool handleSetCommand(const string &command, int board[SIZE][SIZE]);
bool isLegal(const int board[SIZE][SIZE], int row, int col, int value);

int main()
{
  bool running = true;
  int board[SIZE][SIZE] = {
      {-1, 0, 0, -8, 0, 0, -6, -5, 0},
      {0, 0, 0, -9, -1, 0, 0, -2, 0},
      {0, -8, 0, 0, -5, 0, -7, 0, -9},
      {0, 0, 0, 0, 0, 0, 0, -9, 0},
      {0, -5, -3, 0, -4, 0, -1, -7, 0},
      {0, -4, 0, 0, 0, 0, 0, 0, 0},
      {-5, 0, -2, 0, -9, 0, 0, -3, 0},
      {0, -9, 0, 0, -7, -5, 0, 0, 0},
      {0, -7, -6, 0, 0, -2, 0, 0, -5}};

  loadGame("autosave.txt", board);

  while (running)
  {
    clearScreen();

    printBoard(board);

    cout << "Enter a command: ";
    string command;
    getline(cin, command);

    if (command == "exit")
    {
      saveGame("autosave.txt", board);
      running = false;
    }
    else if (command.rfind("set", 0) == 0)
    {
      if (!handleSetCommand(command, board))
      {
        cout << "Invalid command format.\n";
      }
    }
    else
    {
      cout << "Unknown command.\n";
    }
  }

  return 0;
}

void printBoard(const int board[SIZE][SIZE])
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
bool saveGame(const string &filename, int board[SIZE][SIZE])
{
  ofstream fout;

  fout.open(filename);

  if (fout.fail())
  {
    fout.close();
    return false;
  }

  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      fout << board[i][j] << " ";
    }
    fout << '\n';
  }

  fout.close();
  return true;
}
bool loadGame(const string &filename, int board[SIZE][SIZE])
{
  ifstream fin;

  fin.open(filename);

  if (fin.fail())
  {
    fin.close();
    return false;
  }

  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      fin >> board[i][j];
    }
  }

  fin.close();
  return true;
}
bool handleSetCommand(const string &command, int board[SIZE][SIZE])
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
    cout << "This cell is fixed.\n";
    return true;
  }

  if (board[row][col] > 0)
  {
    cout << "Cell already occupied.\n";
    return true;
  }

  if (!isLegal(board, row, col, value))
  {
    cout << "Illegal move.\n";
    return true;
  }

  board[row][col] = value;
  return true;
}
bool isLegal(const int board[SIZE][SIZE], int row, int col, int value)
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
