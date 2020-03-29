import csv
import numpy as np
import pylab
import matplotlib
from matplotlib import ticker
import matplotlib.pyplot as plt
cols = ['b', 'g', 'r', 'c', 'm']

def doPlot():
	for i in ['rand_rd_', 'rand_rmw_', 'seq_rd_', 'seq_rmw_']:    
		with open('csvData/cachetime_' + i + '1t.csv', newline='') as f:
			reader = csv.reader(f)
			data = np.array(list(reader))
			
		with open('csvData/cachetime_' + i + '2t.csv', newline='') as f:
			reader = csv.reader(f)
			data1 = np.array(list(reader))
			
		with open('csvData/cachetime_' + i + '4t.csv', newline='') as f:
			reader = csv.reader(f)
			data2 = np.array(list(reader))
			
		with open('csvData/cachetime_' + i + '8t.csv', newline='') as f:
			reader = csv.reader(f)
			data3 = np.array(list(reader))
			
		with open('csvData/cachetime_' + i + '16t.csv', newline='') as f:
			reader = csv.reader(f)
			data4 = np.array(list(reader))
			
		a = data[1:, 0].astype(int)
		b = data[1:, 1].astype(float)
		b1 = data1[1:, 1].astype(float)
		b2 = data2[1:, 1].astype(float)
		b3 = data3[1:, 1].astype(float)
		b4 = data4[1:, 1].astype(float)
		
		fig, ax = plt.subplots(figsize=(20, 10), dpi=144, facecolor='w', edgecolor='b')
		
		if 'rand' in i:
			ax.set_xlim(2**5, 2**26)
			ax.set_ylim(0, 80)
			ax.set_xscale('log', basex=2)
			ax.xaxis.set_major_locator(ticker.LogLocator(base=2.0, numticks=22))
			ax.set_xlabel(r'Memory size (Bytes)$\rightarrow$', fontsize='x-large')
		else:
			ax.set_xlim(0, 1088)
			ax.set_ylim(0, 50)
			ax.set_xticks(list(np.arange(18)*64))
			ax.set_xlabel(r'Memory stride (Bytes)$\rightarrow$', fontsize='x-large')

		ax.set_ylabel(r'Latency (ns) $\rightarrow$', fontsize='x-large')
		ax.grid(True)
		ax.plot(a, b, label='1 thread', marker='o', linestyle='dashed', c=cols[0])
		
		ax.legend(loc='upper left')
		
		if i == 'rand_rd_':
			ax.set_title('Read Performance for Random Access', fontsize='xx-large')
		elif i == 'rand_rmw_':
			ax.set_title('Read-Modify-Write Performance for Random Access', fontsize='xx-large')
		elif i == 'seq_rd_':
			ax.set_title('Read Performance for Sequential Access', fontsize='xx-large')
		else:
			ax.set_title('Read-Modify-Write Performance for Sequential Access', fontsize='xx-large')
			
		fig.savefig('plots/cachetime_' + i + '1t')
		
		
		
		
		
		# Now plot Multithreaded Data
		
		fig, ax = plt.subplots(figsize=(20, 10), dpi=144, facecolor='w', edgecolor='b')
		
		if 'rand' in i:
			ax.set_xlim(2**5, 2**26)
			ax.set_xscale('log', basex=2)
			ax.xaxis.set_major_locator(ticker.LogLocator(base=2.0, numticks=22))
			ax.set_xlabel(r'Memory size (Bytes)$\rightarrow$', fontsize='x-large')
		else:
			ax.set_xlim(0, 1088)
			ax.set_xticks(list(np.arange(18)*64))
			ax.set_xlabel(r'Memory stride (Bytes)$\rightarrow$', fontsize='x-large')

		ax.set_ylabel(r'Latency (ns) $\rightarrow$', fontsize='x-large')
		ax.grid(True)
		ax.plot(a, b, label='1 thread', marker='o', linestyle='dashed', c=cols[0])
		ax.plot(a, b1, label='2 threads', marker='D', linestyle='dashed', c=cols[1])
		ax.plot(a, b2, label='4 threads', marker='x', linestyle='dashed', c=cols[2])
		ax.plot(a, b3, label='8 threads', marker='s', linestyle='dashed', c=cols[3])
		ax.plot(a, b4, label='16 threads', marker= '^', linestyle='dashed', c=cols[4])
		
		ax.legend(loc='upper left')
		
		if i == 'rand_rd_':
			ax.set_title('Read Performance for Random Access', fontsize='xx-large')
		elif i == 'rand_rmw_':
			ax.set_title('Read-Modify-Write Performance for Random Access', fontsize='xx-large')
		elif i == 'seq_rd_':
			ax.set_title('Read Performance for Sequential Access', fontsize='xx-large')
		else:
			ax.set_title('Read-Modify-Write Performance for Sequential Access', fontsize='xx-large')
			
		fig.savefig('plots/cachetime_' + i + 'mt')
		
	return 'Plot generated in path \'plots/\''