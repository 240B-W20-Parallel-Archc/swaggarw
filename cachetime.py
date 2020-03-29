import time
import argparse
import subprocess
import os

parser = argparse.ArgumentParser()
parser.add_argument('-t', '--threads', type=int, default='1')
parser.add_argument('-s', '--sequential', action='store_true')
parser.add_argument('-ss', '--stridesize', type=int, default='64')
parser.add_argument('-r', '--random', action='store_true')
parser.add_argument('-m', '--modify', action='store_true')
parser.add_argument('-p', '--plot', action='store_true') 

args = parser.parse_args()

reps = 0
raw_output = []
cmd = []

if (args.plot):
	import makeplots
	print(makeplots.doPlot())

else:
	if not args.sequential and not args.random:
		args.random = True

	if args.sequential:
		seqOrRand = '-s'
	else:
		seqOrRand = '-r'

	cmd.append('./cachetime.out')
	if args.sequential:
		cmd.append('-s')
		cmd.append('-ss')	
		cmd.append(str(args.stridesize))
	else:
		cmd.append('-r')
		
	if args.modify:
		cmd.append('-m')
	if args.threads:
		cmd.append('-t')
		cmd.append(str(args.threads))
		
	# cmd.append(args.outnm)
	print('Running command: ' + str(' '.join(cmd)))

	try:
		os.mkdir('plots')
		os.mkdir('csvData')
	except:
		pass
		
	try:
		raw_output = subprocess.call(cmd)
	except:
		print("Error: running cachetime")
		exit(-1)