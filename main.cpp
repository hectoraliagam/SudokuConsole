#include <iostream>

using namespace std;

void waitForEnter();

int main()
{
  int board[9][9] = {};

  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      cout << board[i][j] << ' ';
    }
    cout << '\n';
  }

  waitForEnter();
  return 0;
}

void waitForEnter()
{
  cout << "\nPress Enter to exit...";
  string dummy;
  getline(cin, dummy);
}
