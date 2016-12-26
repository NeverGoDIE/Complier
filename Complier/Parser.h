#ifndef Parser_H
#define Parser_H

#include "Scaner.h"
#include <set>
#include <map>
#include <stack>


#define MAX 507


using namespace std;


/*class QUAD {
public:
	string op = "";
	string argv1 = "";
	string argv2 = "";
	string result = "";

	QUAD(string op, string argv1, string argv2, string result) {
		this->op = op;
		this->argv1 = argv1;
		this->argv2 = argv2;
		this->result = result;
	}
};*/

class inputType {
public:
	string type = "-1";
	string word = "";


	inputType(string type, string word) {
		this->type = type;
		this->word = word;
	}


};

class WF {
private:
	string left;
	set<string> right;
public:
	WF(char s[]) {
		left = s;
	}
	void print();
	void insert( char s[] );
	string getLeft() {
		return left;
	}
	set<string> getRight() {
		return right;
	}
};

class PredictMethod {
private:
	map<string, set<char>> first;
	map<string, set<char>> follow;
	map<string, int> VN_dic;
	vector<WF> VN_set;
	bool used[MAX];
	string filename = "testParse.txt";
	vector<map<char, string>> predict_table;
	vector<char> letter;
	Scaner scn = Scaner("testscan.txt");

	//vector<QUAD> Quad_table;
	map<string, int> Quad_dic;

	stack<string> aStack;
	stack<stack<string>> bStack;

public:
	void dfs(int x);
	void make_first();

	void append(const string& str1, const string& str2);
	void make_follow();

	bool check_first(const string& text, char ch);
	bool check_follow(const string& text, char ch);
	void make_table();

	bool analyze(vector<wordtype> input);
	inputType trans2VT(wordtype w);

	//void symantics(string aSt, string iSt);
	void test();
};

#endif