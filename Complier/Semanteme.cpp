#include "Semanteme.h"

QUAD::QUAD(string op, string argv1, string argv2, string result) {
	this->op = op;
	this->argv1 = argv1;
	this->argv2 = argv2;
	this->result = result;
}

QUAD::QUAD() {
	this->op = "";
	this->argv1 = "";
	this->argv2 = "";
	this->result = "";
}

void Semanteme::Parse() {
	cout << "======================" ;
	cout << "Parse" ;
	cout << "======================" << endl;
	cout << nWord.type << nWord.word << endl;
	Match(symbol_dic["main"]);
	Match(symbol_dic["("]);
	Match(symbol_dic[")"]);
	Statement_Block();
	cout << "======================" ;
	cout << "end Parse!" ;
	cout << "======================" << endl;
	cout << "Done!!!!!!!!!" << endl;
}

void Semanteme::Statement_Block() {
	cout << "<======================";
	cout << "Statement_Block";
	cout << "======================" << endl;
	Match(symbol_dic["{"]);
	Statement_Sequence();
	Match(symbol_dic["}"]);
	cout << "======================";
	cout << "end Statement_Block";
	cout << "<======================" << endl;
}

void Semanteme::Statement_Sequence() {
	cout << "======================";
	cout << "Statement_Sequence";
	cout << "======================" << endl;
	Statement();
	Match(symbol_dic[";"]);
	//readNext();
	while (nWord.type == symbol_dic["id"]
		|| nWord.type == symbol_dic["do"]
		|| nWord.type == symbol_dic["if"]) {
		Statement();
		Match(symbol_dic[";"]);
		//readNext();
	}
	cout << "======================" ;
	cout << "end Statement_Sequence";
	cout << "======================" << endl;
}

void Semanteme::Statement() {
	cout << "======================" ;
	cout << "Statement";
	cout << "======================" << endl;
	cout << "nword.type" << nWord.type << endl;
	cout << "nword.word" << nWord.type << endl;
	if (nWord.type == symbol_dic["id"]) {
		cout << ">>> Match: " << nWord.word << endl;
		Assignment_Statement();
	}
	else if (nWord.type == symbol_dic["do"]) {
		cout << ">>> Match: " << nWord.word << endl;
		Loop_Statement();
	}
	else if (nWord.type == symbol_dic["if"]) {
		cout << ">>> Match: " << nWord.word << endl;
		Condition_Statement();
	}

	cout << "======================";
	cout << "end Statement" ;
	cout << "======================" << endl;
}

void Semanteme::Assignment_Statement() {
	cout << "======================"; 
	cout << "Assignment_Statement" ;
	cout << "======================" << endl;
	Match(symbol_dic["id"]);
	Match(symbol_dic["="]);
	Expression();
	cout << "======================";
	cout << "end Assignment_Statement" ;
	cout << "======================" << endl;
}

void Semanteme::Condition_Statement() {
	cout << "======================" ;
	cout << "Condition_Statement";
	cout << "======================" << endl;
	Match(symbol_dic["if"]);
	Match(symbol_dic["("]);
	Condition();
	Match(symbol_dic[")"]);
	Statement_Block();
	if (nWord.type == symbol_dic["else"]) {
		cout << ">>> Match:	" << nWord.word << endl;
		readNext();
		Statement_Block();
	}
	//else
	//	retract();
	cout << "======================" ;
	cout << "end Condition_statement";
	cout << "======================" << endl;
}

void Semanteme::Loop_Statement() {
	cout << "======================";
	cout << "Loop_Statement" ;
	cout << "======================" << endl;
	Match(symbol_dic["do"]);
	Statement_Block();
	Match(symbol_dic["while"]);
	Match(symbol_dic["("]);
	Condition();
	Match(symbol_dic[")"]);
	cout << "======================" ;
	cout << "Loop_Statement" ;
	cout << "======================" << endl;
}

void Semanteme::Condition() {
	cout << "======================" ;
	cout << "Condition";
	cout << "======================" << endl;
	Expression();
	
	if (!(nWord.type == symbol_dic[">"]
		|| nWord.type == symbol_dic[">="]
		|| nWord.type == symbol_dic["<"]
		|| nWord.type == symbol_dic["<="]
		|| nWord.type == symbol_dic["=="]
		|| nWord.type == symbol_dic["!="]))
	{
		error("op");
	}
	cout << ">>> Match:" << nWord.word << endl;
	readNext();
	Expression();
	cout << "======================" ; 
	cout << "end Condition" ;
	cout << "======================" << endl;
}

void Semanteme::Expression() {
	cout << "======================" ;
	cout << "Expression" ;
	cout << "======================" << endl;	
	Item();
	while (nWord.type == symbol_dic["+"]
		|| nWord.type == symbol_dic["-"]) {
		cout << ">>> Match: " << nWord.word << endl;
		readNext();
		Item();
	}
	cout << "======================";
	cout << "end Expression";
	cout << "======================" << endl;
}

void Semanteme::Item() {
	cout << "======================" ;
	cout << "Item";
	cout << "======================" << endl;
	Factor();
	while (nWord.type == symbol_dic["*"]
		|| nWord.type == symbol_dic["/"]) {
		cout << ">>> Match: " << nWord.word << endl;
		readNext();
		Factor();
	}
	cout << "======================" ;
	cout << "end Item" ;
	cout << "======================" << endl;
}

void Semanteme::Factor() {
	cout << "======================";
	cout << "Factor" ;
	cout << "======================" << endl;
	if (nWord.type == symbol_dic["id"]
		|| nWord.type == symbol_dic["digits"]) {
		cout << ">>> Match: " << nWord.word << endl;
		readNext();
		cout << "======================";
		cout << "end Factor";
		cout << "======================" << endl;
		return;
	}
	else {
		Match(symbol_dic["("]);
		Expression();
		Match(symbol_dic[")"]);
	}
	cout << "======================";
	cout << "end Factor" ;
	cout << "======================" << endl;
}

void Semanteme::gen(QUAD quaternion) {
}

bool Semanteme::Match(int type) {
	cout << "Word.word == " << nWord.word << endl;
	cout << "Word.type == " << nWord.type << endl;
	cout << "wanted type == " << type << endl;
	if (nWord.type == type) {
		cout << ">>> Match: " << nWord.word << endl;
		readNext();
		return true;
	}
	else
		error(nWord.word);
	
}

string Semanteme::Newtemp() {
	return "";
}

int Semanteme::merg(int p1, int p2) {
	return 1;
}

void Semanteme::bp(int p, int t) {

}

void Semanteme::readNext() {
	if (it != scan.input.end() - 1) {
		it++;
		nWord.type = it->type;
		nWord.word = it->word;

	}
}

void Semanteme::retract() {
	it--;
	nWord.type = it->type;
	nWord.word = it->word;
}

void Semanteme::error(string errmsg) {
	cout << "error:" << errmsg << endl;
	system("pause");
}

void Semanteme::test() {
	Parse();
}