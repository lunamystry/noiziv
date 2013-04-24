#include<iostream>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include "noiziv.h"

using namespace std;
using namespace noiziv;

// TESTS
// NO timing currently performed by tests. Should add timing to all tests & record times in logfile. [LESIBA!]
void tests()
{
    const int nTestsEach = 10;
    srand(time(NULL));
    ofstream logfile("noiziv_base.log");

    int nTests = 0;
    int nPassed = 0;

    int a, b;
    Matrix *Mptr;

    // test default constructor & basic Matrix operations
    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;
        nTests++;
        logfile << nTests << ". Constructing a " << a << "x" <<b << " matrix on the Stack...";
        Matrix M(a,b);
        if(M.num_rows() == a && M.num_cols() == b){
            logfile << " Size correct!" << endl;
            nPassed++;
        }else{
            logfile << " Size incorrect!" << endl;
        }
    }


    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;
        nTests++;
        logfile << nTests << ". Constructing a " << a << "x" <<b << " matrix on the Heap...";
        Mptr = new Matrix(a,b);
        if(Mptr->num_rows() == a && Mptr->num_cols() == b){
            logfile << " Size correct!" << endl;
            nPassed++;
        }else{
            logfile << " Size incorrect!" << endl;
        }
        delete Mptr;
    }


    // test constructor from array and size

    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;
        double *arr = new double[a*b];
        for(int i=0; i<a*b; i++)arr[i] = i;
        nTests++;
        logfile << nTests << ". Constructing a " << a << "x" <<b << " matrix from an array...";
        Mptr = new Matrix(arr, a, b);
        int p_a = rand()%a;
        int p_b = rand()%b;

        if(Mptr->num_rows() == a && Mptr->num_cols() == b && (*Mptr)(p_a, p_b) == arr[p_b*a+p_a]){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
        delete Mptr;
    }

    // test constructor from string and size
    // NOTE: this test is not general enough and should be improved
    string s = "1 2 3 4 5 6 7 8 9";
    Matrix M_str(s, 3,3);
    nTests++;
    logfile << nTests << ". Testing constructor from string...";
    if(M_str.num_rows() == 3 && M_str.num_cols() == 3 && M_str(1,2) == 6){
        logfile << " passed!" << endl;
        nPassed++;
    }else{
        logfile << " failed!" << endl;
    }


    // test copy constructor
    // NOTE: this test is not general enough and should be improved

    Matrix M_to_be_copied(10);
    M_to_be_copied.to_zero();
    M_to_be_copied(5,3) = 10;
    M_to_be_copied(4,8) = 5;
    Matrix M_copied(M_to_be_copied); // copy constructor
    M_to_be_copied(1,1) = 2;
    nTests++;
    logfile << nTests << ". Testing copy constructor...";
    if(M_copied.num_rows() == 10 && M_copied.num_cols() == 10 && M_copied(1,1) == 0 && M_copied(5,3)==10 && M_copied(4,8) == 5){
        logfile << " passed!" << endl;
        nPassed++;
    }else{
        logfile << " failed!" << endl;
    }

    // test operator=
    // again, not a complete test...
    M_to_be_copied(1,1) = 0;
    Matrix M_copy_2 = M_to_be_copied;
    M_to_be_copied(1,1) = 2;
    nTests++;
    logfile << nTests << ". Testing operator=...";
    if(M_copy_2.num_rows() == 10 && M_copy_2.num_cols() == 10 && M_copy_2(1,1) == 0 && M_copy_2(5,3)==10 && M_copy_2(4,8) == 5){
        logfile << " passed!" << endl;
        nPassed++;
    }else{
        logfile << " failed!" << endl;
    }

    M_to_be_copied(1,1) = 0;
    Matrix M_copy_3(2); //originally a different size
    M_copy_3 = M_to_be_copied;
    M_to_be_copied(1,1) = 2;
    nTests++;
    logfile << nTests << ". Testing operator=...";
    if(M_copy_3.num_rows() == 10 && M_copy_3.num_cols() == 10 && M_copy_3(1,1) == 0 && M_copy_3(5,3)==10 && M_copy_3(4,8) == 5){
        logfile << " passed!" << endl;
        nPassed++;
    }else{
        logfile << " failed!" << endl;
    }

    // test in-place transpose Matrix.t
    Matrix M1, M1t;
    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;
        double *arr = new double[a*b];
        for(int i=0; i<a*b; i++)arr[i] = rand()%1000;
        nTests++;
        logfile << nTests << ". Testing in-place transpose of a " << a << "x" <<b << " matrix...";
        M1 = Matrix(arr, a, b);
        M1t = M1; // copy with copy-constructor
        M1t.t();

        bool passed = true; //innocent until proven guilty
        for(int i=0; i<M1.num_rows(); i++)
            for(int j=0; j<M1.num_cols(); j++){
                if(M1(i,j) != M1t(j,i)){
                    passed = false;
                    break;
                }
                if(!passed)break;
            }

        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }

    // test length() and size()
    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;
        Matrix M(a,b);

        pair<int, int> p = M.size();
        nTests++;
        logfile << nTests << ". Testing length() and size()...";

        if(p.first == a && p.second == b && M.length() == max(a,b)){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }

    // test randM
    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+50;
        b = rand()%100+50;

        int n = a*b;

        Matrix M = randM(a,b);
        nTests++;
        logfile << nTests << ". Testing randM function...";

        double tot = 0;
        for(int i=0; i<a; i++) for(int j=0; j<b; j++){
                tot+=M(i,j);
        }
        double evar = 111*sqrt(n)/80; // all but one in a million trials will have mean between n/2-evar and n/2+evar. We assume this is always true. In extremely rare cases, tests may fail for correct code.
        //double evar = 121*sqrt(n)/80; // all but one in ten million trials will have mean between n/2-evar and n/2+evar.

        if(M.num_rows() == a && M.num_cols() == b && tot > n/2 - evar && tot < n/2 + evar)
        {
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }


    // test unary operator+, operator-
    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;
        Matrix M = randM(a,b);
        Matrix M1 = +M;
        Matrix M2 = -M;

        nTests++;
        logfile << nTests << ". Testing unary operator+ and operator-...";


        bool passed = true; //innocent until proven guilty
        for(int i=0; i<M.num_rows(); i++)
            for(int j=0; j<M.num_cols(); j++){
                if(M1(i,j) != M(i,j) || M2(i,j) != -M(i,j)){
                    passed = false;
                    break;
                }
                if(!passed)break;
            }


        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }

    // test operator++ and operator-- (prefix and postfix)

    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;

        Matrix M = randM(a,b)+10;
        Matrix M1 = M;                      // M1 and M agree
        ++M;                                // M is now one larger than M1
        --M1;                               // M is now two larger than M1
        Matrix M2 = M1++;                   // M is now one larger than M1, but two larger than M2
        Matrix M3 = --M;                    // M is now equal to M1 and M3, but one larger than M2

        nTests++;
        logfile << nTests << ". Testing operator++ and operator-- ...";

        bool passed = true; //innocent until proven guilty
        for(int i=0; i<M.num_rows(); i++)
            for(int j=0; j<M.num_cols(); j++){
                if(M(i,j) != M1(i,j) || M(i,j) != M3(i,j) || M(i,j) != M2(i,j)+1){
                    passed = false;
                    break;
                }
                if(!passed)break;
            }

        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }

    // test binary operator+ with two matrices

    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;

        Matrix M1 = randM(a,b)+10;
        Matrix M2 = randM(a,b)+10;
        Matrix M3 = M1 + M2;


        nTests++;
        logfile << nTests << ". Testing binary operator+ with matrices ...";

        bool passed = true; //innocent until proven guilty
        for(int i=0; i<a; i++)
            for(int j=0; j<b; j++){
                if(abs(M3(i,j) - (M1(i,j) + M2(i,j))) > 1e-14){
                    passed = false;
                    break;
                }
                if(!passed)break;
            }

        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }

    // test binary operator+ with a matrix and a scalar

    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;

        Matrix M = randM(a,b)+10;
        double c = rand()%100;

        Matrix M1 = M + c;

        nTests++;
        logfile << nTests << ". Testing binary operator+ with matrix and scalar ...";

        bool passed = true; //innocent until proven guilty
        for(int i=0; i<a; i++)
            for(int j=0; j<b; j++){
                if(abs(M1(i,j) - (M(i,j) + c)) > 1e-14){
                    passed = false;
                    break;
                }
                if(!passed)break;
            }

        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }

    // test binary operator- with two matrices

    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;

        Matrix M1 = randM(a,b)+10;
        Matrix M2 = randM(a,b)+10;
        Matrix M3 = M1 - M2;


        nTests++;
        logfile << nTests << ". Testing binary operator- with matrices ...";

        bool passed = true; //innocent until proven guilty
        for(int i=0; i<a; i++)
            for(int j=0; j<b; j++){
                if(abs(M3(i,j) - (M1(i,j) - M2(i,j))) > 1e-14){
                    passed = false;
                    break;
                }
                if(!passed)break;
            }

        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }


    // test binary operator- with a matrix and a scalar

    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;

        Matrix M = randM(a,b)+10;
        double c = rand()%100;

        Matrix M1 = M - c;

        nTests++;
        logfile << nTests << ". Testing binary operator- with matrix and scalar ...";

        bool passed = true; //innocent until proven guilty
        for(int i=0; i<a; i++)
            for(int j=0; j<b; j++){
                if(abs(M1(i,j) - (M(i,j) - c)) > 1e-14){
                    passed = false;
                    break;
                }
                if(!passed)break;
            }

        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }


    // test operator==

    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;

        Matrix M1 = randM(a,b)+10;
        Matrix M2 = M1;
        Matrix M3 = M1 + 0.0001;

        nTests++;
        logfile << nTests << ". Testing binary operators == and != with matrices ...";

        if(M1 == M2 && !(M1 == M3) && M1!=M3 && !(M1!=M2)){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }

    // test operator > and operator <

    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;

        Matrix M1 = randM(a,b)+10;
        Matrix M2 = M1 - 0.0001;
        Matrix M3 = M1 + 0.0001;
        Matrix M4 = randM(a,b)+10;

        nTests++;
        logfile << nTests << ". Testing binary operators > and < with matrices ...";

        if(M1 > M2 && !(M2>M1) && M3>M1 && !(M1>M3) && M2<M1 && !(M1<M2) && M1<M3 && !(M3<M1) && !(M1<M4) && !(M1>M4)){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }

    // test binary operator* with two matrices

    for(int counter = 0; counter<nTestsEach; counter++){
        // we'll test multiplication by testing the distributive property of multiplication
        a = rand()%100+1;
        b = rand()%100+1;
        int c = rand()%100+1;

        Matrix M1 = randM(a,b)+10;
        Matrix M2 = randM(b,c)+10;
        Matrix M2p = randM(b,c)+10;
        Matrix M3 = M1 * M2;
        Matrix M4 = M1 * M3;
        Matrix M5 = M3 + M4;
        Matrix M6 = M1 * (M2 + M3);

        // if the distributive property holds, then M6 = M5

        nTests++;
        logfile << nTests << ". Testing binary operator* with matrices ...";

        bool passed = (M5==M6);

        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }

    // test binary operator* with a matrix and a scalar
    // TODO: LESIBA!

    // test times() friend function


    // test times() member function

    // test operator+=
    // TODO: LESIBA

    // test operator-=
    // TODO: LESIBA

    // test operator*=
    // TODO: LESIBA


    // Test zeros
    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;

        Matrix M = zeros(a,b);

        nTests++;
        logfile << nTests << ". Testing zeros ...";

        bool passed = (a==M.num_rows() && b == M.num_cols());
        for(int i =0; i<a; i++){
            if(!passed) break;
            for(int j = 0; j<b; j++)if(M(i,j)!=0){ passed = false; break; }
        }

        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }

    // Test ones
    // TODO: LESIBA!


    // Test eye
    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;

        Matrix M = eye(a);

        nTests++;
        logfile << nTests << ". Testing eye ...";

        bool passed = (a==M.num_rows() && a == M.num_cols());
        for(int i =0; i<a; i++){
            if(!passed) break;
            for(int j = 0; j<a; j++)if((i!=j && M(i,j)!=0) || (i==j && M(i,j)!=1))
            {
                    passed = false; break;
            }
        }

        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            cout << M << endl;
            logfile << " failed!" << endl;
        }
    }

    // Test tr
    for(int counter = 0; counter<nTestsEach; counter++){
        // we test trace of known matrices
        a = rand()%100+1;

        Matrix M1 = eye(a);     // trace = a
        Matrix M2 = ones(a);    // trace = a
        Matrix M3 = zeros(a);   // trace = 0
        Matrix M4 = 3*ones(a);  // trace = 3a
        Matrix M5 = randM(a);   // trace unknown, let it be x
        Matrix M6 = 4*M5;       // then trace of this = 4x

        double t1 = tr(M1);
        double t2 = tr(M2);
        double t3 = tr(M3);
        double t4 = tr(M4);
        double t5 = tr(M5);
        double t6 = tr(M6);

        nTests++;
        logfile << nTests << ". Testing trace ...";

        if(t1 == a && t2 == a && t3 == 0 && t4 == 3*a && t6 == 4*t5){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }


    // Test transpose
    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;

        Matrix M = randM(a,b)*100+200;
        Matrix Mt = transpose(M);

        nTests++;
        logfile << nTests << ". Testing transpose of a " << a << "x" <<b << " matrix...";


        bool passed = (a == M.num_rows() && b == M.num_cols() && a == Mt.num_cols() && b == Mt.num_rows());

        for(int i=0; i<a; i++){
            if(!passed)break;
            for(int j=0; j<b; j++) if(M(i,j) != Mt(j,i)){
                    passed = false;
                    break;
            }
        }

        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }


    // Test triu
    // TODO: LESIBA

    // Test tril
    // TODO: LESIBA

    // Test diag
    // TODO: LESIBA

    // Test floor
    // TODO: LESIBA

    // Test l2norm
    // TODO: LESIBA

    // Test Map from outside matrix [ both inplace and other version]
    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;

        Matrix M = randM(a,b);
        Matrix sinM = Map(sin, M);                  // apply sin to ALL entries in M, place results in sinM
        Matrix cosM = Map(cos, M);                  // apply cos to ALL entries in M, place results in cosM
        Matrix M2 = M;                              // copy of the original
        M2.Map(exp);                                // apply exp to ALL entries in M2, in-place

        nTests++;
        logfile << nTests << ". Testing Map...";

        bool passed = (a == sinM.num_rows() && a == cosM.num_rows() && a == M2.num_rows() && b == sinM.num_cols() && b == cosM.num_cols() && b == M2.num_cols());

        for(int i=0; i<a; i++){
            if(!passed)break;
            for(int j=0; j<b; j++) if(abs(sinM(i,j) - sin(M(i,j))) >1e-14 || abs(cosM(i,j) - cos(M(i,j)))>1e-14 || abs(M2(i,j) - exp(M(i,j))) > 1e-14)
            {
                    passed = false;
                    break;
            }
        }

        if(passed){
            logfile << " passed!" << endl;
            nPassed++;
        }else{
            logfile << " failed!" << endl;
        }
    }


    // SUMMARY OF TESTS

    logfile << endl << endl << "-------------------------------------------------------" << endl << endl;
    logfile << nPassed << " tests passed out of " << nTests << endl;
    cout << nPassed << " tests passed out of " << nTests << endl;
    cout << "Please refer to the log file, noiziv_base.log, for more details!" << endl;

    logfile.close();

}


int main()
{
    // Run all the tests
    tests();
    return 0;
}
