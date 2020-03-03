#!/usr/bin/python
# -*- coding: UTF-8 -*-
import sys 
import cmath 
import random 
#----------------------------------------------
def decode_str(str):
	return str.replace("###"," ")
#----------------------------------------------

def fuc1():
	 
	v1=decode_str(sys.argv[2])
	v2=decode_str(sys.argv[3])
	v3=decode_str(sys.argv[4])
	v4=decode_str(sys.argv[5])
	 
	v1=int(v1) 
	v2=float(v2) 
	v3=float(v3) 
	 
	v1=v1+100000 
	v1=hex(v1) 
	 
	v2=v2+v3 
	 
	v3=cmath.sqrt(v3) 
	 
	r=random.random()*100 
	r=round(r) 
	 
	 
	 
	print(v1,v2,v4,r,sep="###")
#----------------------------------------------
def main( fuc_name ):
	if fuc_name == "fuc1":
		fuc1()
	else:
		print ("function name is wrong!")

main(sys.argv[1])
