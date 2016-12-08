#include "stdio.h"
#include "string.h"
#include "conio.h"
#include "malloc.h"//mallocͷ�ļ�����
#include "stdlib.h"//exit(0)ͷ�ļ�����
char prog[100],token[8],ch;//prog����װ���������ַ���ch���������
char *keyWord[6]={"begin","if","then","while","do","end"};
int syn,p,m,n,sum,q;
int kk;//����͸�ֵΪ1����ʼ��Ϊ0����һ�������ͼ�һ������newtemp()
//һ����ֵ��ʽ�Ľṹ��result1=agll opl ag21
struct{
	char result1[8];
	char agll[8];
	char opl[8];
	char ag21[8];
}quad[20];

char *factor();
char *expression();
int isEnd();
char *term();
int statement();
int lrparser();
char *newTemp();
void scaner();
void emit(char *result,char *ag1,char *op,char *ag2);

main(){
	int j;
	FILE *fp;
	q=p=kk=0;
	//ͨ���ļ�����
	if ((fp=fopen("tokens.txt","r"))==NULL){
		printf("cannot open this file\n");
		exit(0);
	}
	while ((ch=fgetc(fp))!=EOF){
		prog[p++]=ch;//��ch����е�ÿ���ַ���ֵ��prog����
	}
	fclose(fp);
	p=0;
	scaner();
	lrparser();
	if (q>19){
		printf("  too long sentense!\n");
	}else{
		for(j=0;j<q;j++){
			printf("    %s = %s  %s  %s \n\n",quad[j].result1,quad[j].agll,quad[j].opl,quad[j].ag21);
		}
	}
	//system("pause");
	getch();//����ͷ�ļ�conio.h
}
//parser���﷨����
int lrparser(){
	int schain=0;
	kk=0;
	//�Ƿ�begin?
	if (syn==1){
		scaner();
		schain=isEnd();
		//�Ƿ�end?
		if (syn==6){
			scaner();
			//�Ƿ�#��&&��
			//printf("%d",kk);
			if ((syn==0)){//&&(kk==0)?
				printf("Success!\n");
			}
		}else{
			if (kk!=1){
				printf("short of 'end' !\n");
			}
			kk=1;
			getch();
			exit(0);
		}
	}else{
		printf("short of 'begin' !\n");
		kk=1;
		getch();	
		exit(0);	
	}		
	return (schain);
}
//ͨ��statement���������Ƴ��ԷֺŽ�����������䣬Ϊ�˵õ����һ�����ʣ������ж��Ƿ���end
int isEnd(){
	int schain=0;
	schain=statement();
	//�Ƿ� ;
	while(syn==26){
		scaner();
		schain=statement();
	}
	return (schain);
}
//��䴮����
//��ʶ�� 
int statement(){
	char tt[8],eplace[8];
	int schain=0;
	//�Ƿ��Ǳ�ʶ��������ĸ��������ɣ�����token�����У�
	if (syn==10){
		strcpy(tt,token);
		scaner();
		//�Ƿ��ǵȺ�
		if (syn==18){
			scaner();
			strcpy(eplace,expression());
			emit(tt,eplace,"","");
			schain=0;
		}else{
			printf("short of sign ':=' !\n");//����
			kk=1;
			getch();
			exit(0);
		}
		return (schain);
	}
}
//���Ⱥź�ģ����ʽ����=<��>{+<��>|-<��>}
char *expression(){
	char *result,*ep2,*eplace,*ope;
	result=(char *)malloc(12);
	ep2=(char *)malloc(12);
	eplace=(char *)malloc(12);
	ope=(char *)malloc(12);
	strcpy(eplace,term());//�ַ����ĸ��ƣ���term()����ַ������Ƶ�eplace
	//case '+':syn=13;break;case '-':syn=14;break;
	while ((syn==13)||syn==14){
		if (syn==13){
			strcpy(ope,"+");
		}else{
			strcpy(ope,"-");
		}
		scaner();
		strcpy(ep2,term());
		strcpy(result,newTemp());//newtemp��ʱ����
		emit(result,eplace,ope,ep2);
		//eplace=result;?
		strcpy(eplace,result);
	}
	return (eplace);//�õ�����һ��������
}
//�������<��>����=<����>{*<����>|/<����>}
char *term(){
	char *result,*ep2,*eplace,*ope;
	result=(char *)malloc(12);
	ep2=(char *)malloc(12);
	eplace=(char *)malloc(12);
	ope=(char *)malloc(12);
	strcpy(eplace,factor());//�ַ����ĸ��ƣ���term()����ַ������Ƶ�eplace
	//case '*':syn=15;break;case '/':syn=16;break;
	while ((syn==15)||syn==16){
		if (syn==15){
			strcpy(ope,"*");
		}else{
			strcpy(ope,"/");
		}
		scaner();
		strcpy(ep2,factor());
		strcpy(result,newTemp());
		emit(result,eplace,ope,ep2);
		strcpy(eplace,result);
	}
	return (eplace);
}

//���ӣ�<����>����=ID|NUM|(<���ʽ>)
char *factor(){
	char *fplace;
	fplace=(char *)malloc(12);//���䳤��Ϊnum_bytes�ֽڵ��ڴ��
	strcpy(fplace,"");
	if (syn==10){//�Ƿ��ʶ����
		strcpy(fplace,token);
		scaner();
	}else if(syn==11){//�Ƿ���������
		itoa(sum,fplace,10);
		scaner();
	}else if(syn==27){//�Ƿ� '('?
		scaner();
		fplace=expression();
		if (syn==28){//�Ƿ� ')'��
			scaner();
		}else{
			printf("error on ')'  !\n");
			kk=1;
			getch();
			exit(0);
		}
	}else{
		printf("error on factor begin!\n");
		kk=1;
		getch();
		exit(0);
	}
	return (fplace);
}
//�������ʱ����
char *newTemp(){
	char *p;
	char m[8];
	p=(char *)malloc(8);
	kk++;
	//itoa()����һ��������Ҫת�������֣��ڶ���������Ҫд��ת�������Ŀ���ַ�����������������ת������ʱ���õĻ�����ʮ���ơ������Ƶȣ���
	itoa(kk,m,10);//10����ʮ����
	strcpy(p+1,m);
	p[0]='t';
	return (p);//��� t1,t2,......
}
//��ȡ��һ�����ʷ��ţ�ɨ���ж����ַ����Ͳ�����syn�ֱ���
void scaner(){
	sum=0;
	for (m=0;m<8;m++){//token�����ʼ��������8
		token[m++]=NULL;
	}
	m=0;
	ch=prog[p++];//��һ��ɨ���ʱ��pΪ0
	//����ǿո�������ɨ��prog������һ���ַ�
	while(ch==' '){
		ch=prog[p++];
	}
	//�������ĸ
	if (((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A'))){
		//����ѭ���ж���������һ���ַ���ֱ��������ĸ������
		while (((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A'))||((ch>='0')&&(ch<='9'))){
			token[m++]=ch;
			ch=prog[p++];
		}
		p--;//�ж�����һ���ַ��������������Լ���Ϊ���´��ܷ���������ַ�
		syn=10;//��־��
		token[m++]='\0';//�ַ����Ľ�β��־�������봮������Ҫռ�ڴ�ռ䡢'\0'��ASCII��0
		for (n=0;n<6;n++){
			//�ؼ���rwtab={"begin","if","then","while","do","end"};�ֱ�����1-6
			if (strcmp(token,keyWord[n])==0){
				syn=n+1;
				break;
			}
		}
	//���������
	}else if ((ch>='0')&&(ch<='9')){
		while ((ch>='0')&&(ch<='9')){
			sum=sum*10+ch-'0';
			ch=prog[p++];
		}
		p--;
		syn=11;
	//��������
	}else{
		switch (ch){
		case '<':m=0;
			ch=prog[p++];
			if (ch=='>'){
				syn=21;
			}else if (ch=='='){
				syn=22;
			}else{
				syn=20;
				p--;
			}
			break;
		case '>':m=0;
			ch=prog[p++];
			if (ch=='='){
				syn=24;
			}else{
				syn=23;
				p--;
			}
			break;
		case ':':m=0;
			ch=prog[p++];
			if (ch=='='){
				syn=18;
			}else{
				syn=17;
				p--;
			}
			break;
		case '+':syn=13;break;
		case '-':syn=14;break;
		case '*':syn=15;break;
		case '/':syn=16;break;
		case '(':syn=27;break;
		case ')':syn=28;break;
		case '=':syn=25;break;
		case ';':syn=26;break;
		case '#':syn=0;break;
		default:syn=-1;break;
		}
	}
}
//��һ�����ʽ�еĸ���ֵ��ֵ���ṹ��quad���������һ�����������������ڶ���������
void emit(char *result,char *agl,char *op,char *ag2){
	strcpy(quad[q].result1,result);
	strcpy(quad[q].agll,agl);
	strcpy(quad[q].opl,op);
	strcpy(quad[q].ag21,ag2);
	q++;
}