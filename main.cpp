#include <iostream>
#include <string>
using namespace std;

void waitForEnter();

int main()
{
  cout << "Sudoku Console\n";

  waitForEnter();

  return 0;
}

void waitForEnter()
{
  cout << "\nPress Enter to exit...";
  string dummy;
  getline(cin, dummy);
}
