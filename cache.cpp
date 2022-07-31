#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)

enum cacheResType {MISS=0, HIT=1};

/* The following implements a random number generator */
unsigned int m_w = 0xABCCAB99;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0xDEAD6902;    /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;  /* 32-bit result */
}

unsigned int memGenA()
{
	static unsigned int addr=0;
	return (addr++)%(DRAM_SIZE);
}

unsigned int memGenB()
{
	static unsigned int addr=0;
	return  rand_()%(64*1024);
}

unsigned int memGenC()
{
	static unsigned int a1=0, a0=0;
	a0++;
	if(a0 == 512) { a1++; a0=0; }
	if(a1 == 128) a1=0;
	return(a1 + a0*128);
}

unsigned int memGenD()
{
	return  rand_()%(16*1024);
}

unsigned int memGenE()
{
	static unsigned int addr=0;
	return (addr++)%(1024*64);
}

unsigned int memGenF()
{
	static unsigned int addr=0;
	return (addr+=64)%(64*4*1024);
}
// my code
#define		LINE_SIZE		(32)
#define		NOWays			(4)

struct Block {
	unsigned int valid;
	unsigned int tag;
};

vector<vector<Block>> cache;

// N-way Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
	int shifts = log(LINE_SIZE) / log(2);// number of shifts to exclude Block Number

	int lines = CACHE_SIZE / LINE_SIZE;		// number of lines (Blocks)
	int numSets = lines / NOWays;			// number of sets (ex: 512)

	int sets = numSets - 1;					// get ones to and with it

	unsigned int setNum = (addr >> shifts) & sets; // setNum is the vector index

	int tagShift = shifts + log(numSets) / log(2);	// number of tag shifts
	unsigned int tag = addr >> tagShift;
	
	for (int i = 0; i < cache[setNum].size(); i++)
	{
		if(cache[setNum][i].valid == 1 && cache[setNum][i].tag == tag) {
			return HIT;
		}
	}
	// FIFO replacement way
	// first erase the first block
	cache[setNum].erase(cache[setNum].begin());
	// create a new block with the given tag
	Block block;
	block.valid = 1;
	block.tag = tag;
	// push it from the end
	cache[setNum].push_back(block);
	return MISS;
}


char *msg[2] = {"Miss","Hit"};

#define		NO_OF_Iterations	1000000		// CHange to 1,000,000

int main()
{
	int lines = CACHE_SIZE / LINE_SIZE;
	int sets = lines / NOWays;
	// cache initialization
	for (int i = 0; i < sets; i++)
	{
		vector<Block> vob(NOWays);
		for (int j = 0; j < NOWays; j++)
		{
			vob[j].tag = 0;
			vob[j].valid = 0;
		}
		cache.push_back(vob);
	}

	unsigned int hit = 0;
	cacheResType r;
	
	unsigned int addr;
	cout << "Cache Simulator\n";

	for(int inst=0;inst<NO_OF_Iterations;inst++)
	{
		addr = memGenA();
		
		r = cacheSimDM(addr);
		if(r == HIT) 
			hit++;

		if(inst % 1000 == 0)
			cout <<"0x" << setfill('0') << setw(8) << hex << addr <<" ("<< msg[r] <<")\n";
	}
	cout << dec << "Hit Count = " << hit << " ,Miss Count = " << (NO_OF_Iterations - hit) << endl;
	cout << "Hit ratio = " << dec << (100.0*hit/NO_OF_Iterations) << "%" << endl;
	cout << "Miss ratio = " << dec << 100 - (100.0*hit/NO_OF_Iterations) << "%" << endl;

	system("pause");
}
