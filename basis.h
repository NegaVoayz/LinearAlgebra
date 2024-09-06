#pragma once

namespace __mastermind__{
    inline int gcd(const int &argA, const int &argB)
    {
        register int a=argA;
        register int b=argB;
        if( a<b )
        {
            a^=b;
            b^=a;
            a^=b;
        }
        while(b)
        {
            int t = a%b;
            a = b;
            b = t;
        }
        return a;
    }
    inline int abs(const int &a) { return (a<0? (-a):a); }
}