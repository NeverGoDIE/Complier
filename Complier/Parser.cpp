#include "Parser.h"

void WF::print() {
	cout << left.c_str() << "->";
	set<string>::iterator it = right.begin();
	if (right.begin() != right.end()) {
		cout << it->c_str() << "|";
		it ++;
	}
	for (; it != right.end(); it++) {
		cout << it->c_str();
	}
	puts("");//change line
}

void WF::insert(char s[]) {
	right.insert(s);
}

void PredictMethod::dfs(int x) {
	//cout << "enter DFS " << x << endl;
	if (used[x]) return;
	used[x] = 1;
	string& left = VN_set[x].getLeft();
	set<string> right = VN_set[x].getRight();
	set<string>::iterator it = right.begin();
	for (; it != right.end(); it++) {
		for (int i = 0; i < it->length(); i++) {
			if (!isupper(it->at(i)) && it->at(i) != '\'') {
				first[left].insert(it->at(i));
				break;
			}
			if (isupper(it->at(i))) {
				int y;
				if (i != it->length() - 1 && it->at(i + 1) == '\'')
					//if it is a Non-terminal(like E'), read in 2 words.
					y = VN_dic[it->substr(i, 2)] - 1;
				else y = VN_dic[it->substr(i, 1)] - 1;
					//if it is a Non-terminal(like E), read in 1 word.
				string& tleft = VN_set[y].getLeft();	//get left part
				dfs(y);
				set<char>& temp = first[tleft];
				//create a pointer point to the left part of VN(which we are dealing with)
				//if you modify the value of temp, value of first[tleft] modified either.
				set<char>::iterator it1 = temp.begin();
				bool flag = true;
				cout << "============================" << endl;
				for (; it1 != temp.end(); it1++) {
					cout << *it1 << endl;
					if (*it1 == '&') flag = false;
					first[left].insert(*it1);
				}
				cout << "============================" << endl;
				if (flag) break;
			}
			else continue;
		}
	}
	//cout << "end DFS " << x << endl;
}

void PredictMethod::make_first() {
	memset(used, 0, sizeof(used));
	for (int i = 0; i < VN_set.size(); i++)
		dfs(i);
	map<string, set<char>>::iterator it = first.begin();
	for (; it != first.end(); it++) {
		cout << "FIRST" << it->first.c_str() << "={";
		set<char> & temp = it->second;
		set<char>::iterator it1 = temp.begin();
		bool flag = false;
		for (; it1 != temp.end(); it1++) {
			if (flag) cout << ",";
			cout << *it1;
			flag = true;
		}
		puts("}");
	}
}

void PredictMethod::test() {
	cout << "test start!" << endl;
	int n;
	char s[MAX]; 
	ifstream ifile;
	ifile.open("testParse.txt");
	cout << filename << " file opened!" << endl;
	cout << "start dealing with file!" << endl;
	ifile.getline(s, 256);
	cout << s << endl;
	n = atoi(s);
	cout << n << endl;
	for (int i = 0; i < n; i++) {
		//cout << "Loop" << i << endl;
		ifile.getline(s, 256);
		int len = strlen(s), j;
		for (j = 0; j < len; j++)
			if (s[j] == '-') break;
		s[j] = 0;
		if (!VN_dic[s]) {
			VN_set.push_back(s);
			VN_dic[s] = VN_set.size();
		}
		int x = VN_dic[s] - 1;
		VN_set[x].insert(s + j + 2);
	}

	vector<WF>::iterator it = VN_set.begin();
	for (; it != VN_set.end(); it++) {
		it->print();
	}
	map<string, int>::iterator it1 = VN_dic.begin();
	for (; it1 != VN_dic.end(); it1++) {
		cout << "<" << it1->first << "\t, ";
		cout << it1->second << ">" << endl;
	}
	make_first();
	make_follow();

	puts("****************FOLLOW¼¯**********************");
	map<string, set<char> >::iterator it2 = follow.begin();
	for (; it2 != follow.end(); it2++)
	{
		printf("FOLLOW(%s)={", it2->first.c_str());
		set<char> & temp = it2->second;
		set<char>::iterator it3 = temp.begin();
		bool flag = false;
		for (; it3 != temp.end(); it3++)
		{
			if (flag) printf(",");
			printf("%c", *it3);
			flag = true;
		}
		puts("}");
	}
	make_table();
	cout << analyze(scn.input) << endl;

	cout << "test ended!" << endl;
}

void PredictMethod::append(const string& str1, const string& str2) {
	//append follow[str1] to follow[str2]
	set<char>& from = follow[str1];
	set<char>& to = follow[str2];
	set<char>::iterator it = from.begin();
	for (; it != from.end(); it++) {
		to.insert(*it);
	}
}

void PredictMethod::make_follow() {
	cout << "start make Follow." << endl;
	string s = VN_set.at(0).getLeft();
	follow[s].insert('$');
	while (true) {

		bool goon = false;
		for (int i = 0; i < VN_set.size(); i++) {
			//cout << "start analyze whole expression!" << endl;
			string& left = VN_set[i].getLeft();
			set<string> right = VN_set[i].getRight();
			set<string>::iterator it = right.begin();
			for (; it != right.end(); it++) {
				//cout << "start analyze single expression:" << *it << endl;
				bool flag = true;
				const string& str = *it;
				for (int j = it->length() - 1; j >= 0; j--) {
					if (str[j] == '\'') {
						//cout << "a" << endl;
						//if last character is ', which means it is a VN.
						int x = VN_dic[it->substr(j - 1, 2)] - 1;
						if (flag) {
							int tt = follow[it->substr(j - 1, 2)].size();
							append(left, it->substr(j - 1, 2));
							int tt1 = follow[it->substr(j - 1, 2)].size();
							if (tt1 > tt) goon = true;
							//if a new element had insert into follow, set goon.
							if (!VN_set[x].getRight().count("~"))
								flag = false;
							//if a epsilon can be deducted from VN, set flag.
						}	
						for (int k = j + 1; k < it->length(); k++) {
							//rule2 A->aAb
							if (!isupper(str[k])) {
								string id;
								if(k!=it->length()-1 && str[k+1]=='\''){
									id = it->substr(k, 2);
								}
								else id = it->substr(k, 1);
								set<char>& from = first[id];
								set<char>& to = follow[it->substr(j - 1, 2)];
								int tt = to.size();
								set<char>::iterator it1 = from.begin();
								for (; it1 != from.end(); it1++) {
									if (*it1 != '~')
										to.insert(*it1);
									int tt1 = follow[it->substr(j - 1, 2)].size();
									if (tt1 > tt) goon = true;
									if (!VN_set[VN_dic[id] - 1].getRight().size())
										break;
								}
							}
							else if (str[k] != '\'') {
								int tt = follow[it->substr(j - 1, 2)].size();
								follow[it->substr(j - 1, 2)].insert(str[k]);
								int tt1 = follow[it->substr(j - 1, 2)].size();
								if (tt1 > tt) goon = true;
								break;
							}
							else continue;
						}
						j--;
					}
					else if (isupper(str[j])) {
						int x = VN_dic[it->substr(j, 1)] - 1;
						//the verb we are analyzing.
						if (flag) {
							int tt = follow[it->substr(j, 1)].size();
							append(left, it->substr(j, 1));
							if (!VN_set[x].getRight().count("~"))
								flag = false;
							int tt1 = follow[it->substr(j, 1)].size();
							if (tt1 > tt) goon = true;
						}
						for (int k = j + 1; k < it->length(); k++) {
							if (isupper(str[k])) {
								string id;
								if (k != it->length() - 1 && str[k + 1] == '\'')
									id = it->substr(k, 2);
								else id = it->substr(k, 1);
								set<char>& from = first[id];
								set<char>& to = follow[it->substr(j, 1)];
								set<char>::iterator it1 = from.begin();
								int tt = follow[it->substr(j, 1)].size();
								for (; it1 != from.end(); it1++)
									if (*it1 != '~')
										to.insert(*it1);
								int tt1 = follow[it->substr(j, 1)].size();
								if (tt1 > tt) goon = true;
								if (!VN_set[VN_dic[id] - 1].getRight().count("~"))
									break;
							}
							else if (str[k] != '\'') {
								int tt = follow[it->substr(j, 1)].size();
								follow[it->substr(j, 1)].insert(str[k]);
								int tt1 = follow[it->substr(j, 1)].size();
								if (tt1 > tt) goon = true;
								break;

							}
							else continue;
						}
					}
					else flag = false;
				}
			}
		}
		if (!goon) break;
	}
	/*string s = VN_set.at(0).getLeft();
	follow[s].insert('$');
	while (true) {
		cout << "start make Follow." << endl;
		for (int i = 0; i < VN_set.size(); i++) {
			cout << "start analyze whole expression!" << endl;
			string left = VN_set.at(i).getLeft();
			set<string> right = VN_set.at(i).getRight();
			set<string>::iterator it = right.begin();
			for (; it != right.end(); it++) {
				cout << "start analyze single expression:" << *it << endl;
				for (int j = it->size() - 1; j >= 0; j--) {
					//analyze single word
					
				}
			}

		}
	}*/
}

bool PredictMethod::check_first(const string& text, char ch) {
	for (int i = 0; i < text.length(); i++) {
		bool hasEmpty = false;
		if (!isupper(text[i]) && text[i] != '\'') {
			if (text[i] == ch) return true;
			else return false;
		}
		else if (isupper(text[i])) {
			string temp;
			if (i != text.length() - 1 && text[i + 1] == '\'')
				temp = text.substr(i, 2);
			else
				temp = text.substr(i, 1);
			set<char>& dic = first[temp];
			set<char>::iterator it = dic.begin();
			for (; it != dic.end(); it++) {
				if (*it == '~') hasEmpty = true;
				if (*it == ch) return true;
			}
			if (!hasEmpty) break;
		}
		else continue;
	}
	return false;
}

bool PredictMethod::check_follow(const string& text, char ch) {
	set<char>& dic = follow[text];
	set<char>::iterator it = dic.begin();
	for (; it != dic.end(); it++) {
		if (*it == ch) return true;
	}
	return false;
}

void PredictMethod::make_table() {
	map<char, string> temp;
	bool vis[500];
	memset(vis, 0, sizeof(vis));
	for (int i = 0; i < VN_set.size(); i++) {
		set<string>& right = VN_set[i].getRight();
		set<string>::iterator it = right.begin();
		for (; it != right.end(); it++) {
			for (int j = 0; j < it->length(); j++) {
				if (!isupper(it->at(j)) && it->at(j) != '\'') {
					if (vis[it->at(j)]) continue;
					vis[it->at(j)] = true;
					cout << "it->at(j) :" << it->at(j) << endl;
					letter.push_back(it->at(j));
				}
			}
		}
	}
	letter.push_back('$');
	for (int i = 0; i < VN_set.size(); i++) {
		temp.clear();
		string& left = VN_set[i].getLeft();
		set<string>& right = VN_set[i].getRight();
		set<string>::iterator it = right.begin();
		for (; it != right.end(); it++) {
			for (int j = 0; j < letter.size(); j++) {
				if (check_first(*it, letter[j])) {
					temp[letter[j]] = *it;
				}
				if (it->at(0) == '~' && check_follow(left, letter[j]))
					temp[letter[j]] = *it;
			}
		}
		predict_table.push_back(temp);
	}

	for (int i = 0; i <= (letter.size() + 1) * 6; i++)
		printf("-");
	puts("");
	printf("|%7s", "|");
	for (int i = 0; i < letter.size(); i++)
		printf("%3c%3s", letter[i], "|");
	puts("");
	for (int i = 0; i <= (letter.size() + 1) * 6; i++)
		printf("-");
	puts("");
	for (int i = 0; i < VN_set.size(); i++)
	{
		printf("|%3s%3s", VN_set[i].getLeft().c_str(), "|");
		for (int j = 0; j < letter.size(); j++)
			if (predict_table[i].count(letter[j]))
				printf("%3s%3s", predict_table[i][letter[j]].c_str(), "|");
			else printf("%6s", "|");
			puts("");
			for (int i = 0; i <= (letter.size() + 1) * 6; i++)
				printf("-");
			puts("");
	}
}

bool PredictMethod::analyze(vector<wordtype> input) {
	
	cout << "complier start!!!!!!" << endl;

	vector<wordtype>::iterator it = input.begin();
	for (; it != input.end(); it++) {
		cout << '<' << it->type << ", ";
		cout << it->word << '>' << endl;
	}

	

	stack<string> analyzeStack;
	stack<inputType> inputStack;
	analyzeStack.push("$");
	inputStack.push(inputType("$", ""));
	for (int i = input.size() - 1; i >= 0; i--) {
		inputStack.push(trans2VT(input[i]));
	}
	
	analyzeStack.push(VN_set[0].getLeft());
	cout << ">>>>>>>>>>>>>>>>>>>>" << endl;
	while (analyzeStack.top()!="$" || inputStack.top().type!="$") {

		cout << "aSize" << aStack.size() << endl;
		cout << "bSize" << bStack.size() << endl;
		if (!empty(aStack)) {
			cout << "aStack=" << aStack.top() << endl;
		}
		if (!empty(bStack)) {
			cout << "bStack=" << bStack.top().top() << endl;
		}
		
		cout << "analyzeStackTop = " << analyzeStack.top() << endl;
		cout << "inputStacktop = " << inputStack.top().type;
		cout << ", " << inputStack.top().word << endl;
		while ((analyzeStack.top() == "T" || analyzeStack.top() == "T'") && inputStack.top().type=="}") {
			if (analyzeStack.top() == "T") {
				while (analyzeStack.top() == "T" && inputStack.top().type == "}") {
					analyzeStack.pop();
					//analyzeStack.pop();
					//analyzeStack.pop();
					cout << "pop T" << endl;
					bStack.top().pop();
					while ((!empty(aStack)) && (!empty(bStack)) && empty(bStack.top())) {
						aStack.pop();
						bStack.pop();
						if (bStack.size() > 0)
							bStack.top().pop();
					}
				}
			}
			else {
				while (analyzeStack.top() == "T'" && inputStack.top().type == "}") {
					analyzeStack.pop();
					cout << "pop T'" << endl;
					bStack.top().pop();
					while ((!empty(aStack)) && (!empty(bStack)) && empty(bStack.top())) {
						aStack.pop();
						bStack.pop();
						if (bStack.size() > 0)
							bStack.top().pop();
					}
				}
				analyzeStack.pop();
				bStack.top().pop();
				while ((!empty(aStack)) && (!empty(bStack)) && empty(bStack.top())) {
					aStack.pop();
					bStack.pop();
					if (bStack.size() > 0)
						bStack.top().pop();
				}
				cout << "pop ;" << endl;
			}
		}
		cout << "analyzeStackTop = " << analyzeStack.top() << endl;
		cout << "inputStacktop = " << inputStack.top().type;
		cout << ", " << inputStack.top().word << endl;
		
		if (isupper(analyzeStack.top()[0])) {
			cout << "VN on top!" << endl;
			cout << "a====:" << inputStack.size() << endl;
			string s = analyzeStack.top();
			string s1 = analyzeStack.top();
			

			analyzeStack.pop();
			int seq = VN_dic[s] - 1;
			//cout << "seq:" << seq << endl;
			char headletter = inputStack.top().type.c_str()[0];
			s = predict_table[seq][headletter];
			cout << "predict:" << s << endl;
			if (s == "~") {
				cout << "push ~" << endl;
				bStack.top().pop();
				while ((!empty(aStack)) && (!empty(bStack)) && empty(bStack.top())) {
					aStack.pop();
					bStack.pop();
					if (bStack.size() > 0)
						bStack.top().pop();
				}
			}
			else {
				aStack.push(s1);
				stack<string> r;
				for (int j = s.size() - 1; j >= 0; j--) {
					if (s[j] == '\'') {
						analyzeStack.push(s.substr(j - 1, 2));
						r.push(s.substr(j - 1, 2));
						cout << "push:" << s.substr(j - 1, 2) << endl;
						j--;
						continue;
					}
					analyzeStack.push(s.substr(j, 1));
					r.push(s.substr(j, 1));
					cout << "push:" << s.substr(j, 1) << endl;
				}
				bStack.push(r);

			}
			
			if (s.empty()) {
				cout << "aS:" << endl;
				for (int i = analyzeStack.size() - 1; i >= 0; i--) {
					cout << analyzeStack.top();
					analyzeStack.pop();
				}
				cout << endl;
				for (int i = inputStack.size() - 1; i >= 0; i--) {
					cout << inputStack.top().word;
					inputStack.pop();
				}
				cout << endl;
				return false;
			}
				
		}
		else {
			cout << "VT on top" << endl;
			if (analyzeStack.top() == inputStack.top().type)	{
				analyzeStack.pop();
				inputStack.pop();
///////////////////////////////////////////////////////
				bStack.top().pop();
				cout << bStack.size() << endl;
				
				while ( (!empty(aStack)) && (!empty(bStack)) && empty(bStack.top())) {
					aStack.pop();
					bStack.pop();
					if (bStack.size() > 0)
						bStack.top().pop();
				}
	
				
///////////////////////////////////////////////////////
			}
			else {
				cout << "aS:" << endl;
				for (int i = analyzeStack.size() - 1; i >= 0; i--) {
					cout << analyzeStack.top();
					analyzeStack.pop();
				}
				cout << endl; 
				for (int i = inputStack.size() - 1; i >= 0; i--) {
					cout << inputStack.top().word;
					inputStack.pop();
				}
				cout << endl;
				return false;
			}
		}
		cout << "<<<<<<<<<<<<<<<<<<<<" << endl;
	}



	cout << "analyzeStackTop = " << analyzeStack.top() << endl;
	cout << "inputStacktop = " << inputStack.top().type;
	cout << ", " << inputStack.top().word << endl;


	cout << "aSize" << aStack.size() << endl;
	cout << "bSize" << bStack.size() << endl;

	if (analyzeStack.top() == "$" && inputStack.top().type == "$")
		return true;
	else {
		cout << "aS:" << endl;
		for (int i = analyzeStack.size() - 1; i >= 0; i--) {
			cout << i << analyzeStack.top();
			analyzeStack.pop();
		}
		cout << endl;
		for (int i = inputStack.size() - 1; i >= 0; i--) {
			cout << inputStack.top().word;
			inputStack.pop();
		}
		cout << endl;
		return false;
	}
}

inputType PredictMethod::trans2VT(wordtype w) {
	if (w.type == 10)
		return inputType("i", w.word);
	else if (w.type == 20)
		return inputType("d", w.word);
	else if (w.type == 1)
		return inputType("m", w.word);
	else if (w.type == 6)
		return inputType("f", w.word);
	else if (w.type == 7)
		return inputType("e", w.word);
	else if (w.type == 8)
		return inputType("o", w.word);
	else if (w.type == 9)
		return inputType("w", w.word);
	else if (w.type == 35)
		return inputType("s", w.word);
	else if (w.type == 36)
		return inputType("q", w.word);
	else if (w.type == 37)
		return inputType("n", w.word);
	else if (w.type == 33)
		return inputType("l", w.word);
	else return inputType(w.word, w.word);
}

void symantics(string aSt, string iSt) {

}