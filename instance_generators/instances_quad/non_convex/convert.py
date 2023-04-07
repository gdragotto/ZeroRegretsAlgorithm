import numpy as np
import scipy.io as sio
import pyarma as pa
import os
import glob, os

os.chdir("./")
converted="./converted/"
for filename in glob.glob("*.mat"):
	print("Converting: ",filename)
	data = sio.loadmat(filename)
	#print(data['Omega'])
	n = data['N'][0][0]
	m = data['n_nus'][0][0]
	lb = data['lb'][0][0]
	ub = data['ub'][0][0]
	instance_name = filename.replace(".mat","")
	with open(converted+instance_name+".txt", 'w') as f:
		f.write(str(n)+" "+str(m)+"\n")
		f.write(str(lb)+" "+str(ub))
	for i in range(n):
		Omega = data['Omega']
		Gf = data['Gf']
		n_constr = len(Omega[0][i])
		#Constraints
		A=pa.mat(list(Omega[0][i]))
		A.save(converted+instance_name+"-"+str(i)+"-A.matrix", pa.arma_binary);
		b=pa.mat(list(Omega[1][i]))
		b.save(converted+instance_name+"-"+str(i)+"-b.matrix", pa.arma_binary);
		#Objective, with minus, since the original is minimizing
		Q = -pa.mat(list(Gf[1][i]))
		Q.save(converted+instance_name+"-"+str(i)+"-Q.matrix", pa.arma_binary);
		c = -pa.mat(list(Gf[2][i]))
		c.save(converted+instance_name+"-"+str(i)+"-c.matrix", pa.arma_binary);
		C = -pa.mat(list(Gf[0][i]))
		index = 0
		for o in range(n):
			if i!=o:
				Ctemp = C[0:m-1,m*index:m*(index+1)-1]
				Ctemp.save(converted+instance_name+"-"+str(i)+"-C-"+str(o)+".matrix", pa.arma_binary)
				index = index+1
		




