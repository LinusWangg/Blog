#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
using namespace std;
string reserve[15] = { "program","const","var","procedure","begin","end","if","then","else","while","do","call","read","write","odd" };
string line, temp;
int pos = 0;
int row = 0;
int num = 0;
struct syntx {
	string symbol;
	string identity;
	int line;
	int col;
}Syn[10000];
bool const_analyse();
bool factor_analyse();
bool exp_analyse();
bool term_analyse();
bool lexp_analyse();
bool statement_analyse();
bool body_analyse();
bool proc_analyse();
bool vardecl_analyse();
bool condecl_analyse();
bool block_analyse();
bool prog_analyse();

void Getchar(char& ch)
{
	ch = line[pos];
	pos++;
}

void GetFirstChar(char& ch)
{
	ch = line[pos];
	pos++;
	while (ch == ' ' || ch == '\t')
	{
		ch = line[pos];
		pos++;
	}
}

void backchar(char& ch)
{
	ch = NULL;
	pos--;
}

bool isAlpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return true;
	return false;
}

bool isDigit(char c)
{
	if (c >= '0' && c <= '9')
		return true;
	return false;
}

string isreserve(string temp)
{
	for (int i = 0; i < 15; i++)
	{
		if (temp == reserve[i])
			return reserve[i];
	}
	return "identity";
}

void word()
{
	fstream infile, outfile;
	infile.open("1.txt", ios::in);
	outfile.open("2.txt", ios::out);
	if (infile.fail())
	{
		outfile << "error" << endl;
		exit(0);
	}
	char ch = ' ';
	int i = 0;
	while (!infile.eof())
	{
		int flag_for_num = 0;
		row++;
		getline(infile, line, '\n');
		outfile << line << endl;
		while (pos < line.size())
		{
			GetFirstChar(ch);
			if (isAlpha(ch))
			{
				temp.push_back(ch);
				while (isAlpha(ch) || isDigit(ch))
				{
					Getchar(ch);
					temp.push_back(ch);
				}
				backchar(ch);
				temp.pop_back();
				outfile << isreserve(temp) << ":" << temp;
				Syn[num].symbol = isreserve(temp);
				Syn[num].identity = temp;
				Syn[num].line = row;
				Syn[num].col = pos-temp.size();
				num++;
				temp.clear();
				outfile << endl;
			}
			else if (isDigit(ch))
			{
				temp.push_back(ch);
				while (isDigit(ch))
				{
					Getchar(ch);
					temp.push_back(ch);
					if (isAlpha(ch))
					{
						while (ch != '\t' && ch != ' ' && ch != '\n' && ch != '\0')
						{
							Getchar(ch);
							temp.push_back(ch);
						}
						backchar(ch);
						outfile << "Error Not Legal!" << row << "行" << pos << "列" << "---" << temp << endl;
						temp.clear();
						flag_for_num = 1;
					}
				}
				if (flag_for_num == 0)
				{
					backchar(ch);
					temp.pop_back();
					Syn[num].symbol = "num";
					Syn[num].identity = temp;
					Syn[num].line = row;
					Syn[num].col = pos-temp.size();
					num++;
					outfile << "num:" << temp;
					temp.clear();
					outfile << endl;
				}
				flag_for_num = 0;
			}
			else if (ch == '=')
			{
				Syn[num].symbol = "lop";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos-1;
				num++;
				outfile << "Equal: " << ch << endl;
			}
			else if (ch == '+')
			{
				Syn[num].symbol = "aop";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos-1;
				num++;
				outfile << "Plus: " << ch << endl;
			}
			else if (ch == '-')
			{
				Syn[num].symbol = "aop";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos-1;
				num++;
				outfile << "Minus: " << ch << endl;
			}
			else if (ch == '*')
			{
				temp.push_back(ch);
				Getchar(ch);
				if (ch == '*')
				{
					temp.push_back(ch);
					Syn[num].symbol = "mop";
					Syn[num].identity = temp;
					Syn[num].line = row;
					Syn[num].col = pos-2;
					num++;
					outfile << "involution: " << temp << endl;
					temp.clear();
				}
				else
				{
					backchar(ch);
					Syn[num].symbol = "mop";
					Syn[num].identity = temp;
					Syn[num].line = row;
					Syn[num].col = pos-1;
					num++;
					outfile << "Multiple: " << temp << endl;
					temp.clear();
				}
			}
			else if (ch == '/')
			{
				Syn[num].symbol = "mop";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos-1;
				num++;
				outfile << "divide: " << ch;
			}
			else if (ch == '>')
			{
				temp.push_back(ch);
				Getchar(ch);
				if (ch == '=')
				{
					temp.push_back(ch);
					Syn[num].symbol = "lop";
					Syn[num].identity = temp;
					Syn[num].line = row;
					Syn[num].col = pos-2;
					num++;
					outfile << "Notless: " << temp << endl;
					temp.clear();
				}
				else
				{
					backchar(ch);
					Syn[num].symbol = "lop";
					Syn[num].identity = temp;
					Syn[num].line = row;
					Syn[num].col = pos-1;
					num++;
					outfile << "more: " << temp << endl;
					temp.clear();
				}
			}
			else if (ch == '<')
			{
				temp.push_back(ch);
				Getchar(ch);
				if (ch == '=')
				{
					temp.push_back(ch);
					Syn[num].symbol = "lop";
					Syn[num].identity = temp;
					Syn[num].line = row;
					Syn[num].col = pos-2;
					num++;
					outfile << "Notmore: " << temp << endl;
					temp.clear();
				}
				else if (ch == '>')
				{
					temp.push_back(ch);
					Syn[num].symbol = "lop";
					Syn[num].identity = temp;
					Syn[num].line = row;
					Syn[num].col = pos-2;
					num++;
					outfile << "Notequal: " << temp << endl;
					temp.clear();
				}
				else
				{
					backchar(ch);
					Syn[num].symbol = "less";
					Syn[num].identity = temp;
					Syn[num].line = row;
					Syn[num].col = pos-1;
					num++;
					outfile << "less: " << temp << endl;
					temp.clear();
				}
			}
			else if (ch == ':')
			{
				temp.push_back(ch);
				Getchar(ch);
				if (ch == '=')
				{
					temp.push_back(ch);
					Syn[num].symbol = "Assign";
					Syn[num].identity = temp;
					Syn[num].line = row;
					Syn[num].col = pos-2;
					num++;
					outfile << "Assign: " << temp << endl;
					temp.clear();
				}
				else
				{
					temp.push_back(ch);
					while (ch != '\t' && ch != ' ' && ch != '\n' && ch != '\0')
					{
						Getchar(ch);
						temp.push_back(ch);
					}
					backchar(ch);
					outfile << "Error Not Defined!" << row << "行" << pos << "列" << "---" << temp << endl;
					temp.clear();
				}
			}
			else if (ch == ',')
			{
				Syn[num].symbol = "comma";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos-1;
				num++;
				outfile << "comma: " << ch << endl;
			}
			else if (ch == ';')
			{
				Syn[num].symbol = "semicolon";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos-1;
				num++;
				outfile << "semicolon: " << ch << endl;
			}
			else if (ch == '(')
			{
				Syn[num].symbol = "leftbrackets";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos-1;
				num++;
				outfile << "leftbrackets: " << ch << endl;
			}
			else if (ch == ')')
			{
				Syn[num].symbol = "rightbrackets";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos-1;
				num++;
				outfile << "rightbrackets: " << ch << endl;
			}
			else
			{
				temp.push_back(ch);
				while (ch != '\t' && ch != ' ' && ch != '\n' && ch != '\0')
				{
					Getchar(ch);
					temp.push_back(ch);
				}
				backchar(ch);
				outfile << "Error Not Defined!" << row << "行" << pos << "列" << "---" << temp << endl;
				temp.clear();
			}
		}
		outfile << "-------------------------------------" << endl;
		line.clear();
		pos = 0;
	}
	infile.close();
	outfile.close();
}

int analyse_num = 0;

bool const_analyse()
{
	if (Syn[analyse_num].symbol == "identity")
	{
		analyse_num++;
		if (Syn[analyse_num].symbol == "Assign")
		{
			analyse_num++;
			if (Syn[analyse_num].symbol == "num")
			{
				return 1;
			}
			else
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "赋值号后缺少数字！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "const后缺少定义变量！" << endl;
			return 0;
		}
	}
	else
	{
		cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "标识符错误！" << endl;
		return 0;
	}
}

bool factor_analyse()
{
	int now_num = 0;
	if (Syn[analyse_num].symbol == "identity")
	{
		return 1;
	}
	else if (Syn[analyse_num].symbol == "num")
	{
		return 1;
	}
	else if (Syn[analyse_num].symbol == "leftbrackets")
	{
		analyse_num++;
		now_num = analyse_num;
		int temp = exp_analyse();
		if (temp == 1)
		{
			analyse_num++;
			if (Syn[analyse_num].symbol != "rightbrackets")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "缺少右括号！" << endl;
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是EXP！" << endl;
			return 0;
		}
	}
}

bool exp_analyse()
{
	int now_num = 0;
	if (Syn[analyse_num].identity == "+" || Syn[analyse_num].identity == "-")
	{
		analyse_num++;
		now_num = analyse_num;
	}
	int temp = term_analyse();
	if (temp == 0)
	{
		cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是term！" << endl;
		return 0;
	}
	else
	{
		analyse_num++;
		while (Syn[analyse_num].symbol == "aop")
		{
			analyse_num++;
			int temp = term_analyse();
			if (temp == 0)
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是term！" << endl;
				return 0;
			}
			else
			{
				analyse_num++;
			}
		}
		analyse_num--;
		return 1;
	}
}

bool term_analyse()
{
	int now_num = analyse_num;
	int temp = factor_analyse();
	if (temp == 1)
	{
		analyse_num++;
		if (Syn[analyse_num].symbol != "mop")
		{
			analyse_num--;
			return 1;
		}
		while (Syn[analyse_num].symbol == "mop")
		{
			analyse_num++;
			now_num = analyse_num;
			int temp2 = factor_analyse();
			if (temp2 == 1)
			{
				analyse_num++;
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是factor！" << endl;
				return 0;
			}
		}
		analyse_num--;
		return 1;
	}
	else
	{
		cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是factor！" << endl;
		return 0;
	}
}

bool lexp_analyse()
{
	int now_num = analyse_num;
	if (Syn[analyse_num].symbol == "odd")
	{
		analyse_num++;
		now_num = analyse_num;
		int temp = exp_analyse();
		if (temp == 1)
		{
			return 1;
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是EXP！" << endl;
			return 0;
		}
	}
	else
	{
		int temp = exp_analyse();
		if (temp == 1)
		{
			analyse_num++;
			now_num = analyse_num;
			if (Syn[analyse_num].symbol == "lop")
			{
				analyse_num++;
				now_num = analyse_num;
				int temp2 = exp_analyse();
				if (temp2 == 1)
				{
					return 1;
				}
				else
				{
					cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是EXP！" << endl;
					return 0;
				}
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是factor！" << endl;
			}
		}
	}
}

bool statement_analyse()
{
	int now_num = 0;
	if (Syn[analyse_num].symbol == "identity")
	{
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].identity == ":=")
		{
			analyse_num++;
			now_num = analyse_num;
			int temp = exp_analyse();
			if (temp == 1)
			{
				return 1;
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！EXP错误！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少:=！" << endl;
			return 0;
		}
	}
	else if (Syn[analyse_num].symbol == "if")
	{
		analyse_num++;
		now_num = analyse_num;
		int temp = lexp_analyse();
		if (temp == 1)
		{
			analyse_num++;
			now_num = analyse_num;
			if (Syn[analyse_num].symbol == "then")
			{
				analyse_num++;
				now_num = analyse_num;
				int temp2 = statement_analyse();
				if (temp2 == 1)
				{
					analyse_num++;
					if (Syn[analyse_num].symbol == "else")
					{
						analyse_num++;
						now_num = analyse_num;
						int temp3 = statement_analyse();
						if (temp3 == 1)
						{
							return 1;
						}
						else
						{
							cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是Statement！" << endl;
							return 0;
						}
					}
					else
					{
						analyse_num--;
						return 1;
					}
				}
				else
				{
					cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是Statement！" << endl;
					return 0;
				}
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是THEN！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是Statement！" << endl;
			return 0;
		}
	}
	else if (Syn[analyse_num].symbol == "while")
	{
		analyse_num++;
		now_num = analyse_num;
		int temp = lexp_analyse();
		if (temp == 1)
		{
			analyse_num++;
			now_num = analyse_num;
			if (Syn[analyse_num].symbol == "do")
			{
				analyse_num++;
				now_num = analyse_num;
				int temp2 = statement_analyse();
				if (temp2 == 1)
				{
					return 1;
				}
				else
				{
					cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是Statement！" << endl;
					return 0;
				}
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是do！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是LEXP！" << endl;
			return 0;
		}
	}
	else if (Syn[analyse_num].symbol == "call")
	{
		analyse_num++;;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol == "identity")
		{
			analyse_num++;
			now_num = analyse_num;
			if (Syn[analyse_num].identity == "(")
			{
				analyse_num++;
				now_num = analyse_num;
				if (Syn[analyse_num].identity == ")")
				{
					return 1;
				}
				else
				{
					now_num = analyse_num;
					int temp = exp_analyse();
					if (temp == 1)
					{
						analyse_num++;
						while (Syn[analyse_num].identity == ",")
						{
							analyse_num++;
							now_num = analyse_num;
							int temp2 = exp_analyse();
							if (temp2 == 1)
							{
								analyse_num++;
							}
							else
							{
								cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是EXP！" << endl;
								return 0;
							}
						}
						now_num = analyse_num-1;
						if (Syn[analyse_num].identity == ")")
						{
							return 1;
						}
						else
						{
							cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少右括号！" << endl;
							return 0;
						}
					}
					else
					{
						cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是EXP！" << endl;
						return 0;
					}
				}
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少左括号！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是ID！" << endl;
			return 0;
		}
	}
	else if (Syn[analyse_num].symbol == "read")
	{
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].identity == "(")
		{
			analyse_num++;
			now_num = analyse_num;
			if (Syn[analyse_num].symbol == "identity")
			{
				analyse_num++;
				now_num = analyse_num;
				while (Syn[analyse_num].identity == ",")
				{
					analyse_num++;
					now_num = analyse_num;
					if (Syn[analyse_num].symbol == "identity")
					{
						analyse_num++;
						now_num = analyse_num;
					}
					else
					{
						cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是ID！" << endl;
						return 0;
					}
				}
				if (Syn[analyse_num].identity == ")")
				{
					return 1;
				}
				else
				{
					cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是右括号！" << endl;
					return 0;
				}
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是ID！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[now_num-1].line << "行" << Syn[now_num-1].col << "列" << "语法错误！不是左括号！" << endl;
			return 0;
		}
	}
	else if (Syn[analyse_num].symbol == "write")
	{
		analyse_num++;;
		now_num = analyse_num;
		if (Syn[analyse_num].identity == "(")
		{
			analyse_num++;
			now_num = analyse_num;
			int temp = exp_analyse();
			if (temp == 1)
			{
				analyse_num++;
				while (Syn[analyse_num].identity == ",")
				{
					now_num = analyse_num;
					analyse_num++;
					int temp2 = exp_analyse();
					if (temp2 == 1)
					{
						analyse_num++;
					}
					else
					{
						cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是EXP！" << endl;
						return 0;
					}
				}
				now_num = analyse_num;
				if (Syn[analyse_num].identity == ")")
				{
					return 1;
				}
				else
				{
					cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少右括号！" << endl;
					return 0;
				}
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！不是EXP！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少左括号！" << endl;
			return 0;
		}
	}
	else {
		now_num = analyse_num;
		int temp = body_analyse();
		if (temp == 1)
		{
			return 1;
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少body！" << endl;
			return 0;
		}
	}
}

bool body_analyse()
{
	int now_num = analyse_num;
	if (Syn[analyse_num].symbol == "begin")
	{
		analyse_num++;
		now_num = analyse_num;
		int temp = statement_analyse();
		if (temp == 1)
		{
			analyse_num++;
			now_num = analyse_num;
			while (Syn[analyse_num].identity == ";")
			{
				analyse_num++;
				now_num = analyse_num;
				if (Syn[analyse_num].symbol == "end")
				{
					break;
				}
				int temp2 = statement_analyse();
				if (temp2 == 1)
				{
					analyse_num++;
					if ((Syn[analyse_num].identity != ";" && Syn[analyse_num + 1].symbol != "end")&&(Syn[analyse_num].symbol!="end"))
					{
						cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少;！" << endl;
						return 0;
					}
				}
				else
				{
					cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！statement错误！" << endl;
					return 0;
				}
			}
			if (Syn[analyse_num].symbol == "end")
			{
				return 1;
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少end！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！statement错误！" << endl;
			return 0;
		}
	}
	else
	{
		cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少begin！" << endl;
		return 0;
	}
}

bool proc_analyse()
{
	int now_num = analyse_num;
	if (Syn[analyse_num].symbol == "procedure")
	{
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol == "identity")
		{
			analyse_num++;
			now_num = analyse_num;
			if (Syn[analyse_num].identity == "(")
			{
				analyse_num++;
				now_num = analyse_num;
				if (Syn[analyse_num].symbol == "identity")
				{
					analyse_num++;
					now_num = analyse_num;
					while (Syn[analyse_num].identity == ",")
					{
						analyse_num++;
						now_num = analyse_num;
						if (Syn[analyse_num].symbol == "identity")
						{
							analyse_num++;
						}
						else
						{
							cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少id！" << endl;
							return 0;
						}
					}
				}
				if (Syn[analyse_num].identity == ")")
				{
					analyse_num++;
					now_num = analyse_num;
					if (Syn[analyse_num].identity == ";")
					{
						analyse_num++;
						now_num = analyse_num;
						int temp = block_analyse();
						if (temp == 1)
						{
							analyse_num++;
							while (Syn[analyse_num].identity == ";")
							{
								analyse_num++;
								now_num = analyse_num;
								int temp2 = proc_analyse();
								if (temp2 == 1)
								{
									analyse_num++;
									now_num = analyse_num;
								}
								else
								{
									cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少proc！" << endl;
									return 0;
								}
							}
							analyse_num--;
							return 1;
						}
						else
						{
							cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少block！" << endl;
							return 0;
						}
					}
					else
					{
						cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少;！" << endl;
						return 0;
					}
				}
				else
				{
					cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少右括号！" << endl;
					return 0;
				}
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少左括号！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少id！" << endl;
			return 0;
		}
	}
	else
	{
		cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少procedure！" << endl;
		return 0;
	}
}

bool vardecl_analyse()
{
	int now_num = analyse_num;
	if (Syn[analyse_num].symbol == "var")
	{
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol == "identity")
		{
			analyse_num++;
			now_num = analyse_num;
			while (Syn[analyse_num].identity == ",")
			{
				analyse_num++;
				now_num = analyse_num;
				if (Syn[analyse_num].symbol == "identity")
				{
					analyse_num++;
					now_num = analyse_num;
				}
				else
				{
					cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少id！" << endl;
					return 0;
				}
			}
			if (Syn[analyse_num].identity == ";")
			{
				return 1;
			}
			else
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "缺少;！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少id！" << endl;
			return 0;
		}
	}
	else
	{
		cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少var！" << endl;
		return 0;
	}
}

bool condecl_analyse() 
{
	int now_num = analyse_num;
	if (Syn[analyse_num].symbol == "const")
	{
		analyse_num++;
		now_num = analyse_num;
		int temp = const_analyse();
		if (temp == 1)
		{
			analyse_num++;
			now_num = analyse_num;
			while (Syn[analyse_num].identity == ",")
			{
				analyse_num++;
				now_num = analyse_num;
				int temp2 = const_analyse();
				if (temp2 == 1)
				{
					analyse_num++;
					now_num = analyse_num;
				}
				else
				{
					cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！const错误！" << endl;
					return 0;
				}
			}
			if (Syn[analyse_num].identity == ";")
			{
				return 1;
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少;！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！const错误！" << endl;
			return 0;
		}
	}
	else
	{
		cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少const！" << endl;
		return 0;
	}
}

bool block_analyse()
{
	int now_num = analyse_num;
	if (Syn[analyse_num].identity == "const")
	{
		int temp = condecl_analyse();
		if (temp == 1)
		{
			analyse_num++;
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！const错误！" << endl;
			return 0;
		}
	}
	now_num = analyse_num;
	if (Syn[analyse_num].identity == "var")
	{
		int temp = vardecl_analyse();
		if (temp == 1)
		{
			analyse_num++;
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！var错误！" << endl;
			return 0;
		}
	}
	now_num = analyse_num;
	if (Syn[analyse_num].identity == "procedure")
	{
		int temp = proc_analyse();
		if (temp == 1)
		{
			analyse_num++;
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！proc错误！" << endl;
			return 0;
		}
	}
	int temp = body_analyse();
	if (temp == 1)
	{
		return 1;
	}
	else
	{
		cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！body错误！" << endl;
		return 0;
	}
}

bool prog_analyse()
{
	int now_num = analyse_num;
	if (Syn[analyse_num].symbol == "program")
	{
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol == "identity")
		{
			analyse_num++;
			now_num = analyse_num;
			if (Syn[analyse_num].identity == ";")
			{
				analyse_num++;
				now_num = analyse_num;
				int temp = block_analyse();
				if (temp == 1)
				{
					cout << "编译成功" << endl;
					return 1;
				}
				else
				{
					cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！block出错！" << endl;
					return 0;
				}
			}
			else
			{
				cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少;！" << endl;
				return 0;
			}
		}
		else
		{
			cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少id！" << endl;
			return 0;
		}
	}
	else
	{
		cout << Syn[now_num].line << "行" << Syn[now_num].col << "列" << "语法错误！缺少program！" << endl;
		return 0;
	}
}

void Syn_analyse()
{

}

int main()
{
	word();
	for(int i=0;i<num;i++)
	{
		cout<<Syn[i].line<<" "<<Syn[i].col<<" "<<Syn[i].identity<<"--"<<Syn[i].symbol<<endl;
	}
	prog_analyse();
	return 0;
}