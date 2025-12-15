#include <iostream>
#include <string>

using namespace std;

void printBoard(const int board[9][9]);
bool handleSetCommand(const string &command, int board[9][9]);
bool isLegal(const int board[9][9], int row, int col, int value);

int main()
{
  bool running = true;
  int board[9][9] = {};

  while (running)
  {
    printBoard(board);

    cout << "Enter a command: ";
    string command;
    getline(cin, command);

    if (command == "exit")
    {
      running = false;
    }
    else if (command.substr(0, 3) == "set")
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

void printBoard(const int board[9][9])
{
  cout << "\n    1 2 3   4 5 6   7 8 9\n\n";

  for (int i = 0; i < 9; i++)
  {
    if (i % 3 == 0 && i != 0)
    {
      cout << "    ------+-------+------\n";
    }

    cout << char('A' + i) << "   ";

    for (int j = 0; j < 9; j++)
    {
      if (j % 3 == 0 && j != 0)
      {
        cout << "| ";
      }

      cout << board[i][j] << ' ';
    }

    cout << '\n';
  }

  cout << '\n';
}
bool handleSetCommand(const string &command, int board[9][9])
{
  // Expected format: set a1 5
  if (command.length() != 8)
  {
    return false;
  }

  char rowChar = toupper(command[4]);
  char colChar = command[5];
  char valChar = command[7];

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

  if (board[row][col] != 0)
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
bool isLegal(const int board[9][9], int row, int col, int value)
{
  if (value < 1 || value > 9)
  {
    return false;
  }

  // Check row
  for (int j = 0; j < 9; j++)
  {
    if (board[row][j] == value)
    {
      return false;
    }
  }

  // Check column
  for (int i = 0; i < 9; i++)
  {
    if (board[i][col] == value)
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
      if (board[i][j] == value)
      {
        return false;
      }
    }
  }

  return true;
}
