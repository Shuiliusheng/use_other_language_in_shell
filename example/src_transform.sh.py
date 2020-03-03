#!/usr/bin/python
# -*- coding: UTF-8 -*-
import sys 
import cmath 
import random 
import random
import math
#----------------------------------------------
def divide(v):
	if v > 0:
		return "positive"
	else 
		return "negative"
		 
#----------------------------------------------

def fun1():
	 
	v1=sys.argv[2]
	v2=sys.argv[3]
	v3=sys.argv[4]
	 
	v1=int(v1) 
	v2=float(v2) 
	v3=float(v3) 
	 
	v1=v1+100000 
	v1=hex(v1) 
	 
	v2=v2+v3 
	 
	v3=cmath.sqrt(v3) 
	 
	r=random.random()*100 
	r=round(r) 
	 
	 
	 
	print(v1,v2,v3,r,sep="###")
#----------------------------------------------
def main( fuc_name ):
	if fuc_name == "fun1":
		fun1()
	else:
		print ("function name is wrong!")

main(sys.argv[1])
