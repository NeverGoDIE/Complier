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

Semanteme::Semanteme() {
	QUAD_table.push_back(QUAD("", "", "", ""));
}

void Semanteme::Parse() {
	cout << "======================" ;
	cout << "Parse" ;
	cout << "======================" << endl;
	cout << nWord.type << nWord.word << endl;
	int nChain = 0;
	Match(symbol_dic["main"]);
	Match(symbol_dic["("]);
	Match(symbol_dic[")"]);
	Statement_Block();
	gen(QUAD("", "", "", "EOF"));
	cout << "======================" ;
	cout << "end Parse!" ;
	cout << "======================" << endl;
	cout << "Done!!!!!!!!!" << endl;
}

int Semanteme::Statement_Block() {
	cout << "<======================";
	cout << "Statement_Block";
	cout << "======================" << endl;
	int sChain = nNXQ;
	Match(symbol_dic["{"]);
	Statement_Sequence();
	Match(symbol_dic["}"]);
	cout << "======================";
	cout << "end Statement_Block";
	cout << "<======================" << endl;
	return sChain;
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
	string op = "", argv1 = "", argv2 = "", result = "";
	result = nWord.word;
	Match(symbol_dic["id"]);
	op = nWord.word;
	Match(symbol_dic["="]);
	argv1 = Expression();
	gen(QUAD(op, argv1, argv2, result));
	cout << "======================";
	cout << "end Assignment_Statement" ;
	cout << "======================" << endl;
}

void Semanteme::Condition_Statement() {
	cout << "======================" ;
	cout << "Condition_Statement";
	cout << "======================" << endl;

	int ntc = 1, nfc = 1, s1jump;
	int s1, s2;
	Match(symbol_dic["if"]);
	Match(symbol_dic["("]);
	Condition(&ntc, &nfc);
	Match(symbol_dic[")"]);
	//nChainTemp = nfc;
	bp(ntc, nNXQ);
	Statement_Block();
	s1jump = nNXQ;
	gen(QUAD("goto", "", "", "0"));
	if (nWord.type == symbol_dic["else"]) {
		cout << ">>> Match:	" << nWord.word << endl;
		readNext();
		s2 = Statement_Block();
		bp(nfc, s2);
		bp(s1jump, nNXQ);
	}
	else {
		bp(nfc, nNXQ);
		bp(s1jump, nNXQ);
	}
	cout << "======================" ;
	cout << "end Condition_statement";
	cout << "======================" << endl;
}

void Semanteme::Loop_Statement() {
	cout << "======================";
	cout << "Loop_Statement" ;
	cout << "======================" << endl;
	int ntc = 1, nfc = 1, loopEnterence;
	Match(symbol_dic["do"]);
	loopEnterence = Statement_Block();
	Match(symbol_dic["while"]);
	Match(symbol_dic["("]);
	Condition(&ntc, &nfc);
	Match(symbol_dic[")"]);
	bp(ntc, loopEnterence);
	bp(nfc, nNXQ);
	cout << "======================" ;
	cout << "Loop_Statement" ;
	cout << "======================" << endl;
}

void Semanteme::Condition(int *etc, int *efc) {
	cout << "======================" ;
	cout << "Condition";
	cout << "======================" << endl;

	string op = "", argv1 = "", argv2 = "", result = "";
	
	argv1 = Expression();
	
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
	op = nWord.word;
	readNext();
	argv2 = Expression();
	result = Newtemp();
	gen(QUAD(op, argv1, argv2, result));
	*etc = nNXQ;
	*efc = nNXQ + 1;
	gen(QUAD("if", result, "goto", "0"));
	gen(QUAD("goto", "", "", "0"));
	cout << "next : " << nNXQ << endl;
	cout << "======================" ; 
	cout << "end Condition" ;
	cout << "======================" << endl;
}

string Semanteme::Expression() {
	cout << "======================" ;
	cout << "Expression" ;
	cout << "======================" << endl;	
	
	string op = "", argv1 = "", argv2 = "", result = "";

	argv1 = Item();
	while (nWord.type == symbol_dic["+"]
		|| nWord.type == symbol_dic["-"]) {
		cout << ">>> Match: " << nWord.word << endl;
		op = nWord.word;
		readNext();
		argv2 = Item();
		result = Newtemp();
		gen(QUAD(op, argv1, argv2, result));
		argv1 = result;
	}
	cout << "======================";
	cout << "end Expression";
	cout << "======================" << endl;
	return argv1;
}

string Semanteme::Item() {
	cout << "======================" ;
	cout << "Item";
	cout << "======================" << endl;

	string op="", argv1="", argv2="", result ="";

	argv1 = Factor();
	while (nWord.type == symbol_dic["*"]
		|| nWord.type == symbol_dic["/"]) {
		cout << ">>> Match: " << nWord.word << endl;
		op = nWord.word;
		readNext();
		argv2 = Factor();
		result = Newtemp();
		gen(QUAD(op, argv1, argv2, result));
		argv1 = result;
	}
	cout << "======================" ;
	cout << "end Item" ;
	cout << "======================" << endl;
	return argv1;
}

string Semanteme::Factor() {
	cout << "======================";
	cout << "Factor" ;
	cout << "======================" << endl;

	string argv;
	if (nWord.type == symbol_dic["id"]
		|| nWord.type == symbol_dic["digits"]) {
		cout << ">>> Match: " << nWord.word << endl;
		argv = nWord.word;
		readNext();
		cout << "======================";
		cout << "end Factor";
		cout << "======================" << endl;
		return argv;
	}
	else {
		Match(symbol_dic["("]);
		argv = Expression();
		Match(symbol_dic[")"]);
	}
	cout << "======================";
	cout << "end Factor" ;
	cout << "======================" << endl;
	return argv;
}

void Semanteme::gen(QUAD quaternion) {
	//cout << "<<<<<< generate >>>>>>>";
	QUAD_table.push_back(quaternion);
	nNXQ++;
	//print_QUADtable();
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
	string t;
	t.push_back('T');
	t += to_string(nsuffix);
	nsuffix++;
	return t;
}

int Semanteme::merg(int p1, int p2) {
	int p, nResult;
	if (p2 == 0) nResult = p1;
	else {
		nResult = p = p2;
		while (stoi(QUAD_table[p].result)) {
			p = stoi(QUAD_table[p].result);
		}
		QUAD_table[p].result = p1;
	}
	//cout << "<<<<<< merge >>>>>>>" ;
	//print_QUADtable();
	return nResult;
}

void Semanteme::bp(int p, int t) {
	int w, q = p;
	while (q) {
		w = stoi(QUAD_table[q].result);
		QUAD_table[q].result = to_string(t);
		q = w;
	}
	//cout << "<<<<<< backpatch >>>>>>>";
	//print_QUADtable();
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

void Semanteme::print_QUADtable() {
	vector<QUAD>::iterator it = QUAD_table.begin()+1;
	cout << endl;
	cout << "*******print QUAD_table********" << endl; 
	for (int i=1; it != QUAD_table.end(); it++,i++) {
		cout << "(" << setw(3) << i << ") ";
		cout.flags(ios::left);
		cout << "< " 
			<< setw(8) << it->op << ", "
			<< setw(8) << it->argv1 << ", "
			<< setw(8) << it->argv2 << ", "
			<< setw(8) << it->result << "> " << endl;
	}
}
void Semanteme::test() {
	Parse();
	print_QUADtable();
}