//----------------Matrix----------------

//I need data no more!
template <typename T>
void Matrix<T>::clear()
{
    //if there actually IS a matrix.
    if( data == nullptr )
        return;
    
    //free the matrix
    register size_t i;
    if( !isSubMatrix )
        for(i=0;i<height;++i)
            delete []data[i];
    free(data);
    data = nullptr;
    return;
}

//ask for space for data
template <typename T>
void Matrix<T>::allocate()
{
    register size_t i;
    //free the current Matrix
    clear();
    isSubMatrix = 0;
    //assign space for the new Matrix
    data = (T**) malloc(height * sizeof(T*));
    for(i=0;i<height;++i)
        data[i] = new T[width];
    return;
}

//set the square matrix to a unit matrix
template <typename T>
void Matrix<T>::unitize()
{
    register size_t i;
    for(i=0;i<height;++i)
        data[i][i]=1;
    return;
}

//create a submatrix with the same data space
//NSFW function
template <typename T>
void Matrix<T>::__SubMatrix
    (Matrix<T> &ans,
    const size_t& startRow,
    const size_t& startCol,
    const size_t& subHeight,
    const size_t& subWidth)
const {
    register size_t i;
    ans.clear();
    ans.isSubMatrix = 1;
    ans.height = subHeight;
    ans.width = subWidth;
    ans.data = (T**) malloc(subHeight * sizeof(T*));
    for(i=0;i<subHeight;++i)
        ans.data[i] = data[i+startRow]+startCol;
    return;
}

template <typename T>
Matrix<T> Matrix<T>::__Elimination(ColVector<T> *b, T *c) const
{
    //b must have the same height as A
    //to form the equation Ax=b
    if(b && b->height != height)
        throw ERROR::INVALID_ARGUMENT;
    if(c) (*c)=1;

    Matrix<T> U(*this);
    // i, j for row numbers
    // k for column numbers
    register size_t i,j,k,pivot;

    //i refers to the row number of which row we are now finding the pivot
    for(i=pivot=0;i<height && pivot<width;++i,++pivot)
    {
        //when the pivot position is a zero, unfortunately, we have to add another row to it.
        if( U[i][pivot] == 0 )
        {
            //find the desired row which has a non-zero number in the pivot position
            //j refers to the row number of the desired row
            j=i+1;
            while( j < height && U[j][pivot] == 0 ) { ++j; }
            //if j is height, which means there's no such a row, we failed.
            if(j!=height)
            {
                //now add the rows to solve the problem
                if(b) (*b)[i] += (*b)[j];
                for(k=pivot;k<width;++k)
                    U[i][k] += U[j][k];
            }
            //if we found a suitable row
            //add it to the faulty one
            else
            {
                //in the next cycle, i will be increased by 1, so will pivot.
                //so just decrease i by 1 and the total outcome is ++pivot only.
                //which is our purpose.
                --i;
                continue;
            }
            //since the pivot has to be zero this time
            //skip the elimination cuz it's impossible
        }
        //here we've overcome the zero pivot problem

        //Firstly unitize the current row
        if(b) (*b)[i] /= U[i][pivot];
        if(c) (*c) *= U[i][pivot];
        //k is used to traverse the row
        for(k=pivot+1;k<width;++k)
            if(U[i][k] != 0)
                U[i][k] /= U[i][pivot];
        U[i][pivot] = 1;

        //now start unitize the pivot column (set it to all zero but 1 as the pivot)
        //j refers to row number again.
        for(j=0;j<height;++j)
            //the pivot itself and the zeros need no change.
            if(j != i && U[j][pivot] != 0)
            {
                //subtract row j by pivot row
                if(b) (*b)[j] -= (*b)[i]*U[j][pivot];
                //k used to traverse the row
                for(k=pivot+1;k<width;++k)
                    if(U[i][k] != 0)
                        U[j][k] -= U[i][k]*U[j][pivot];
                U[j][pivot] = 0;
            }
    }
    return U;
}

//set this matrix an alias of matrix A
//NSFW function
template <typename T>
void Matrix<T>::__is(Matrix<T> &A)
{
    clear();
    isSubMatrix = 1;
    height = A.height;
    width = A.width;
    data = A.data;
}

template <typename T>
void Matrix<T>::__timeadd (const Matrix<T> &A  , const Matrix<T> &B)
{
//------------------------------------------------------NEED GPU------------------------------------------------------
    if(A.height < 4 || A.width < 4 || B.width < 4)
    {
        register size_t i,j,k;
        for(i=0;i<A.height;++i)
            for(j=0;j<B.width;++j)
                for(k=0;k<A.width;++k)
                    data[i][j] += A[i][k] * B[k][j];
        return;
    }
    //let's tear it up
    //get size of 1st block
    //---------------------------------------FIXED-------------------------------------------
    
    //A blocks: mxn
    //B blocks: nxp
    register size_t m,n,p;
    m = (A.height+1) >> 1;
    n = (A.width+1) >> 1;
    p = (B.height+1) >> 1;

    Matrix<T> A1,A2,A3,A4;
    Matrix<T> B1,B2,B3,B4;
    Matrix<T> C1,C2,C3,C4;
    //tear matrixA
    A.__SubMatrix(A1,0,0,m,n);
    A.__SubMatrix(A2,0,n,m,A.width-n);
    A.__SubMatrix(A3,m,0,A.height-m,n);
    A.__SubMatrix(A4,m,n,A.height-m,A.width-n);
    //tear matrixB
    B.__SubMatrix(B1,0,0,n,p);
    B.__SubMatrix(B2,0,p,n,B.width-p);
    B.__SubMatrix(B3,n,0,B.height-n,p);
    B.__SubMatrix(B4,n,p,B.height-n,B.width-p);
    //tear matrixC(answer matrix)
    __SubMatrix(C1,0,0,m,p);
    __SubMatrix(C2,0,p,m,width-p);
    __SubMatrix(C3,m,0,height-m,p);
    __SubMatrix(C4,m,p,height-m,width-p);
    //calculate parts
    C1.__timeadd(A1,B1);
    C1.__timeadd(A2,B3);

    C2.__timeadd(A1,B2);
    C2.__timeadd(A2,B4);

    C3.__timeadd(A3,B1);
    C3.__timeadd(A4,B3);

    C4.__timeadd(A3,B2);
    C4.__timeadd(A4,B4);

    return;
}

//default setting ---- empty
template <typename T>
Matrix<T>::Matrix(): height(0), width(0), isSubMatrix(0), data(nullptr) {}

//default setting ---- zero matrix
template <typename T>
Matrix<T>::Matrix(const size_t& argHeight, const size_t& argWidth):
    height(argHeight), width(argWidth), isSubMatrix(0), data(nullptr) { allocate(); }

//default setting ---- identity matrix
template <typename T>
Matrix<T>::Matrix(const size_t& argSize ):
    height(argSize), width(argSize), isSubMatrix(0), data(nullptr) { allocate(); unitize(); }

//copy setting ---- identical matrix
template <typename T>
Matrix<T>::Matrix(const Matrix<T>& argMatrix ):
    height(argMatrix.height), width(argMatrix.width), isSubMatrix(0), data(nullptr)
{ allocate(); *this = argMatrix; }

template <typename T>
Matrix<T>::~Matrix() { clear(); }

template <typename T>
void Matrix<T>::Resize(const size_t &argHeight, const size_t &argWidth)
{
    if(height != argHeight || width != argWidth)
    {
        clear();
        height=argHeight;
        width=argWidth;
        allocate();
    }
}

template <typename T>
const size_t &Matrix<T>::GetHeight() { return height; }

template <typename T>
const size_t &Matrix<T>::GetWidth() { return width; }

template <typename T>
T* Matrix<T>::operator[] (size_t s) const{ return data[s]; }

template <typename T>
const Matrix<T> &Matrix<T>::operator= (const Matrix<T> &B)
{
    Resize(B.height, B.width);
    register size_t i,j;
    for(i=0;i<height;++i)
        for(j=0;j<width;++j)
            data[i][j]=B.data[i][j];
    return B;
}

template <typename T>
const Matrix<T> &Matrix<T>::operator= (const T &c)
{
    if(height != 1 || width != 1)
    {
        clear();
        height=1;
        width=1;
        allocate();
    }
    data[0][0]=c;
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::SubMatrix(
    const size_t &startRow,
    const size_t &startCol,
    const size_t &subHeight,
    const size_t &subWidth) const
{
    Matrix<T> ans(subHeight, subWidth);
    register size_t i,j;
    for(i=0;i<subHeight;++i)
        for(j=0;j<subWidth;++j)
            ans[i][j] = data[i+startRow][j+startCol];
    return ans;
}

template <typename T>
void Matrix<T>::SetColumn(const ColVector<T> &col, const size_t &pos)
{
    register size_t i;
    for(i=0;i<height;++i)
        data[i][pos] = col[i];
    return;
}

template <typename T>
void Matrix<T>::SetRow(const RowVector<T> &row, const size_t &pos)
{
    register size_t i;
    for(i=0;i<width;++i)
        data[pos][i] = row[i];
    return;
}

//return a column vector
template <typename T>
ColVector<T> Matrix<T>::GetColumn(const size_t &c) const
{
    ColVector<T> ans(height);
    register size_t i;
    for(i=0;i<height;++i)
        ans[i]=data[i][c];
    return ans;
}

//return a row vector
template <typename T>
RowVector<T> Matrix<T>::GetRow(const size_t &r) const
{
    RowVector<T> ans(width);
    register size_t i;
    for(i=0;i<width;++i)
        ans[i]=data[r][i];
    return ans;
}

template <typename T>
Matrix<T> Matrix<T>::Transpose()
{
    Matrix<T> ans(width, height);
    register size_t i,j;
    for(i=0;i<width;++i)
        for(j=0;j<height;++j)
            ans[i][j]=data[j][i];
    return ans;
}

template <typename T>
Matrix<T> Matrix<T>::Elimination(ColVector<T> *b) const
{
    return __Elimination(b,nullptr);
}

template <typename T>
Matrix<T> Matrix<T>::Solution(ColVector<T> *argB, ColVector<T> *specificSolution) const
{
    //store positions of free variables
    ColVector<bool> freeVariables(width);
    ColVector<T> *b = nullptr;
    if(argB)
    {
        b = new ColVector<T>;
        (*b) = (*argB);
    }
    Matrix<T> U;
    Matrix<T> ans(1,1);
    register size_t i,j,k,cnt;
    U=__Elimination(b, nullptr);
    if(specificSolution != nullptr)
        specificSolution->Resize(width);
    //start
/*
1.find free variables
2.set vector:
    pivot pos: copy
    free pos: zero
    self pos: one
*/
    i=0;
    j=0;
    cnt=0;
    while(j<width && U[0][j]==0)
    {
        freeVariables[j]=1;
        ++cnt;
        ++j;
    }
    if(specificSolution != nullptr && j < width) (*specificSolution)[j]=(*b)[i];
    while(i<height && j<width)
    {
        //if the next pivot is just right
        while(i<height-1 && j<width-1 && U[i+1][j+1]==1)
        {
            //one step forward
            ++i;
            ++j;
            if(specificSolution) (*specificSolution)[j]=(*b)[i];
        }
        ++j;
        if(j==width)
            break;
        freeVariables[j]=1;
        ++cnt;
    }
    if(!cnt)
    {
        ans.Resize(1,1);
        return ans;
    }
    ans.Resize(width,cnt);
    i=0;
    while(cnt)
    {
        --cnt;
        while(freeVariables[i]==0) ++i;
        k=0;
        for(j=0;j<width;++j)
            if(freeVariables[j]==0)
                ans[j][cnt]=-U[k++][i];
        ans[i][cnt]=1;
        ++i;
    }
    return ans;
}

template <typename T>
Matrix<T> Matrix<T>::Inverse() const
{
    if(width != height)
        throw ERROR::UNSUITED_SIZE;
    Matrix<T> U(*this), I(height);
    register size_t i,j,k;
    T temp;
    for(i=0;i<height;++i)
    {
        if( U[i][i] == 0 )
        {
            j=i+1;
            while( j < height && U[j][i] == 0 ) { ++j; }
            if(j!=height)
                for(k=0;k<width;++k)
                {
                    U[i][k] += U[j][k];
                    I[i][k] += I[j][k];
                }
            //if we really find a suitable row
            //then just add it to the faulty one
            else
                throw ERROR::INVALID_ARGUMENT;
            //since the pivot has to be zero this time
            //it has no inverse
        }
        
        for(j=i+1;j<height;++j)
            if(U[j][i] != 0)
            {
                if(U[j][i] == 0) continue;
                temp = U[j][i];
                U[j][i] = 0;
                for(k=i+1;k<width;++k)
                    if(U[i][k] != 0)
                    {
                        U[i][k] /= U[i][i];
                        I[j][k] /= U[i][i];
                        U[j][k] -= U[i][k]*temp;
                        I[j][k] -= I[i][k]*temp;
                    }
            }
    }
    return I;
}

template <typename T>
T Matrix<T>::Determinant() const
{
    if(height != width)
        throw ERROR::INVALID_ARGUMENT;
    Matrix<T> U;
    T ans;
    U=__Elimination(nullptr, &ans);
    register size_t i;
    for(i=0;i<height;++i)
        ans *= U[i][i];
    return ans;
}

//----------------column vector----------------
template <typename T>
ColVector<T>::ColVector(): Matrix<T>() {}

template <typename T>
ColVector<T>::ColVector(size_t s): Matrix<T>(s,1) {}

template <typename T>
void ColVector<T>::Resize(const size_t &argHeight) { Matrix<T>::Resize(argHeight, 1); }

template <typename T>
T &ColVector<T>::operator[] (const size_t &s) const{ return Matrix<T>::data[s][0]; }

template <typename T>
const ColVector<T> ColVector<T>::operator= (const Matrix<T> &A)
{
    if(A.width != 1)
        throw ERROR::UNSUITED_SIZE;
    Resize(A.height);
    register size_t i;
    for(i=0;i<Matrix<T>::height;++i)
        Matrix<T>::data[i][0] = A[i][0];
    return *this;
}

template <typename T>
const ColVector<T> ColVector<T>::operator= (const ColVector<T> &A)
{
    Resize(A.height);
    register size_t i;
    for(i=0;i<Matrix<T>::height;++i)
        Matrix<T>::data[i][0] = A[i];
    return *this;
}

//----------------row vector----------------
template <typename T>
RowVector<T>::RowVector(): Matrix<T>() {}

template <typename T>
RowVector<T>::RowVector(size_t s): Matrix<T>(1,s) {}

template <typename T>
void RowVector<T>::Resize(const size_t &argWidth) { Matrix<T>::Resize(1, argWidth); }

template <typename T>
T &RowVector<T>::operator[] (const size_t &s) const{ return Matrix<T>::data[0][s]; }

template <typename T>
const RowVector<T> &RowVector<T>::operator= (const Matrix<T> &A)
{
    if(A.width != 1)
        throw ERROR::UNSUITED_SIZE;
    Resize(A.width);
    register size_t i;
    for(i=0;i<Matrix<T>::width;++i)
        Matrix<T>::data[0][i] = A[0][i];
    return *this;
}

template <typename T>
const RowVector<T> &RowVector<T>::operator= (const ColVector<T> &A)
{
    Resize(A.width);
    register size_t i;
    for(i=0;i<Matrix<T>::width;++i)
        Matrix<T>::data[0][i] = A[0][i];
    return *this;
}

//----------------matrix algebra----------------
template <typename T>
const Matrix<T> &Matrix<T>::operator+= (const Matrix<T> &A)
{
    //whether sizes match
    if( A.height != height || A.width != width )
        throw ERROR::UNSUITED_SIZE;
    //add matrices
    register size_t i,j;
    for(i=0;i<height;++i)
        for(j=0;j<width;++j)
            data[i][j] += A[i][j];
    return *this;
}

template <typename T>
const Matrix<T> &Matrix<T>::operator-= (const Matrix<T> &A)
{
    //whether sizes match
    if( A.height != height || A.width != width )
        throw ERROR::UNSUITED_SIZE;
    //add matrices
    register size_t i,j;
    for(i=0;i<height;++i)
        for(j=0;j<width;++j)
            data[i][j] -= A[i][j];
    return *this;
}

template <typename T>
const Matrix<T> &Matrix<T>::operator*= (const T &c)
{
    register size_t i,j;
    for(i=0;i<height;++i)
        for(j=0;j<width;++j)
            data[i][j] *= c;
    return *this;
}

template <typename T>
Matrix<T> operator+ (const Matrix<T> &A  , const Matrix<T> &B)
{
    Matrix<T> ans(A);
    ans+=B;
    return ans;
}
template <typename T>
Matrix<T> operator- (const Matrix<T> &A  , const Matrix<T> &B)
{
    Matrix<T> ans(A);
    ans-=B;
    return ans;
}
template <typename T>
Matrix<T> operator* (const T         &c  , const Matrix<T> &A)
{
    Matrix<T> ans(A);
    ans *= c;
    return ans;
}
template <typename T>
Matrix<T> operator* (const Matrix<T> &A  , const Matrix<T> &B)
{
    //whether sizes match
    if( A.width != B.height )
        throw ERROR::UNSUITED_SIZE;

    Matrix<T> C(A.height, B.width);
    C.__timeadd(A,B);
    return C;
}


#ifdef _GLIBCXX_OSTREAM
template <typename T>
std::ostream &operator<< (std::ostream &out, const Matrix<T> &A)
{
    register size_t i,j;
    if(A.height==1)
    {
        out<<'['<<A[0][0];
        for(j=1;j<A.width;++j)
            out<<'\t'<<A[0][j];
        out<<"]\n";
        return out;
    }
    out<<"["<<A[0][0];
    for(j=1;j<A.width;++j)
        out<<'\t'<<A[0][j];
    out<<"]"<<'\n';
    for(i=1;i<A.height-1;++i)
    {
        out<<'|'<<A[i][0];
        for(j=1;j<A.width;++j)
            out<<'\t'<<A[i][j];
        out<<'|'<<'\n';
    }
    //now i = A.height-1
    out<<"["<<A[i][0];
    for(j=1;j<A.width;++j)
        out<<'\t'<<A[i][j];
    out<<"]"<<'\n';
    return out;
}
#endif
#ifdef _GLIBCXX_ISTREAM
template <typename T>
std::istream &operator>> (std::istream &in, Matrix<T> &A)
{
    register size_t i,j;
    for(i=0;i<A.height;++i)
        for(j=0;j<A.width;++j)
            in>>A[i][j];
    return in;
}
#endif