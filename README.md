# use_other_language_in_shell
 directly use other language code in shell script

**./xxx srcFilename dstFilename**
************ **python transform** ******************

1. python code block start with #python-code-start

2. python code block end with #python-code-end

3. input parameters to python codes should be defined as:
    input python_var_name=shell_varname
    
4. output parameters to shell script should be defined as:
    output shell_varname=python_var_name
    
5. python codes support use "import" to define the package for using

6. python codes support define function in block

7. python codes support call the defined python code by using the fuc define place number
    function number start with 1 to ...
    
8. if the input is string, it will replace all blanks to string "###" as inputs

    

************ **C++ transform** ******************

1. C++ code block start with #C++-code-start
2. C++ code block end with #C++-code-end
3. input parameters to C++ codes should be defined as:
    input type cpp_var_name=shell_varname
4. output parameters to shell script should be defined as:
    output shell_varname=cpp_var_name
5. C++ codes support use "#include<>" to define the package for using
6. C++ codes support define function in block
     function start with: #subfuc; end with: #endfuc
7. C++ codes support call the defined C++ code by using the fuc define place number
    function number start with 1 to ...
8. if the input is string, it will replace all blanks to string "###" as inputs
