#include "Scaner.h"
#include <map>
#include <stdlib.h>


class QUAD {
public:
	string op;
	string argv1;
	string argv2;
	string result;
	QUAD(string op, string argv1, string argv2, string result); 
	QUAD();
};

class Semanteme {
private:
	vector<QUAD> QUAD_table;	//store quaternion
	
	int nsuffix, nNXQ, ntc, nfc;
	Scaner scan = Scaner("testscan.txt"); 
	vector<wordtype>::iterator it = scan.input.begin();
	wordtype nWord = { it->type, it->word };

	map<string, int> symbol_dic = {
		{"main", 1},
		{"int", 2},
		{"float", 3},
		{"double", 4},
		{"char", 5}, 
		{"if", 6},
		{"else", 7},
		{"do", 8},
		{"while", 9},
		{"id", 10},
		{"digits", 20},
		{"=", 21},
		{"+", 22},
		{"-", 23},
		{"*", 24},
		{"/", 25},
		{"(", 26},
		{")", 27},
		{"{", 28},
		{"}", 29},
		{",", 30},
		{";", 31},
		{">", 32},
		{">=", 33},
		{"<", 34},
		{"<=", 35},
		{"==", 36},
		{"!=", 37},
		{"#", 0}
	};

public:
	void Expression();
	void Item();
	void Factor();
	void Condition();
	void Condition_Statement();
	void Loop_Statement();
	void Assignment_Statement();
	
	void Statement();
	void Statement_Sequence();
	void Statement_Block();
	void Parse();
	
	void gen(QUAD quaternion);
	bool Match(int type);
	string Newtemp();
	int merg(int p1, int p2);
	void bp(int p, int t);
	void readNext();
	void retract();
	void error(string errmsg);

	void test();
};