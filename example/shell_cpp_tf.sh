#!/bin/bash

str1="123"
str2="hello world asdf asdf asdf adf asdf"
str3="14.4"

#C++-code-start
pr=`./shell_cpp_tf.sh.cpp_exe fuc1  ${str1// /"###"} ${str3// /"###"} ${str2// /"###"}`

r1=`echo $pr| awk -F "###" '{print $1}'`
r2=`echo $pr| awk -F "###" '{print $2}'`
r3=`echo $pr| awk -F "###" '{print $3}'`
r4=`echo $pr| awk -F "###" '{print $4}'`
#C++-code-end

echo $r1, $r2
echo $r3, $r4


#C++-code-start
pr=`./shell_cpp_tf.sh.cpp_exe fuc2 `

test=`echo $pr| awk -F "###" '{print $1}'`
#C++-code-end

echo "cmp result:" $test

