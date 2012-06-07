#include "lq+.hh"
#include <iostream>

using namespace std;

int main()
{
    for(auto x : lq::from({ 1, 2, 3, 4, 5, 6, 7, 8 })/*.where([](int x) { return x > 5; })*/) {
        
    }
}