#ifndef SCANER_H
#define SCANER_H

#include <fstream>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

#define KEYWDNUM 9

typedef int POSITION;


class wordtype {
public:
	int type;
	string word;
};




class Scaner{
private:
    char ch;            //the character we retrieved just now
    string token;       //storage the word read
    wordtype pretoken = {-1, ""};
    string filename;    //storage the name of the file we scanned
    char *readBuffer;   //buffer that storage part of the file;
    POSITION pos;       //mark the position in suffer;
    string kwtable[KEYWDNUM] = {"main", "int", "float", "double", "char", "if", "else", "do", "while"};
    int fileLen;

public:
    Scaner(string filename);
    wordtype scan();
    bool gainch();
    void getbc();
    string concat(char ch, string str);
    bool isLetter(char ch);
    bool isDigit(char ch);
    int reserve(string str);
    void retract();
    string dtb(string str);
    void readFile();
    void test();
    void delAnnotation();
	vector<wordtype> input;
	void makevector();
	void printInput();
};
#endif // SCANER
