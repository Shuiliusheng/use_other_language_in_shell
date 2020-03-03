#include "transform.h"

void cpp_help()
{
	cout <<"************ C++ transform ******************"<<endl;
	cout <<"1. C++ code block start with #C++-code-start"<<endl;
	cout <<"2. C++ code block end with #C++-code-end"<<endl;
	cout <<"3. input parameters to C++ codes should be defined as: "<<endl;
	cout <<"    input type cpp_var_name=shell_varname"<<endl;
	cout <<"4. output parameters to shell script should be defined as: "<<endl;
	cout <<"    output shell_varname=cpp_var_name"<<endl;
	cout <<"5. C++ codes support use \"#include<>\" to define the package for using"<<endl;
	cout <<"6. C++ codes support define function in block"<<endl;
	cout <<"     function start with: #subfuc; end with: #endfuc"<<endl;
	cout <<"7. C++ codes support call the defined C++ code by using the fuc define place number"<<endl;
	cout <<"    function number start with 1 to ..."<<endl;
	cout <<"8. if the input is string, it will replace all blanks to string \"###\" as inputs "<<endl;
	cout <<endl;
}

//处理python文件
class Process_cpp{
public:
	Process_cpp(char name[])
	{
		strcpy(this->filename,name);
		fuc_num=0;
	}

	//处理函数中的每一行
	void get_line(char str[])
	{
		if(input_line(str)||output_line(str)||include_line(str))
			return;
		
		if(strcmp(str,"#subfuc")==0)
		{
			subfuc_flag=true;
			return;
		}
		else if(strcmp(str,"#endfuc")==0)
		{
			subfuc_flag=false;
			return;
		}
		

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
		sprintf(temp.text,"\nvoid fuc%d(char **args)\n{\n",fuc_num);
		fuc_text.push_back(temp);

		if(fuc_num==1)
		{
			sprintf(temp.text,"#include<string.h>\n#include<stdlib.h>\n");
			imp_text.push_back(temp);

			sprintf(temp.text,"#include<stdio.h>\n#include<iostream>\nusing namespace std;\n");
			imp_text.push_back(temp);
		}
	}

	//遇到python的结束
	void fuc_end(char back[])
	{

		//for python
		Line temp;
		sprintf(temp.text,"\tcout <<%s<<endl; \n}\n",self_output);
		fuc_text.push_back(temp);


		//for shell
		int i=0;
			//call python script
		sprintf(back,"pr=`./%s_exe fuc%d ", filename,fuc_num);
		for(i=0;i<input.size();i++)
			sprintf(back,"%s ${%s// /\"###\"}",back,input[i].text);
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

		fprintf(p,"//----------------------------------------------\n");
		//write subfuc line
		this->add_decode_subfuc();
		for(i=0;i<subfuc_text.size();i++)
			fprintf(p,"%s",subfuc_text[i].text);
		fprintf(p,"//----------------------------------------------\n");

		//write fuc line
		for(i=0;i<fuc_text.size();i++)
			fprintf(p,"%s",fuc_text[i].text);
		fprintf(p,"//----------------------------------------------\n");

		//write main
		fprintf(p,"int main(int argv, char** args )\n{\n");
		fprintf(p,"\tif(strcmp(args[1],\"fuc1\")==0)\n");
		fprintf(p,"\t\tfuc1(args);\n");
		for(i=2;i<=fuc_num;i++)
		{
			fprintf(p,"\telse if(strcmp(args[1],\"fuc%d\")==0)\n",i);
			fprintf(p,"\t\tfuc%d(args);\n",i);
		}
		fprintf(p,"\telse\n\t\tcout <<\"function name is wrong!\"<<endl; \n");
		
		fprintf(p,"\n\treturn 0; \n}\n");
		
		fclose(p);
		
		char cmd[200];
		sprintf(cmd,"g++ %s -o %s_exe",filename,filename);
		cout <<cmd<<endl;
		system(cmd);
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
		char t=0;
		for(i=0;i<strlen(str);i++)
		{
			if(str[i]==' '||str[i]=='<')
			{
				t=str[i];
				str[i]='\0';
				break;
			}
		}
		
		if(strcmp(str,substr)==0)
		{
			str[i]=t;
			return i+1;
		}
		else
		{
			str[i]=t;
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
	bool include_line(char str[])
	{
		int i=start_str(str,"#include");
		if(i==-1)
			return false;
		Line temp;
		sprintf(temp.text,"%s\n",str);
		imp_text.push_back(temp);
		return true;
	}

	//将转换后的字符串解码
	void add_decode_subfuc()
	{
		Line temp;
		
		sprintf(temp.text,"char* decode_str(char str[])\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"{\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\tchar* temp = (char*)malloc(strlen(str));\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\tint n=0;\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\tfor(int i=0;i<strlen(str)-2;i++,n++)\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t{\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\tif(str[i]=='#'&&str[i+1]=='#'&&str[i+2]=='#')\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t{\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t\ttemp[n]=' ';\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t\ti=i+2;\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t\tcontinue;\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t}\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\telse if(i==strlen(str)-3)\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t{\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t\ttemp[n]=str[i];\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t\ttemp[n+1]=str[i+1];\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t\ttemp[n+2]=str[i+2];\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t\tn=n+3;\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t\tbreak;\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\t}\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t\ttemp[n]=str[i];\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\t}\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\ttemp[n]='\\0';\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"\treturn temp;\n");
		subfuc_text.push_back(temp);
		sprintf(temp.text,"}\n");
		subfuc_text.push_back(temp);
	}


	//char, int, double, float, char*
	int get_input_type(char str[], int &p)
	{
		int i=0;
		for(i=0;i<strlen(str);i++)
		{
			p++;
			if(str[i]==' ')
			{
				str[i]='\0';
				break;
			}
		}
		if(strcmp(str,"char")==0)
			return 0;
		else if(strcmp(str,"int")==0)
			return 1;
		else if(strcmp(str,"double")==0)
			return 2;
		else if(strcmp(str,"float")==0)
			return 3;
		else if(strcmp(str,"char*")==0)
			return 4;
		else 
			return -1;
	}

	//处理input行
	bool input_line(char str[])
	{
		int i=start_str(str,"input");
		if(i==-1)
			return false;
		
		int varType=get_input_type(&str[i],i);

		char p1[30],p2[30];
		get_params(&str[i],p1,p2);
		
		Line temp,temp1;
		sprintf(temp.text,"\tchar* args_decode%d=decode_str(args[%d]);\n",input_num,input_num);
		fuc_text.push_back(temp);
		
		switch(varType){
			case 0: sprintf(temp.text,"\tchar %s=args_decode%d[0];\n",p1,input_num);break;
			case 1: sprintf(temp.text,"\tint %s=atoi(args_decode%d);\n",p1,input_num);break;
			case 2: sprintf(temp.text,"\tdouble %s=atof(args_decode%d);\n",p1,input_num);break;
			case 3: sprintf(temp.text,"\tfloat %s=(float)atof(args_decode%d);\n",p1,input_num);break;
			case 4: sprintf(temp.text,"\tchar* %s=args_decode%d;\n",p1,input_num);break;
			default: sprintf(temp.text,"\tchar* %s=args_decode%d;\n",p1,input_num);
		}
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
			sprintf(self_output,"%s<<\"###\"",p2);
		else
			sprintf(self_output,"%s<<%s<<\"###\"",self_output,p2);
		
		Line temp;
		sprintf(temp.text,"%s",p1);
		output.push_back(temp);
		return true;

	}
};