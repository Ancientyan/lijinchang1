#include<iostream>
#include<string.h>
#include<string>
#include<fstream>
using namespace std;
char *reserved_word[5] = { "if","else","while","do","break" };//保留字
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
*/    //数字代表的内容

class Lexicalanalyzer  //语法分析封装成类
{
private:
	int terminator[1000]; //按照内容存储对应的数字
	int terminatornum;    //词法分析过程对数组计数
	int i;                //语法分析过程计数
public:
	Lexicalanalyzer()
	{
		terminatornum = 0;
		i = 0;
	}
	void word();
	void program();//开始
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
void Lexicalanalyzer::word()//词法分析
{
	string filename;
	cout<<"请输入源代码文件路径: "<<endl;
	cin>>filename;
	cout<<endl;
	ifstream f(filename,ios::in);//打开文件
	char *code=new char[1000];
	f.getline(code, 1000, EOF);//代码存入动态数组中
	f.close();//关闭文件
	cout<<"词法分析如下：(左边为数字代号，右边为单词)"<<endl;
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
			default:cout << "词法分析出错,非法字符:" << *code << endl;_error();
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
	cout<<"语法分析如下："<<endl;
	cout<<"program -> block"<<endl;
	block();
	if (i == terminatornum)
		cout<<"语法分析完成"<<endl;
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
		if (terminator[i] == 14)i++;   //当前单词为'='则取下一个单词
		else { _error(); }
		expr();
		if (terminator[i] == 17)  //当前单词为';'
		{
				i++;
				cout<<"遇到符号 ';'"<<endl<<endl;
		}
		else { _error(); }
		break;
	case 2:cout<<"stmt -> if ( bool_ ) stmt stmt1"<<endl;
		i++;
		if (terminator[i] == 15)i++;  //当前单词为'(' 则取下个单词
		else { _error(); }
		bool_();
		if (terminator[i] == 16)i++;//当前单词为')' 则取下个单词
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
	case 5:cout<<"stmt -> do stmt while ( bool_ );"<<endl; //若当前单词为"do"取下个单词并调用stmt()
		i++;
		stmt();
		if (terminator[i] == 4)i++;  //当前单词若为"while"则取下个单词
		else { _error(); }
		if (terminator[i] == 15)i++; //若当前单词为'('则取下个单词并调用bool_()
		else { _error(); }   //否则出错处理
		bool_();  
		if (terminator[i] == 16)i++;
		else { _error(); }
		if (terminator[i] == 17)
		{
				i++;
				cout<<"遇到符号 ';'"<<endl<<endl;
		}
		else { _error(); }
		break;
	case 6:cout<<"stmt -> break"<<endl; //当前单词为"break"则取下个单词，若所取单词不为';'则转到错误处理
		i++;
		if (terminator[i] == 17)
		{
			i++;
			cout<<"遇到符号 ';'"<<endl<<endl;
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
	if (terminator[i] == 3)  //若当前单词为"else"则取下个单词并调用stmt()
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
	if (terminator[i] == 12)//若当前单词为"<"则取下个单词
	{
		i++;
		if (terminator[i] == 14)//若当前单词为"="则取下个单词并调用expr()
		{
			i++;
			cout<<"bool1 -> expr <= expr"<<endl;
			expr();
		}
		else //否则直接调用expr()
		{ 
			cout<<"bool1 -> expr < expr"<<endl;
			expr(); 
		}
	}
	else if (terminator[i] == 13)//若当前单词不为"<"而是">"时,取下个单词并进行处理
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
	if (terminator[i] == 8)//若当前单词为'+'号则取下个单词并调用term()和expr1()
	{
		cout<<"expr1 -> + term expr1"<<endl;
		i++;
		term();
		expr1();
	}
	else if (terminator[i] == 9)//若为'-'则取下个单词并调用term()和expr1()
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
	if (terminator[i] == 10)//若为'*'则取下个单词并调用factor()和term1()
	{
		cout<<"term1 -> * factor term1"<<endl;
		i++;
		factor();
		term1();
	}
	else if (terminator[i] == 11)//若为'/'则取下个单词并调用factor()和term1()
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
	if (terminator[i] == 15) //为'('时
	{
		cout<<"factor -> ( expr )"<<endl;
		i++;
		expr();
		if (terminator[i] == 16)i++; 
		else _error();
	}
	else if (terminator[i] == 1)//为标识符时
	{
		cout<<"factor -> id"<<endl;
		i++;
		return;
	}
	else if (terminator[i] == 7)//为数字时
	{
		cout<<"factor -> num"<<endl;
		i++;
		return;
	}
	else _error();//否则出错处理
}
void Lexicalanalyzer::_error()
{
	cout<<"出现错误，中止分析"<<endl;
	exit(1);
}
int main()
{
	Lexicalanalyzer lex;
	lex.word();
	lex.program();//由program()开始
	return 0;
}