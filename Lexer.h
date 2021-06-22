#include<iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

ifstream fin("main.txt");



typedef enum {
	OpenParen, CloseParen, Colon, Semicolon, Qmark, Variable,
	OpensqrBracket, ClosesqrBracket, OpenBrace, CloseBrace, Address,
	Number, Plus, Increment, Minus, Decrement, Lessthan, Div, Not,
	Mul, Greaterthan, Modulus, Struct, Class, Protected, String,
	If, Else, Do, Int, Main, Break, Switch, Auto, Cin, Char, CommaSep,
	Void, For, Return, Default, Private, Case, Cout, Stringlitral,
	Float, Endl, While, Double, Goto, True, False, Public, Const,
	Static, Divequal, Mulequal, EqualEqual, Shiftright, Dot, Equal,
	Shiftleft, Notequal, And, Or, Plusequal, Minusequal, Modequal,
	Greaterthanequal, Lessthanequal, undefined
}token;


/*This array of string is used as a mapper because enum token returns an integer
so for output to show the token in string this will help us map the enum token and it
will return the corresponding string literal of that token*/
string mapEnum[] = { "OpenParen", "CloseParen", "Colon", "Semicolon", "Qmark", "Variable",
"OpensqrBracket", "ClosesqrBracket", "OpenBrace", "CloseBrace", "Address",
"Number", "Plus", "Increment", "Minus", "Decrement", "Lessthan", "Div", "Not",
"Mul", "Greaterthan", "Modulus", "Struct(keyword)", "Class(keyword)",
"Protected(keyword)", "String(keyword)", "If(keyword)", "Else(keyword)", "Do(keyword)",
"Int(keyword)", "Main(keyword)", "Break(keyword)", "Switch(keyword)", "Auto(keyword)",
"Cin(keyword)", "Char(keyword)", "CommaSep", "Void(keyword)", "For(keyword)",
"Return(keyword)", "Default(keyword)", "Private(keyword)", "Case(keyword)", "Cout(keyword)",
"Stringlitral", "Float(keyword)", "Endl(keyword)", "While(keyword)", "Double(keyword)",
"Goto(keyword)", "True(keyword)", "False(keyword)", "Public(keyword)", "Const(keyword)",
"Static(keyword)", "Divequal", "Mulequal", "EqualEqual", "Shiftright", "Dot", "Equal",
"Shiftleft", "Notequal", "And", "Or", "Plusequal", "Minusequal", "Modequal",
"Greaterthanequal", "Lessthanequal", "undefined" };

struct TokenRecord
{
	int index;
	string tkn_name;
	token tok;
};
vector<TokenRecord> totalFileTkns; // This Vector container store all the other operators,keywords,seperators in whole file;
vector<TokenRecord> STtkn;    //T his vector container add all the Variables/Identifiers to Symbol table

static int lexId = 1;//for filetable
static int idCount = 1; // for symbol table
TokenRecord t1;
// This Function is creating an object of Symbol is Symbol Table

void createSymboltable(string name, token t)
{
	t1.index = idCount;
	t1.tkn_name = name;
	t1.tok = t;
	STtkn.push_back(t1);
	idCount++;
}

// This functions is searching if given symbol is already in container or not

void searchVarTkn(string t)
{
	for (auto i = STtkn.begin(); i != STtkn.end(); i++)
	{
		if ((*i).tkn_name == t)
		{
			return;
		}
	}
	createSymboltable(t, token(Variable));
}

// This func is to insert all possible tokens

void insertLexeme(string s, token t)
{
	t1.index = lexId;
	t1.tkn_name = s;
	t1.tok = t;
	totalFileTkns.push_back(t1);
	lexId++;
}

// This func is to add single character tokens

void insertSingleLiterals(char c, token t)
{
	t1.index = lexId;
	t1.tkn_name = c;
	t1.tok = t;
	totalFileTkns.push_back(t1);
	lexId++;
}
// This Function is iterating over a file and assigning tokens to symbols
TokenRecord tempObj;
char ch;
token getToken()
{

	if (!fin.is_open())
	{
		cout << "Error while opening the file\n";
		system("pause");
		exit(0);
	}
	while (!fin.eof())
	{
		ch = ' ';
		//skipping comments of all types
		fin.get(ch);
		if (ch == ' ' || ch == '\n' || ch == '\t')
			continue;
		if (ch == '/')
		{
			ch = fin.get();

			if (ch == '*')
			{
				ch = ' ';

				while (ch != '*')
				{

					ch = fin.get();

					if (ch == '*')
					{

						ch = fin.get();

						if (ch == '/')
						{
							ch = ' ';
							break;
						}
						else
						{
							fin.seekg(-1, ios::cur);
							ch = ' ';
						}
					}
				}

			}
			else if (ch == '/')
			{
				while (ch != '\n')
				{
					fin.get(ch);
				}
			}
			else
				fin.seekg(-1, ios::cur);

		}
		if (ch == '"')
		{
			string str = "";
			do {
				str += ch;
				fin.get(ch);
			} while (ch != '"');
			str += '"';
			insertLexeme(str, token(Stringlitral));
			return token(Stringlitral);
		}
		if (isdigit(ch))
		{
			string str = "";
			do
			{
				str = str + ch;
				fin.get(ch);
			} while (isdigit(ch));
			insertLexeme(str, token(Number));
			fin.seekg(-1, ios::cur);
			return token(Number);
		}
		if (isalpha(ch))
		{
			string str = "";
			do
			{
				str = str + ch;
				fin.get(ch);
			} while (isalpha(ch) || isdigit(ch));
			fin.seekg(-1, ios::cur);
			// Start Checking for Keywords 
			if (str == "if")
			{
				insertLexeme(str, token(If));
				return token(If);
			}
			else if (str == "else")
			{
				insertLexeme(str, token(Else));
				return token(Else);
			}
			else if (str == "for"){
				insertLexeme(str, token(For));
				return token(For);
			}
			else if (str == "while"){
				insertLexeme(str, token(While));
				return token(While);
			}
			else if (str == "do"){
				insertLexeme(str, token(Do));
				return token(Do);
			}
			else if (str == "struct"){
				insertLexeme(str, token(Struct));
				return token(Struct);
			}
			else if (str == "class"){
				insertLexeme(str, token(Class));
				return token(Class);
			}
			else if (str == "protected"){
				insertLexeme(str, token(Protected));
				return token(Protected);
			}
			else if (str == "string"){
				insertLexeme(str, token(String));
				return token(String);
			}
			else if (str == "int"){
				insertLexeme(str, token(Int));
				return token(Int);
			}
			else if (str == "main"){
				insertLexeme(str, token(Main));
				return token(Main);
			}
			else if (str == "break"){
				insertLexeme(str, token(Break));
				return token(Break);
			}
			else if (str == "switch"){
				insertLexeme(str, token(Switch));
				return token(Switch);
			}
			else if (str == "auto"){
				insertLexeme(str, token(Auto));
				return token(Auto);
			}
			else if (str == "cin"){
				insertLexeme(str, token(Cin));
				return token(Cin);
			}
			else if (str == "char"){
				insertLexeme(str, token(Char));
				return token(Char);
			}
			else if (str == "void"){
				insertLexeme(str, token(Void));
				return token(Void);
			}
			else if (str == "return"){
				insertLexeme(str, token(Return));
				return token(Return);
			}
			else if (str == "default"){
				insertLexeme(str, token(Default));
				return token(Default);
			}
			else if (str == "private"){
				insertLexeme(str, token(Private));
				return token(Private);
			}
			else if (str == "case"){
				insertLexeme(str, token(Case));
				return token(Case);
			}
			else if (str == "cout"){
				insertLexeme(str, token(Cout));
				return  token(Cout);
			}
			else if (str == "float"){
				insertLexeme(str, token(Float));
				return token(Float);
			}
			else if (str == "endl"){
				insertLexeme(str, token(Endl));
				return token(Endl);
			}
			else if (str == "double"){
				insertLexeme(str, token(Double));
				return token(Double);
			}
			else if (str == "goto"){
				insertLexeme(str, token(Goto));
				return token(Goto);
			}
			else if (str == "true"){
				insertLexeme(str, token(True));
				return token(True);
			}
			else if (str == "false"){
				insertLexeme(str, token(False));
				return token(False);
			}
			else if (str == "public"){
				insertLexeme(str, token(Public));
				return token(Public);
			}
			else if (str == "static"){
				insertLexeme(str, token(Static));
				return token(Static);
			}
			else if (str == "const"){
				insertLexeme(str, token(Const));
				return token(Const);
			}
			// End Checking for Keywords 
			else 	//from here the enteries in Symbol table will be made
			{
				bool flag = 0;
				searchVarTkn(str);
				return token(Variable);
			}
		}
		switch (ch)
		{
		case '(':
			insertSingleLiterals(ch, token(OpenParen));
			return token(OpenParen);
			break;
		case ')':
			insertSingleLiterals(ch, token(CloseParen));
			return  token(CloseParen);
			break;
		case ';':
			insertSingleLiterals(ch, token(Semicolon));
			return token(Semicolon);
			break;
		case ':':
			insertSingleLiterals(ch, token(Colon));
			return token(Colon);
			break;
		case '<':
		{
					string s;
					s += ch;
					fin.get(ch);
					if (ch == '<')
					{
						s += ch;
						insertLexeme(s, token(Shiftleft));
						return token(Shiftleft);
					}
					else if (ch == '=')
					{
						s += ch;
						insertLexeme(s, token(Lessthanequal));
						return token(Lessthanequal);
					}
					else
					{
						ch = ' ';
						fin.seekg(-2, ios::cur);
						fin.get(ch);
						insertSingleLiterals(ch, token(Lessthan));
						return token(Lessthan);
					}
		}
			break;


		case '>':
		{
					string s;
					s += ch;
					fin.get(ch);
					if (ch == '>')
					{
						s += ch;
						insertLexeme(s, token(Shiftright));
						return token(Shiftright);
					}
					else if (ch == '=')
					{
						s += ch;
						insertLexeme(s, token(Greaterthanequal));
						return token(Greaterthanequal);
					}
					else
					{
						ch = ' ';
						fin.seekg(-2, ios::cur);
						fin.get(ch);
						insertSingleLiterals(ch, token(Greaterthan));
						return token(Greaterthan);
					}
		}
			break;
		case '?':
			cout << "?";
			insertSingleLiterals(ch, token(Qmark));
			return token(Qmark);
			break;
		case '%':
		{
					string s;
					s += ch;
					fin.get(ch);
					if (ch == '=')
					{
						s += ch;
						insertLexeme(s, token(Modequal));
						return token(Modequal);
					}
					else
					{
						ch = ' ';
						fin.seekg(-2, ios::cur);
						fin.get(ch);
						insertSingleLiterals(ch, token(Modulus));
						return token(Modulus);
					}
		}
			break;
		case '&':
		{
					string s;
					s += ch;
					fin.get(ch);
					if (ch == '&')
					{
						s += ch;
						insertLexeme(s, token(And));
						return token(And);
					}
					else
					{
						ch = ' ';
						fin.seekg(-2, ios::cur);
						fin.get(ch);
						insertSingleLiterals(ch, token(Address));
						return token(Address);
					}
		}
			break;
		case '{':
			insertSingleLiterals(ch, token(OpenBrace));
			return token(OpenBrace);
			break;
		case '}':
			insertSingleLiterals(ch, token(CloseBrace));
			return token(CloseBrace);
			break;
		case '[':
			insertSingleLiterals(ch, token(OpensqrBracket));
			return token(OpensqrBracket);
			break;
		case ']':
			insertSingleLiterals(ch, token(ClosesqrBracket));
			return token(ClosesqrBracket);
			break;
		case '+':
		{
					string s;
					s += ch;
					fin.get(ch);
					if (ch == '=')
					{
						s += ch;
						insertLexeme(s, token(Plusequal));
						return token(Plusequal);
					}
					else if (ch == '+')
					{
						s += ch;
						insertLexeme(s, token(Increment));
						return token(Increment);
					}
					else
					{
						ch = ' ';
						fin.seekg(-2, ios::cur);
						fin.get(ch);
						insertSingleLiterals(ch, token(Plus));
						return token(Plus);
					}

		}
			break;
		case '-':
		{
					string s;
					s += ch;
					fin.get(ch);
					if (ch == '=')
					{
						s += ch;
						insertLexeme(s, token(Minusequal));
						return token(Minusequal);
					}
					else if (ch == '-')
					{
						s += ch;
						insertLexeme(s, token(Decrement));
						return token(Decrement);
					}
					else
					{
						ch = ' ';
						fin.seekg(-2, ios::cur);
						fin.get(ch);
						insertSingleLiterals(ch, token(Minus));
						return token(Minus);
					}
		}
			break;
		case '*':
		{
					string s;
					s += ch;
					fin.get(ch);
					if (ch == '=')
					{
						s += ch;
						insertLexeme(s, token(Mulequal));
						return token(Mulequal);
					}
					else
					{
						ch = ' ';
						fin.seekg(-2, ios::cur);
						fin.get(ch);
						insertSingleLiterals(ch, token(Mul));
						return token(Mul);
					}
		}
			break;
		case '.':
			insertSingleLiterals(ch, token(Dot));
			return  token(Dot);
			break;
		case '/':
		{
					string s;
					s += ch;
					fin.get(ch);
					if (ch == '=')
					{
						s += ch;
						insertLexeme(s, token(Divequal));
						return token(Divequal);
					}
					else
					{
						ch = ' ';
						fin.seekg(-2, ios::cur);
						fin.get(ch);
						insertSingleLiterals(ch, token(Div));
						return token(Div);
					}
		}
			break;
		case '!':
		{
					string s;
					s += ch;
					fin.get(ch);
					if (ch == '=')
					{
						s += ch;
						insertLexeme(s, token(Notequal));
						return token(Notequal);
					}
					else
					{
						ch = ' ';
						fin.seekg(-2, ios::cur);
						fin.get(ch);
						insertSingleLiterals(ch, token(Not));
						return token(Not);
					}
		}
			break;
		case '=':
		{
					string s;
					s += ch;
					fin.get(ch);
					if (ch == '=')
					{
						s += ch;
						insertLexeme(s, token(EqualEqual));
						return token(EqualEqual);
					}
					else
					{
						ch = ' ';
						fin.seekg(-2, ios::cur);
						fin.get(ch);
						insertSingleLiterals(ch, token(Equal));
						return token(Equal);
					}
		}
			break;
		case '|':
		{
					string s;
					s += ch;
					fin.get(ch);
					if (ch == '|')
					{
						s += ch;
						insertLexeme(s, token(Or));
						return token(Or);
					}
					else
					{
						fin.seekg(-2, ios::cur);
						ch = ' ';
					}
		}
			break;
		case ',':
			insertSingleLiterals(ch, token(CommaSep));
			return token(CommaSep);
			break;
		default:
			break;
		}
	}
	return token(undefined);
}


