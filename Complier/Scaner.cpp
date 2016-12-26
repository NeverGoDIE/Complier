#include  "Scaner.h"


Scaner::Scaner(string filename){
    this->filename = filename;
    this->pos = 0;
    readFile();
    delAnnotation();
	makevector();
	printInput();
}

wordtype Scaner::scan(){
	cout << "=========================" << endl;
    cout << "scan start!" << endl;
    wordtype wdt = {-1};
    token.clear();
	cout << "pos = " << pos << endl;
    gainch();
    cout << "ch = " << ch << endl;
	cout << "pos = " << pos << endl;
    getbc();
	
    if(isLetter(ch)){
        cout << "found a letter first!" << endl;
        while(isLetter(ch) || isDigit(ch)){
            token = concat(ch, token);
            if(!gainch())
                break;
        }
        if(pos < fileLen)
                retract();
        int type = reserve(token);
        pretoken.word = token;
        pretoken.type = type;
        wdt.type = type;
        wdt.word = token;
        return wdt;
    }
    else if(ch=='+' || ch=='-' || isDigit(ch)){
        cout << "maybe digits!" << endl;
        if(ch=='+' || ch=='-'){
            cout << "have a +/-!" << endl;
            token = concat(ch, token);
            if(pretoken.type==20 || pretoken.type==10 || pretoken.type==27) {
                if(ch=='+'){
                    cout << "after digit, it's a +!" << endl;
                    pretoken.word = token;
                    pretoken.type = 22;
                    wdt.type = 22;
                    wdt.word = token;
                }else if (ch=='-'){
                    cout << "after digit, it's a -!" << endl;
                    pretoken.word = token;
                    pretoken.type = 23;
                    wdt.type = 23;
                    wdt.word = token;
                }
                return wdt;
            }
            if(!gainch()){
                cout << "a +/- !" << endl;
                if(ch=='+'){
                    cout << "end,it's a +!" << endl;
                    pretoken.word = token;
                    pretoken.type = 22;
                    wdt.type = 22;
                    wdt.word = token;
                }else if (ch=='-'){
                    cout << "end,it's a -!" << endl;
                    pretoken.word = token;
                    pretoken.type = 23;
                    wdt.type = 23;
                    wdt.word = token;
                }
                return wdt;
            }
            if(!isDigit(ch)){
                cout << ch << "not a digit!" << endl;
                retract();
                if(ch=='+'){
                    cout << "it's a +!" << endl;
                    pretoken.word = token;
                    pretoken.type = 22;
                    wdt.type = 22;
                    wdt.word = token;
                }else if (ch=='-'){
                    cout << "it's a -!" << endl;
                    pretoken.word = token;
                    pretoken.type = 23;
                    wdt.type = 23;
                    wdt.word = token;
                }
                return wdt;
            }
        }
        while(isDigit(ch)){
            token = concat(ch, token);
            if(!gainch())
                break;
        }
        cout << "got integer!" << endl;
        if(ch!='.' && ch!='e'){
            if(!isDigit(ch))
				retract();
			pretoken.word = dtb(token);
            pretoken.type = 20;
            wdt.type = 20;
            wdt.word = dtb(token);
            return wdt;
        }
        if(ch=='.'){
            cout << "start getting a float." << endl;
            gainch();

            if (isDigit(ch)){
                retract();
                token = concat(ch, token);
                gainch();
                token = concat(ch, token);
                while(gainch() && isDigit(ch)){
                    token = concat(ch,token);
                }

                cout << "finished." << endl;
				cout << "pos:" << pos << endl;
			}
			else {
				retract();
				token = concat(ch, token);
				pretoken.word = token;
				pretoken.type = -1;
				wdt.type = -1;
				wdt.word = token;
				return wdt;
			}
            if(ch=='e'){
            }else{//do some error dealing staff
                if(!isdigit(ch))
					retract();
                pretoken.word = dtb(token);
                pretoken.type = 20;
                wdt.type = 20;
                wdt.word = dtb(token);
                return wdt;
            }
        }
        if(ch=='e'){
            cout << "start e part." << endl;
            gainch();
            if(ch=='+' || ch=='-'){
                cout << "start e1. ch=" << ch << endl;
                gainch();
                cout << "ch = " << ch << endl;
                if(isDigit(ch)){
                    cout << "if" << endl;
                    retract();
                    cout << "ch = " << ch << endl;
                }else{//error
                    cout << "else" << endl;
                    retract();
                    cout << "ch = " << ch << endl;
                    retract();
                    token = concat(ch, token);
                    cout << "ch = " << ch << endl;
                    pretoken.word = token;
                    pretoken.type = -1;
                    wdt.type = -1;
                    wdt.word = token;
                    return wdt;
                }
            }
            if(ch=='+' || ch=='-' || isDigit(ch)){
                cout << "start e2." << endl;
                retract();
                token = concat(ch, token);
                gainch();
                token = concat(ch, token);
                gainch();
                while(isDigit(ch)){
                    token = concat(ch, token);
                    if(!gainch())
                        break;
                }
                if(!isDigit(ch))
                    retract();

            }
        }
        pretoken.word = dtb(token);
        pretoken.type = 20;
        wdt.type = 20;
        wdt.word = dtb(token);
		cout << "pos" << pos << endl;
        return wdt;
    }
    else if(ch == '='){
        token = concat(ch, token);
        if(!gainch()){
            wdt.type = 21;
            wdt.word = token;
            pretoken.type = wdt.type;
            pretoken.word = wdt.word;
            return wdt;
        }
        if(ch == '='){
			token = concat(ch, token);
            wdt.type = 36;
            wdt.word = token;
            pretoken.type = wdt.type;
            pretoken.word = wdt.word;
            return wdt;
        }
        retract();
        wdt.type = 21;
        wdt.word = token;
    }
    else if(ch == '*'){
        token = concat(ch, token);
        wdt.type = 24;
        wdt.word = token;
    }
    else if(ch == '/'){
        token = concat(ch, token);
        wdt.type = 25;
        wdt.word = token;

    }
    else if(ch == '('){
        token = concat(ch, token);
        wdt.type = 26;
        wdt.word = token;
    }
    else if(ch == ')'){
        token = concat(ch, token);
        wdt.type = 27;
        wdt.word = token;
    }
    else if(ch == '{'){
        token = concat(ch, token);
        wdt.type = 28;
        wdt.word = token;

    }
    else if(ch == '}'){
        token = concat(ch, token);
        wdt.type = 29;
        wdt.word = token;

    }
    else if(ch == ','){
        token = concat(ch, token);
        wdt.type = 30;
        wdt.word = token;

    }
    else if(ch == ';'){
        token = concat(ch, token);
        wdt.type = 31;
        wdt.word = token;

    }
    else if(ch == '<'){
        token = concat(ch, token);
        if(!gainch()){
            wdt.type = 32;
            wdt.word = token;
            pretoken.type = wdt.type;
            pretoken.word = wdt.word;
            return wdt;
        }
        if(ch == '='){
			token = concat(ch, token);
            wdt.type = 33;
            wdt.word = token;
            pretoken.type = wdt.type;
            pretoken.word = wdt.word;
            return wdt;
        }
        retract();
        wdt.type = 32;
        wdt.word = token;
    }
    else if(ch == '>'){
        token = concat(ch, token);
        if(!gainch()){
            wdt.type = 34;
            wdt.word = token;
            pretoken.type = wdt.type;
            pretoken.word = wdt.word;
            return wdt;
        }
        if(ch == '='){
			token = concat(ch, token);
            wdt.type = 35;
            wdt.word = token;
            pretoken.type = wdt.type;
            pretoken.word = wdt.word;
            return wdt;
        }
        retract();
        wdt.type = 34;
        wdt.word = token;
    }
    else if(ch == '!'){
        token = concat(ch, token);
        if(!gainch()){
            return wdt;
        }
        if(ch == '='){
			token = concat(ch, token);
            wdt.type = 37;
            wdt.word = token;
            pretoken.type = wdt.type;
            pretoken.word = wdt.word;
            return wdt;
        }
        retract();
    }
    else if(ch == '#'){
        token = concat(ch, token);
        wdt.type = 0;
        wdt.word = token;
    }
	else {
		token = concat(ch, token);
		wdt.type = -2;
		wdt.word = token;
	}
    pretoken.type = wdt.type;
    pretoken.word = wdt.word;
    return wdt;
}


bool Scaner::isLetter(char ch){
    //if this character is a letter, return true.
    if(('a'<=ch && 'z'>=ch) || ('A'<=ch && 'Z'>=ch)){
        return true;
    }
    return false;
}


bool Scaner::isDigit(char ch){
    //if this character is a digit, return true.
    if('0'<=ch && '9'>= ch){
        return true;
    }
    return false;
}


string Scaner::concat(char ch, string str){
    string newstr = string(1, ch);
    str += newstr;
    return str;
    /*while(gainch()){
        //cout << pos;
        getbc();
        cout << ch;
    }*/
}


bool Scaner::gainch(){
    //get a character from file

    if( pos < fileLen ){
        ch = readBuffer[pos];
        //cout << ch << "  pos = " << pos << endl;
        //cout << ch;
        pos++;
        //cout << pos;
        return true;
    }
    return false;
}


void Scaner::getbc(){
    //if the character we got is blank, retrieve next ch.
    while(ch=='\0' || ch=='\t' || ch=='\r' || ch=='\n'
        || ch=='\v' || ch=='\f' || ch=='\x20' && pos<fileLen){
			
            if(!gainch())
                break;
            //cout << "  pos = " << pos << endl;
        }
}

int Scaner::reserve(string str){
    //check table to judge whether token is a keyword.
    int i;
    int flag = 0;
    //cout << "str : " << str << endl;
    for (i=0; i<KEYWDNUM; i++){
        //cout << "keywd : " << kwtable[i] << endl;
        if(str.compare(kwtable[i])==0){
           flag = 1;
           break;
        }
    }
    if (flag==1)
        return i+1;
    else return 10;
}

void Scaner::delAnnotation(){
    //delete all annotations
    int p_start = -1;
    int p_end = -1;
    while(gainch()){
        p_start = -1;
        p_end = -1;
        if(ch=='/'){
            p_start = pos - 1;
            if(!gainch()){
                cout << "annotation error" << endl;
            }else{
                if(ch=='/'){
                    cout << "found // here: " << pos-1 << endl;
                    while(ch!='\n' && gainch()){}
                    cout << "found \\n here: " << pos-1 <<endl;
                    p_end = pos -1;
                }
                else if(ch=='*'){
                    cout << "found /* here: " << pos << endl;
                    while(gainch()){
                        if(ch=='*' && gainch()){
                            if(ch=='/')
                                cout << "found */ here.: " << pos << endl;
                                p_end = pos;
                                break;
                        }
                    }
                }
                cout << "p_start:" << p_start << endl;
                cout << "p_end:" << p_end << endl;
                if(p_start>=0 && p_end>=0){
                    for(int i=p_start; i<p_end; i++){
                        readBuffer[i] = '\n';
                    }
                }
            }
        }
    }
    pos = 0;
}

void Scaner::readFile(){
    //read in part of a file
    cout << "read file start!" << endl;
    ifstream ifile;
	std::vector<char> fnstr(filename.begin(), filename.end());
	fnstr.push_back('\0');
	char* cstr = &fnstr[0];
    cout << "start open file:" << filename << endl;
    ifile.open(cstr, ios::in);
    cout << "file opened!" << endl;
    if(! ifile.is_open()){
        cout << "Error opening file" << endl;
    }
    cout << "retrieve start!" << endl;
    ifile.seekg(0, std::ios::end);
    int length = ifile.tellg();
    fileLen = length;
    cout << "file has " << length << "bytes." << endl;
    ifile.seekg(0, std::ios::beg);
    char *buffer = new char[length];
    ifile.read(buffer, length);
	cout << fileLen << endl;
    cout << "retrieve complete!" << endl;
    ifile.close();
    cout << "file closed!" << endl;
    readBuffer = buffer;
}

string Scaner::dtb(string str){
//convert
	bool isnegetive = false;
    double n;
    double num_n;
    int num_e,i_n;
    char * writable = new char[str.size() + 1];
    memcpy(writable, str.c_str(), str.size()+1);
    writable[str.size()] = '\0';
    int i = 0;
	if (str[0] == '-')
		isnegetive = true;
	//cout << "1" << endl;
    for(i=0; i<str.size(); i++){
        if(writable[i]=='e'){
            break;
        }
    }
    char *sub_n = new char[i+1];
    char *sub_e = new char[str.size() - i + 1];
    memcpy(sub_n, &writable[0], i);
    sub_n[i] = '\0';
    memcpy(sub_e, &writable[i+1], str.size()-i);
    sub_e[str.size()-i] = '\0';
    //cout << "n:" << sub_n << " e:" << sub_e << endl;
	int b=0;
	int dot=0;
	i_n = 0;
	//cout << "2" << endl;
	for (int j = i - 1; j >=0; j--) {
		//cout << "i_n:" << i_n << endl;
		if (sub_n[j] == '.') {
			dot = i-1-j;
			continue;
		}
		if (sub_n[j] == '+' || sub_n[j] == '-') {
			break;
		}
		//cout << "j:" << j; 
		//cout << " sub_n[j]:" << sub_n[j] << endl;
			
		int a = (int)sub_n[j] - '0';

		for (int k = b; k > 0; k--) {
			a *= 10;
		}
		i_n += a;
		b++;

	}
	//cout << "a:" << i_n << endl;
	//cout << "3" << endl;
	num_n = (double)i_n;
	//cout << "dot" << dot << endl;
	for (int k = dot; k > 0; k--) {
		num_n /= 10.0;
	}

	//printf("num_n=%.10g\n", num_n);
	//cout << "num_n:" << num_n << endl;

    num_e = atoi(sub_e);
	//printf("sub_e=%s\n", sub_e);
	//cout << num_e << endl;
    n = num_n;
    if(num_e > 0){
        for(i=0; i<num_e; i++)
            n *= 10.0;
    }else{
        for(i=0; i>num_e; i--)
            n /= 10.0;
    }
	if (isnegetive)
		n = -n;
    //cout << "www" << n << endl;
	char s[255];
	sprintf_s(s, "%.13g", n);
	//cout << " s:" << s << endl;
	str = s;
    return str;
}

void Scaner::retract(){//reverse a position
    pos -= 2;
    gainch();
}

void Scaner::makevector() {
	//ofstream file;
	//file.open("result.txt", ios::out);
	//cout << "f" << fileLen << endl;
	//cout << "fileLen: " << fileLen - 1 << endl;
	while (pos <= fileLen - 1) {
		wordtype w = scan();
		if (w.type == -2)
			continue;
		else {
			input.push_back(w);
		}
	}
	//file.close();
}

void Scaner::test(){
    /*cout << "start!" << endl;
    cout << "initial complete!" << endl;
    char* c;
    cout << fileLen << endl;
    cout << "============================================" << endl;
    cout << readBuffer << endl;
    cout << "============================================" << endl;
    cout << "len = " << fileLen << endl;
    /*while(gainch()){
        cout << ch;
    }*/
    /*while(gainch()){
        //cout << pos;
        getbc();
        cout << ch;
    }
    for(int i = 0; i<=10; i++){
        gainch();
        cout << ch;
    }
    cout << endl;
    for(int i = 0; i<=5; i++){
        retract();
        cout << ch;
    }
    string str = concat('a', "nihao");
    cout << str;*//*
    cout << "======================================" << endl;
    while(gainch()){
        cout << ch;
    }
    cout << "======================================" << endl;
    delAnnotation();
    cout << "======================================" << endl;
    while(gainch()){
        //cout << pos;
        getbc();
        cout << ch;
    }
    cout << "$End$" << endl;
    cout << "======================================" << endl;*//*
    cout << "match result: " << isLetter('D') << endl;
    cout << "match result: " << isLetter('z') << endl;
    cout << "match result: " << isLetter('1') << endl;
    cout << "match result: " << isDigit('s') << endl;
    cout << "match result: " << isDigit('1') << endl;*/

    ofstream file;
    file.open("result.txt", ios::out);
	cout << "f" << fileLen << endl;
	cout << "fileLen: " << fileLen -1 << endl;
    while(pos < fileLen-1){
		wordtype w = scan();
		if (w.type == -2)
			continue;
		else if(w.type == 20){
			cout << setiosflags(ios::fixed);
			cout << "wd:( " << w.type << ",  " << w.word << ", " << pos << ")" << endl;
			cout << "++++++++++++++++++++++++++++++++++++++" << endl;
			file << "(" << w.type << ", ";
			file << setprecision(10) << w.word << ")" << endl;
		}
		else if (w.type != -1){
			cout << "wd:( " << w.type << ",  " << w.word << ", " << pos << ")" << endl;
            cout << "++++++++++++++++++++++++++++++++++++++" << endl;
            file << "(" << w.type << ", " << w.word << ")" <<endl;
		}else{
           cout << "wd:( " << w.type << ",  " << w.word << "," << pos  << ") UNKNOWN TOKEN!" << endl;
            file << "(" << w.type << ", " << w.word << ") UNKNOWN TOKEN!" <<endl;
		}
    }
    file.close();

    /*cout << dtb("-12.345e-1") << endl;
    cout << dtb("-12") << endl;
    cout << dtb("-1324e-1000") << endl;*/


}

void Scaner::printInput() {
	vector<wordtype>::iterator it = input.begin();
	for (; it != input.end(); it++) {
		cout << '<' << it->type << ", ";
		cout << it->word << '>' << endl;
	}
}
