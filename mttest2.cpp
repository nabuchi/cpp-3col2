#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <list>
#include <algorithm>
#include <vector>
#include <functional>
#include <ctime>
#include "mt.h"

using namespace std;

#ifndef uint32
#define uint32 unsigned int
#endif

uint32 thash(uint32 m)
{
    uint32 ret;
    init_genrand( (unsigned long)m );
    ret = genrand_int32();
    return ret;
}

int main()
{
    uint32 seed;
    int count;
    cout << "種と、摘要回数を指定(空白で)";
    cin >> seed >> count;
    seed = (uint32)seed;

    printf( "%u:%u:%u\n", seed, thash(seed), thash(thash(seed)) );
    for(int i=0; i<count; i++) {
        seed = thash(seed); 
    }
    printf( "%u:\n", seed);
    return 0;
}

