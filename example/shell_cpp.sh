#!/bin/bash

str1="123"
str2="hello world asdf asdf asdf adf asdf"
str3="14.4"

#C++-code-start

#include<math.h>
#include <stdlib.h> 

#subfuc
bool cmp(int a,int b)
{
    if(a==b*2)
        return true;
    else
        return false;
}
#endfuc

input int v1=str1;
input float v2=str3;
input char* str=str2;

int r=rand()%100;
v1 = v1*v1;
v2 = sqrt(v2);

for(int i=0;i<strlen(str);i++)
    str[i];

output r1=r;
output r2=v1;
output r3=v2;
output r4=str;

#C++-code-end

echo $r1, $r2
echo $r3, $r4


#C++-code-start

char a[100];
gets(a);


output test=a;

#C++-code-end

echo "cmp result:" $test
