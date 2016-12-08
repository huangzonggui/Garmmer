#include "stdio.h"
#include "string.h"
#include "conio.h"
#include "malloc.h"//malloc头文件所在
#include "stdlib.h"//exit(0)头文件所在
char prog[100],token[8],ch;//prog数组装的是语句的字符，ch是整个语句
char *keyWord[6]={"begin","if","then","while","do","end"};
int syn,p,m,n,sum,q;
int kk;//出错就赋值为1，初始化为0，有一个变量就加一个，见newtemp()
//一个赋值等式的结构体result1=agll opl ag21
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
	//通过文件输入
	if ((fp=fopen("tokens.txt","r"))==NULL){
		printf("cannot open this file\n");
		exit(0);
	}
	while ((ch=fgetc(fp))!=EOF){
		prog[p++]=ch;//将ch语句中的每个字符赋值给prog数组
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
	getch();//所在头文件conio.h
}
//parser：语法分析
int lrparser(){
	int schain=0;
	kk=0;
	//是否begin?
	if (syn==1){
		scaner();
		schain=isEnd();
		//是否end?
		if (syn==6){
			scaner();
			//是否‘#’&&？
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
//通过statement来分析，移除以分号结束的所有语句，为了得到最后一个单词，便于判断是否是end
int isEnd(){
	int schain=0;
	schain=statement();
	//是否 ;
	while(syn==26){
		scaner();
		schain=statement();
	}
	return (schain);
}
//语句串分析
//标识符 
int statement(){
	char tt[8],eplace[8];
	int schain=0;
	//是否是标识符（由字母和数字组成，存在token数组中）
	if (syn==10){
		strcpy(tt,token);
		scaner();
		//是否是等号
		if (syn==18){
			scaner();
			strcpy(eplace,expression());
			emit(tt,eplace,"","");
			schain=0;
		}else{
			printf("short of sign ':=' !\n");//出错
			kk=1;
			getch();
			exit(0);
		}
		return (schain);
	}
}
//（等号后的）表达式：：=<项>{+<项>|-<项>}
char *expression(){
	char *result,*ep2,*eplace,*ope;
	result=(char *)malloc(12);
	ep2=(char *)malloc(12);
	eplace=(char *)malloc(12);
	ope=(char *)malloc(12);
	strcpy(eplace,term());//字符串的复制，将term()里的字符串复制到eplace
	//case '+':syn=13;break;case '-':syn=14;break;
	while ((syn==13)||syn==14){
		if (syn==13){
			strcpy(ope,"+");
		}else{
			strcpy(ope,"-");
		}
		scaner();
		strcpy(ep2,term());
		strcpy(result,newTemp());//newtemp临时变量
		emit(result,eplace,ope,ep2);
		//eplace=result;?
		strcpy(eplace,result);
	}
	return (eplace);//得到的是一个计算结果
}
//项分析：<项>：：=<因子>{*<因子>|/<因子>}
char *term(){
	char *result,*ep2,*eplace,*ope;
	result=(char *)malloc(12);
	ep2=(char *)malloc(12);
	eplace=(char *)malloc(12);
	ope=(char *)malloc(12);
	strcpy(eplace,factor());//字符串的复制，将term()里的字符串复制到eplace
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

//因子：<因子>：：=ID|NUM|(<表达式>)
char *factor(){
	char *fplace;
	fplace=(char *)malloc(12);//分配长度为num_bytes字节的内存块
	strcpy(fplace,"");
	if (syn==10){//是否标识符？
		strcpy(fplace,token);
		scaner();
	}else if(syn==11){//是否整常数？
		itoa(sum,fplace,10);
		scaner();
	}else if(syn==27){//是否 '('?
		scaner();
		fplace=expression();
		if (syn==28){//是否 ')'？
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
//结果的临时变量
char *newTemp(){
	char *p;
	char m[8];
	p=(char *)malloc(8);
	kk++;
	//itoa()：第一个参数是要转换的数字，第二个参数是要写入转换结果的目标字符串，第三个参数是转移数字时所用的基数（十进制、二进制等）。
	itoa(kk,m,10);//10代表十进制
	strcpy(p+1,m);
	p[0]='t';
	return (p);//结果 t1,t2,......
}
//读取下一个单词符号：扫描判断其字符类型并赋上syn种别码
void scaner(){
	sum=0;
	for (m=0;m<8;m++){//token数组初始化长度是8
		token[m++]=NULL;
	}
	m=0;
	ch=prog[p++];//第一次扫描的时候p为0
	//如果是空格，跳过，扫描prog数组下一个字符
	while(ch==' '){
		ch=prog[p++];
	}
	//如果是字母
	if (((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A'))){
		//继续循环判断连续的下一个字符，直到不是字母和数字
		while (((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A'))||((ch>='0')&&(ch<='9'))){
			token[m++]=ch;
			ch=prog[p++];
		}
		p--;//判断了下一个字符不符合条件，自减，为了下次能分析到这个字符
		syn=10;//标志符
		token[m++]='\0';//字符串的结尾标志，不计入串长，但要占内存空间、'\0'的ASCII是0
		for (n=0;n<6;n++){
			//关键字rwtab={"begin","if","then","while","do","end"};种别码是1-6
			if (strcmp(token,keyWord[n])==0){
				syn=n+1;
				break;
			}
		}
	//如果是数字
	}else if ((ch>='0')&&(ch<='9')){
		while ((ch>='0')&&(ch<='9')){
			sum=sum*10+ch-'0';
			ch=prog[p++];
		}
		p--;
		syn=11;
	//其他符号
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
//将一个表达式中的各个值赋值给结构体quad（结果，第一个参数，操作符，第二个参数）
void emit(char *result,char *agl,char *op,char *ag2){
	strcpy(quad[q].result1,result);
	strcpy(quad[q].agll,agl);
	strcpy(quad[q].opl,op);
	strcpy(quad[q].ag21,ag2);
	q++;
}