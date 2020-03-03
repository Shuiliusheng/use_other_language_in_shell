#!/bin/bash

str1="123"
str2=12
str3="14.4"
str4="hello world"


#python-code-start

input v1=str1;
input v2=str2;
input v3=str3;
input v4=str4;

v1=int(v1)
v2=float(v2)
v3=float(v3)

v1=v1+100000
v1=hex(v1)

v2=v2+v3

v3=cmath.sqrt(v3)

r=random.random()*100
r=round(r)


output r1=v1
output r2=v2
output r3=v4
output r4=r

#python-code-end
echo $r1,$r2,$r3,$r4
