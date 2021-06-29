#include<iostream>
#include "Lexer.h"
#include<iomanip>
#include <fstream>
#include <string>
using namespace std;
bool factor();
bool expression();
bool term();
bool statement();
bool statement_list();
bool Comparison_expr();
bool Shift_expr();
bool Equality_expr();
bool Logical_AND_expr();
bool Logical_OR_expr();
static bool flag = 0;
token tkn;
static token curr_Tkn = undefined;
void ungettoken()
{
	flag = 1;
}
token gettoken()
{
	if (flag == 0){
		curr_Tkn = getToken();
		return curr_Tkn;
	}
	else
	{
		flag = 0;
		return curr_Tkn;
	}
}
bool statement_list()
{
	if (statement())
	{
		do
		{
			tkn = gettoken();
			
			if (tkn == If || tkn == Do ||tkn==While || tkn == Variable||tkn==For)
			{
				ungettoken();
				if (!statement())
				{
					return false;
				}
				
			}
			else
			{
				ungettoken();
				break;
			}
		} while (1);
		return true;
	}
	return false;
}
bool statement()
{

	tkn = gettoken();
	if (tkn == Variable)
	{
		tkn = gettoken();
		if (tkn == Equal || tkn == Notequal || tkn == Plusequal || tkn == Minusequal || tkn == Plus || tkn == Lessthan || tkn == Lessthanequal || tkn == Greaterthan || tkn == Greaterthanequal)
		{
			if (Logical_OR_expr())
			{
				tkn = gettoken();
				if (tkn == Semicolon)
					return true;
			}
		}
	}
	else if (tkn == If)
	{
	Label:
		tkn = gettoken();
		if (tkn == OpenParen)
		{
			if (Logical_OR_expr())
			{
				tkn = gettoken();
				if (tkn == CloseParen)
				{
					tkn = gettoken();
					if (tkn == OpenBrace)
					{
						if (statement_list())
						{
							tkn = gettoken();
							if (tkn == CloseBrace)
							{
								tkn = gettoken();
								if (tkn == Else)
								{
									tkn = gettoken();
									if (tkn == OpenBrace)
									{
										if (statement_list())
										{
											tkn = gettoken();
											if (tkn == CloseBrace)
												return true;
										}
									}
									else if (tkn == If)
									{
										goto Label;
									}
								}
								else
									return true;
							}
						}
					}
				}
			}
		}
	}
	else if (tkn == Do)
	{
		tkn = gettoken();
		if (tkn == OpenBrace)
		{
			if (statement_list())
			{
				tkn = gettoken();
				if (tkn == CloseBrace)
				{
					tkn = gettoken();
					if (tkn == While)
					{
						tkn = gettoken();
						if (tkn == OpenParen)
						{
							if (Logical_OR_expr())
							{
								tkn = gettoken();
								if (tkn == CloseParen)
								{
									tkn = gettoken();
									if (tkn == Semicolon)
									{
										return true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else if (tkn == While)
	{
			tkn = gettoken();
			if (tkn == OpenParen)
			{
				if (Logical_OR_expr())
				{
					tkn = gettoken();
					if (tkn == CloseParen)
					{
						tkn = gettoken();
						if (tkn == OpenBrace)
						{
							if (statement_list())
							{
								tkn = gettoken();
								if (tkn == CloseBrace)
								{
									return true;
								}
							}
						}
					}
				}
			}
	}
	else if (tkn == For)
	{
		tkn = gettoken();
		if (tkn == OpenParen)
		{
			tkn = gettoken();
			if (tkn == Int || tkn == Double || tkn == Float || tkn == Char || tkn == Auto||tkn==Variable)
			{
				if (tkn == Variable)
					ungettoken();
				if (statement())
				{
					if (statement())
					{
						tkn = gettoken();
						if (tkn == Variable)
						{
							tkn = gettoken();
							if (tkn == Increment)
							{
								tkn = gettoken();
								if (tkn == CloseParen)
								{
									tkn = gettoken();
									if (tkn == OpenBrace)
									{
										if (statement_list())
										{
											tkn = gettoken();
											if (tkn == CloseBrace)
											{
												return true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

	}
	return false;
}
bool expression()
{
	do
	{
		if (!term())
			return false;
		tkn = gettoken();
	} while (tkn == Plus || tkn == Minus);
	ungettoken();
	return true;
}
bool term()
{
	do
	{
		if (!factor())
			return false;
		tkn = gettoken();
	} while (tkn == Mul || tkn == Div || tkn == Modulus);
	ungettoken();
	return true;
}
bool factor()
{
	tkn = gettoken();
	if (tkn == Variable)
		return true;
	else if (tkn == Number)
		return true;
	else if (tkn == OpenParen)
	{
		if (expression())
		{
			tkn = gettoken();
			if (tkn == CloseParen)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	return false;
}
bool Logical_OR_expr()
{
	do
	{
		if (!Logical_AND_expr())
			return false;
		tkn = gettoken();
	} while (tkn == Or);
	ungettoken();
	return true;
}
bool Logical_AND_expr()
{
	do
	{
		if (!Equality_expr())
			return false;
		tkn = gettoken();
	} while (tkn == And);
	ungettoken();
	return true;
}
bool Equality_expr()
{
	do
	{
		if (!Comparison_expr())
			return false;
		tkn = gettoken();
	} while (tkn == EqualEqual || tkn == Notequal);
	ungettoken();
	return true;
}
bool Comparison_expr()
{
	do
	{
		if (!Shift_expr())
			return false;
		tkn = gettoken();
	} while (tkn == Lessthan || tkn == Lessthanequal || tkn == Greaterthan || tkn == Greaterthanequal);
	ungettoken();
	return true;
}
bool Shift_expr()
{
	do
	{
		if (!expression())
			return false;
		tkn = gettoken();
	} while (tkn == Shiftright || tkn == Shiftleft);
	ungettoken();
	return true;
}

int main()
{
	cout << "\n\t\tParser Output\n\n";
	bool res = statement_list();
	if (res)
		cout << "Input File is Valid according to the specified Language\n\n";
	else
		cout << "Input Files have syntax erros according to specified Language\n\n";
	cout << "\n\t---Symbol Table---\n\n";
	cout << " Name " << "\t\t\t\t   ID-no." << "\t\t  TokenType\n\n";
	for (auto i = STtkn.begin(); i != STtkn.end(); i++)
	{
		cout << left << setw(35) << (*i).tkn_name;
		cout << "<Id," << (*i).index << setw(20) << ">";
		cout << mapEnum[(*i).tok] << endl;
	}
	cout << "\n\t---All Lexemes token---\n\n";
	cout << " Name " << "\t\t\t\t   ID-no." << "\t\t  TokenType\n\n";
	for (auto i = totalFileTkns.begin(); i != totalFileTkns.end(); i++)
	{

		cout << left << setw(35) << (*i).tkn_name;
		cout << "<Id," << (*i).index << setw(20) << ">";
		cout << mapEnum[(*i).tok] << endl;

	}

	fin.close();
	system("pause");
	return 0;
}