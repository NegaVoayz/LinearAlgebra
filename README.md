# Simple & Light Linear Algebra Header

contains only one header for definitions and one cpp file for realizations.

#### Stream I/O OK!

### 1. Requirements

Use **Matrix<T>** to create your own matrix with any type you want.
Likewise you can create your own **ColVector<T>** and **RowVector<T>**
since they are technically all matrices. XD

But for the type T, the **operators below is needed**:

> +, -, *, /
> +=, -=, *=, /=
> =, ==

T should be able to be compared with zero and able to be assigned an integer.

### 2. How To Include It ?

Of course use
```
        #include "Matrix.h"
```
and everything will be ok.
Other files? Just leave them in the same folder.

### 3. How To Use It ?

- First, about initializations:

```
         //default setting ---- empty
         Matrix<T>();

         //default setting ---- zero matrix
         Matrix<T>(const size_t& argHeight, const size_t& argWidth);

         //default setting ---- identity matrix
         Matrix<T>(const size_t& argSize );

         //copy ---- identical matrix
         Matrix<T>(const Matrix<T>& argMatrix );
```

- Second, about simple operations:

```
        const size_t &GetHeight();

        const size_t &GetWidth();

        void Resize(const size_t &argHeight, const size_t &argWidth);

        void SetColumn(const ColVector<T> &col, const size_t &pos);

        void SetRow(const RowVector<T> &row, const size_t &pos);

        ColVector<T> GetColumn(const size_t &c) const;

        RowVector<T> GetRow(const size_t &r) const;
```

- Third,  about linear algebra functions:

```
        Matrix<T> SubMatrix(
            const size_t &startRow,
            const size_t &startCol,
            const size_t &subHeight,
            const size_t &subWidth) const;

        Matrix<T> Transpose();

        Matrix<T> Elimination(ColVector<T> *b=nullptr) const;

        Matrix<T> Solution(ColVector<T> *argB=nullptr, ColVector<T> *specificSolution=nullptr) const;

        Matrix<T> Inverse() const;

        T Determinant() const;
```

### 4. Isn't it too simple ?

# YES

### 5. Extra service

A quotient class is included in the folder.
```
#include "Quotient.h"
```
to include and use.

- Features
   Functions:
```
        Quotient inverse();

        void Simplify();

        void setQuotient(int argP, int argQ);

        int getNumerator();

        int getDenominator();

        float getQuotient();

        void setNumerator(int argP);

        void setDenominator(int argQ);

```
    Stream I/O
    Integer assignment
    Fundamental opertions of arithmetic
