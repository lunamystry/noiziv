#ifndef NOIZIVORP_H
#define NOIZIVORP_H

#include<cmath>
#include<sstream>
#include<vector>
#include<complex>
#include<cstdlib>
#include<cstring>

#include<iostream>


typedef std::complex<double> Complex;

namespace noiziv{

    const double epsilon = 1e-14;

    //The matrix class: will handle matrices of all shapes and sizes
    class Matrix{
	private:

		inline void init(int m, int n){
			if(m==0)m=1; //default to 1x1 matrix
            if(n==0)n=m;
            nRows = m;
            nCols = n;
            M = new double[nRows*nCols];


            // Initiate print variables to defaults xYxY
            printType = 0;
            printPrecision = -99;
            printProperties = 0;
		}

    protected:
        double * M; //the matrix
        int nRows, nCols;   //size of the matrix

        bool isPD; 			//is the matrix positive definite?
        bool isTriu; 		//is the matrix upper triangular?
        bool isTril; 		//is the matrix lower triangular?
		bool isTridiagonal; //is the matrix tridiangular?
		bool isSym;  		//is the matrix symmetric?
        bool isDiagDom;     //is the matrix diagonally dominant?

        //static const double epsilon = 1e-17; //"precision" of our arithmetic

    public:

		// Decalre global print variables xYxY
        int printType;
        int printPrecision;
        int printProperties;

        //default constructor for matrices
        Matrix(int m = 0, int n=0, bool ism=false, bool iu=false, bool il=false, bool ipd=false, bool itd=false, bool iDD=false): isSym(ism), isTridiagonal(itd), isPD(ipd), isTriu(iu), isTril(il), isDiagDom(iDD){
            init(m,n);
        }

        //constructor from an array and a size spec
        Matrix(double *arr, int m, int n, bool ism=false, bool iu=false, bool il=false, bool ipd=false, bool itd=false, bool iDD=false): M(arr), nRows(m), nCols(n), isSym(ism), isTridiagonal(itd), isPD(ipd), isTriu(iu), isTril(il), isDiagDom(iDD){
            printType = 0;
            printPrecision = -99;
            printProperties = 0;
        }

		//constructor from a string and a size spec
		Matrix(std::string s, int m=0, int n=0){
			init(m,n);
			std::istringstream is(s);
			for(int i=0; i<nRows; i++) for(int j=0; j<nCols; j++) is >> M[j*nRows+i];
		}

        //copy-constructor for matrices
        Matrix(const Matrix& mc):  nRows(mc.nRows), nCols(mc.nCols), isPD(mc.isPD), isTriu(mc.isTriu), isTril(mc.isTril) {
            M = new double[nRows*nCols];
            memcpy(M, mc.M, nRows*nCols*sizeof(double));
        }

        //destructor for matrices
        ~Matrix(){
            delete []M;
        }

        //code to set and get properties
        inline void setUT(bool val=true){isTriu = val; }
        inline void setLT(bool val=true){isTril = val; }
		inline void setTdg(bool val=true){isTridiagonal = val; }
		inline void setSym(bool val=true){isSym = val; }
		inline void setPosDef(bool val=true) {isPD = val; }
		inline void setDDom(bool val=true) {isDiagDom = val;}
        inline bool getUT() const { return isTriu; }
        inline bool getLT() const { return isTril; }
		inline bool getTdg() const {return isTridiagonal; }
		inline bool getSym() const {return isSym;}
        inline bool getDDom() const {return isDiagDom;}
        inline bool getPosDef() const {return isPD;}


		//other Matrix methods

        Matrix& t(); //inplace transpose

        int num_rows() const { return nRows; }
        int num_cols() const { return nCols; }
        const int length() const { return nRows>nCols?nRows:nCols; }
        const std::pair<int, int> size() const {return std::make_pair(nRows, nCols);}

        //unary operators without side-effects

        const Matrix operator+()const;
        const Matrix operator-()const;

        //unary operators with side-effects

        const Matrix& operator++(); //prefix
        const Matrix operator++(int); //postfix
        const Matrix& operator--(); //prefix
        const Matrix operator--(int); //postfix

        //binary operators with no side effects

        const Matrix operator+(const Matrix& rv) const;
        const Matrix operator-(const Matrix& rv) const;
        const Matrix operator*(const Matrix& rv) const;
        const Matrix operator/(const Matrix& rv) const; //TODO: IMPLEMENT
        friend Matrix times(const Matrix &A, const Matrix &B); // element-wise product


        Matrix& operator=(const Matrix& rv);
        Matrix & operator+=(const Matrix& rv);
        Matrix & operator-=(const Matrix& rv);
        Matrix & operator*=(const Matrix& rv);
        Matrix& operator/=(const Matrix& rv); //TODO: IMPLEMENT
        Matrix& times(const Matrix& rv); //element-wise inplace product

        int operator==(const Matrix& rv) const;
        int operator!=(const Matrix& rv) const;
        int operator>(const Matrix& rv) const;
        int operator<(const Matrix& rv) const;

        inline void to_zero() { memset(M, 0, sizeof(double)*nRows*nCols); }

        //indexing elements of the matrix
        inline double& operator()(int i, int j)const { return M[j*nRows+i]; }
        inline double& operator()(int i)const {return M[i];}

        //Write the contents of a Matrix to an ostream
        void print(std::ostream& os) const;

        // Declare PrintSetup Functions xYxY
		void PrintSetup(int printType, int printPrecision , int printProperties);
		void PrintSetupType(int printType);
		void PrintSetupPrecision(int printPrecision);
		void PrintSetupProperties(int printProperties);

        //friend functions: operators between matrices and other types
        friend std::ostream& operator<<(std::ostream& os, const Matrix& M); //writing the matrix to an ostream
        friend std::istream& operator>>(std::istream& is, Matrix& M); //reading the matrix in from an istream

        //operators involving matricies and other things
        friend Matrix& operator*=(Matrix& M, double c); //scalar multiplication
        friend Matrix operator*(double c, const Matrix& M); // "
        friend Matrix operator*(const Matrix& M, double c); // "
        friend Matrix& operator+=(Matrix& M, double c); //addition to a constant
        friend Matrix operator+(double c, const Matrix& M); // addition to a constant
        friend Matrix operator+(const Matrix& M, double c); // addition to a constant
        friend Matrix& operator-=(Matrix& M, double c);     // subtraction...
        friend Matrix operator-(double c, const Matrix& M);
        friend Matrix operator-(const Matrix& M, double c);
        friend Matrix& operator/=(Matrix& M, double c); //scalar division
        friend Matrix operator/(const Matrix& M, double c); // "

        void Map(double (*fn)(double)); // in-place map function

        //basic matrix operations
        friend Matrix zeros(int m, int n=0); //matrix of zeros
        friend Matrix ones(int m, int n=0); //matrix of ones
        friend Matrix randM(int m, int n=0); //random matrix
        friend Matrix eye(int m); //identity matrix
        friend double tr(const Matrix& M); //trace of a matrix
        friend Matrix transpose(const Matrix& M); // transpose of a matrix
        friend Matrix triu(const Matrix& M, int k=0); //upper triangular piece of a matrix
        friend Matrix tril(const Matrix& M, int k=0); //lower triangular piece of a matrix
        friend Matrix diag(const Matrix& M, int k=0); //diagonal piece of a matrix (or if M is a vector, turn it into a diagonal matrix)
        friend Matrix floor(const Matrix& M);
        friend double l2norm(const Matrix& M);
        friend Matrix Map(double (*fn)(double),const Matrix& M);

    };

    //DECLARATIONS OF THE FRIEND FUNCTIONS AND OPERATORS FOR MATRIX
    std::ostream& operator<<(std::ostream& os, const Matrix& M);
    std::istream& operator>>(std::istream& is, Matrix& M);
    Matrix zeros(int m, int n);
    Matrix ones(int m, int n);
    Matrix randM(int m, int n);
    Matrix eye(int m);
    Matrix& operator*=(Matrix& M, double c);
    Matrix& operator/=(Matrix& M, double c);
    Matrix operator*(double c, const Matrix& M);
    Matrix operator*(const Matrix& M, double c);
    Matrix operator/(const Matrix& M, double c);
    Matrix& operator+=(Matrix& M, double c);
    Matrix operator+(const Matrix& M, double c);
    Matrix operator+(double c, const Matrix &M);
    Matrix& operator-=(Matrix& M, double c);
    Matrix operator-(const Matrix& M, double c);
    Matrix operator-(double c, const Matrix &M);
    Matrix times(const Matrix &A, const Matrix &B);
    double tr(const Matrix& M);
    Matrix transpose(const Matrix& M);
    Matrix triu(const Matrix& M, int k);
    Matrix tril(const Matrix& M, int k);
    Matrix diag(const Matrix& M, int k);
    Matrix floor(const Matrix& M);
    double l2norm(const Matrix& M);
    Matrix map(double (*fn)(double),const Matrix& M); // map a function onto the elements of a matrix


    //NUMERICS functions for acting on Matrices
    //They are not FRIENDS because we can access matrices fine from outside
    // Most of these are still NOT IMPLEMENTED!

    Matrix operator^(const Matrix& p, int e);  //Exponential matrix operator - matrixExp(const Matrix& p, int e);

	Matrix vcat(std::vector<Matrix&> v);//vertical concatenation: with an array.
	Matrix vcat(const Matrix& A, const Matrix& B, ...);//vertical concatenation: with two Matrices.
	Matrix hcat(std::vector<Matrix&> v);//horizontal concatenation (override): with an array.
	Matrix hcat(const Matrix& A, const Matrix& B);//horizontal concatenation (override): with two Matrices.

    Matrix sum(const Matrix& M, int rc= 1);
    Matrix prod(const Matrix& M, int rc = 1 );
    Matrix cumsum(const Matrix& M,  int rc = 1 );
    Matrix cumprod(const Matrix& M, int rc = 1 );
    Matrix diff( const Matrix& M, int n=1, int dim=0); //"diff" function for matrices

    Matrix linspace(double low, double hi, int n);
    Matrix range_int(int low, int hi, int step=1);
    Matrix range(double low, double hi, double step=1);
    Matrix logspace(double low, double hi, int n);

    std::vector<Matrix> sort( const Matrix& M, int rc ); //sort the entries of a Matrix

    Matrix randn(int n);
    Matrix randn(int n, int m);
    double randn();
    Matrix randomPermutaion(Matrix &pMatrix);

    //add other algorithms here...

} //namespace

#endif
