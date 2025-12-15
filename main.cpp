#include <iostream>

using namespace std;

void waitForEnter();
void printBoard(const int board[9][9]);

int main()
{
  int board[9][9] = {};

  printBoard(board);

  waitForEnter();
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
}
