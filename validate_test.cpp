#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
using namespace std;

#define        DBG                1
#define        DRAM_SIZE        (64*1024*1024)
#define        CACHE_SIZE        (32*32)

enum cacheResType {MISS=0, HIT=1};

// my code
#define        LINE_SIZE        (32)
#define        NOWays            (1)

struct Block {
    unsigned int valid;
    unsigned int tag;
};

vector<vector<Block>> cache;

// Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
    int shifts = log(LINE_SIZE) / log(2);// number of shifts to exclude Block Number

    int lines = CACHE_SIZE / LINE_SIZE;        // number of lines (Blocks)
    int numSets = lines / NOWays;            // number of sets (ex: 512)

    int sets = numSets - 1;                    // get ones to and with it

    unsigned int setNum = (addr >> shifts) & sets; // setNum is the vector index

    int tagShift = shifts + log(numSets) / log(2);    // number of tag shifts
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

#define        NO_OF_Iterations    12       // CHange to 1,000,000

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
    
    int add[12] = {0, 4, 16, 132, 232, 160, 1024, 30, 140, 3100, 180, 2180};

    for(int inst=0;inst<NO_OF_Iterations;inst++)
    {
        addr = add[inst];
        
        r = cacheSimDM(addr);
        if(r == HIT)
            hit++;

        //if(inst % 1000 == 0)
            //cout <<"0x" << setfill('0') << setw(8) << hex << addr <<" ("<< msg[r] <<")\n";
    }
    cout << dec << "Hit Count = " << hit << " ,Miss Count = " << (NO_OF_Iterations - hit) << endl;
    cout << "Hit ratio = " << dec << (100.0*hit/NO_OF_Iterations) << "%" << endl;
    cout << "Miss ratio = " << dec << 100 - (100.0*hit/NO_OF_Iterations) << "%" << endl;

    system("pause");
}
