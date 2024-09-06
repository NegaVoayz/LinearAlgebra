#include "errordef.h"
#include "basis.h"
#include <stdlib.h>

template <typename T>
class Matrix;

template <typename T>
class ColVector;
template <typename T>
class RowVector;

template <typename T>
Matrix<T> operator+ (const Matrix<T> &A  , const Matrix<T> &B);
template <typename T>
Matrix<T> operator- (const Matrix<T> &A  , const Matrix<T> &B);
template <typename T>
Matrix<T> operator* (const T         &c  , const Matrix<T> &A);
template <typename T>
Matrix<T> operator* (const Matrix<T> &A  , const Matrix<T> &B);

#ifdef _GLIBCXX_OSTREAM
template <typename T>
std::ostream &operator<< (std::ostream &out, const Matrix<T> &A);
#endif
#ifdef _GLIBCXX_ISTREAM
template <typename T>
std::istream &operator>> (std::istream &in, Matrix<T> &A);
#endif

template <typename T>
class Matrix{
    protected:
        bool isSubMatrix;

        //I need data no more!
        void clear();

        //ask for space for data
        void allocate();

        //set the square matrix to a unit matrix
        void unitize();

        //create a submatrix with the same data space
        //NSFW function
        void __SubMatrix
            (Matrix<T> &ans,
            const size_t& startRow,
            const size_t& startCol,
            const size_t& subHeight,
            const size_t& subWidth) const;
        Matrix<T> __Elimination(ColVector<T> *b=nullptr, T *c=nullptr) const;
        void __is(Matrix<T> &A);
        void __timeadd (const Matrix<T> &A  , const Matrix<T> &B);
        T **data;

    public:
        size_t height, width;

        //default setting ---- empty
        Matrix<T>();

        //default setting ---- zero matrix
        Matrix<T>(const size_t& argHeight, const size_t& argWidth);

        //default setting ---- identity matrix
        Matrix<T>(const size_t& argSize );

        //copy setting ---- identical matrix
        Matrix<T>(const Matrix<T>& argMatrix );

        ~Matrix<T>();

        const size_t &GetHeight();
        const size_t &GetWidth();
        void Resize(const size_t &argHeight, const size_t &argWidth);

        T* operator[] (size_t s) const;
        const Matrix<T> &operator= (const Matrix<T> &B);
        const Matrix<T> &operator= (const T &c);
        Matrix<T> SubMatrix(
            const size_t &startRow,
            const size_t &startCol,
            const size_t &subHeight,
            const size_t &subWidth) const;
        void SetColumn(const ColVector<T> &col, const size_t &pos);
        void SetRow(const RowVector<T> &row, const size_t &pos);
        ColVector<T> GetColumn(const size_t &c) const;
        RowVector<T> GetRow(const size_t &r) const;
        Matrix<T> Transpose();
        Matrix<T> Elimination(ColVector<T> *b=nullptr) const;
        Matrix<T> Solution(ColVector<T> *argB=nullptr, ColVector<T> *specificSolution=nullptr) const;
        Matrix<T> Inverse() const;
        T Determinant() const;

        const Matrix<T> &operator+= (const Matrix<T> &A);
        const Matrix<T> &operator-= (const Matrix<T> &A);
        const Matrix<T> &operator*= (const T &c);
        friend Matrix<T> operator+ <T>(const Matrix<T> &A  , const Matrix<T> &B);
        friend Matrix<T> operator- <T>(const Matrix<T> &A  , const Matrix<T> &B);
        friend Matrix<T> operator* <T>(const T         &c  , const Matrix<T> &A);
        friend Matrix<T> operator* <T>(const Matrix<T> &A  , const Matrix<T> &B);
        #ifdef _GLIBCXX_OSTREAM
        friend std::ostream &operator<< <T>(std::ostream &out, const Matrix<T> &A);
        #endif
        #ifdef _GLIBCXX_ISTREAM
        friend std::istream &operator>> <T>(std::istream &in, Matrix<T> &A);
        #endif
};

template <typename T>
class ColVector: public Matrix<T>
{
    public:
        ColVector<T>();
        ColVector<T>(size_t s);
        void Resize(const size_t &argHeight);
        T &operator[] (const size_t &s) const;
        const ColVector operator= (const Matrix<T> &A);
        const ColVector operator= (const ColVector<T> &A);
};

template <typename T>
class RowVector: public Matrix<T>
{
    public:
        RowVector<T>();
        RowVector<T>(size_t s);
        void Resize(const size_t &argWidth);
        T &operator[] (const size_t &s) const;
        const RowVector &operator= (const Matrix<T> &A);
        const RowVector &operator= (const ColVector<T> &A);
};

#include "matrix.cpp"