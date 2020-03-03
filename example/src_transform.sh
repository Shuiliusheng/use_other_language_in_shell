#!/bin/bash

str1="123"
str2=12
str3="14.4"

#python-code-start
pr=`python3 src_transform.sh.py fun1  $str1 $str2 $str3`

r1=`echo $pr| awk -F "###" '{print $1}'`
r2=`echo $pr| awk -F "###" '{print $2}'`
r3=`echo $pr| awk -F "###" '{print $3}'`
r4=`echo $pr| awk -F "###" '{print $4}'`
#python-code-end

echo $r1, $r2
echo $r3, $r4


