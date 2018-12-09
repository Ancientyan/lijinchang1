#include<iostream>
#include<fstream>
using namespace std;
char *reserved_word[8]={"if","then","else",
	"end","repeat","until","read","write"};
char token[20];// 用于暂存字符
int n;//token数组下标
char filename[30];//保存文件名
char code[1000];//存储代码
void GetToken();
int main()
{
	cout<<"请输入源代码文件路径: "<<endl;
	cin>>filename;
	cout<<endl;
	ifstream f(filename,ios::in);//打开文件
	char ch;
	int i=0;
	while(f.get(ch))//将文件的字符逐一送入数组
	{
		code[i]=ch;
		i++;
	}
	int j=1;
	cout<<"代码如下："<<endl<<endl;
	cout<<j<<"    ";
	for(int k=0;k<i;k++)//输出源代码
	{
		if(code[k]!='\n')
		{
			cout<<code[k];
		}
		else 
		{
			cout<<endl;
			cout<<++j<<"    ";
		}
	}
	cout<<endl;
	cout<<endl<<"词法分析如下："<<endl;
	GetToken();
	cout<<endl;
	return 0;
}

void clear_token()//将token数组清空
{
	n=0;
	for(int y=0;y<20;y++)
		token[y]='\0';
}
void GetToken()
{
	int t=1;//标记行号
	for(int i=0;code[i]!='\0';i++)//遍历字符串
	{
		clear_token();
		if(code[i]=='\n'||code[i]=='\t'||code[i]==' ')
		{
			if(code[i]=='\n')//若遇到换行则行号加一
				t++;
			continue;
		}
		if((code[i]>='a'&&code[i]<='z')||(code[i]>='A'&&code[i]<='Z'))//标识符
		{
			do
			{
				token[n++]=code[i];//符号暂存到token数组
				i++;
				while(code[i]=='\n'||code[i]=='\t'||code[i]==' ')
				{
					if(code[i]=='\n') //若遇到换行则行号加一
						t++;
					i++;
				}
			}while((code[i]>='a'&&code[i]<='z')||(code[i]>='A'&&code[i]<='Z'));
			
			bool a=false; //辅助判断改标识符是否为保留字
			
			for(int j=0;j<8;j++)
			{
				if(strcmp(token,reserved_word[j])==0)//若为保留字则输出保留字
				{
					cout<<"保留字："<<reserved_word[j]<<endl;
					break;
				}
				if(j==7) a=true;
			}
			if(a==true)//若标识符不是保留字则输出标识符
			{
				cout<<"标识符："<<token;
				cout<<endl;
				a=false;
			}
			i--;//返回上个字符
		}
		else if(code[i]>='0'&&code[i]<='9')//数字
		{
			do
			{
				token[n++]=code[i];
				i++;
				
				while(code[i]=='\n'||code[i]=='\t'||code[i]==' ') 
				{
					if(code[i]=='\n')//若遇到换行则行号加一
						++t;
					i++;
				}
			}while(code[i]>='0'&&code[i]<='9');
			cout<<"数字："<<token;
			cout<<endl;
			i--;
		}
		else if(code[i]=='{')//注释
		{
			do
			{
				if(code[i]=='\n') //若遇到换行则行号加一
					t++;
				i++;
			}while(code[i]!='}');
		}
		else
		{
			switch(code[i])
			{
			    case'+':cout<<"特殊符号："<<code[i]<<endl;break;
		            case'-':cout<<"特殊符号："<<code[i]<<endl;break;
			    case'*':cout<<"特殊符号："<<code[i]<<endl;break;
			    case'/':cout<<"特殊符号："<<code[i]<<endl;break;
			    case'=':cout<<"特殊符号："<<code[i]<<endl;break;
			    case'<':cout<<"特殊符号："<<code[i]<<endl;break;
			    case';':cout<<"特殊符号："<<code[i]<<endl;break;
			    default:cout<<"第"<<t<<"行出现错误："<<"‘"<<code[i]<<"’"<<endl;break;//输出错误信息
			}
		}
	}
}//加注释