#include<iostream>
#include<string.h>
#include<string>
#include<fstream>
using namespace std;
char *reserved_word[5] = { "if","else","while","do","break" };//������
/*
1 word
2 if
3 else
4 while
5 do
6 break
7 number
8 +
9 -
10 *
11 /
12 <
13 >
14 =
15 (
16 )
17 ;
18 {
19 }
*/    //���ִ��������

class Lexicalanalyzer  //�﷨������װ����
{
private:
	int terminator[1000]; //�������ݴ洢��Ӧ������
	int terminatornum;    //�ʷ��������̶��������
	int i;                //�﷨�������̼���
public:
	Lexicalanalyzer()
	{
		terminatornum = 0;
		i = 0;
	}
	void word();
	void program();//��ʼ
	void block();
	void stmts();
	void stmt();
	void stmt1();
	void bool_();
	void expr();
	void expr1();
	void term();
	void term1();
	void factor();
	void _error();
};
void Lexicalanalyzer::word()//�ʷ�����
{
	string filename;
	cout<<"������Դ�����ļ�·��: "<<endl;
	cin>>filename;
	cout<<endl;
	ifstream f(filename,ios::in);//���ļ�
	char *code=new char[1000];
	f.getline(code, 1000, EOF);//������붯̬������
	f.close();//�ر��ļ�
	cout<<"�ʷ��������£�(���Ϊ���ִ��ţ��ұ�Ϊ����)"<<endl;
	int sym;
	while (*code != '\0')
	{
		if (*code >= 'a'&&*code <= 'z' || *code >= 'A'&&*code <= 'Z')
		{

			char temp[100];
			char*t = temp;
			while (*code >= 'a'&&*code <= 'z' || *code >= 'A'&&*code <= 'Z')
			{
				*t = *code;
				t++;
				code++;
			}
			sym = 1;
			*t = '\0';
			t = temp;
			for (int i = 0; i < 5; i++)
			{
				if (strcmp(t, reserved_word[i]) == 0)
				{
					sym = 2 + i;
					break;
				}
			}
			cout << '(' << sym << " , " << t << ")" << endl;
			terminator[terminatornum] = sym;
			terminatornum++;
		}
		else if (*code >= '0'&&*code <= '9')
		{
			char temp[100];
			char*t = temp;
			while (*code >= '0'&&*code <= '9')
			{
				*t = *code;
				t++;
				code++;
			}
			sym = 7;
			*t = '\0';
			t = temp;
			cout << '(' << sym << " , " << t << ")" << endl;
			terminator[terminatornum] = sym;
			terminatornum++;
		}
		else if (*code == '/')
		{
			code++;
			if (*code == '/')
			{
				while (*code != '\n')
				{
					code++;
				}
				code++;
			}
			else if (*code == '*')
			{
				code++;
				while (1)
				{
					char front = *code;
					code++;
					char back = *code;
					if (front == '*'&&back == '/')
					{
						code++; break;
					}
				}
			}
			else
			{
				sym = 11;
				cout << '(' << sym << " , /)" << endl;
				terminator[terminatornum] = sym;
				terminatornum++;
			}
		}

		else
		{
			int check = 0;
			switch (*code)
			{
			case'+':sym = 8; check = 1; break;
			case'-':sym = 9; check = 1; break;
			case'*':sym = 10; check = 1; break;
			case'<':sym = 12; check = 1; break;
			case'>':sym = 13; check = 1; break;
			case'=':sym = 14; check = 1; break;
			case'(':sym = 15; check = 1; break;
			case')':sym = 16; check = 1; break;
			case';':sym = 17; check = 1; break;
			case'{':sym = 18; check = 1; break;
			case'}':sym = 19; check = 1; break;
			case' ':break;
			case'\t':break;
			case'\n':break;
			default:cout << "�ʷ���������,�Ƿ��ַ�:" << *code << endl;_error();
			}
			if (check == 1)
			{
				cout << '(' << sym << " , " << *code << ")" << endl;
				terminator[terminatornum] = sym;
				terminatornum++;
			}
			code++;
		}
	}
	cout << endl;
}//

void Lexicalanalyzer::program()
{
	cout<<"�﷨�������£�"<<endl;
	cout<<"program -> block"<<endl;
	block();
	if (i == terminatornum)
		cout<<"�﷨�������"<<endl;
}
void Lexicalanalyzer::block()
{
	cout<<"block -> { stmts }"<<endl;
	if (terminator[i] == 18) i++;
	else  
		_error();
	stmts();
	if (terminator[i] == 19)i++;
	else { _error(); }
}
void Lexicalanalyzer::stmts()
{
	if (terminator[i] == 19)
	{
		cout<<"stmts -> null"<<endl;
		return;
	}
	cout<<"stmts -> stmt stmts"<<endl;
	stmt();
	stmts();
}
void Lexicalanalyzer::stmt()
{
	switch (terminator[i])
	{
	case 1:cout<<"stmt -> id = expr"<<endl;
		i++;
		if (terminator[i] == 14)i++;   //��ǰ����Ϊ'='��ȡ��һ������
		else { _error(); }
		expr();
		if (terminator[i] == 17)  //��ǰ����Ϊ';'
		{
				i++;
				cout<<"�������� ';'"<<endl<<endl;
		}
		else { _error(); }
		break;
	case 2:cout<<"stmt -> if ( bool_ ) stmt stmt1"<<endl;
		i++;
		if (terminator[i] == 15)i++;  //��ǰ����Ϊ'(' ��ȡ�¸�����
		else { _error(); }
		bool_();
		if (terminator[i] == 16)i++;//��ǰ����Ϊ')' ��ȡ�¸�����
		else { _error(); }
		stmt();
		stmt1();
		break;
	case 4:cout<<"stmt -> while ( bool_ ) stmt"<<endl;
		i++;
		if (terminator[i] == 15)i++;
		else { _error(); }
		bool_();
		if (terminator[i] == 16)i++;
		else { _error(); }
		stmt();
		break;
	case 5:cout<<"stmt -> do stmt while ( bool_ );"<<endl; //����ǰ����Ϊ"do"ȡ�¸����ʲ�����stmt()
		i++;
		stmt();
		if (terminator[i] == 4)i++;  //��ǰ������Ϊ"while"��ȡ�¸�����
		else { _error(); }
		if (terminator[i] == 15)i++; //����ǰ����Ϊ'('��ȡ�¸����ʲ�����bool_()
		else { _error(); }   //���������
		bool_();  
		if (terminator[i] == 16)i++;
		else { _error(); }
		if (terminator[i] == 17)
		{
				i++;
				cout<<"�������� ';'"<<endl<<endl;
		}
		else { _error(); }
		break;
	case 6:cout<<"stmt -> break"<<endl; //��ǰ����Ϊ"break"��ȡ�¸����ʣ�����ȡ���ʲ�Ϊ';'��ת��������
		i++;
		if (terminator[i] == 17)
		{
			i++;
			cout<<"�������� ';'"<<endl<<endl;
		}
		else { _error(); }
		break;
	default:cout<<"stmt -> block"<<endl;
		block();
		break;
	}
}
void Lexicalanalyzer::stmt1()
{
	if (terminator[i] == 3)  //����ǰ����Ϊ"else"��ȡ�¸����ʲ�����stmt()
	{
		cout<<"stmt1 -> else stmt"<<endl;
		i++;
		stmt();
	}
	else
		cout<<"stmt1 -> null"<<endl;
}

void Lexicalanalyzer::bool_()
{
	cout<<"bool_ -> expr bool1"<<endl; 
	expr();
	if (terminator[i] == 12)//����ǰ����Ϊ"<"��ȡ�¸�����
	{
		i++;
		if (terminator[i] == 14)//����ǰ����Ϊ"="��ȡ�¸����ʲ�����expr()
		{
			i++;
			cout<<"bool1 -> expr <= expr"<<endl;
			expr();
		}
		else //����ֱ�ӵ���expr()
		{ 
			cout<<"bool1 -> expr < expr"<<endl;
			expr(); 
		}
	}
	else if (terminator[i] == 13)//����ǰ���ʲ�Ϊ"<"����">"ʱ,ȡ�¸����ʲ����д���
	{
		i++;
		if (terminator[i] == 14)
		{
			i++;
			cout<<"bool1 -> expr >= expr"<<endl;
			expr();
		}
		else 
		{ 
			cout<<"bool1 -> expr > expr"<<endl;
			expr(); 
		}
	}
	else cout<<"bool1 -> null"<<endl;
}
void Lexicalanalyzer::expr()
{
	cout<<"expr -> term expr1"<<endl;
	term();
	expr1();
}
void Lexicalanalyzer::expr1()
{
	if (terminator[i] == 8)//����ǰ����Ϊ'+'����ȡ�¸����ʲ�����term()��expr1()
	{
		cout<<"expr1 -> + term expr1"<<endl;
		i++;
		term();
		expr1();
	}
	else if (terminator[i] == 9)//��Ϊ'-'��ȡ�¸����ʲ�����term()��expr1()
	{
		cout<<"expr1 -> - term expr1"<<endl;
		i++;
		term();
		expr1();
	}
	else cout<<"expr -> null"<<endl;
}
void Lexicalanalyzer::term()
{
	cout<<"term -> factor term1"<<endl;
	factor();
	term1();
}
void Lexicalanalyzer::term1()
{
	if (terminator[i] == 10)//��Ϊ'*'��ȡ�¸����ʲ�����factor()��term1()
	{
		cout<<"term1 -> * factor term1"<<endl;
		i++;
		factor();
		term1();
	}
	else if (terminator[i] == 11)//��Ϊ'/'��ȡ�¸����ʲ�����factor()��term1()
	{
		cout<<"term1 -> / factor term1"<<endl;
		i++;
		factor();
		term1();
	}
	else cout<<"term1 -> null"<<endl;
}
void Lexicalanalyzer::factor()
{
	if (terminator[i] == 15) //Ϊ'('ʱ
	{
		cout<<"factor -> ( expr )"<<endl;
		i++;
		expr();
		if (terminator[i] == 16)i++; 
		else _error();
	}
	else if (terminator[i] == 1)//Ϊ��ʶ��ʱ
	{
		cout<<"factor -> id"<<endl;
		i++;
		return;
	}
	else if (terminator[i] == 7)//Ϊ����ʱ
	{
		cout<<"factor -> num"<<endl;
		i++;
		return;
	}
	else _error();//���������
}
void Lexicalanalyzer::_error()
{
	cout<<"���ִ�����ֹ����"<<endl;
	exit(1);
}
int main()
{
	Lexicalanalyzer lex;
	lex.word();
	lex.program();//��program()��ʼ
	return 0;
}