#include <iostream>

using namespace std;

// void waitForEnter();
void printBoard(const int board[9][9]);

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

    if (command.substr(0, 3) == "set")
    {
      int y = command[4] - 'a';
      int x = command[5] - '1';
      int value = command[7] - '0';
      board[x][y] = value;
    }
    if (command == "exit")
    {
      running = false;
    }
  }

  // waitForEnter();
  return 0;
}

void waitForEnter()
{
  cout << "\nPress Enter to exit...";
  string dummy;
  getline(cin, dummy);
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
