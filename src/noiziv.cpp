#include "noiziv.h"
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<iomanip>

namespace noiziv{

/* ***************************************************************************
                        MEMBER Functions for MATRIX
    ************************************************************************ */


    Matrix& Matrix::t(){ //inplace transpose
        // we accomplish this with a funky cyclic-decomposition
        double sw_1, sw_2;
        int i,I,J, cur, prev, sw;
        bool marked[nRows*nCols];
        memset(marked, 0, sizeof(bool)*nRows*nCols);

        for(i=0; i<nCols*nRows; i++)
        {
            if(!marked[i])
            {
                I = i%nRows;
                J = (i-I)/nRows;
                cur = I*nCols+J;
                prev = i;
                marked[i] = true;
                sw_1 = M[i];
                while(cur != i)
                {
                    marked[cur] = true;
                    sw_2 = sw_1; sw_1 = M[cur]; M[cur] = sw_2;
                    prev = cur;
                    I = cur%nRows;
                    J = (cur-I)/nRows;
                    cur = I*nCols+J;
                }
                M[cur] = sw_1;
            }
        }

        sw = nRows;
        nRows = nCols;
        nCols = sw;
        return *this;
    }

    const Matrix Matrix::operator+()const{ //unary "+"
        return *this;
    }

    const Matrix Matrix::operator-()const{ //unary "-"
        double *nM = new double[nRows*nCols];
        for(int i = 0; i< nRows*nCols; i++) nM[i] = -M[i];
        return Matrix(nM, nRows, nCols, isSym, isTriu, isTril, isPD, isTridiagonal, isDiagDom); // allow the compiler to optimize the object creation by creating in the return statement
    }

    const Matrix& Matrix::operator++(){ //Prefix
            for(int i = 0; i< nRows*nCols; i++) M[i]++;
            return *this;
    }

    const Matrix Matrix::operator++(int){ //Postfix
            double *nM = new double[nRows*nCols];
            memcpy(nM, M, nRows*nCols*sizeof(double));
            for(int i = 0; i< nRows*nCols; i++) M[i]++;
            return Matrix(nM, nRows, nCols, isSym, isTriu, isTril, isPD, isTridiagonal, isDiagDom); // allow the compiler to optimize the object creation by creating in the return statement
    }

    const Matrix& Matrix::operator--(){ //Prefix
            for(int i = 0; i< nRows*nCols; i++) M[i]--;
            return *this;
    }

    const Matrix Matrix::operator--(int){ //Postfix
            double *nM = new double[nRows*nCols];
            memcpy(nM, M, nRows*nCols*sizeof(double));
            for(int i = 0; i< nRows*nCols; i++) M[i]--;
            return Matrix(nM, nRows, nCols, isSym, isTriu, isTril, isPD, isTridiagonal, isDiagDom); // allow the compiler to optimize the object creation by creating in the return statement
    }

    const Matrix Matrix::operator+(const Matrix& rv) const{ //binary "+"
            if(rv.nRows !=nRows || rv.nCols != nCols){
                //TODO: HANDLE ERROR properly
                return *this;
            }
            double *nM = new double[nRows*nCols];
            for(int i=0; i<nRows*nCols; i++) nM[i] = M[i] + rv.M[i];
            return Matrix(nM, nRows, nCols, isSym && rv.isSym, isTriu && rv.isTriu, isTril && rv.isTril, isPD && rv.isPD, isTridiagonal && rv.isTridiagonal, isDiagDom && rv.isDiagDom); // allow the compiler to optimize the object creation by creating in the return statement
    }

    const Matrix Matrix::operator-(const Matrix& rv) const{
            if(rv.nRows !=nRows || rv.nCols != nCols){
                //TODO: HANDLE ERROR properly
                return *this;
            }
            double *nM = new double[nRows*nCols];
            for(int i=0; i<nRows*nCols; i++) nM[i] = M[i] - rv.M[i];
            return Matrix(nM, nRows, nCols, isSym && rv.isSym, isTriu && rv.isTriu, isTril && rv.isTril, isPD && rv.isPD, isTridiagonal && rv.isTridiagonal, isDiagDom && rv.isDiagDom); // allow the compiler to optimize the object creation by creating in the return statement
    }

    const Matrix Matrix::operator*(const Matrix& rv) const{ //binary "*"
            if(nRows==1 && nCols==1) return M[0]*rv;
            if(rv.nRows==1 && rv.nCols==1) return rv.M[0]*(*this);
            if(nCols!=rv.nRows){
                //TODO: HANDLE ERROR properly
                return *this;
            }
            double *nM = new double[nRows*rv.nCols];
            Matrix m2(nRows, rv.nCols, false, isTriu && rv.isTriu, isTril && rv.isTril);
            memset(nM, 0, nRows*rv.nCols*sizeof(double));
            for(int i=0; i<nRows; i++) for(int j=0; j<rv.nCols; j++)
            {
                for(int k=0; k<nCols; k++) nM[j*nRows+i] += M[k*nRows+i]*rv.M[j*rv.nRows+k];
            }

            return Matrix(nM, nRows, rv.nCols, isSym && rv.isSym, isTriu && rv.isTriu, isTril && rv.isTril, isPD && rv.isPD, isTridiagonal && rv.isTridiagonal, isDiagDom && rv.isDiagDom); // allow the compiler to optimize the object creation by creating in the return statement
    }

    const Matrix Matrix::operator/(const Matrix& rv) const{
            //TODO: Compute AB^-1
            return *this;
    }

    Matrix& Matrix::times(const Matrix& rv) { //element-wise product
            if(nRows!=rv.nRows || nCols != rv.nCols){
                //TODO: HANDLE ERROR HERE
                return *this;
            }
            for(int i=0; i<nRows*nCols; i++) M[i] *= rv.M[i];
            return *this;
    }



    Matrix& Matrix::operator=(const Matrix& rv){
            if(this == &rv) return *this;

            if(nRows!=rv.nRows || nCols!=rv.nCols){
                //dimension mismatch, we need to reallocate memory
                delete []M;
                M = new double[rv.nRows*rv.nCols];
                nRows = rv.nRows;
                nCols = rv.nCols;
            }
            isSym = rv.isSym;
            isTriu = rv.isTriu;
            isTril = rv.isTril;
            isPD = rv.isPD;
            isTridiagonal = rv.isTridiagonal;
            isDiagDom = rv.isDiagDom;
            memcpy(M, rv.M, nRows*nCols*sizeof(double));
            return *this;
    }

    Matrix&  Matrix::operator+=(const Matrix& rv) {
            if(nRows!=rv.nRows || nCols!=rv.nCols){
                //TODO: HANDLE ERROR HERE
                return *this;
            }
            for(int i=0;i<nRows*nCols;i++) M[i] += rv.M[i];
            isSym = isSym && rv.isSym;
            isTriu = isTriu && rv.isTriu;
            isTril = isTril && rv.isTril;
            isPD = isPD && rv.isPD;
            isTridiagonal = isTridiagonal && rv.isTridiagonal;
            isDiagDom = isDiagDom && rv.isDiagDom;
            return *this;
    }

    Matrix& Matrix::operator-=(const Matrix& rv) {
            if(nRows!=rv.nRows || nCols!=rv.nCols){
                //TODO: HANDLE ERROR HERE
                return *this;
            }
            for(int i=0;i<nRows*nCols;i++) M[i] -= rv.M[i];
            isSym = isSym && rv.isSym;
            isTriu = isTriu && rv.isTriu;
            isTril = isTril && rv.isTril;
            isPD = isPD && rv.isPD;
            isTridiagonal = isTridiagonal && rv.isTridiagonal;
            isDiagDom = isDiagDom && rv.isDiagDom;
            return *this;
    }

    Matrix& Matrix::operator*=(const Matrix& rv){
            if(nCols!=rv.nRows){
                //TODO: HANDLE ERROR HERE
                std::cerr << "Matrices are not compatible sizes" << std::endl;
                return *this;
            }
            if(nCols!=rv.nCols){
                 //will need to allocate new memory
                 //TODO: allocate the right memory...
                 std::cerr << "need new memory, not yet implemented" << std::endl;
                 return *this;
            }

            std::vector<double> tr(nCols);

            for(int i=0; i<nRows; i++) {
                for(int j=0; j<rv.nCols; j++)
                {
                    tr[j] = 0;
                    for(int k=0; k<nCols; k++) tr[j] += M[k*nRows+i]*rv.M[j*rv.nRows+k];
                }
                for(int j=0; j<rv.nCols; j++) M[j*nRows+i] = tr[j];
            }

            isSym = isSym && rv.isSym;
            isTriu = isTriu && rv.isTriu;
            isTril = isTril && rv.isTril;
            isPD = isPD && rv.isPD;
            isTridiagonal = isTridiagonal && rv.isTridiagonal;
            isDiagDom = isDiagDom && rv.isDiagDom;

            return *this;
    }

    Matrix& Matrix::operator/=(const Matrix& rv){
            //TODO
            return *this;
    }

    //Conditional operators

    int Matrix::operator==(const Matrix& rv) const{
            if(nRows!=rv.nRows || nCols != rv.nCols) return false;

            //test equality within epsilon
            for(int i=0; i<nRows*nCols; i++)
                if(M[i]-rv.M[i] > epsilon || M[i]-rv.M[i] < -epsilon)
                    return false;

            return true;
    }

    int Matrix::operator!=(const Matrix& rv) const{
            if(nRows!=rv.nRows || nCols != rv.nCols) return true;

            //test equality within epsilon
            for(int i=0; i<nRows*nCols; i++)
                if(M[i]-rv.M[i] > epsilon || M[i]-rv.M[i] < -epsilon)
                    return true;

            return false;
    }

    int Matrix::operator>(const Matrix& rv) const{
            if(nRows!=rv.nRows || nCols != rv.nCols) return false;

            //test equality within epsilon
            for(int i=0; i<nRows*nCols; i++)
                if(M[i]-rv.M[i] < -epsilon)
                    return false;

            return true;
    }

    int Matrix::operator<(const Matrix& rv) const{
            if(nRows!=rv.nRows || nCols != rv.nCols) return false;
Matrix times(const Matrix &A, const Matrix &B);
            //test equality within epsilon
            for(int i=0; i<nRows*nCols; i++)
                if(M[i]-rv.M[i] > epsilon)
                    return false;

            return true;
    }

    // Print Setup function
    void Matrix::PrintSetup(int Type, int Precision , int Properties)
	{
            PrintSetupType(Type);
            PrintSetupPrecision(Precision);
            PrintSetupProperties(Properties);
    }

    // Print Setup Type function
    void Matrix::PrintSetupType(int Type)
	{
            printType = Type;
    }

    // Print Setup Precision function
    void Matrix::PrintSetupPrecision(int Precision)
	{
            printPrecision = Precision;
    }

    // Print Setup Properties function
    void Matrix::PrintSetupProperties(int Properties)
	{
            printProperties = Properties;
    }

    //in-place Map function
    void Matrix::Map(double (*fn)(double))
    {
        for(int i = 0; i<nRows*nCols; i++) M[i] = fn(M[i]);
    }

    //Write the contents of a Matrix to an ostream
    void Matrix::print(std::ostream& os) const
    {
        if ( printProperties == 1 )
        {
            os << "\n\nMatrix Properties :\n";
            os << "\tThe Matrix has :\n";
            os << "\t\t" << nRows << " Rows\n";
            os << "\t\t" << nCols << " Columns\n";

            if (  isDiagDom  ) os << "\tMatrix is Diagonally Dominant\n";
            if (     isPD    ) os << "\tMatrix is Positive Definite\n";
            if (    isSym    ) os << "\tMatrix is Symmetric\n";
            if (   isTriu    ) os << "\tMatrix is Upper Triangular\n";
            if (   isTril    ) os << "\tMatrix is Lower Triangular\n";
            if (isTridiagonal) os << "\tMatrix is Tridiagonal\n";
        }Matrix times(const Matrix &A, const Matrix &B);

        int Precision = -1;
        bool flagPrintSpecial = false;
        int maxColumns = 80; // Max characters in a line in command prompt
        int max4Columns[nCols];
        int max;

        if ( printType == 0 )
        {

            // Print to Screen
            // Things are about to get wierd...
            int sizes[nRows][nCols];


            if ( printPrecision == -99 )
            {
                // Default is set, Auto-adjsut precision to try get it to fit to screen

                for ( int h = 5; h >= 0; h-- )
                {
                    int rowSum[nRows];

                    for(int i=0; i<nRows; i++)                  // Get the length of all the elements in the array
                    {
                        rowSum[i] = 0;
                        for(int j=0; j<nCols; j++)
                        {
                            std::ostringstream strs;
                            strs << std::setprecision(h) << M[j*nRows+i];   // Save what is actually going to be outputted, eg. 1.234e-16
                            std::string str = strs.str();                   // Make string equal to the above
                            sizes[i][j] = str.length();                     // Get the length of the actual output
                            rowSum[i] += ( sizes[i][j] + 3 );               // Sum the length of the element with a coupkle of spaces
                        }
                    }


                    // Get the max Row length for the current precision
                    max = 0;
                    for ( int row = 0; row < nRows; row++ )
                    {
                        if ( rowSum[row] > max )
                        {
                            max = rowSum[row];
                        }
                    }


                    // Now check if the length of the lengest row will fit in command prompt
                    if ( max <= maxColumns )
                    {
                        // If the length of the longest fits, we've got a maximum precision, lets go!
                        Precision = h;
                        break;
                    }
                    // Else: The row is too long, lets try a smaller precision..
                }

                if ( Precision == -1 )
                {
                    // Unfortunately, even the smallest of precisions would not allow a row to fit within the screen.. We are going to have a print a srolling matrix. Lets set the precision to 5, might aswell get some usefull data printed.
                    Precision = 5;

                    for(int i=0; i<nRows; i++)                  // Get the length of all the elements in the array
                    {
                        for(int j=0; j<nCols; j++)
                        {
                            std::ostringstream strs;
                            strs << std::setprecision(Precision) << M[j*nRows+i];        // Save what is actually going to be outputed, eg. 1.234e-16
                            std::string str = strs.str();                   // Make string equal to the above
                            sizes[i][j] = str.length();                     // Get the length of the actual output
                        }
                    }

                    flagPrintSpecial = true;
                }
            }
            else
            {
                // The Precision is set by the user, do what they say!
                Precision = printPrecision;

                // Lets check the length of the output with the set precision

                int rowSum[nRows];

                for(int i=0; i<nRows; i++)                  // Get the length of all the elements in the array
                {
                    rowSum[i] = 0;
                    for(int j=0; j<nCols; j++)
                    {
                        std::ostringstream strs;
                        strs << std::setprecision(Precision) << M[j*nRows+i];       // Save what is actually going to be outputed, eg. 1.234e-16
                        std::string str = strs.str();                          // Make string equal to the above
                        sizes[i][j] = str.length();                            // Get the length of the actual output
                        rowSum[i] += sizes[i][j] + 3;                          // Sum the length of the element with a couple of spaces
                    }
                }

                max = 0;

                // Get the max Row length for the current precision
                for ( int row = 0; row < nRows; row++ )
                {
                    if ( rowSum[row] > max )
                    {
                        max = rowSum[row];
                    }
                }

                // Now check if the length of the lengest row will fit in command prompt
                if ( max+10 >= maxColumns )
                {
                    // The row is toMatrix times(const Matrix &A, const Matrix &B);o long... We are going to have to print a scrolling matrix!
                    flagPrintSpecial = true;
                }

            }

            for( int t = 0; t < nCols; t++)
            {
                // Initialise max4Columns Array
                max4Columns[t] = 0;
            }

            // Ok, now we've determined the precision, lets look at printing some pretty matrices..


            int maxElement = 0;

            for( int s = 0; s < nRows; s++)
            {
               for( int t = 0; t < nCols; t++)
                {
                    if (sizes[s][t] > max4Columns[t])
                    {
                        // Lets get the widest column
                        max4Columns[t]= sizes[s][t];
                    }
                    if ( sizes[s][t] > maxElement )
                    {
                        // And the longest Element in the Matrix
                        maxElement = sizes[s][t];
                    }
                }
            }



            // Now to determin the spacing between the elements in the matrix

            int Spacing = 0, rowLength = 0;

            for ( int m = 0; m < nCols; m++)
            {
                rowLength = rowLength + max4Columns[m];     // Get the length of the entire row of output
            }

            if ( rowLength < maxColumns )                           // Determine amout of spacing
            {
                Spacing = ( ( maxColumns - rowLength ) / nCols );
                if ( Spacing < 1 )
                {
                    Spacing = 1;                            // If the result is less than 1. i.e. 0, make the spacing 1
                }
            }
            else
            {
                Spacing = 4;                    // Its going to wrap lines any way, might aswell make it neater
            }


            // Ok, now lets print..

            os << "\n";
            os << "\n";


            if ( flagPrintSpecial )
            {Matrix times(const Matrix &A, const Matrix &B);
                // The matrix is not going to fit, lets make a scrolling matrix.

                // We are going to assume that each element is maxElement long, we also making the spacing 5, make it nice and neat.

                int ElemPerColumn = ( maxColumns / ( ( maxElement + 5 ) ) );

                int scrollingColumns = (nCols/(ElemPerColumn -1)) + 1;

                if ( ElemPerColumn > 1 )
                {
                    ElemPerColumn--;
                }

                int currentCol = 0;

                char input;
                bool printedScrollingColumn = false;
                bool DispMenu = true;


                while ( 1 )
                {
                    // First print, if we havnt printed already, then wiat for user input
                    if ( !printedScrollingColumn   )
                    {
                        // Print the scrolling column
                        printedScrollingColumn = true;

                        int column = currentCol * ElemPerColumn;

                        os << "\n";
                        // Print Column Headings
                        os << std::endl << std::endl;

                        os << "r\\c ";
                        for(int j=column; j<=column+ElemPerColumn && j < nCols; j++)
                        {
                            os << j;
                            std::ostringstream colHeading;
                            colHeading << j;
                            std::string ColHeadingStr = colHeading.str();
                            for (int p = ColHeadingStr.length(); p < maxElement + Spacing ; p++)os << " ";  // Fill with spaces
                        }

                        os << std::endl;

                        for(int i=0; i<nRows; i++)
                        {
                            // Print row headings
                            os << i;
                            std::ostringstream rowHeading;
                            rowHeading << i;
                            std::string RowHeadingStr = rowHeading.str();
                            for (int p = RowHeadingStr.length(); p < 4 ; p++)os << " ";      // Fill with spaces

                            // Then the rest of the row
                            for(int j=column; j<=column+ElemPerColumn && j<nCols; j++)
                            {
                                    // No. Print the elements
                                    os << std::setprecision(Precision) << M[j*nRows+i];  // Output the element
                                    for (int p = sizes[i][j]; p < maxElement + Spacing ; p++)os << " ";     // Fill with spaces
                            }
                            os << std::endl << std::endl;
                        }

                        if ( DispMenu )
                        {
                              os << std::endl << "To view the columns up next, enter (n). \nTo view the previous columns, enter (p). \nTo navigate to the beginning of the matrix, enter (b). \nTo navigate to the end of the matrix, enter (e). \nTo jump to a specific column, enter (j) or (jxx) where xx is the column number \n\t you want to jump to.\nEnter (x) to quit printing the current matrix. \nTo hide this message, enter (h).";
                        }
                        os << "\n\nEnter your choice : ";
                    }

                    // Lets find out what the user wants to do..
                    std::cin >> input;
                    if ( input == 'x' )
                    {
                        // The users has had enough, lets leave..
                        return;
                    }
                    else if ( input == 'h' )
                    {
                        // The users feels they know how to use the program without a guide. OK..
                        DispMenu = false;
                        printedScrollingColumn = false;
                        os << "\n\n Menu hidden, to show it again, enter(s).\n\n";
                    }
                    else if ( input == 's' )
                    {
                        // They want the menu again...
                        DispMenu = true;
                        printedScrollingColumn = false;
                    }
                    else if ( input == 'p' )
                    {
                        // Lets go back one set of columns
                        printedScrollingColumn = false;
                        currentCol--;
                    }
                    else if ( input == 'n' )
                    {
                        // Now lets go to the nest set of columns
                        printedScrollingColumn = false;
                        currentCol++;
                    }

                   else if ( input == 'j' )
                   {
                        // The User's in a hurry, lets get them where they want to be..
                        os << "Column to jump to :  ";
                        int column;Matrix times(const Matrix &A, const Matrix &B);
                        std::cin >> column;
                        os << std::endl;

                        currentCol = ( column / ElemPerColumn );
                        printedScrollingColumn = false;
                    }

                    else if ( input == 'e' )
                    {
                        // To the end we go..
                        currentCol = scrollingColumns - 1;
                        printedScrollingColumn = false;
                    }
                   else if ( input == 'b' )
                   {
                        // And back to the beginning we go..
                        currentCol = 0;
                        printedScrollingColumn = false;
                    }

                    if ( currentCol >=  scrollingColumns )
                    {
                        // Where's this guy going?? We've reached the end already....
                        currentCol = scrollingColumns - 1;
                        printedScrollingColumn = false;
                        os << std::endl << std::endl << std::endl << std::endl;
                        os << "           !!!!!   End of matrix reached.   !!!!!";
                        os << std::endl << std::endl << std::endl << std::endl;
                    }
                    else if ( currentCol < 0 )
                    {
                        // Sigh.. Ok, lets notify them we cant display negative entrie...
                        currentCol = 0;
                        printedScrollingColumn = false;
                        os << std::endl << std::endl << std::endl << std::endl;
                        os << "      !!!!!!!   Beginning of matrix reached.   !!!!!!!";
                        os << std::endl << std::endl << std::endl << std::endl;
                    }
                }
            }
            else
            {
                // Print an ordianry matrix

                os << "\n";
                for(int i=0; i<nRows; i++)
                {
                    if(nCols>1)
                    {
                        for(int j=0; j<nCols; j++)
                        {
                            os << std::setprecision(Precision) << M[j*nRows+i];            // Output the element

                            if ( Spacing > 1 )
                            {
                                // Space columns out nicely
                                for (int p = sizes[i][j]; p < maxElement + Spacing ; p++)os << " ";  // Fill with spaces
                            }
                            else
                            {
                                // Space each column separately
                                for (int p = sizes[i][j]; p < max4Columns[j] + Spacing ; p++)os << " "; // Fill with spaces
                            }
                        }
                    }
                    else os << M[i];
                    os << "\n";
                    if(nRows>1)os << std::endl;
                }
            }
        }
        else if ( ( printType >= 1 ) && ( printType <= 3 ) )
        {
            // The user wants to print to file, lets make it nice for them
            if ( printType == 1 )
            {
                // Print to file, look nice
                Precision = printPrecision;

                int sizes2[nRows][nCols];
                int maxi = 0, Spacing = 5;

                for(int i=0; i<nRows; i++)                  // Get the length of all the elements in the array
                {
                    for(int j=0; j<nCols; j++)
                    {
                        std::ostringstream strs;
                        strs << std::setprecision(Precision) << M[j*nRows+i];

                        std::string str = strs.str();       // Make string equal to the above

                        sizes2[i][j] = str.length();         // Get the length of the actual output

                        if ( sizes2[i][j] > maxi )
                        {
                            maxi = sizes2[i][j];                  // Set the overall maximum
                        }
                    }
                }

                for(int i=0; i<nRows; i++)
                {
                    if(nCols>1)
                    {
                        for(int j=0; j<nCols; j++)
                        {
                            os << std::setprecision(Precision) <<  M[j*nRows+i];

                            // We not really pressed for space, lets make things nice..
                            for (int p = sizes2[i][j]; p < maxi + Spacing; p++)os << " ";    // Fill with spaces
                        }
                    }
                    else os << M[i];
                    os << "\n";
                    if(nRows>1)os << std::endl;
                }
            }
            else if ( printType == 2 )
            {
                // Print to file, TAB delimited
                for(int i=0; i<nRows; i++)
                {
                    if(nCols>1)
                    {
                        for(int j=0; j<nCols; j++)
                        {
                            os << std::setprecision(Precision) <<  M[j*nRows+i];
                            if ( j < nCols - 1 )
                            {
                                os << "\t";
                            }
                        }
                    }
                    else os << M[i];
                    //os << "\n";
                    if(nRows>1)os << std::endl;
                }
            }
            else if ( printType == 3 )
            {
                // Print to file, comma delimited, i.e. csv.
                for(int i=0; i<nRows; i++)
                {
                    if(nCols>1)
                    {
                        for(int j=0; j<nCols; j++)
                        {
                            os << std::setprecision(Precision) <<  M[j*nRows+i];
                            if ( j < nCols - 1 )
                            {
                                os << ",";
                            }
                        }
                    }
                    else os << M[i];
                    //os << "\n";
                    if(nRows>1)os << std::endl;
                }
            }
        }
    }



/* ***************************************************************************
                        FRIEND Functions for MATRIX
    ************************************************************************ */


    std::ostream& operator<<(std::ostream& os, const Matrix& M){ //writing the matrix to an ostream
        M.print(os);
        return os;
    }


    std::istream& operator>>(std::istream& is, Matrix& M){ //reading the matrix in from an istream
        for(int i=0; i<M.nRows; i++) for(int j=0; j<M.nCols; j++)
            is >> M.M[j*M.nRows+i];
        return is;
    }

    Matrix zeros(int m, int n){
        if(n==0)n=m;
        double *nM = new double[m*n];
        memset(nM, 0, m*n*sizeof(double));
        return Matrix(nM, m, n);
    }

    Matrix ones(int m, int n){
        if(n==0)n=m;
        double *nM = new double[m*n];
        for(int i = 0; i<m*n; i++) nM[i] = 1;
        return Matrix(nM, m, n);
    }
    Matrix randM(int m, int n){
        if(n==0)n=m;
        Matrix mat(m, n);
        double *nM = new double[m*n];
        for(int i=0; i<m*n; i++)nM[i] =((double)std::rand())/RAND_MAX;
        return Matrix(nM, m, n);
    }

    Matrix eye(int m){
        double *nM = new double[m*m];
        memset(nM, 0, m*m*sizeof(double));
        for(int i=0; i<m; i++) nM[i*m+i] = 1;
        return Matrix(nM, m, m);
    }

    Matrix& operator*=(Matrix& M, double c){
        for(int i=0; i<M.nRows*M.nCols; i++) M.M[i] *= c;
        return M;
    }

    Matrix& operator+=(Matrix& M, double c){
        for(int i=0; i<M.nRows*M.nCols; i++) M.M[i] += c;
        return M;
    }

    Matrix& operator-=(Matrix& M, double c){
        for(int i=0; i<M.nRows*M.nCols; i++) M.M[i] -= c;
        return M;
    }

    Matrix operator*(double c, const Matrix& M){
        double *nM = new double[M.nRows*M.nCols];
        for(int i=0; i<M.nRows*M.nCols; i++) nM[i] = M.M[i]*c;
        return Matrix(nM, M.nRows, M.nCols, M.isSym, M.isTriu, M.isTril, M.isPD, M.isTridiagonal, M.isDiagDom);
    }

    Matrix operator*(const Matrix& M, double c){
        double *nM = new double[M.nRows*M.nCols];
        for(int i=0; i<M.nRows*M.nCols; i++) nM[i] = M.M[i]*c;
        return Matrix(nM, M.nRows, M.nCols, M.isSym, M.isTriu, M.isTril, M.isPD, M.isTridiagonal, M.isDiagDom);
    }

    Matrix operator+(const Matrix& M, double c){
        double *nM = new double[M.nRows*M.nCols];
        for(int i=0; i<M.nRows*M.nCols; i++) nM[i] = M.M[i]+c;
        return Matrix(nM, M.nRows, M.nCols, M.isSym, M.isTriu, M.isTril, M.isPD, M.isTridiagonal, M.isDiagDom);
    }

    Matrix operator+(double c, const Matrix& M){
        double *nM = new double[M.nRows*M.nCols];
        for(int i=0; i<M.nRows*M.nCols; i++) nM[i] = M.M[i]+c;
        return Matrix(nM, M.nRows, M.nCols, M.isSym, M.isTriu, M.isTril, M.isPD, M.isTridiagonal, M.isDiagDom);
    }

    Matrix operator-(const Matrix& M, double c){
        double *nM = new double[M.nRows*M.nCols];
        for(int i=0; i<M.nRows*M.nCols; i++) nM[i] = M.M[i]-c;
        return Matrix(nM, M.nRows, M.nCols, M.isSym, M.isTriu, M.isTril, M.isPD, M.isTridiagonal, M.isDiagDom);
    }

    Matrix operator-(double c, const Matrix& M){
        double *nM = new double[M.nRows*M.nCols];
        for(int i=0; i<M.nRows*M.nCols; i++) nM[i] = c-M.M[i];
        return Matrix(nM, M.nRows, M.nCols, M.isSym, M.isTriu, M.isTril, M.isPD, M.isTridiagonal, M.isDiagDom);
    }

    Matrix& operator/=(Matrix& M, double c){
        for(int i=0; i<M.nRows*M.nCols; i++) M.M[i] /= c;
        return M;
    }

    Matrix operator/(const Matrix& M, double c){
        double *nM = new double[M.nRows*M.nCols];
        for(int i=0; i<M.nRows*M.nCols; i++) nM[i] = M.M[i]/c;
        return Matrix(nM, M.nRows, M.nCols, M.isSym, M.isTriu, M.isTril, M.isPD, M.isTridiagonal, M.isDiagDom);
    }

    Matrix times(const Matrix& A, const Matrix& B){
        if(A.nRows!=B.nRows || A.nCols != B.nCols){
            //TODO: HANDLE ERROR HERE
            return Matrix(0,0);
        }
        double *nM = new double[A.nRows*A.nCols];
        for(int i=0; i<A.nRows*A.nCols; i++) nM[i] = A.M[i]*B.M[i];
        return Matrix(nM, A.nRows, A.nCols, A.isSym && B.isSym, A.isTriu && B.isTriu, A.isTril && B.isTril, A.isPD && B.isPD, A.isTridiagonal && B.isTridiagonal, A.isDiagDom&& B.isDiagDom);
    }

    double tr(const Matrix& M){
        double ans = 0;
        for(int idx=0; idx<M.nRows*M.nCols; idx+=(M.nRows+1))ans += M.M[idx];
        return ans;
    }

    Matrix transpose(const Matrix& M){
        double *nM = new double[M.nCols*M.nRows];
        for(int i=0; i<M.nRows; i++)for(int j=0; j<M.nCols; j++) nM[i*M.nCols+j] = M.M[j*M.nRows+i];
        return Matrix(nM, M.nCols, M.nRows, M.isSym, M.isTril, M.isTriu, M.isPD, M.isTridiagonal, M.isDiagDom);
    }

    Matrix triu(const Matrix& M, int k){
        double *nM = new double[M.nRows*M.nCols];
        memset(nM, 0, M.nRows*M.nCols*sizeof(double));
        for(int i=0; i<M.nRows; i++) for(int j=i+k; j<M.nCols; j++) nM[j*M.nRows+i] = M.M[j*M.nRows+i];
        return Matrix(nM, M.nRows, M.nCols, false, true, M.isTril, M.isPD, M.isTridiagonal, M.isDiagDom);
    }

    Matrix tril(const Matrix& M, int k){
        double *nM = new double[M.nRows*M.nCols];
        memset(nM, 0, M.nRows*M.nCols*sizeof(double));
        for(int i=0; i<M.nRows; i++) for(int j=0; j<=i+k && j<M.nCols; j++) nM[j*M.nRows+i] = M.M[j*M.nRows+i];
        return Matrix(nM, M.nRows, M.nCols, false, M.isTriu, true, M.isPD, M.isTridiagonal, M.isDiagDom);
    }

    Matrix diag(const Matrix& M, int k){
        k = 0; //TODO: work for general k!
        int lng = M.nRows>M.nCols?M.nRows:M.nCols;
        if(lng == 1){ //go from vector to matrix
            double *nM = new double[lng*lng];
            memset(nM, 0, lng*lng*sizeof(double));
            for(int i=0; i<lng; i++) nM[i*lng+i] = M.M[i];
            return Matrix(nM, lng, lng);
        }
        else{ //go from matrix to vector
            lng = M.nRows+M.nCols-lng;
            double *nM = new double[lng];
            for(int i=0; i<lng; i++)nM[i] = M.M[i*M.nRows+i];
            return Matrix(nM, lng, 1);
        }
    }

    Matrix floor(const Matrix& M){
        double *nM = new double[M.nRows*M.nCols];
        for(int i = 0; i<M.nRows*M.nCols; i++) nM[i] = std::floor(M.M[i]);
        return Matrix(nM, M.nRows, M.nCols, M.isSym, M.isTriu, M.isTril, M.isPD, M.isTridiagonal, M.isDiagDom);
    }

    double l2norm(const Matrix& M){
        double ans = 0;
        for(int i=0; i<M.nRows*M.nCols; i++) ans += M.M[i]*M.M[i];
        return std::sqrt(ans);
    }

    Matrix Map(double (*fn)(double),const Matrix& M){
        double *nM = new double[M.nRows*M.nCols];
        for(int i=0; i<M.nRows*M.nCols; i++) nM[i] = fn(M.M[i]);
        return Matrix(nM, M.nRows, M.nCols);
    }

}

