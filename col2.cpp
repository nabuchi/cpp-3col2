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

#ifndef uint32
#define uint32 unsigned int
#endif

using namespace std;

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
struct GoalLess
{
    bool operator()( const uint32* li, const uint32* ri ) const
    {
        return li[1] < ri[1];
    }
};
struct StartLess
{
    bool operator()( const uint32* li, const uint32* ri ) const
    {
        return li[0] < ri[0];
    }
};

const uint32 M = pow(2,32*2/3);
const uint32 N_P = pow(2,32/3);
const uint32 FUKA = 4000;//N_P / 1 
const int COL = 3;
const uint32 L_MAX = 20*pow(2,32/3);
//const uint32 INT_MAX = 0xffffffff;
int main()
{
  //前半
  vector<uint32*> table;
  init_genrand((unsigned long)time(NULL));
  for(int i=0;i<FUKA;i++) {
    uint32 s = i*i;//genrand_int20();
    //printf("=========%u:%u\n", s, i);
    uint32 a = s;
    uint32 l = 0;
    uint32* factor;
    while( l<L_MAX ) {
        a = thash(a);
        //printf("1:%u:%u:%u\n",a,M,s);
        l++;
        if( a < M ) {
            factor = new uint32[3];
            factor[0] = s;
            factor[1] = a;
            factor[2] = l;
            table.push_back(factor);
            //if (i%100 == 0) {printf("1.5(%d):%u:%u\n",i,a,l); }
            //printf("1.5(%d):%u:%u\n",i,a,l);
            break;
        }
    }
  }
    sort(table.begin(), table.end(), GoalLess() );
    
    //print
    vector<uint32*>::iterator it = table.begin();
    while( it != table.end() ) {
        uint32* tmp = *it;
        //printf("2:%u:%u:%u\n",tmp[0],tmp[1],tmp[2]);
        //printf("%u\n",(*it)[2]);
        ++it;
    }

    //後半
    printf("後半\n");
    vector<uint32*> eqlchk;
    it = table.begin();
    //すべてのテーブルについて
    while( it != table.end() ) {
        vector<uint32*>::iterator jt = it+1;
        //ソート済みのテーブルから同じ値のCOL連続を見つける
        while( jt != table.end() && (*jt)[1] == (*it)[1] ) {
            //printf("icchi:%u\n", (*jt)[1]);
            ++jt;
        }
        if( jt >= it+COL ) {//一個多くなかったぴったり
            //========== jtは使っちゃだめ絶対 ==========
            //printf("COL連続見つかった%u=%u  %d\n", (*it)[1], (*(it+1))[1], jt-it);
            // 最大のLを見つける
            uint32 L = 0;
            vector<uint32*>::iterator tt = it;
            while( tt <= jt-1) {
                if( L < (*tt)[2] )
                    L = (*tt)[2];
                ++tt;
            }
            printf( "%u\n", L );
            //一致するチェーンの長さを揃えて衝突を見つける
            //(*tt)[0]:start (*tt)[1]:goal
            //(*tt)[0]:注目地点 (*tt)[1]:一個前の値
            for( int l=L; l>=0; l-- ) {
                //if(l%100==0) {printf("%d\n", l);}
                //if(l < 1055) {printf("%d\n", l);}
                if( !eqlchk.empty() )
                    eqlchk.clear();
                for( tt=it; tt<=jt-1; ++tt) {
                    if( (*tt)[2]>=l ) {
                        //printf("%u:%u:%u\n", (*tt)[0], (*tt)[1], (*tt)[2]);
                        (*tt)[1] = (*tt)[0];
                        (*tt)[0] = thash( (*tt)[0] );
                        eqlchk.push_back( *tt );
                    }
                }

                //同じ値から衝突してないかチェキ
                //注目してるチェーンがCOL個以上なら
                if( eqlchk.size() >= COL ) {
                    //第一要素でソート
                    sort(eqlchk.begin(), eqlchk.end(), StartLess() );
                    //eqlchkを舐める
                    tt = eqlchk.begin();
                    while( tt != eqlchk.end() ) {
                        //printf("%u\n",(*tt)[0]);
                        vector<uint32*>::iterator ut = tt+1;
                        while( ut != eqlchk.end() && (*ut)[0] == (*tt)[0] )
                            ++ut;
                        //同じ値がCOL個以上あったら
                        list<uint32> candidate;
                        candidate.clear();
                        if ( ut >= tt+COL ) { //utは使っちゃだめ絶対
                            vector<uint32*>::iterator vt = tt;
                            while( vt <= ut-1 ) {//utは使っちゃだめ絶対
                                //printf("%u From %u\n",(*vt)[0],(*vt)[1]);
                                //listに突っ込む
                                candidate.push_back( (*vt)[1] );
                                ++vt;
                                //printf("owari\n");
                            }
                            //COL個以上ユニークなら出力
                            candidate.sort();
                            candidate.unique();
                            if( candidate.size() >= COL ) {
                                for(list<uint32>::iterator ii=candidate.begin(); ii != candidate.end(); ++ii)
                                    printf("%u From %u\n",thash(*ii), *ii);
                            }
                        }
                        ++tt;
                    }
                }//衝突チェック終わり
                //if(l<1055) {printf("入ってない\n");}
            }//長さを変えて衝突を見つける
        }//候補チェーンの探索終わり
        it = jt;
    }//次の候補を見つける
    return 0;
}
         

