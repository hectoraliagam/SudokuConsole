#include <iostream>
#include <string>

using namespace std;

void printBoard(const int board[9][9]);
bool handleSetCommand(const string &command, int board[9][9]);

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
        cout << "Invalid command.\n";
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
  cout << '\n';

  for (int i = 0; i < 9; i++)
  {
    if (i % 3 == 0 && i != 0)
    {
      cout << "------+-------+------\n";
    }

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
  if (command.size() != 8)
  {
    return false;
  }

  char rowChar = command[4];   // a-i
  char colChar = command[5];   // 1-9
  char valueChar = command[7]; // 0-9

  if (rowChar < 'a' || rowChar > 'i')
  {
    return false;
  }

  if (colChar < '1' || colChar > '9')
  {
    return false;
  }

  if (valueChar < '0' || valueChar > '9')
  {
    return false;
  }

  int row = rowChar - 'a'; // fila
  int col = colChar - '1'; // columna
  int value = valueChar - '0';

  board[row][col] = value;
  return true;
}
