#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define	norw 11            //norw-1���ؼ���
#define	al 20            //��Ĺؼ��ֵĳ���

#define ID norw//11
#define INT norw+1//12
#define COMMA norw+2//13
#define ENDF norw+3//14
#define COLON norw+4//15
#define SEMIC norw+5//16
#define ADD norw+6//17
#define MINUS norw+7//18
#define MULTI norw+8//19
#define EVALU norw+9//20
#define LE norw+10//21
#define NE norw+11//22
#define LT norw+12//23
#define EQ norw+13//24
#define GE norw+14//25
#define GT norw+15//26
#define DIVISION norw+16//27
#define DECIMAL norw+17//28
#define xhx norw+18//29

char TOKEN[20];                      //�ַ������������δ�ŵ��ʴ��ĵĸ����ַ�

extern int lookup(char *);           //��TOKEN�ַ����鱣���ֱ�
extern void report_error(char);      //��������еĴʷ�����
bool isalpha(char);                  //�жϽ����ַ��Ƿ�Ϊ��ĸ
bool isalnum(char);                  //�жϽ����ַ��Ƿ�Ϊ��ĸ��������
bool isdigit(char);                  //�жϽ����ַ��Ƿ�Ϊ����

FILE* fin;
FILE* fout;

void scanner()
{//�ʷ�������������򣬶�������ı��ļ����дʷ�����
	char ch;
	int i,c;
	int error=0;                         //��¼�ļ��дʷ�����ĸ���
	int decimal=0;               //��������ʵ��������
	ch=fgetc(fin);                        //�������ļ��ж�ȡһ���ַ�

	while(ch!=EOF)
	{//���������ļ����յ��ַ������ļ�������ʱ��ִ��ѭ��
		decimal=0;
		if(isalpha(ch))
		{//����������ļ����յĵ�һ���ַ�����ĸ
			TOKEN[0]=ch;
			ch=fgetc(fin);i=1;
			while(isalnum(ch))
			{
				TOKEN[i]=ch;i++;
				ch=fgetc(fin);
			}
			TOKEN[i]='\0';
			c=lookup(TOKEN);                   //�鱣���ֱ�
			if(c==0) {fprintf(fout,"(%d,%s)\n", ID,TOKEN);} //�����ʶ��
			else fprintf(fout,"(%d,%s)\n", c,TOKEN);                //������յ���Ϊ������
		}
		if(isdigit(ch))                     //����������ļ����յĵ�һ���ַ�������
		{
			TOKEN[0]=ch;
			ch=fgetc(fin);i=1;
			while(isdigit(ch)||ch=='.')
			{//�ӵڶ��������ַ���ʼ����������ʱ��ִ��ѭ��
				TOKEN[i]=ch;
				if(TOKEN[i]=='.')
				{
				    decimal++;     //����С����ʱ��decimal��һ
				}
				i++;
				ch=fgetc(fin);                   //�ظ������ַ���ֱ�����յ�������
			}
			if(isalpha(ch))
			{
				while(isalpha(ch))
				{
					TOKEN[i]=ch;i++;
					ch=fgetc(fin);
				}
				TOKEN[i]='\0';
				error++;
				printf("%s is error\n", TOKEN);
			}
			else
			{
				fseek(fin,-1,1);
			    TOKEN[i]='\0';
			    if(decimal==1)
                {
                    fprintf(fout,"(%d,%s)\n", DECIMAL, TOKEN);                      //������յ���ΪС��(ʵ��)
                }
		     	else if(decimal==0)
                {
                    fprintf(fout,"(%d,%s)\n", INT, TOKEN);                      //������յ���Ϊ����
                }
                else
                {
                    error++;
                    printf("%s is error\n", TOKEN);
                }
			}
		}
		else            //����������ļ����յĵ�һ���ַ��Ȳ�����ĸ�ֲ�������
		switch(ch)
		{//�������յ��ķ����ַ����з��࣬��ȡһ��һ��
			case':':ch=fgetc(fin);
		        if(ch=='=') fprintf(fout,"(%d,:=)\n", EVALU);           //������շ���Ϊ��ֵ��
	 			else
				{
					fseek(fin,-1,1);                        //�ļ������ַ�����һ���ַ�
					fprintf(fout,"(%d,':')\n", COLON);                     //���ð��
				}
				break;
			case',':fprintf(fout,"(%d,',')\n", COMMA); break;                   //�������
			case'.':fprintf(fout,"(%d,'.')\n", ENDF);break;                    //������
			case';':fprintf(fout,"(%d,';')\n", SEMIC);break;                   //����ֺ�
			case'+':fprintf(fout,"(%d,'+')\n", ADD);break;                     //����Ӻ�
			case'-':fprintf(fout,"(%d,'-')\n", MINUS);break;                   //�������
			case'*':fprintf(fout,"(%d,'*')\n", MULTI);break;                   //����˺�
			case'<':ch=fgetc(fin);
				if(ch=='=')fprintf(fout,"(%d,'<=')\n", LE);             //���С�ڻ���ں�
				else if(ch=='>')fprintf(fout,"(%d,'<>')\n", NE);        //��������ں�
						else
						{
						fseek(fin,-1,1);
						fprintf(fout,"(%d,'<')\n", LT);;                 //���С�ں�
						}
			break;
			case'=':fprintf(fout,"(%d,'=')\n", EQ);break;                      //������ں�
			case'>':ch=fgetc(fin);
				if(ch=='=')fprintf(fout,"(%d,'>=')\n", GE);             //������ڻ���ں�
					else
					{
					fseek(fin,-1,1);
					fprintf(fout,"(%d,'>')\n", GT);                     //������ں�
					}
				break;
			case' ':break;
			case'\n':break;
			case'\t':break;
			case'/':ch=fgetc(fin);
				if(ch=='/')
				{
					while(ch!='\n'&&ch!=EOF)
						ch=fgetc(fin);
					break;
				}
				else
				{
					fseek(fin,-1,1);
					fprintf(fout,"(%d,'/')\n", DIVISION);break;
				}
			case'{':ch=fgetc(fin);
				while(ch!='}'&&ch!=EOF)
					ch=fgetc(fin);
				break;
			case'}':
				error++;
				printf("%c is error\n", ch);
				break;
            case'_':
                fprintf(fout,"(%d,'_')\n", xhx);break;                   //����»���`
	        default:printf("%c is error\n", ch);             //���շ������ַ����򱨸�ʷ�����
			error++;break;
		}
		ch=fgetc(fin);                                          //�������ļ��ж�ȡ��һ�����ʣ�ֱ���ļ�����
	}//whileѭ������

	printf("������%d ���ʷ�����!",error);
	return;
}

int lookup(char *token)
{
	int j;
    char word[norw][al];
	strcpy(&(word[1][0]), "begin");
	strcpy(&(word[2][0]), "end");
	strcpy(&(word[3][0]), "var");
	strcpy(&(word[4][0]), "integer");
	strcpy(&(word[5][0]), "while");
	strcpy(&(word[6][0]), "do");
	strcpy(&(word[7][0]), "if");
	strcpy(&(word[8][0]), "then");
	strcpy(&(word[9][0]), "procedure");
	strcpy(&(word[10][0]), "else");

	for(j=1;j<=norw-1;j++)if(strcasecmp(token,word[j])==0) return j;   //��TOKEN�ַ����鱣���ֱ����鵽���ر����������
	return 0;                                              //TOKEN���Ǳ����֣�����0
}



bool isalpha(char c)
{ //�жϽ����ַ��Ƿ�Ϊ��ĸ
if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))return 1;
   else return 0;

}
bool isalnum(char c)
{//�жϽ����ַ��Ƿ�Ϊ��ĸ��������
if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9'))return 1;
   else return 0;
}

bool isdigit(char c)
{//�жϽ����ַ��Ƿ�Ϊ����
if(c>='0'&&c<='9')return 1;
   else return 0;
}

int main()
{
char filename[20];
printf("�������ļ���:");
scanf("%s",filename);
if((fin=fopen(filename,"r"))==NULL) //��Ҫ��ȡ���ı��ļ�
{
	printf("���ܴ��ļ�.\n");
	exit(0);
}

printf("�����뱣�����������ļ���:");
scanf("%s",filename);
if((fout=fopen(filename,"w"))==NULL)
	{
		printf("���ܴ��ļ�.\n");
		exit(0);
	}

scanner();    //���ôʷ���������
getchar();getchar();

fclose(fin);
fclose(fout);
return 0;
}
