#include<iostream>
#include<random>
#include<cstdlib>
#include<chrono>
#include<cmath>
#include<fstream>
#include<omp.h>

using namespace std;
using namespace chrono;

int threadNum;
int strideSize;
int stepStride;
bool sequential;
size_t memSize;
bool rmw;

// Volatile variable to prevent compiler opt.
volatile void* noPrefetch;

// Return memory with randomly linked nodes
void** circularLinkedList(size_t memSize) {
	// cout << "In random" << endl;
	size_t buffLength = memSize/sizeof(void*);
	void** bufferMem = new void*[buffLength];

	size_t* buffIdx = new size_t[buffLength];
	
	for(size_t i = 0; i < buffLength; i++) {
		buffIdx[i] = i;
	}

	// Shuffle all pointer values
	for(size_t i = 0; i < buffLength - 1; i++) {
		std::random_device rd; // Uniform random variable (device)
		std::mt19937 mersenne_generator(rd()); // Mersenne Pseudo-Random Generator with 19937 states
		std::uniform_int_distribution<unsigned int> uniform_distribution(0, buffLength - i);
		size_t idx2 = i + uniform_distribution(mersenne_generator); // Uniform random distribution with Mersenne seed
		
		// Shuffle indices by exchanging pointers having unequal memory value
		if(idx2 != i) {
			std::swap(buffIdx[i], buffIdx[idx2]);
		}
	}

	// Linearly link shuffled pointers in memory
	for(size_t i = 0; i < buffLength - 1; i++) {
		bufferMem[buffIdx[i]] = (void*) &bufferMem[buffIdx[i + 1]]; 
	}
	// Link last pointer to first pointer to make it circular linked-list
	bufferMem[buffIdx[buffLength - 1]] = (void *) &bufferMem[buffIdx[0]];

	// Empty memory for buffer memory indices
	delete[] buffIdx;
	return bufferMem;
}

// Returns memory with linked nodes - "stride" bytes apart each other
void** stridedLinkedList(size_t memSize, int memStride) {
	size_t buffLength = memSize/sizeof(void*);
	void** bufferMem = new void*[buffLength];

	size_t* buffIdx = new size_t[buffLength];
	
	for(size_t i = 0; i < buffLength; i++) {
		buffIdx[i] = i;
	}

	// Assign strided values to buffer indices
	if(strideSize > 0) {
		for(size_t i = 0; i < buffLength; i++) {
			buffIdx[i] = ((i*memStride) / sizeof(void*)) % buffLength;
		}
	} else {
		for(size_t i = 0; i < buffLength; i++) {
			buffIdx[i] = i;
		}
	}

	// Linearly link shuffled pointers in memory
	for(size_t i=0; i < buffLength - 1; i++) {
		bufferMem[buffIdx[i]] = (void*) &bufferMem[buffIdx[i+1]]; 
	}
	
	// Link last pointer to first pointer to make it circular linked-list
	bufferMem[buffIdx[buffLength - 1]] = (void *) &bufferMem[buffIdx[0]];
	
	// Empty memory for buffer memory indices
	delete[] buffIdx;
	
	return bufferMem;
}

// Pointer Chasing
double chasePointer(void** bufferMem, size_t iter) {
	void** buffPtr = (void**) bufferMem;
	void** nxtPtr;

	auto begin = chrono::system_clock::now();
	
	// if rmw is 1, then RMW, else only Read
	double tottime = 0;
	
	if(rmw) {
		for(int i = 0; i < iter; i++) {
			// Read Next Pointer
			nxtPtr = (void**) *buffPtr;
			
			// Perform RMW as described in project description
			void** temp = (void**) *buffPtr;
			*buffPtr = (void**) ((uintptr_t)temp & 0x7fffffffffffffff);
			
			buffPtr = nxtPtr;
		}
	} else {
		for(int i = 0; i < iter; i++) {
			// cout << "In loop " << i << endl;
			// Only Read Next Pointer
			buffPtr = (void**) *buffPtr;
		}
	}
	
	tottime = ((std::chrono::duration<double>)(system_clock::now() - begin)).count();
	
	// The following disables cache prefetching via compiler opt.
	noPrefetch = *buffPtr;

	//Measure avg time taken
	return tottime;
}	


string getCSVName() {
	string name = "csvData/cachetime_";
	
	if(sequential) {
		name += "seq_";
	} else {
		name += "rand_";
	}
	
	if(rmw) {
		name += "rmw_";
	} else {
		name += "rd_";
	}
	
	name += std::to_string(threadNum);
	name += "t.csv";
	
	return name;
}

int main(int argc, char *argv[]) {
	threadNum = 1; 			// Default no. of threads
	strideSize = 64;		// Default stride size
	sequential = false;		// Defaut random access
	memSize = 1024*1024*32;	// Maximum size of allotted memory
	rmw = false;
	
	
	//Parse Command Line Arguements
	for(int i=1; i<argc; i++) {
		string arg = argv[i];
		if (arg == "-t") {
			if(i+1 < argc) {
				threadNum = atoi(argv[++i]);
			} else { exit(0); }
		} else if (arg == "-m") {
			rmw = true;
		} else if (arg == "-s") {
			sequential = true;
		} else if(arg == "-r") {
			sequential = false;
		} else if (arg == "-ss") {
			if(i+1 < argc) {
				strideSize = atoi(argv[++i]);
			}
		} else {
			exit(0);
		}
	}
	
	size_t minSize;
	size_t maxSize;
	size_t stepSize;
	int minStride;
	int maxStride;

	if(!sequential) {
		minSize = 64; 			// Minimum size of 64B
		maxSize = 1024*1024*32; // Maximum size of 64 MB
		stepSize = 1;	
		minStride = 0;
		maxStride = 0;
		stepStride = 1;
	} else {
		minSize = 1024*1024*32;
		maxSize = 1024*1024*32; // Constant size of 64 MB in sequential access
		stepSize = 1;	
		minStride = 0;			// Minimum stride of 0B
		maxStride = 1024;		// Maximum stride of 1024B
		stepStride = strideSize;// Stepof stride is 64 by default
	}
	
	ofstream data;
	string csvName = getCSVName();
	cout << "Saving to file: " << csvName << endl;
	data.open(csvName);
	
	if(!sequential) {
		data << "Size (B),Time (ns),Number of threads,Sequential?,Stride Size\n";
		cout << "Size (B)\t\t\tTime (ns)" << endl;
	} else {
		data << "Stride (B),Time (ns),Number of threads,Sequential?,Stride Size\n";
		cout << "Stride (B)\t\t\tTime (ns)" << endl;
	}

	data.close();
	
	size_t minIter, maxIter, stepIter;
	
// Run pointer chasing over different memory sizes and/or strides
	
	if(!sequential) {
		minIter = minSize;
		maxIter = maxSize;
		stepIter = stepSize;
	}
	else {
		minIter = minStride;
		maxIter = maxStride;
		stepIter = strideSize;
	}
	
	for(size_t i = minSize; i <= maxSize; i *= 2) {
		for(size_t j = minStride; j <= maxStride; j += stepStride) {
			double avgTime = 0;
		
			size_t maxIter = 1 << 30;
			size_t iter = std::max(i * 16, maxIter);

			//Random or Sequential access	
			if(!sequential) { // Random Access
				//Run on a number of threads using omp
				
				#pragma omp parallel num_threads(threadNum)
				{
					void** bufferMem = circularLinkedList(i); // Simple Circular Buffer for random access
					
					double time = chasePointer(bufferMem, iter); //Call pointer chasing for above buffer
						
					//Atomically accumulate total time from all threads
					#pragma omp atomic
						avgTime += time;
					
					delete[] bufferMem;
				}
			} else { // Sequential Access
				//Run on a number of threads using omp
				
				#pragma omp parallel num_threads(threadNum)
				{
					void** bufferMem = stridedLinkedList(i, j); // Strided Circular Buffer for Sequential Access
					
					double time = chasePointer(bufferMem, iter); //Call pointer chasing for above buffer
					
					//Atomically accumulate total time from all threads
					#pragma omp atomic
						avgTime += time;
					
					delete[] bufferMem;
				}
			}
			
			// Calculate average time for one access
			avgTime = ((avgTime/threadNum)/iter)*(1000000000);

			//Write latency to csv file
			data.open(csvName, ios::app);
			
			if(!sequential) {
				data << i << "," << avgTime << "," << threadNum << "," << "N" << "," << 1 << "\n";
				cout << i << "\t\t\t\t" << avgTime << endl;
			} else {
				data << j << "," << avgTime << "," << threadNum << "," << "N" << "," << strideSize << "\n";
				cout << j << "\t\t\t\t" << avgTime << endl;
			}
			data.close();
		}
	}
	
	return 0;
}