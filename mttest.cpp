#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <list>
#include <algorithm>
#include <vector>
#include <functional>
#include <ctime>
//#include<boost/random.hpp>

#include "mt.h"
//#include "md5Main.h"
using namespace std;
//using namespace boost;

#ifndef uint32
#define uint32 unsigned int
#endif


//s.c_str(); str to char
//str = chr chr to str
/*
string randString() 
{
    string ret;
    string set = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int rd;
    for(int i=0; i<30; i++) {
        rd = (int)(rand() * (set.length() + 0.0) / (1.0 + RAND_MAX) );
        ret[i] = set[rd];       
    }
    return ret;
}
*/
uint32 thash(uint32 m)
{
    uint32 ret;
    init_genrand((unsigned long)m);
    ret = genrand_int32();
    return ret;   
}
/*
uint32 thash2(uint32 m) {
    mt19937 gen(static_cast<unsigned long>(m));
    uniform_int<> dst1(INT_MIN, INT_MAX);
    variate_generator< mt19937,uniform_int<> > rand1(gen, dst1);
    uint32 ret = (uint32)rand1();
    return ret;
}
*/
int main()
{
    uint32 seed, seed2;
    int count;
    cout << "種と、摘要回数を指定(空白で)";
    cin >> seed >> count;
    seed = (uint32)seed;
    seed2 = seed;

    printf( "%u:%u:%u\n", seed, thash(seed), thash(thash(seed)) );
    for(int i=0; i<count; i++) {
        seed = thash(seed); 
    }
    printf( "%u:%u\n", seed, seed2);
    return 0;
}
         

