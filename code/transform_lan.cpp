#include "process_python.h"
#include "process_c++.h"


void readfile(char filename[],char dstname[])
{
	FILE *p=fopen(filename,"r");
	if(p==NULL)
	{
		cout <<filename<<" is not exist!"<<endl;
		return;
	}
	char str[300];


	bool python_flag=false;
	char pyname[300];
	sprintf(pyname,"%s.py",dstname);
	Process_py py(pyname);
	
	bool cpp_flag=false;
	char cppname[300];
	sprintf(cppname,"%s.cpp",dstname);
	Process_cpp cpp(cppname);

	
	FILE *w=fopen(dstname,"w");
	while(!feof(p))
	{
		fgets(str,300,p);
		str[strlen(str)-1]='\0';
		if(strcmp(str,"#python-code-start")==0)
		{
			python_flag=true;
			py.init_fucinfo();
			continue;
		}
		else if(strcmp(str,"#python-code-end")==0)
		{
			python_flag=false;
			char ret[1000];
			py.fuc_end(ret);
			fprintf(w,"#python-code-start\n");
			fprintf(w,"%s\n",ret);
			fprintf(w,"#python-code-end\n");
			continue;
		}
		else if(strcmp(str,"#C++-code-start")==0)
		{
			cpp_flag=true;
			cpp.init_fucinfo();
			continue;
		}
		else if(strcmp(str,"#C++-code-end")==0)
		{
			cpp_flag=false;
			char ret[1000];
			cpp.fuc_end(ret);
			fprintf(w,"#C++-code-start\n");
			fprintf(w,"%s\n",ret);
			fprintf(w,"#C++-code-end\n");
			continue;
		}

		if(python_flag)
			py.get_line(str);
		else if (cpp_flag)
			cpp.get_line(str);
		else
			fprintf(w,"%s\n",str);
		
		strcpy(str,"");
	}

	fclose(p);
	fclose(w);

	py.write_script();
	cpp.write_script();

}

int main(int argv, char **args)
{
	if(argv<3)
	{
		cout <<"''"<<endl;
		cout <<"input parameter is not enough"<<endl;
		cout <<"./xxx srcFilename dstFilename"<<endl;
		python_help();
		cout <<endl;
		cpp_help();
		return 0;
	}
	readfile(args[1],args[2]);
	//readfile("src1.sh","dst1.sh");
	return 0;
}