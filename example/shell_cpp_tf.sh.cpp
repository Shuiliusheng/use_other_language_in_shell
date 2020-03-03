#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
using namespace std;
#include<math.h>
#include <stdlib.h> 
//----------------------------------------------
bool cmp(int a,int b)
{
    if(a==b*2)
        return true;
    else
        return false;
}
char* decode_str(char str[])
{
	char* temp = (char*)malloc(strlen(str));
	int n=0;
	for(int i=0;i<strlen(str)-2;i++,n++)
	{
		if(str[i]=='#'&&str[i+1]=='#'&&str[i+2]=='#')
		{
			temp[n]=' ';
			i=i+2;
			continue;
		}
		else if(i==strlen(str)-3)
		{
			temp[n]=str[i];
			temp[n+1]=str[i+1];
			temp[n+2]=str[i+2];
			n=n+3;
			break;
		}
		temp[n]=str[i];
	}
	temp[n]='\0';
	return temp;
}
//----------------------------------------------

void fuc1(char **args)
{
	
	
	
	char* args_decode2=decode_str(args[2]);
	int v1=atoi(args_decode2);
	char* args_decode3=decode_str(args[3]);
	float v2=(float)atof(args_decode3);
	char* args_decode4=decode_str(args[4]);
	char* str=args_decode4;
	
	int r=rand()%100;
	v1 = v1*v1;
	v2 = sqrt(v2);
	
	for(int i=0;i<strlen(str);i++)
	    str[i];
	
	
	cout <<r<<"###"<<v1<<"###"<<v2<<"###"<<str<<"###"<<endl; 
}

void fuc2(char **args)
{
	
	char a[100];
	gets(a);
	
	
	
	cout <<a<<"###"<<endl; 
}
//----------------------------------------------
int main(int argv, char** args )
{
	if(strcmp(args[1],"fuc1")==0)
		fuc1(args);
	else if(strcmp(args[1],"fuc2")==0)
		fuc2(args);
	else
		cout <<"function name is wrong!"<<endl; 

	return 0; 
}
