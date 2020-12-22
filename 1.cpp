#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
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
	string er;
	int line;
	int col;
}err[10000];

struct table {
	string identity;  //��������
	string type;  //����-const��
	int val;  //ֵ
	int addr;  //ƫ����
};

struct tablelink {
	vector<table> level_table;
	int level;
}Tablelink[200]; //������

struct target_code {
	string func;
	int L; //���
	int a; //ƫ�Ƶ�ַ
}Code[200];

//�ö���һ��ȫ�ֱ�����˭�ܽ��뵽������ʱ�
int now_level = 0;//��ǰ����
int now_level2 = 0;//���ܲ�
int now_table = 1;//��ǰ��
int check_table = 0;
int fa_table[100];//����
int table_num = 1;//������
int analyse_num = 0;//��ǰ����
int err_num = 0;//�������
int level_idnum[100];//ÿһ���������
int now_code = 1;//��ǰĿ���������

int find(string id)
{
	for (int i = check_table; i; i = fa_table[i])
	{
		for (int j = 0; j < Tablelink[i].level_table.size(); j++)
		{
			if (Tablelink[i].level_table[j].identity == id)
			{
				return i;
			}
		}
	}
	return -1;
}

void enter(string id,string type)
{
	int find_result = find(id);  //�鶨��
	if (find_result >= 0) {
		err[err_num].line = Syn[analyse_num].line;
		err[err_num].col = Syn[analyse_num].col;
		err[err_num].er = "����"+id+"�ظ����壡";
		err_num++;
		return;
	}
	table temp;
	temp.identity = id;
	temp.type = type;
	temp.val = 0;
	temp.addr = level_idnum[now_level];
	Tablelink[now_table].level_table.push_back(temp);
}

void gen(string func, int L, int a)
{
	Code[now_code].func = func;
	Code[now_code].L = L;
	Code[now_code].a = a;
	now_code++;
}

bool const_analyse();
bool factor_analyse();
bool exp_analyse();
bool term_analyse();
bool lexp_analyse();
bool statement_analyse();
bool body_analyse();
bool proc_analyse(int,int);
bool vardecl_analyse();
bool condecl_analyse();
bool block_analyse(int,int);
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
				Syn[num].col = pos - temp.size();
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
						outfile << "Error Not Legal!" << row << "��" << pos << "��" << "---" << temp << endl;
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
					Syn[num].col = pos - temp.size();
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
				Syn[num].col = pos - 1;
				num++;
				outfile << "Equal: " << ch << endl;
			}
			else if (ch == '+')
			{
				Syn[num].symbol = "aop";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos - 1;
				num++;
				outfile << "Plus: " << ch << endl;
			}
			else if (ch == '-')
			{
				Syn[num].symbol = "aop";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos - 1;
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
					Syn[num].col = pos - 2;
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
					Syn[num].col = pos - 1;
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
				Syn[num].col = pos - 1;
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
					Syn[num].col = pos - 2;
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
					Syn[num].col = pos - 1;
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
					Syn[num].col = pos - 2;
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
					Syn[num].col = pos - 2;
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
					Syn[num].col = pos - 1;
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
					Syn[num].col = pos - 2;
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
					outfile << "Error Not Defined!" << row << "��" << pos << "��" << "---" << temp << endl;
					temp.clear();
				}
			}
			else if (ch == ',')
			{
				Syn[num].symbol = "comma";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos - 1;
				num++;
				outfile << "comma: " << ch << endl;
			}
			else if (ch == ';')
			{
				Syn[num].symbol = "semicolon";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos - 1;
				num++;
				outfile << "semicolon: " << ch << endl;
			}
			else if (ch == '(')
			{
				Syn[num].symbol = "leftbrackets";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos - 1;
				num++;
				outfile << "leftbrackets: " << ch << endl;
			}
			else if (ch == ')')
			{
				Syn[num].symbol = "rightbrackets";
				Syn[num].identity = ch;
				Syn[num].line = row;
				Syn[num].col = pos - 1;
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
				outfile << "Error Not Defined!" << row << "��" << pos << "��" << "---" << temp << endl;
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

bool const_analyse()
{
	if (Syn[analyse_num].symbol == "identity")
	{
		enter(Syn[analyse_num].identity, "const");
		level_idnum[now_level2]++;
		analyse_num++;
		if (Syn[analyse_num].symbol != "Assign" && Syn[analyse_num + 1].symbol == "num")
		{
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "�﷨���󣬸�ֵ�Ŵ���";
			err_num++;
		}
		analyse_num++;
		if (Syn[analyse_num].symbol != "num")
		{
			if (Syn[analyse_num].identity == "," || Syn[analyse_num].identity == ";")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ�����֣�";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].identity == "," || Syn[analyse_num + 1].identity == ";")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨���󣬲������֣�";
				err_num++;
			}
		}
		else {
			Tablelink[now_table].level_table[Tablelink[now_table].level_table.size() - 1].val = atoi(Syn[analyse_num].identity.c_str());
		}
		return 1;
	}
	while (Syn[analyse_num + 1].identity != "," && Syn[analyse_num].identity != ";")
	{
		analyse_num++;
	}
	return 1;
}

bool factor_analyse()
{
	int now_num = 0;
	if (Syn[analyse_num].symbol == "identity")
	{
		if (find(Syn[analyse_num].identity) == -1) {
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "����" + Syn[analyse_num].identity + "δ���壡";
			err_num++;
		}
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
		exp_analyse();
		analyse_num++;
		if (Syn[analyse_num].symbol != "rightbrackets")
		{
			if (Syn[analyse_num].symbol != "mop" && Syn[analyse_num].symbol != "aop" && Syn[analyse_num].identity != ")" && Syn[analyse_num].symbol != "lop" && Syn[analyse_num].symbol != "then" && Syn[analyse_num].symbol != "do" && Syn[analyse_num].identity != "," && Syn[analyse_num].identity != ";" && Syn[analyse_num].symbol != "else" && Syn[analyse_num].symbol != "end")
			{
				err[err_num].line = Syn[now_num].line;
				err[err_num].col = Syn[now_num].col;
				err[err_num].er = "�﷨����ȱ��)��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].symbol != "mop" && Syn[analyse_num + 1].symbol != "aop" && Syn[analyse_num + 1].identity != ")" && Syn[analyse_num + 1].symbol != "lop" && Syn[analyse_num + 1].symbol != "then" && Syn[analyse_num + 1].symbol != "do" && Syn[analyse_num + 1].identity != "," && Syn[analyse_num + 1].identity != ";" && Syn[analyse_num + 1].symbol != "else" && Syn[analyse_num + 1].symbol != "end")
			{
				err[err_num].line = Syn[now_num].line;
				err[err_num].col = Syn[now_num].col;
				err[err_num].er = "�﷨����)����";
				err_num++;
			}
		}
		else
		{
			return 1;
		}
	}
	err[err_num].line = Syn[analyse_num].line;
	err[err_num].col = Syn[analyse_num].col;
	err[err_num].er = "�﷨����factorȱʧ��";
	err_num++;
	analyse_num--;
	return 1;
}

bool exp_analyse()
{
	int now_num = 0;
	if (Syn[analyse_num].identity == "+" || Syn[analyse_num].identity == "-")
	{
		analyse_num++;
		now_num = analyse_num;
	}
	term_analyse();
	analyse_num++;
	while (Syn[analyse_num].symbol == "aop")
	{
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol == "mop" || Syn[analyse_num].identity == ")" || Syn[analyse_num].symbol == "lop" || Syn[analyse_num].symbol == "then" || Syn[analyse_num].symbol == "do" || Syn[analyse_num].identity == "," || Syn[analyse_num].identity == ";" || Syn[analyse_num].symbol == "else" || Syn[analyse_num].symbol == "end")
		{
			err[err_num].line = Syn[now_num].line;
			err[err_num].col = Syn[now_num].col;
			err[err_num].er = "�﷨����ȱ��term���ӣ�";
			err_num++;
			break;
		}
		term_analyse();
		analyse_num++;
	}
	analyse_num--;
	return 1;
}

bool term_analyse()
{
	int now_num = analyse_num;
	factor_analyse();
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
		if (Syn[analyse_num].symbol == "aop" || Syn[analyse_num].identity == ")" || Syn[analyse_num].symbol == "lop" || Syn[analyse_num].symbol == "then" || Syn[analyse_num].symbol == "do" || Syn[analyse_num].identity == "," || Syn[analyse_num].identity == ";" || Syn[analyse_num].symbol == "else" || Syn[analyse_num].symbol == "end")
		{
			err[err_num].line = Syn[now_num].line;
			err[err_num].col = Syn[now_num].col;
			err[err_num].er = "�﷨����ȱ��factor���ӣ�";
			err_num++;
			break;
		}
		factor_analyse();
		analyse_num++;
	}
	analyse_num--;
	return 1;
}

bool lexp_analyse()
{
	int now_num = analyse_num;
	if (Syn[analyse_num].symbol == "odd")
	{
		analyse_num++;
		now_num = analyse_num;
		exp_analyse();
		return 1;
	}
	else
	{
		exp_analyse();
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol != "lop")
		{
			if (Syn[analyse_num].symbol == "aop" || Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��lop��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "(")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����lop����";
				err_num++;
			}
		}
		analyse_num++;
		now_num = analyse_num;
		exp_analyse();
	}
	return 1;
}

bool statement_analyse()
{
	int now_num = 0;
	if (Syn[analyse_num].symbol == "identity")
	{
		if (find(Syn[analyse_num].identity) == -1) {
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "����" + Syn[analyse_num].identity + "δ���壡";
			err_num++;
		}
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].identity == ":=")
		{
			analyse_num++;
			now_num = analyse_num;
			exp_analyse();
			return 1;
		}
		else
		{
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "�﷨����,:=����";
			err_num++;
		}
	}
	else if (Syn[analyse_num].symbol == "if")
	{
		analyse_num++;
		now_num = analyse_num;
		lexp_analyse();
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol != "then")
		{
			if (Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "if" || Syn[analyse_num].symbol == "while" || Syn[analyse_num].symbol == "call" || Syn[analyse_num].symbol == "begin" || Syn[analyse_num].symbol == "read" || Syn[analyse_num].symbol == "write")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��THEN��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "if" || Syn[analyse_num + 1].symbol == "while" || Syn[analyse_num + 1].symbol == "call" || Syn[analyse_num + 1].symbol == "begin" || Syn[analyse_num + 1].symbol == "read" || Syn[analyse_num + 1].symbol == "write")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����THEN����";
				err_num++;
			}
		}
		analyse_num++;
		now_num = analyse_num;
		statement_analyse();
		analyse_num++;
		if (Syn[analyse_num].symbol == "else")
		{
			analyse_num++;
			now_num = analyse_num;
			statement_analyse();
			return 1;
		}
		else
		{
			analyse_num--;
			return 1;
		}
	}
	else if (Syn[analyse_num].symbol == "while")
	{
		analyse_num++;
		now_num = analyse_num;
		lexp_analyse();
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol != "do")
		{
			if (Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "if" || Syn[analyse_num].symbol == "while" || Syn[analyse_num].symbol == "call" || Syn[analyse_num].symbol == "begin" || Syn[analyse_num].symbol == "read" || Syn[analyse_num].symbol == "write")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��do��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "if" || Syn[analyse_num + 1].symbol == "while" || Syn[analyse_num + 1].symbol == "call" || Syn[analyse_num + 1].symbol == "begin" || Syn[analyse_num + 1].symbol == "read" || Syn[analyse_num + 1].symbol == "write")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����do����";
				err_num++;
			}
		}
		analyse_num++;
		now_num = analyse_num;
		statement_analyse();
		return 1;
	}
	else if (Syn[analyse_num].symbol == "call")
	{
		analyse_num++;;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol != "identity")
		{
			if (Syn[analyse_num].identity == "(")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��id��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].identity == "(")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����id����";
				err_num++;
			}
		}
		else
		{
			if (find(Syn[analyse_num].identity) == -1) {
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "����" + Syn[analyse_num].identity + "δ���壡";
				err_num++;
			}
		}
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].identity != "(")
		{
			if (Syn[analyse_num].symbol == "aop" || Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��(��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "(")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����(����";
				err_num++;
			}
		}
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].identity == ")")
		{
			return 1;
		}
		else
		{
			now_num = analyse_num;
			exp_analyse();
			analyse_num++;
			while (Syn[analyse_num].identity == ",")
			{
				analyse_num++;
				now_num = analyse_num;
				if (Syn[analyse_num].identity == ")")
				{
					err[err_num].line = Syn[now_num].line;
					err[err_num].col = Syn[now_num].col;
					err[err_num].er = "�﷨����ȱ�ٱ��ʽ��";
					err_num++;
					break;
				}
				exp_analyse();
				analyse_num++;
			}
			now_num = analyse_num - 1;
			if (Syn[analyse_num].identity == ")")
			{
				return 1;
			}
			else
			{
				if (Syn[analyse_num].symbol == "end" || Syn[analyse_num].identity == ";")
				{
					err[err_num].line = Syn[analyse_num].line;
					err[err_num].col = Syn[analyse_num].col;
					err[err_num].er = "�﷨����ȱ��)��";
					err_num++;
					analyse_num--;
				}
				else if (Syn[analyse_num + 1].symbol == "end" || Syn[analyse_num + 1].identity == ";")
				{
					err[err_num].line = Syn[analyse_num].line;
					err[err_num].col = Syn[analyse_num].col;
					err[err_num].er = "�﷨����)����";
					err_num++;
				}
			}
		}
	}
	else if (Syn[analyse_num].symbol == "read")
	{
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].identity != "(")
		{
			if (Syn[analyse_num].symbol == "identity")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��(��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].symbol == "identity")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����(����";
				err_num++;
			}
		}
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol != "identity")
		{
			if (Syn[analyse_num].identity == "," || Syn[analyse_num].identity == ")")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��id��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].identity == "," || Syn[analyse_num + 1].identity == ")")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����id����";
				err_num++;
			}
		}
		else {
			if (find(Syn[analyse_num].identity) == -1) {
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "����" + Syn[analyse_num].identity + "δ���壡";
				err_num++;
			}
		}
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
				if (find(Syn[analyse_num].identity) == -1) {
					err[err_num].line = Syn[analyse_num].line;
					err[err_num].col = Syn[analyse_num].col;
					err[err_num].er = "����" + Syn[analyse_num].identity + "δ���壡";
					err_num++;
				}
			}
			else
			{
				if (Syn[analyse_num].identity == "," || Syn[analyse_num].identity == ")")
				{
					err[err_num].line = Syn[analyse_num].line;
					err[err_num].col = Syn[analyse_num].col;
					err[err_num].er = "�﷨����ȱ��id��";
					err_num++;
				}
				else if (Syn[analyse_num + 1].identity == "," || Syn[analyse_num + 1].identity == ")")
				{
					err[err_num].line = Syn[analyse_num].line;
					err[err_num].col = Syn[analyse_num].col;
					err[err_num].er = "�﷨����id����";
					err_num++;
					analyse_num++;
				}
			}
		}
		if (Syn[analyse_num].identity == ")")
		{
			return 1;
		}
		else
		{
			if (Syn[analyse_num].symbol == "end" || Syn[analyse_num].identity == ";")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��)��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].symbol == "end" || Syn[analyse_num + 1].identity == ";")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����)����";
				err_num++;
			}
		}
	}
	else if (Syn[analyse_num].symbol == "write")
	{
		analyse_num++;;
		now_num = analyse_num;
		if (Syn[analyse_num].identity != "(")
		{
			if (Syn[analyse_num].symbol == "aop" || Syn[analyse_num].symbol == "identity" || Syn[analyse_num].symbol == "num" || Syn[analyse_num].identity == "(")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��(��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].symbol == "aop" || Syn[analyse_num + 1].symbol == "identity" || Syn[analyse_num + 1].symbol == "num" || Syn[analyse_num + 1].identity == "(")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����(����";
				err_num++;
			}
		}
		analyse_num++;
		now_num = analyse_num;
		exp_analyse();
		analyse_num++;
		while (Syn[analyse_num].identity == ",")
		{
			now_num = analyse_num;
			analyse_num++;
			if (Syn[analyse_num].identity == ")")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ�ٱ��ʽ��";
				err_num++;
				break;
			}
			exp_analyse();
			analyse_num++;
		}
		now_num = analyse_num;
		if (Syn[analyse_num].identity == ")")
		{
			return 1;
		}
		else
		{
			if (Syn[analyse_num].symbol == "end" || Syn[analyse_num].identity == ";")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��)��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].symbol == "end" || Syn[analyse_num + 1].identity == ";")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����)����";
				err_num++;
			}
		}
	}
	else {
		now_num = analyse_num;
		body_analyse();
		return 1;
	}
	while (Syn[analyse_num + 1].identity != ";" && Syn[analyse_num + 1].symbol != "else" && Syn[analyse_num + 1].symbol != "end")
	{
		analyse_num++;
	}
	return 1;
}

bool body_analyse()
{
	int now_num = analyse_num;
	if (Syn[analyse_num].symbol == "begin")
	{
		analyse_num++;
		now_num = analyse_num;
		statement_analyse();
		analyse_num++;
		now_num = analyse_num;
		while (1)
		{
			if (Syn[analyse_num].identity == ";")
				analyse_num++;
			else if (Syn[analyse_num].identity != "end")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��;��";
				err_num++;
			}
			now_num = analyse_num;
			if (Syn[analyse_num].symbol == "end")
			{
				break;
			}
			statement_analyse();
			analyse_num++;
			now_num = analyse_num;
		}
		if (Syn[analyse_num].symbol == "end")
		{
			return 1;
		}
		else
		{
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "�﷨����ȱ��end��";
			err_num++;
		}
	}
	else
	{
		err[err_num].line = Syn[analyse_num].line;
		err[err_num].col = Syn[analyse_num].col;
		err[err_num].er = "�﷨����ȱ��begin��";
		err_num++;
	}
	while (Syn[analyse_num + 1].identity != ";" && Syn[analyse_num + 1].symbol != "else" && Syn[analyse_num + 1].symbol != "end")
	{
		analyse_num++;
	}
	return 1;
}

bool proc_analyse(int pre_table,int level)
{
	int now_num = analyse_num;
	now_level++;
	now_level2 = level;
	if (Syn[analyse_num].symbol == "procedure")
	{
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol != "identity")
		{
			if (Syn[analyse_num].identity == "(")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��id��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].identity == "(")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����id����";
				err_num++;
			}
		}
		else {
			check_table = now_table + 1;
			fa_table[now_table + 1] = pre_table;
			enter(Syn[analyse_num].identity, "procedure");
		}
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].identity != "(")
		{
			if (Syn[analyse_num].identity == ")" || Syn[analyse_num].symbol == "identity")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��(��";
				err_num++;
				analyse_num--;
			}
			else if (Syn[analyse_num + 1].identity == ")" || Syn[analyse_num + 1].symbol == "identity")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����(����";
				err_num++;
			}
		}
		now_table++;  //�½���
		Tablelink[now_table].level = level;
		analyse_num++;
		now_num = analyse_num;
		int flag = 0;
		if (Syn[analyse_num].symbol == "identity")
		{
			enter(Syn[analyse_num].identity, "var");
			level_idnum[now_level2]++;
			analyse_num++;
			now_num = analyse_num;
			if (Syn[analyse_num].symbol == "identity") {
				flag = 1;
				enter(Syn[analyse_num].identity, "var");
				level_idnum[now_level2]++;
			}
			while (Syn[analyse_num].identity == "," || flag)
			{
				if (flag)
				{
					err[err_num].line = Syn[analyse_num].line;
					err[err_num].col = Syn[analyse_num].col;
					err[err_num].er = "�﷨����ȱ��,!";
					err_num++;
					analyse_num--;
				}
				flag = 0;
				analyse_num++;
				now_num = analyse_num;
				if (Syn[analyse_num].symbol == "identity")
				{
					enter(Syn[analyse_num].identity, "var");
					level_idnum[now_level2]++;
					analyse_num++;
				}
				else
				{
					err[err_num].line = Syn[analyse_num].line;
					err[err_num].col = Syn[analyse_num].col;
					err[err_num].er = "�﷨����ȱ��id��";
					err_num++;
				}
			}
		}
		if (Syn[analyse_num].identity == ")")
		{
			analyse_num++;
			now_num = analyse_num;
			if (Syn[analyse_num].identity != ";")
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��;��";
				err_num++;
				analyse_num--;
			}
			analyse_num++;
			now_num = analyse_num;
			block_analyse(now_table,level);
			now_level--;  //�����block����������
			analyse_num++;
			while (Syn[analyse_num].identity == ";")
			{
				analyse_num++;
				now_num = analyse_num;
				proc_analyse(pre_table, level);
				analyse_num++;
				now_num = analyse_num;
			}
			analyse_num--;
			return 1;
		}
	}
	while (Syn[analyse_num + 1].identity != ";" && Syn[analyse_num + 1].symbol != "else" && Syn[analyse_num + 1].symbol != "begin")
	{
		analyse_num++;
	}
	return 1;
}

bool vardecl_analyse()
{
	int now_num = analyse_num;
	int flag = 0;
	if (Syn[analyse_num].symbol == "var")
	{
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol == "identity")
		{
			enter(Syn[analyse_num].identity, "var");
			level_idnum[now_level2]++;
			analyse_num++;
			now_num = analyse_num;
			while (Syn[analyse_num].identity == ",")
			{
				if (Syn[analyse_num].identity == ";")
					break;
				analyse_num++;
				now_num = analyse_num;
				if (Syn[analyse_num].symbol == "identity")
				{
					enter(Syn[analyse_num].identity, "var");
					level_idnum[now_level2]++;
					analyse_num++;
					now_num = analyse_num;
				}
				else
				{
					err[err_num].line = Syn[now_num].line;
					err[err_num].col = Syn[now_num].col;
					err[err_num].er = "�﷨����ȱ��id��";
					err_num++;
				}
			}
			if (Syn[analyse_num].identity == ";")
			{
				return 1;
			}
			else
			{
				err[err_num].line = Syn[now_num].line;
				err[err_num].col = Syn[now_num].col;
				err[err_num].er = "�﷨����ȱ��;��";
				err_num++;
				analyse_num--;
			}
		}
		else
		{
			err[err_num].line = Syn[now_num].line;
			err[err_num].col = Syn[now_num].col;
			err[err_num].er = "�﷨����ȱ��id��";
			err_num++;
		}
	}
	while (Syn[analyse_num + 1].symbol != "procedure" && Syn[analyse_num + 1].symbol != "begin")
	{
		analyse_num++;
	}
	return 1;
}

bool condecl_analyse()
{
	int now_num = analyse_num;
	int flag = 0;
	if (Syn[analyse_num].symbol == "const")
	{
		analyse_num++;
		now_num = analyse_num;
		const_analyse();
		analyse_num++;
		now_num = analyse_num;
		if (Syn[analyse_num].symbol == "identity")
			flag = 1;
		while (Syn[analyse_num].identity == "," || flag)
		{
			if (flag)
			{
				err[err_num].line = Syn[analyse_num].line;
				err[err_num].col = Syn[analyse_num].col;
				err[err_num].er = "�﷨����ȱ��,��";
				err_num++;
				analyse_num--;
			}
			flag = 0;
			analyse_num++;
			now_num = analyse_num;
			const_analyse();
			analyse_num++;
			now_num = analyse_num;
			if (Syn[analyse_num].symbol == "identity")
				flag = 1;
		}
		if (Syn[analyse_num].identity == ";")
		{
			return 1;
		}
		else
		{
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "�﷨����ȱ��;��";
			err_num++;
			analyse_num--;
		}
	}
	else
	{
		err[err_num].line = Syn[analyse_num].line;
		err[err_num].col = Syn[analyse_num].col;
		err[err_num].er = "�﷨����ȱ��const��";
		err_num++;
		analyse_num--;
	}
	while (Syn[analyse_num + 1].symbol != "var" && Syn[analyse_num + 1].symbol != "procedure" && Syn[analyse_num + 1].symbol != "begin")
	{
		analyse_num++;
	}
	return 1;
}

bool block_analyse(int pre_table,int level)
{
	int now_num = analyse_num;
	if (Syn[analyse_num].identity == "const")
	{
		condecl_analyse();
		analyse_num++;
	}
	now_num = analyse_num;
	if (Syn[analyse_num].identity == "var")
	{
		vardecl_analyse();
		analyse_num++;
	}
	now_num = analyse_num;
	if (Syn[analyse_num].identity == "procedure")
	{
		proc_analyse(now_table,level+1);
		analyse_num++;
	}
	body_analyse();
	check_table = fa_table[pre_table];
	return 1;
}

bool prog_analyse()
{
	int now_num = analyse_num;
	if (Syn[analyse_num].symbol != "program")
	{
		if (Syn[analyse_num + 1].symbol == "identity")
		{
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "�﷨����program����";
			err_num++;
		}
		else
		{
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "�﷨����programȱʧ��";
			analyse_num--;
			err_num++;
		}
	}
	analyse_num++;
	now_num = analyse_num;
	if (Syn[analyse_num].symbol != "identity")
	{
		if (Syn[analyse_num].identity == ";")
		{
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "�﷨����ȱ��id��";
			err_num++;
			analyse_num--;
		}
		else if (Syn[analyse_num + 1].identity == ";")
		{
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "�﷨����id����";
			err_num++;
		}
	}
	analyse_num++;
	now_num = analyse_num;
	if (Syn[analyse_num].identity != ";")
	{
		if (Syn[analyse_num].identity == "const" || Syn[analyse_num].identity == "var" || Syn[analyse_num].identity == "procedure" || Syn[analyse_num].identity == "begin")
		{
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "�﷨����ȱ��;��";
			err_num++;
			analyse_num--;
		}
		else if (Syn[analyse_num + 1].identity == "const" || Syn[analyse_num + 1].identity == "var" || Syn[analyse_num + 1].identity == "procedure" || Syn[analyse_num + 1].identity == "begin")
		{
			err[err_num].line = Syn[analyse_num].line;
			err[err_num].col = Syn[analyse_num].col;
			err[err_num].er = "�﷨����;����";
			err_num++;
		}
	}
	analyse_num++;
	now_num = analyse_num;
	int temp = block_analyse(now_table,0);
	return 1;
}

void Syn_analyse()
{

}

int main()
{
	word();
	for (int i = 0; i < num; i++)
	{
		cout << Syn[i].line << " " << Syn[i].col << " " << Syn[i].identity << "--" << Syn[i].symbol << endl;
	}
	prog_analyse();
	if (err_num)
	{
		for (int i = 0; i < err_num; i++)
		{
			cout << err[i].line << "��" << err[i].col << "��" << err[i].er << endl;
		}
	}
	else
		cout << "����ɹ�" << endl;
	return 0;
}

