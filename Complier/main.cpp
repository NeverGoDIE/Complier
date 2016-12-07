#include <iostream>
#include <string>
#include "Scaner.h"

using namespace std;

int main()
{
    string str = "testscan.txt";
    Scaner s(str);
    s.test();
	cin.get();
}
