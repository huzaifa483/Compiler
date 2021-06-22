#include<iostream>
#include "Lexer.h"
#include <fstream>
#include <string>
using namespace std;
bool factor();
bool expression();
bool term();
bool expr();
bool statement();
bool statement_list();
bool exp();
bool cmpexp();
bool shiftexp();
bool eqexp();
bool andexp();
bool Orexp();
static bool flag = 0;
token tokn;
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
			tokn = gettoken();
			if (tokn == If || tokn == Do ||tokn==While || tokn == Variable)
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

	tokn = gettoken();
	if (tokn == Variable)
	{
		tokn = gettoken();
		if (tokn == Equal || tokn == Notequal || tokn == Plusequal || tokn == Minusequal||tokn==Plus||tokn == OpenParen)
		{
			if (Orexp())
			{
				tokn = gettoken();
				if (tokn == Semicolon)
					return true;
			}
		}
	}
	else if (tokn == If)
	{
	Label:
		tokn = gettoken();
		if (tokn == OpenParen)
		{
			if (Orexp())
			{
				tokn = gettoken();
				if (tokn == CloseParen)
				{
					tokn = gettoken();
					if (tokn == OpenBrace)
					{
						if (statement_list())
						{
							tokn = gettoken();
							if (tokn == CloseBrace)
							{
								tokn = gettoken();
								if (tokn == Else)
								{
									tokn = gettoken();
									if (tokn == OpenBrace)
									{
										if (statement_list())
										{
											tokn = gettoken();
											if (tokn == CloseBrace)
												return true;
										}
									}
									else if (tokn == If)
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
	else if (tokn == Do)
	{
		tokn = gettoken();
		if (tokn == OpenBrace)
		{
			if (statement_list())
			{
				tokn = gettoken();
				if (tokn == CloseBrace)
				{
					tokn = gettoken();
					if (tokn == While)
					{
						tokn = gettoken();
						if (tokn == OpenParen)
						{
							if (Orexp())
							{
								tokn = gettoken();
								if (tokn == CloseParen)
								{
									tokn = gettoken();
									if (tokn == Semicolon)
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
	else if (tokn == While)
	{
			tokn = gettoken();
			if (tokn == OpenParen)
			{
				if (Orexp())
				{
					tokn = gettoken();
					if (tokn == CloseParen)
					{
						tokn = gettoken();
						if (tokn == OpenBrace)
						{
							if (statement_list())
							{
								tokn = gettoken();
								if (tokn == CloseBrace)
								{
									return true;
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
		tokn = gettoken();
	} while (tokn == Plus || tokn == Minus);
	ungettoken();
	return true;
}
bool term()
{
	do
	{
		if (!factor())
			return false;
		tokn = gettoken();
	} while (tokn == Mul || tokn == Div || tokn == Modulus);
	ungettoken();
	return true;
}
bool factor()
{
	tokn = gettoken();
	if (tokn == Variable)
		return true;
	else if (tokn == Number)
		return true;
	else if (tokn == OpenParen)
	{
		if (expression())
		{
			tokn = gettoken();
			if (tokn == CloseParen)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	return false;
}
bool Orexp()
{
	do
	{
		if (!andexp())
			return false;
		tokn = gettoken();
	} while (tokn == Or);
	ungettoken();
	return true;
}
bool andexp()
{
	do
	{
		if (!eqexp())
			return false;
		tokn = gettoken();
	} while (tokn == And);
	ungettoken();
	return true;
}
bool eqexp()
{
	do
	{
		if (!cmpexp())
			return false;
		tokn = gettoken();
	} while (tokn == EqualEqual || tokn == Notequal);
	ungettoken();
	return true;
}
bool cmpexp()
{
	do
	{
		if (!shiftexp())
			return false;
		tokn = gettoken();
	} while (tokn == Lessthan || tokn == Lessthanequal || tokn == Greaterthan || tokn == Greaterthanequal);
	ungettoken();
	return true;
}
bool shiftexp()
{
	do
	{
		if (!expression())
			return false;
		tokn = gettoken();
	} while (tokn == Shiftright || tokn == Shiftleft);
	ungettoken();
	return true;
}

int main()
{
	bool res = statement_list();
	if (res)
		cout << "true\n";
	else
		cout << "false\n";
	fin.close();
	system("pause");
	return 0;
}
bool expr()
{
	if (Orexp())
	{
		tokn = gettoken();
		if (tokn == Greaterthan || tokn == Lessthan || tokn== Greaterthanequal || tokn == Lessthanequal || tokn == EqualEqual || tokn ==Notequal)
		{
			if (Orexp())
				return true;
			else
				return false;
		}
		ungettoken();
		return true;
	}
	return false;
}