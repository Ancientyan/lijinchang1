#include<iostream>
#include<fstream>
using namespace std;
char *reserved_word[8]={"if","then","else",
	"end","repeat","until","read","write"};
char token[20];// �����ݴ��ַ�
int n;//token�����±�
char filename[30];//�����ļ���
char code[1000];//�洢����
void GetToken();
int main()
{
	cout<<"������Դ�����ļ�·��: "<<endl;
	cin>>filename;
	cout<<endl;
	ifstream f(filename,ios::in);//���ļ�
	char ch;
	int i=0;
	while(f.get(ch))//���ļ����ַ���һ��������
	{
		code[i]=ch;
		i++;
	}
	int j=1;
	cout<<"�������£�"<<endl<<endl;
	cout<<j<<"    ";
	for(int k=0;k<i;k++)//���Դ����
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
	cout<<endl<<"�ʷ��������£�"<<endl;
	GetToken();
	cout<<endl;
	return 0;
}

void clear_token()//��token�������
{
	n=0;
	for(int y=0;y<20;y++)
		token[y]='\0';
}
void GetToken()
{
	int t=1;//����к�
	for(int i=0;code[i]!='\0';i++)//�����ַ���
	{
		clear_token();
		if(code[i]=='\n'||code[i]=='\t'||code[i]==' ')
		{
			if(code[i]=='\n')//�������������кż�һ
				t++;
			continue;
		}
		if((code[i]>='a'&&code[i]<='z')||(code[i]>='A'&&code[i]<='Z'))//��ʶ��
		{
			do
			{
				token[n++]=code[i];//�����ݴ浽token����
				i++;
				while(code[i]=='\n'||code[i]=='\t'||code[i]==' ')
				{
					if(code[i]=='\n') //�������������кż�һ
						t++;
					i++;
				}
			}while((code[i]>='a'&&code[i]<='z')||(code[i]>='A'&&code[i]<='Z'));
			
			bool a=false; //�����жϸı�ʶ���Ƿ�Ϊ������
			
			for(int j=0;j<8;j++)
			{
				if(strcmp(token,reserved_word[j])==0)//��Ϊ�����������������
				{
					cout<<"�����֣�"<<reserved_word[j]<<endl;
					break;
				}
				if(j==7) a=true;
			}
			if(a==true)//����ʶ�����Ǳ������������ʶ��
			{
				cout<<"��ʶ����"<<token;
				cout<<endl;
				a=false;
			}
			i--;//�����ϸ��ַ�
		}
		else if(code[i]>='0'&&code[i]<='9')//����
		{
			do
			{
				token[n++]=code[i];
				i++;
				
				while(code[i]=='\n'||code[i]=='\t'||code[i]==' ') 
				{
					if(code[i]=='\n')//�������������кż�һ
						++t;
					i++;
				}
			}while(code[i]>='0'&&code[i]<='9');
			cout<<"���֣�"<<token;
			cout<<endl;
			i--;
		}
		else if(code[i]=='{')//ע��
		{
			do
			{
				if(code[i]=='\n') //�������������кż�һ
					t++;
				i++;
			}while(code[i]!='}');
		}
		else
		{
			switch(code[i])
			{
			    case'+':cout<<"������ţ�"<<code[i]<<endl;break;
		            case'-':cout<<"������ţ�"<<code[i]<<endl;break;
			    case'*':cout<<"������ţ�"<<code[i]<<endl;break;
			    case'/':cout<<"������ţ�"<<code[i]<<endl;break;
			    case'=':cout<<"������ţ�"<<code[i]<<endl;break;
			    case'<':cout<<"������ţ�"<<code[i]<<endl;break;
			    case';':cout<<"������ţ�"<<code[i]<<endl;break;
			    default:cout<<"��"<<t<<"�г��ִ���"<<"��"<<code[i]<<"��"<<endl;break;//���������Ϣ
			}
		}
	}
}//��ע��