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
struct error {
	string error;
	int line;
	int col;
}temperr,Err[10000];
int flag[10000];
int error(string identity,string error_id);
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

int error(string identity, string error_id)
{
	if (error_id == "id")
	{
		if (Syn[analyse_num].symbol == "identity")
		{
			temperr.line = 0;
			return 3;
		}
	}
	if (error_id == "num")
	{
		if (Syn[analyse_num].symbol == "num")
		{
			temperr.line = 0;
			return 3;
		}
	}
	if (error_id == "mop")
	{
		if (Syn[analyse_num].symbol == "mop")
		{
			temperr.line = 0;
			return 3;
		}
	}
	if (error_id == "aop")
	{
		if (Syn[analyse_num].symbol == "aop")
		{
			temperr.line = 0;
			return 3;
		}
	}
	if (error_id == "lop")
	{
		if (Syn[analyse_num].symbol == "lop")
		{
			temperr.line = 0;
			return 3;
		}
	}
	if (error_id == Syn[analyse_num].identity)
	{
		temperr.line = 0;
		return 3;
	}
	if (identity == "prog")
	{
		if (error_id == "id")
		{
			if (Syn[analyse_num].symbol != "identity" && Syn[analyse_num + 1].identity == ";")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "id_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == ";")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "id_lack" << endl;
				return 1;
			}
		}
		if (error_id == ";")
		{
			if (Syn[analyse_num].identity != ";" && (Syn[analyse_num+1].identity == "begin"|| Syn[analyse_num + 1].identity == "const"|| Syn[analyse_num + 1].identity == "var"|| Syn[analyse_num + 1].identity == "procedure"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ";_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == "begin" || Syn[analyse_num].identity == "const" || Syn[analyse_num].identity == "var" || Syn[analyse_num].identity == "procedure")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ";_lack" << endl;
				return 1;
			}
		}
	}
	if (identity == "condecl")
	{
		if (error_id == "const")
		{
			if (Syn[analyse_num].identity != "const" && (Syn[analyse_num + 1].symbol == "identity"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "const_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "const_lack!" << endl;
				return 1;
			}
		}
		if (error_id == ",")
		{
			if (Syn[analyse_num].identity != "," && (Syn[analyse_num + 1].symbol == "identity"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ",_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ",_lack!" << endl;
				return 1;
			}
		}
		if (error_id == ";")
		{
			if (Syn[analyse_num].identity != ";" && (Syn[analyse_num + 1].identity == "begin" || Syn[analyse_num + 1].identity == "var" || Syn[analyse_num + 1].identity == "procedure"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ";_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == "begin" || Syn[analyse_num].identity == "var" || Syn[analyse_num].identity == "procedure")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ";_lack" << endl;
				return 1;
			}
		}
	}
	if (identity == "const")
	{
		if (error_id == "id")
		{
			if (Syn[analyse_num].symbol != "identity" && Syn[analyse_num+1].identity == ":=")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "id_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == ":=")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "id_lack" << endl;
				return 1;
			}
		}
		if (error_id == ":=")
		{
			if (Syn[analyse_num].identity != ":=" && Syn[analyse_num + 1].symbol == "num")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ":=_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "num")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ":=_lack" << endl;
				return 1;
			}
		}
		if (error_id == "num")
		{
			if (Syn[analyse_num].symbol != "num" && (Syn[analyse_num + 1].identity == ","|| Syn[analyse_num + 1].identity == ";"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "num_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == "," || Syn[analyse_num].identity == ";")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "num_lack" << endl;
				return 1;
			}
		}
	}
	if (identity == "vardecl")
	{
		if (error_id == "var")
		{
			if (Syn[analyse_num].identity != "var" && (Syn[analyse_num + 1].symbol == "identity"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "var_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "var_lack!" << endl;
				return 1;
			}
		}
		if (error_id == ",")
		{
			if (Syn[analyse_num].identity != "," && (Syn[analyse_num + 1].symbol == "identity"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ",_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ",_lack!" << endl;
				return 1;
			}
		}
		if (error_id == ";")
		{
			if (Syn[analyse_num].identity != ";" && (Syn[analyse_num + 1].identity == "begin" || Syn[analyse_num + 1].identity == "procedure"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ";_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == "begin" || Syn[analyse_num].identity == "procedure")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ";_lack" << endl;
				return 1;
			}
		}
		if (error_id == "id")
		{
			if (Syn[analyse_num].symbol != "identity" && (Syn[analyse_num + 1].identity == ","|| Syn[analyse_num + 1].identity == ";"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "id_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == ",")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "id_lack" << endl;
				return 1;
			}
		}
	}
	if (identity == "proc")
	{
		if (error_id == "procedure")
		{
			if (Syn[analyse_num].identity != "procedure" && (Syn[analyse_num + 1].symbol == "identity"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "procedure_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "procedure_lack!" << endl;
				return 1;
			}
		}
		if (error_id == ",")
		{
			if (Syn[analyse_num].identity != "," && (Syn[analyse_num + 1].symbol == "identity"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ",_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ",_lack!" << endl;
				return 1;
			}
		}
		if (error_id == ";")
		{
			if (Syn[analyse_num].identity != ";" && (Syn[analyse_num + 1].identity == "procedure" || Syn[analyse_num + 1].identity == "const" || Syn[analyse_num + 1].identity == "var" || Syn[analyse_num + 1].identity == "begin"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ";_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == "procedure" || Syn[analyse_num].identity == "const" || Syn[analyse_num].identity == "var" || Syn[analyse_num].identity == "begin")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ";_lack" << endl;
				return 1;
			}
		}
		if (error_id == "id")
		{
			if (Syn[analyse_num].symbol != "identity" && (Syn[analyse_num + 1].identity == "(" || Syn[analyse_num + 1].identity == "," || Syn[analyse_num + 1].identity == ")"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "id_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == "(" || Syn[analyse_num].identity == "," || Syn[analyse_num].identity == ")")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "id_lack" << endl;
				return 1;
			}
		}
		if (error_id == "(")
		{
			if (Syn[analyse_num].identity != "(" && (Syn[analyse_num + 1].identity == ")" || Syn[analyse_num + 1].symbol == "identity"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "(_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == ")" || Syn[analyse_num].symbol == "identity")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "(_lack" << endl;
				return 1;
			}
		}
		if (error_id == ")")
		{
			if (Syn[analyse_num].identity != ")" && (Syn[analyse_num + 1].identity == ";"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ")_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == ";")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ")_lack" << endl;
				return 1;
			}
		}
	}
	if (identity == "body")
	{
		if (error_id == "begin")
		{
			if (Syn[analyse_num].identity != "begin" && (Syn[analyse_num + 1].symbol == "identity"|| Syn[analyse_num + 1].identity == "if"|| Syn[analyse_num + 1].symbol == "while"|| Syn[analyse_num + 1].symbol == "call"||Syn[analyse_num + 1].symbol == "begin"|| Syn[analyse_num + 1].symbol == "read"|| Syn[analyse_num + 1].symbol == "write"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "begin_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity" || Syn[analyse_num].identity == "if" || Syn[analyse_num].symbol == "while" || Syn[analyse_num].symbol == "call" || Syn[analyse_num].symbol == "begin" || Syn[analyse_num].symbol == "read" || Syn[analyse_num].symbol == "write")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "begin_lack!" << endl;
				return 1;
			}
		}
		if (error_id == ";")
		{
			if (Syn[analyse_num].identity != ";" && (Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].identity == "if" || Syn[analyse_num + 1].symbol == "while" || Syn[analyse_num + 1].symbol == "call" || Syn[analyse_num + 1].symbol == "begin" || Syn[analyse_num + 1].symbol == "read" || Syn[analyse_num + 1].symbol == "write"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ";_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity" || Syn[analyse_num].identity == "if" || Syn[analyse_num].symbol == "while" || Syn[analyse_num].symbol == "call" || Syn[analyse_num].symbol == "begin" || Syn[analyse_num].symbol == "read" || Syn[analyse_num].symbol == "write")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ";_lack!" << endl;
				return 1;
			}
		}
	}
	if (identity == "statement")
	{
		if (error_id == "id")
		{
			if (Syn[analyse_num].identity != "id" && (Syn[analyse_num + 1].identity == ":=" || Syn[analyse_num + 1].identity == "(" || Syn[analyse_num + 1].identity == "," || Syn[analyse_num + 1].identity == ")"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "begin_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].identity == ":=" || Syn[analyse_num].identity == "(" || Syn[analyse_num].identity == "," || Syn[analyse_num].identity == ")")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "begin_lack!" << endl;
				return 1;
			}
		}
		if (error_id == ":=")
		{
			if (Syn[analyse_num].identity != ":=" && (Syn[analyse_num + 1].symbol == "mop" || Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "("))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ":=_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "mop" || Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ":=_lack" << endl;
				return 1;
			}
		}
		if (error_id == "if")
		{
			if (Syn[analyse_num].identity != "if" && (Syn[analyse_num + 1].symbol == "mop" || Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "("))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "if_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "mop" || Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "if_lack" << endl;
				return 1;
			}
		}
		if (error_id == "then")
		{
			if (Syn[analyse_num].identity != "then" && (Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].identity == "if" || Syn[analyse_num + 1].symbol == "while" || Syn[analyse_num + 1].symbol == "call" || Syn[analyse_num + 1].symbol == "begin" || Syn[analyse_num + 1].symbol == "read" || Syn[analyse_num + 1].symbol == "write"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "then_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity" || Syn[analyse_num].identity == "if" || Syn[analyse_num].symbol == "while" || Syn[analyse_num].symbol == "call" || Syn[analyse_num].symbol == "begin" || Syn[analyse_num].symbol == "read" || Syn[analyse_num].symbol == "write")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "then_lack!" << endl;
				return 1;
			}
		}
		if (error_id == "else")
		{
			if (Syn[analyse_num].identity != "else" && (Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].identity == "if" || Syn[analyse_num + 1].symbol == "while" || Syn[analyse_num + 1].symbol == "call" || Syn[analyse_num + 1].symbol == "begin" || Syn[analyse_num + 1].symbol == "read" || Syn[analyse_num + 1].symbol == "write"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "else_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity" || Syn[analyse_num].identity == "if" || Syn[analyse_num].symbol == "while" || Syn[analyse_num].symbol == "call" || Syn[analyse_num].symbol == "begin" || Syn[analyse_num].symbol == "read" || Syn[analyse_num].symbol == "write")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "else_lack!" << endl;
				return 1;
			}
		}
		if (error_id == "while")
		{
			if (Syn[analyse_num].identity != "while" && (Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "("))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "while_error" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "aop" || Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "while_lack" << endl;
				return 1;
			}
		}
		if (error_id == "do")
		{
			if (Syn[analyse_num].identity != "do" && (Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].identity == "if" || Syn[analyse_num + 1].symbol == "while" || Syn[analyse_num + 1].symbol == "call" || Syn[analyse_num + 1].symbol == "begin" || Syn[analyse_num + 1].symbol == "read" || Syn[analyse_num + 1].symbol == "write"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "do_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity" || Syn[analyse_num].identity == "if" || Syn[analyse_num].symbol == "while" || Syn[analyse_num].symbol == "call" || Syn[analyse_num].symbol == "begin" || Syn[analyse_num].symbol == "read" || Syn[analyse_num].symbol == "write")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "do_lack!" << endl;
				return 1;
			}
		}
		if (error_id == "call")
		{
			if (Syn[analyse_num].identity != "call" && (Syn[analyse_num + 1].symbol == "identity"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "call_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "call_lack!" << endl;
				return 1;
			}
		}
		if (error_id == "(")
		{
			if (Syn[analyse_num].identity != "(" && (Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "("))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "(_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "aop" || Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "(_lack!" << endl;
				return 1;
			}
		}
		if (error_id == ")")
		{
			if (Syn[analyse_num].identity != ")" && (Syn[analyse_num + 1].symbol == "end" || Syn[analyse_num + 1].identity == ";" || Syn[analyse_num + 1].symbol == "else"||Syn[analyse_num+1].identity=="begin"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ")_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "end" || Syn[analyse_num].identity == ";" || Syn[analyse_num].symbol == "else" || Syn[analyse_num].identity == "begin")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ")_lack!" << endl;
				return 1;
			}
		}
		if (error_id == ",")
		{
			if (Syn[analyse_num].identity != "," && (Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "("))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ",_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "aop" || Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ",_lack!" << endl;
				return 1;
			}
		}
		if (error_id == "read")
		{
			if (Syn[analyse_num].identity != "read" && (Syn[analyse_num + 1].symbol == "identity"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "read_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "read_lack!" << endl;
				return 1;
			}
		}
		if (error_id == "write")
		{
			if (Syn[analyse_num].identity != "write" && (Syn[analyse_num + 1].symbol == "identity"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列"  "write_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "write_lack!" << endl;
				return 1;
			}
		}
	}
	if (identity == "factor")
	{
		if (error_id == "id")
		{
			if (Syn[analyse_num].symbol != "id" && (Syn[analyse_num + 1].symbol == "mop" || Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "lop" || Syn[analyse_num + 1].identity == "then"|| Syn[analyse_num + 1].identity == "do"|| Syn[analyse_num + 1].identity == ")"))
			{
				temperr.line = Syn[analyse_num].line;
				temperr.col = Syn[analyse_num].col;
				temperr.error = "id_error!";
				return 0;
			}
			else if (Syn[analyse_num].symbol == "mop" || Syn[analyse_num].symbol == "aop" || Syn[analyse_num].symbol == "lop" || Syn[analyse_num].identity == "then" || Syn[analyse_num].identity == "do" || Syn[analyse_num].identity == ")")
			{
				temperr.line = Syn[analyse_num].line;
				temperr.col = Syn[analyse_num].col;
				temperr.error = "id_error!";
				return 1;
			}
		}
		if (error_id == "num")
		{
			if (Syn[analyse_num].symbol != "num" && (Syn[analyse_num + 1].symbol == "mop" || Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "lop" || Syn[analyse_num + 1].identity == "then" || Syn[analyse_num + 1].identity == "do" || Syn[analyse_num + 1].identity == ")"))
			{
				temperr.line = Syn[analyse_num].line;
				temperr.col = Syn[analyse_num].col;
				temperr.error = "num_error!";
				return 0;
			}
			else if (Syn[analyse_num].symbol == "mop" || Syn[analyse_num].symbol == "aop" || Syn[analyse_num].symbol == "lop" || Syn[analyse_num].identity == "then" || Syn[analyse_num].identity == "do" || Syn[analyse_num].identity == ")")
			{
				temperr.line = Syn[analyse_num].line;
				temperr.col = Syn[analyse_num].col;
				temperr.error = "num_error!";
				return 1;
			}
		}
		if (error_id == ")")
		{
			if (Syn[analyse_num].identity != ")" && (Syn[analyse_num + 1].symbol == "mop" || Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "lop" || Syn[analyse_num + 1].identity == "then" || Syn[analyse_num + 1].identity == "do" || Syn[analyse_num + 1].identity == ")"))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ")_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "mop" || Syn[analyse_num].symbol == "aop" || Syn[analyse_num].symbol == "lop" || Syn[analyse_num].identity == "then" || Syn[analyse_num].identity == "do" || Syn[analyse_num].identity == ")")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << ")_lack!" << endl;
				return 1;
			}
		}
		if (error_id == "(")
		{
			if (Syn[analyse_num].identity != "(" && (Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "("))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "(_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "aop" || Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "(_lack!" << endl;
				return 1;
			}
		}
	}
	if (identity == "exp")
	{
		if (error_id == "aop")
		{
			if (Syn[analyse_num].symbol != "aop" && (Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "("))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "aop_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "aop_lack!" << endl;
				return 1;
			}
		}
	}
	if (identity == "term")
	{
		if (error_id == "mop")
		{
			if (Syn[analyse_num].symbol != "mop" && (Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "("))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "mop_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "mop_lack!" << endl;
				return 1;
			}
		}
	}
	if (identity == "lexp")
	{
		if (error_id == "odd")
		{
			if (Syn[analyse_num].identity != "odd" && (Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "("))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "odd_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "aop" || Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "odd_lack!" << endl;
				return 1;
			}
		}
		if (error_id == "lop")
		{
			if (Syn[analyse_num].symbol != "lop" && (Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "("))
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "lop_error!" << endl;
				return 0;
			}
			else if (Syn[analyse_num].symbol == "lop" || Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				cout << Syn[analyse_num].line << "行" << Syn[analyse_num].col << "列" << "lop_lack!" << endl;
				return 1;
			}
		}
	}
	return 2;
}

bool const_analyse()
{
	if (error("const", "id") != 1)
	{
		analyse_num++;
	}
	if (error("const", ":=") != 1)
	{
		analyse_num++;
	}
	if (error("const", "num") != 1)
	{

	}
	cout << "----------const------------" << endl;
	return 1;
}

bool factor_analyse()
{
	if (error("factor", "id") != 1)
	{
		return 1;
	}
	if (error("factor", "num") != 1)
	{
		return 1;
	}
	if (error("factor", "(") != 1)
		analyse_num++;
	exp_analyse();
	analyse_num++;
	if (error("factor", ")") == 2)
		cout << "有点错误" << endl;
	cout << "----------factor------------" << endl;
	if (temperr.line != 0)
	{
		cout << temperr.line << "行" << temperr.col << "列" << temperr.error << endl;
	}
	return 1;
}

bool exp_analyse()
{
	if (Syn[analyse_num].symbol == "aop")
		analyse_num++;
	term_analyse();
	analyse_num++;
	while (Syn[analyse_num].symbol == "aop")
	{
		analyse_num++;
		term_analyse();
		analyse_num++;
	}
	if (Syn[analyse_num].symbol != "aop")
		analyse_num--;
	cout << "----------exp------------" << endl;
	return 1;
}

bool term_analyse()
{
	factor_analyse();
	analyse_num++;
	while (Syn[analyse_num].symbol=="mop")
	{
		analyse_num++;
		factor_analyse();
		analyse_num++;
	}
	if (Syn[analyse_num].symbol != "mop")
		analyse_num--;
	cout << "----------term------------" << endl;
	return 1;
}

bool lexp_analyse()
{
	if (Syn[analyse_num].identity=="odd")
	{
		analyse_num++;
		exp_analyse();
	}
	else
	{
		exp_analyse();
		analyse_num++;
		if (error("lexp", "lop") != 1)
			analyse_num++;
		exp_analyse();
	}
	cout << "----------lexp------------" << endl;
	return 1;
}

bool statement_analyse()
{
	int now_num = 0;
	if (Syn[analyse_num].symbol == "identity")
	{
		analyse_num++;
		if (error("statement", ":=") != 1)
		{
			analyse_num++;
		}
		exp_analyse();
	}
	else if (Syn[analyse_num].symbol == "if")
	{
		analyse_num++;
		lexp_analyse();
		analyse_num++;
		if (error("statement", "then") != 1)
			analyse_num++;
		statement_analyse();
		analyse_num++;
		if (error("statement", "else") != 1 || Syn[analyse_num].identity == "else")
		{
			analyse_num++;
			statement_analyse();
		}
		else if (error("statement", "else") == 1)
		{
			statement_analyse();
		}
	}
	else if (Syn[analyse_num].symbol == "while")
	{
		analyse_num++;
		lexp_analyse();
		analyse_num++;
		if (error("statement", "do") != 1)
			analyse_num++;
		statement_analyse();
	}
	else if (Syn[analyse_num].symbol == "call")
	{
		analyse_num++;
		if (error("statement", "id") != 1)
			analyse_num++;
		if (error("statement", "(") != 1)
			analyse_num++;
		exp_analyse();
		analyse_num++;
		if (error("statement", ")") != 2)
		{
			if (error("statement", ")") == 1)
				analyse_num--;
			return 1;
		}
		int temp = error("statement", ",");
		if (temp != 1)
		{
			analyse_num++;
			while (Syn[analyse_num].identity!=")")
			{
				exp_analyse();
				analyse_num++;
				if (Syn[analyse_num].identity == ")" || error("statement", ")") != 2)
					break;
				if (error("statement", ",") != 1)
					analyse_num++;
			}
		}
		else if (error("statement", ",") == 1)
		{
			while (Syn[analyse_num].identity != ")")
			{
				exp_analyse();
				analyse_num++;
				if (Syn[analyse_num].identity == ")" || error("statement", ")") != 2)
					break;
				if (error("statement", ",") != 1)
					analyse_num++;
			}
		}
	}
	else if (Syn[analyse_num].symbol == "read")
	{
		analyse_num++;
		if (error("statement", "(") != 1)
			analyse_num++;
		if (Syn[analyse_num].symbol == "identity")
			analyse_num++;
		if (error("statement", ")") != 2)
		{
			if (error("statement", ")") == 1)
				analyse_num--;
			return 1;
		}
		if (error("statement", ",") != 1)
		{
			analyse_num++;
			while (Syn[analyse_num].identity != ")")
			{
				if(Syn[analyse_num].symbol=="identity")
					analyse_num++;
				if (Syn[analyse_num].identity == ")" || error("statement", ")") != 2)
					break;
				if (error("statement", ",") != 1)
					analyse_num++;
			}
		}
		else if (error("statement", ",") == 1)
		{
			while (Syn[analyse_num].identity != ")")
			{
				if (Syn[analyse_num].symbol == "identity")
					analyse_num++;
				if (Syn[analyse_num].identity == ")" || error("statement", ")") != 2)
					break;
				if (error("statement", ",") != 1)
					analyse_num++;
			}
		}
	}
	else if (Syn[analyse_num].symbol == "write")
	{
		analyse_num++;
		if (error("statement", "(") != 1)
			analyse_num++;
		if (Syn[analyse_num].symbol == "identity")
			analyse_num++;
		if (error("statement", ")") != 2)
		{
			if (error("statement", ")") == 1)
				analyse_num--;
			return 1;
		}
		if (error("statement", ",") != 1)
		{
			analyse_num++;
			while (Syn[analyse_num].identity != ")")
			{
				exp_analyse();
				analyse_num++;
				if (Syn[analyse_num].identity == ")" || error("statement", ")") != 2)
					break;
				if (error("statement", ",") != 1)
					analyse_num++;
			}
		}
		else if (error("statement", ",") == 1)
		{
			while (Syn[analyse_num].identity != ")")
			{
				exp_analyse();
				analyse_num++;
				if (Syn[analyse_num].identity == ")" || error("statement",")")!=2)
					break;
				if (error("statement", ",") != 1)
					analyse_num++;
			}
		}
	}
	else {
		body_analyse();
	}
	cout << "----------statement------------" << endl;
	return 1;
}

bool body_analyse()
{
	if (error("body", "begin") != 1)
		analyse_num++;
	statement_analyse();
	analyse_num++;
	if (Syn[analyse_num].symbol == "end")
		return 1;
	while (1)
	{
		if (error("body", ";") != 1)
			analyse_num++;
		statement_analyse();
		analyse_num++;
		if (Syn[analyse_num].symbol == "end" || error("body","end")!=2)
			break;
	}
	cout << "----------body------------" << endl;
	return 1;
}

bool proc_analyse()
{
	int flag = 0;
	if (error("proc", "procedure") != 1)
		analyse_num++;
	if (error("proc", "id") != 1)
		analyse_num++;
	if (error("proc", "(") != 1)
		analyse_num++;
	if (Syn[analyse_num].symbol == "identity")
	{
		flag = 1;
		if (error("proc", "id") != 1)
			analyse_num++;
		while (1)
		{
			if (error("proc", ")") != 1)
			{
				analyse_num++;
				break;
			}
			if (error("proc", ",") != 1)
				analyse_num++;
			if (error("proc", "id") != 1)
				analyse_num++;
		}
	}
	if (flag == 0)
	{
		if (error("proc", ")") != 1)
			analyse_num++;
	}
	if (error("proc", ";") != 1)
		analyse_num++;
	block_analyse();
	analyse_num++;
	while (Syn[analyse_num].identity == ";")
	{
		analyse_num++;
		proc_analyse();
	}
	if (Syn[analyse_num].identity != ";")
		analyse_num--;
	cout << "----------proc------------" << endl;
	return 1;
}

bool vardecl_analyse()
{
	if (error("vardecl", "var") != 1)
		analyse_num++;
	if (error("vardecl", "id") != 1)
		analyse_num++;
	while (Syn[analyse_num].identity == ",")
	{
		analyse_num++;
		if (error("vardecl", "id") == 0 || Syn[analyse_num].symbol == "identity")
			analyse_num++;
	}
	cout << "----------vardecl------------" << endl;
	if (error("vardecl", ";") != 1)
	{
		return 1;
	}
	analyse_num--;
	return 1;
}

bool condecl_analyse() 
{
	if (error("condecl", "const") != 1)
		analyse_num++;
	const_analyse();
	analyse_num++;
	while (Syn[analyse_num].identity == ",")
	{
		analyse_num++;
		if (error("condecl", ",") != 1 || Syn[analyse_num].identity == ",")
			analyse_num++;
		const_analyse();
		analyse_num++;
	}
	cout << "----------condecl------------" << endl;
	if (error("condecl", ";") != 1)
		return 1;
	analyse_num--;
	return 1;
}

bool block_analyse()
{
	if (Syn[analyse_num].identity == "const")
	{
		condecl_analyse();
		analyse_num++;
	}
	if (Syn[analyse_num].identity == "var")
	{
		vardecl_analyse();
		analyse_num++;
	}
	if (Syn[analyse_num].identity == "procedure")
	{
		proc_analyse();
		analyse_num++;
	}
	if (Syn[analyse_num].identity == "begin")
	{
		statement_analyse();
	}
	cout << "----------block------------" << endl;
	return 1;
}

bool prog_analyse()
{
	if (error("prog", "program") != 1)
		analyse_num++;
	if (error("prog", "id") != 1)
		analyse_num++;
	if (error("prog", ";") != 1)
		analyse_num++;
	block_analyse();
	cout << "----------prog------------" << endl;
	return 1;
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