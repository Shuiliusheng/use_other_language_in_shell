#include<iostream>
#include<stdio.h>
#include<string.h>
#include<vector>
using namespace std;

typedef struct{
	char text[200];
}Line;
typedef vector<Line> Lines;

void python_help()
{
	cout <<"************ python transform ******************"<<endl;
	cout <<"1. python code block start with #python-code-start"<<endl;
	cout <<"2. python code block end with #python-code-end"<<endl;
	cout <<"3. input parameters to python codes should be defined as: "<<endl;
	cout <<"    input python_var_name=shell_varname"<<endl;
	cout <<"4. output parameters to shell script should be defined as: "<<endl;
	cout <<"    output shell_varname=python_var_name"<<endl;
	cout <<"5. python codes support use \"import\" to define the package for using"<<endl;
	cout <<"6. python codes support define function in block"<<endl;
	cout <<"7. python codes support call the defined python code by using the fuc define place number"<<endl;
	cout <<"    function number start with 1 to ..."<<endl;
	cout <<endl;
}


//处理python文件
class Process_py{
public:
	Process_py(char name[])
	{
		strcpy(this->filename,name);
		fuc_num=0;
	}

	//处理函数中的每一行
	void get_line(char str[])
	{
		if(input_line(str)||output_line(str)||import_line(str))
			return;
		
		if(start_str(str,"def")!=-1)
			subfuc_flag=true;
		else if(str[0]!='\t')
			subfuc_flag=false;
		

		Line temp;
		if(subfuc_flag){
			sprintf(temp.text,"%s\n",str);
			subfuc_text.push_back(temp);
		}
		else{
			sprintf(temp.text,"\t%s\n",str);
			fuc_text.push_back(temp);
		}
	}

	//遇到python的开始
	void init_fucinfo()
	{
		input.clear();
	 	output.clear();
		strcpy(self_output,"");
		input_num=2;
		subfuc_flag=false;
		fuc_num++;

		Line temp;
		sprintf(temp.text,"\ndef fun%d():\n",fuc_num);
		fuc_text.push_back(temp);

		sprintf(temp.text,"#!/usr/bin/python\n# -*- coding: UTF-8 -*-\n");
		imp_text.push_back(temp);

		sprintf(temp.text,"import sys \nimport cmath \nimport random \n");
		imp_text.push_back(temp);
	}

	//遇到python的结束
	void fuc_end(char back[])
	{

		//for python
		Line temp;
		sprintf(temp.text,"\tprint(%s,sep=\"###\")\n",self_output);
		fuc_text.push_back(temp);


		//for shell
		int i=0;
			//call python script
		sprintf(back,"pr=`python3 %s fun%d ", filename,fuc_num);
		for(i=0;i<input.size();i++)
			sprintf(back,"%s $%s",back,input[i].text);
		sprintf(back,"%s`\n",back);
	
			//get outputs
		for(i=0;i<output.size();i++)
			sprintf(back,"%s\n%s=`echo $pr| awk -F \"###\" '{print $%d}'`",back,output[i].text,i+1);

	}

	void write_script()
	{
		if(fuc_text.size()==0)
			return ;
		int i=0;
		FILE *p=fopen(filename,"w");
		
		//write import line
		for(i=0;i<imp_text.size();i++)
			fprintf(p,"%s",imp_text[i].text);

		fprintf(p,"#----------------------------------------------\n");
		//write subfuc line
		for(i=0;i<subfuc_text.size();i++)
			fprintf(p,"%s",subfuc_text[i].text);
		fprintf(p,"#----------------------------------------------\n");

		//write fuc line
		for(i=0;i<fuc_text.size();i++)
			fprintf(p,"%s",fuc_text[i].text);
		fprintf(p,"#----------------------------------------------\n");

		//write main
		fprintf(p,"def main( fuc_name ):\n");
		fprintf(p,"\tif fuc_name == \"fun1\":\n");
		fprintf(p,"\t\tfun1()\n");
		for(i=2;i<=fuc_num;i++)
		{
			fprintf(p,"\telif fuc_name == \"fun%d\":\n",i);
			fprintf(p,"\t\tfun%d()\n",i);
		}
		fprintf(p,"\telse:\n\t\tprint (\"function name is wrong!\")\n");
		
		fprintf(p,"\nmain(sys.argv[1])\n");
		
		fclose(p);
		
	}

private:
	char filename[200];
	Lines imp_text;
	Lines fuc_text;
	Lines subfuc_text;
	Lines input;
	Lines output;
	char self_output[200];
	int fuc_num;
	int input_num;
	bool subfuc_flag;

		//判断子串
	int start_str(char str[], char substr[])
	{
		int i=0;
		for(i=0;i<strlen(str);i++)
		{
			if(str[i]==' ')
			{
				str[i]='\0';
				break;
			}
		}
		
		if(strcmp(str,substr)==0)
		{
			str[i]=' ';
			return i+1;
		}
		else
		{
			str[i]=' ';
			return -1;
		}
	}
	
	//获取input/output中的等号参数
	void get_params(char str[], char p1[], char p2[])
	{
		int i=0;
		int n=0;
		for(i=0;i<strlen(str);i++)
		{
			if(str[i]=='=')
				break;
			p1[n]=str[i];
			n++;
		}
		p1[n]='\0';
		n=0;
		for(i=i+1;i<strlen(str);i++)
		{
			if(str[i]==';')
				break;
			p2[n]=str[i];
			n++;
		}
		p2[n]='\0';
		n=0;
	}

	//process import
	bool import_line(char str[])
	{
		int i=start_str(str,"import");
		if(i==-1)
			return false;
		Line temp;
		sprintf(temp.text,"%s\n",str);
		imp_text.push_back(temp);
		return true;
	}

	//处理input行
	bool input_line(char str[])
	{
		int i=start_str(str,"input");
		if(i==-1)
			return false;
		
		char p1[30],p2[30];
		get_params(&str[i],p1,p2);
		
		Line temp,temp1;
		sprintf(temp.text,"\t%s=sys.argv[%d]\n",p1,input_num);
		fuc_text.push_back(temp);
		input_num++;

		sprintf(temp1.text,"%s",p2);
		input.push_back(temp1);

		return true;
	}

	//process output line
	bool output_line(char str[])
	{
		int i=start_str(str,"output");
		if(i==-1)
			return false;
		
		char p1[30],p2[30];
		get_params(&str[i],p1,p2);
		
		if(strlen(self_output)==0)
			sprintf(self_output,"%s",p2);
		else
			sprintf(self_output,"%s,%s",self_output,p2);
		
		Line temp;
		sprintf(temp.text,"%s",p1);
		output.push_back(temp);
		return true;

	}
};