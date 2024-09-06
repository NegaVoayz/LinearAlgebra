__kernel void multiply( __global const int2* inp1,
                    __global const int2* inp2,
                    __global int2* out,
                    short m,
                    short n,
                    short p)
{
    int2 temp,ans;
    int a,b,t;
    const short posC = get_global_id(0);
    const short posR = get_global_id(1);
    short i;

    ans=0;
    for(i=0;i<n;++i)
    {
        temp = inp1[posC*m+i]*inp2[i*n+posR];
        ans.s0 = ans.s0 * temp.s1 + ans.s1 * temp.s0;
        ans.s1 = ans.s1 * temp.s1;
        a=ans.s0;
        b=ans.s1;
        if( a<b )
        {
            a^=b;
            b^=a;
            a^=b;
        }
        while(b)
        {
            t = a%b;
            a = b;
            b = t;
        }
        ans /= a;
    }
    out[posC*m + posR] = ans;
    return;
}