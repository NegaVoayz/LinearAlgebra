#include "errordef.h"
#include "basis.h"
// process a rational number as p/q
class Quotient{
    private:
        int p;
        int q;
    public:
        void setNumerator(int argP) { p = argP; }
        void setDenominator(int argQ)
        {
            if(argQ == 0)
                throw ERROR::ZERO_DIVISOR;
            if(argQ<0)
            {
                p = -p;
                q = -argQ;
            }
            else
                q = argQ;
        }
        void setQuotient(int argP, int argQ) { p = argP; q = argQ; }
        int getNumerator() { Simplify();return p; }
        int getDenominator() { Simplify();return q; }
        float getQuotient() { return (float)p / (float)q; }


        void Simplify()
        {
            if( q==0 )
                throw ERROR::ZERO_DIVISOR;
            if( !p )
            {
                q=1;
                return;
            }
            if( q<0 )
            {
                p=-p;
                q=-q;
            }
            int t = __mastermind__::gcd(__mastermind__::abs(p), q);
            p /= t;
            q /= t;
            return;
        }
        Quotient inverse()
        {
            Quotient ans;
            if(p<0)
                ans.setQuotient(-q,-p);
            else
                ans.setQuotient(q,p);
            return ans;
        }
        Quotient(): p(0), q(1) {}
        Quotient(int argP, int argQ): p(argP), q(argQ){ Simplify(); }
        Quotient(int argP): p(argP), q(1){}
        const Quotient operator- () const
        {
            Quotient ans(-p,q);
            return ans;
        }
        const Quotient operator*(const Quotient &a) const
        {
            Quotient ans(a.p * p, a.q * q);
            return ans;
        }
        const Quotient &operator*=(const Quotient &a)
        {
            p*=a.p;
            q*=a.q;
            return *this;
        }
        const Quotient operator/(const Quotient &a) const
        {
            Quotient ans(a.q * p, a.p * q);
            return ans;
        }
        const Quotient &operator/=(const Quotient &a)
        {
            p*=a.q;
            q*=a.p;
            return *this;
        }
        const Quotient operator+(const Quotient &a) const
        {
            int t = q / __mastermind__::gcd(q, a.q) * a.q;
            Quotient ans(t /q *p + t /a.q *a.p, t);
            return ans;
        }
        const Quotient &operator+=(const Quotient &a)
        {
            int t = q / __mastermind__::gcd(q, a.q) * a.q;
            p=t /q *p + t /a.q *a.p;
            q=t;
            return *this;
        }
        const Quotient operator-(const Quotient &a) const
        {
            int t = q / __mastermind__::gcd(q, a.q) * a.q;
            Quotient ans(t /q *p - t /a.q *a.p, t);
            return ans;
        }
        const Quotient &operator-=(const Quotient &a)
        {
            int t = q / __mastermind__::gcd(q, a.q) * a.q;
            p=t /q *p - t /a.q *a.p;
            q=t;
            Simplify();
            return *this;
        }
        const Quotient &operator=(const Quotient &a)
        {
            p=a.p;
            q=a.q;
            return a;
        }
        const bool operator==(const Quotient &a) const
        {
            return (p*a.q == q*a.p);
        }
        const bool operator!=(const Quotient &a) const
        {
            return (p*a.q != q*a.p);
        }
        const bool operator<(const Quotient &a) const
        {
            return (p*a.q < q*a.p);
        }
        const bool operator>(const Quotient &a) const
        {
            return (a < *this);
        }
        const bool operator>=(const Quotient &a) const
        {
            return !(*this < a);
        }
        const bool operator<=(const Quotient &a) const
        {
            return !(a < *this);
        }


        #ifdef _GLIBCXX_OSTREAM
        friend std::ostream &operator<< (std::ostream &out, Quotient &A);
        #endif
        #ifdef _GLIBCXX_ISTREAM
        friend std::istream &operator>> (std::istream &in, Quotient &A);
        #endif
};
#ifdef _GLIBCXX_OSTREAM
std::ostream &operator<< (std::ostream &out, Quotient &A)
{
    A.Simplify();
    if(A.p>=0)
        out<<' ';
    out<<A.p;
    if(A.q != 1) out<<'/'<<A.q;
    return out;
}
#endif
#ifdef _GLIBCXX_ISTREAM
std::istream &operator>> (std::istream &in, Quotient &A)
{
    bool sgn = 0;
    char ch[24];
    in>>ch;
    A.p = 0;
    A.q = 0 ;
    size_t i = 0;
    if(ch[0] == '-')
    {
        ++i;
        sgn = 1;
    }
    for(;i<24 && ch[i]!=0;++i)
    {
        if(ch[i] == '/') break;
        A.p = A.p * 10 + ch[i] - '0';
    }
    if(ch[i] != '/')
    {
        A.q = 1;
    }
    else
    {
        ++i;
        if(ch[i] == '-')
        {
            ++i;
            sgn ^= 1;
        }
        for(;i<24 && ch[i]!=0;++i)
            A.q = A.q * 10 + ch[i] - '0';
    }
    if(sgn) A.p = -A.p;
    return in;
}
#endif