/*
The C++11 string conversion functions (stoi) 
don't work in MinGW (which is what comes with Code::Blocks by default).

Title : 개인 도서 대출 관리 프로그램.
Author : 정현섭 (21600665)

*/


#include <iomanip> //For setw()
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctype.h>

#define READFILE "books.txt"
#define SIZE 1000

using namespace std;


typedef struct book {
	string title;
	string author;
	int year;
	string edition;
	string borrower;
	int days;
}book;



void load();
void save(string filename);
void print();
void insert(string parser);
void lend(string parser);
void returned(string parser);
void passday();

string trim(string str);
int isNumber(string str);
void print_nth_list(int n);
void print_category();
void print_manual();
void print_returned(int n);
void print_lend();
void announce();	//반납일이거나 반납일이 지나간 사람들 announce.

namespace mine {		//
	book books[SIZE];
	int size = 0;
}



int main(void) {
	string command;
	string parser = "";
	load();					//initial screen
	print();
	while (1) {				//event loop
		print_manual();
		cout << ">> ";
		cin >> command;
		getline(std::cin, parser, '\n');
		if (_stricmp(command.c_str(), "insert") == 0) {
			insert(parser);
		}
		else if (_stricmp(command.c_str(), "save") == 0) {
			save(parser);
		}
		else if (_stricmp(command.c_str(), "exit") == 0) {
			break;
		}
		else if (_stricmp(command.c_str(), "lend") == 0)
			lend(parser);
		else if (_stricmp(command.c_str(), "print") == 0)
			print();
		else if (_stricmp(command.c_str(), "returned") == 0)
			returned(parser);
		else if (_stricmp(command.c_str(), "passday") == 0)
			passday();
		else
			cout << "  Error : There is no command " << command << " !  Please check the command." << endl;
	}

	return 0;
}

void load() {
	ifstream rdata(READFILE);

	string parser;
	string str;

	for (int i = 0; getline(rdata, parser, '\n'); i++) {
		char ch;
		int div = 0;
		for (unsigned int j = 0; j < parser.size() + 1; j++) {
			if (j < parser.size())
				ch = parser[j];
			if (ch != ';' && j < parser.size()) {
				str += ch;
			}
			else {
				switch (div) {
				case 0:
					mine::books[i].title = trim(str);
					break;
				case 1:
					mine::books[i].author = trim(str);
					break;
				case 2:
					mine::books[i].year = std::stoi(trim(str));
					break;
				case 3:
					mine::books[i].edition = trim(str);
					break;
				case 4:
					mine::books[i].borrower = trim(str);
					break;
				case 5:
					mine::books[i].days = std::stoi(trim(str));
					break;
				}
				str = "";
				div++;
			}
		}
		mine::size++;
	}

}

void save(string filename) {
	filename = trim(filename);
	for (int i = 0; i < (signed)filename.size(); i++) {
		if (filename[i] == ' ') {
			cout << "  Filename should not contain space." << endl;
			return;
		}
	}
	if (filename.size() == 0){
		filename = READFILE;
	}
	
	ofstream wdata(filename);
	
	for (int i = 0; i < mine::size; i++) {
		wdata << mine::books[i].title << "; " << mine::books[i].author << "; " << mine::books[i].year << "; " << mine::books[i].edition << "; " << mine::books[i].borrower << "; " << mine::books[i].days << "; " << endl;
	}

	cout << "\nSaved in "<<filename << "!!" << endl;

}

void insert(string parser){
	string str;
	char ch;
	int div = 0;
	int i = 0;
	string title, author, year,edition;
	while(i< (signed)parser.size()+1){
		
		ch = parser[i++];
		
		if (ch != ';' && ch != '\0') {
			str += ch;
		}
		else {
			switch (div) {
			case 0:
				title = trim(str);
				break;
			case 1:
				author = trim(str);
				break;
			case 2:
				year = trim(str);
				break;
			case 3:
				edition = trim(str);
				break;
			}
				str = "";
				div++;
		}
	}
	if (div != 4 || title.size() < 1 || author.size() < 1 || year.size() < 1 || edition.size() < 1 || !isNumber(year)) {
		cout << endl<<"  Error: wrong INSERT Commend. Please check the format!" << endl;
		return;
	}

	mine::books[mine::size].title = title;
	mine::books[mine::size].author = author;
	mine::books[mine::size].year = std::stoi(year);
	mine::books[mine::size].edition = edition;
	mine::books[mine::size].borrower = "None";
	mine::books[mine::size].days = 0;
	mine::size++;

	cout << endl<<"  Inserted The " << title << " successfully!" << endl;

	print();

}

void lend(string parser) {
	string str;
	char ch;
	int div = 0;
	int i = 0;
	
	string title, borrower, days;
	
	while (i< (signed)parser.size() + 1) {
		ch = parser[i++];
		if (ch != ';' && ch != '\0') {
			str += ch;
		}
		else {
			switch (div) {
			case 0:
				title = trim(str);
				break;
			case 1:
				borrower = trim(str);
				break;
			case 2:
				days = trim(str);
				break;
			}
			str = "";
			div++;
		}
	}
	
	//error check
	if (div != 3 || title.size() < 1 || borrower.size() < 1 || days.size() < 1 || !isNumber(days)) {
		cout <<endl<< "  Error: Wrong LEND Commend. Check the format!" << endl;
		return;
	}
	//if borrower name is "none"
	if (_stricmp(borrower.c_str(), "none") == 0) {
		cout << endl << "  Error: Borrower name must not be 'none'. Please use a different name." << endl;
		return;
	}
	int n = 0;

	if (stoi(days) <= 0) {
		cout << "  Lend for more days" << endl;
	}
	for (int n = 0; n < mine::size; n++) {		//book title search
		if (_stricmp(title.c_str(), mine::books[n].title.c_str()) == 0) {	//if book is exist
			if (_stricmp("none", mine::books[n].borrower.c_str()) != 0) {	//and if someone already borrow the book
				cout <<"  Someone already borrowed the book." << endl;
				return;
			}
			else if(stoi(days) > 0){		//book을 빌릴 수 있을때. days가 0초과 인지 검사.
				mine::books[n].borrower = borrower;
				mine::books[n].days = std::stoi(days);	
				print_lend();
				return;
			}
			else {	// book을 찾았으나 days가 0 이하면
				return;
			}
		}
	}

	//끝까지 book을 못찾으면.
	cout <<"  NO SUCH BOOK!" << endl;
	return;
}

void returned(string parser) {
	parser = trim(parser);

	for (int i = 0; i < mine::size; i++) {
		if (_stricmp(mine::books[i].title.c_str(), parser.c_str()) == 0) { // if search success
			if (_stricmp(mine::books[i].borrower.c_str(), "none") == 0) {
				cout << endl << "  ATTENTION No one borrowed that book!!" << endl;
				return;
			}
			mine::books[i].borrower = "None";
			mine::books[i].days = 0;
			print_returned(i);
			return;
		}
	}

	cout << endl << "  ATTENTION No Such Book!" << endl;

}


string trim(string str) {
		while (1) {
			if (str.size() > 0 && isspace(str[0])) {
				str = str.substr(1, str.size());
			}
			else
				break;
		}
		while (1) {
			if (str.size() > 0&& isspace(str[str.size() - 1]))
				str = str.substr(0, str.size() - 1);
			else
				break;
		}
	return str;
}

int isNumber(string str) {
	if (str.size() < 1)
		return 0;
	if (str[0] == '-')
		str = str.substr(1, str.size());
	if (str.size() < 1)
		return 0;
	for (unsigned int i = 0; i < str.size(); i++)
		if (!('0' <= str[i] && str[i] <= '9'))
			return 0;
	return 1;

}
void print_nth_list(int n) {

	cout.setf(ios::left);

	cout << setw(24) << mine::books[n].title
		<< setw(18) << mine::books[n].author
		<< setw(18) << mine::books[n].year
		<< setw(9) << mine::books[n].edition
		<< setw(11) << mine::books[n].borrower
		<< setw(
			12) << mine::books[n].days
		<< endl;
}

void print_category() {
	cout.setf(ios::left);
	
	cout << endl;
	cout << setw(24) << "Title"
		<< setw(18) << "Author"
		<< setw(18) << "Published Year"
		<< setw(9) << "Edition"
		<< setw(11) << "Borrower"
		<< setw(12) << "Days Borrowed"
		<< endl;
}

void print() {
	cout << endl;
	for (int i = 0; i < 40; i++)
		cout << "=";
	cout << " Book Catalog ";
	for (int i = 0; i < 40; i++)
		cout << "=";
	cout << endl;

	print_category();

	for (int i = 0; i < mine::size; i++) {
		print_nth_list(i);
	}
	for (int i = 0; i < 45; i++)
		cout << "-";
	cout << " END ";
	for (int i = 0; i < 45; i++)
		cout << "-";
	cout << endl;
	announce();
	
	
}

void print_manual() {
	cout << endl;
	for (int i = 0; i < 15; i++)
		cout << "=";
	cout << " Available Commands & Format ";
	for (int i = 0; i < 15; i++)
		cout << "=";
	cout << endl << endl;

	cout << "1. INSERT BookTitle; Author; PubYear; Edition" << endl
		<< "2. LEND BookTitle; Person Borrowing; How many days" << endl
		<< "3. SAVE " << "new_filename.txt" << endl
		<< "4. RETURNED BookTitle" << endl
		<< "5. PASSDAY" << endl
		<< "6. PRINT" << endl
		<< "7. EXIT" << endl;
	cout << endl;
	for (int i = 0; i < 60 ; i++)
		cout << "=";
	cout << endl;
}

void print_returned(int n) {
	cout << endl;

	for (int i = 0; i < 30; i++)
		cout << "=";
	cout << " Books Returned ";
	for (int i = 0; i < 30; i++)
		cout << "=";
	cout << endl;

	print_category();
	print_nth_list(n);

	for (int i = 0; i < 45; i++)
		cout << "-";
	cout << " END ";
	for (int i = 0; i < 45; i++)
		cout << "-";
	cout << endl;
	announce();

}

void print_lend() {
	cout << endl;

	for (int i = 0; i < 35; i++)
		cout << "=";
	cout << " Books Successfully Lent ";
	for (int  i = 0; i < 35; i++)
		cout << "=";
	cout << endl;

	print_category();

	for (int i = 0; i < mine::size; i++) {		//빌려간 것 리스트 출력.
		if (_stricmp("none", mine::books[i].borrower.c_str()) != 0) {
			print_nth_list(i);
		}
	}
	for (int i = 0; i < 45; i++)
		cout << "-";
	cout << " END ";
	for (int i = 0; i < 45; i++)
		cout << "-";
	cout << endl;
	announce();
}

void passday() {

	for (int i = 0; i < mine::size; i++) {
		if(_stricmp(mine::books[i].borrower.c_str(),"None") != 0)	//if borrower is not none
			mine::books[i].days--;
	}

	cout << endl;
	for (int i = 0; i < 36; i++)
		cout << "=";
	cout << " Books Currently Lent ";
	for (int i = 0; i < 36; i++)
		cout << "=";
	cout << endl;

	print_category();

	for (int i = 0; i < mine::size; i++) {		//빌려간 것 리스트 출력.
		if (_stricmp("none", mine::books[i].borrower.c_str()) != 0) {
			print_nth_list(i);
		}
	}
	for (int i = 0; i < 45; i++)
		cout << "-";
	cout << " END ";
	for (int i = 0; i < 45; i++)
		cout << "-";
	cout << endl;
	announce();
}

void announce() {		//print announcement to who have to hurry up to bring the book
	for (int i = 0; i < mine::size; i++) {
		if (_stricmp(mine::books[i].borrower.c_str(), "none") != 0) {
			if (mine::books[i].days == 0) {
				cout << mine::books[i].title << " should be returned today by " << mine::books[i].borrower << endl;
			}
			else if (mine::books[i].days < 0) {
				cout << mine::books[i].title << " SHOULD HAVE BEEN RETURNED ALREADY by " <<mine::books[i].borrower <<endl;
			}
		}
	}
}