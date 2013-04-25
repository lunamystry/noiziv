/**
 *   @file noizivTests.h
 *   @authors Mandla Mbuli (0705871Y),
 *
 *   @version 03
 *   @date 24/04/2013
 *   @brief Test for the noiziv base
 *
 **/
#include <vector>
#include "gtest/gtest.h"
#include "TestDataReader.h"
#include "noiziv.h"

using namespace std;
using namespace noiziv;

const int nTestsEach = 10;

// TEST(noiziv, readInData) {
//   NoizivDataList knownDataList = getTestData("validMatrices.txt");
//   NoizivTestData knownData;
//   Matrix calculatedInv;
// 
//   NoizivDataListIter iter = knownDataList.begin();
//   for(iter; iter != knownDataList.end(); iter++) {
//      knownData = *iter;
//      //calculatedInv = Inv(knownData.matrix).calculate();
//      //ASSERT_TRUE(knownData.matrix == knownData.invMatrix);
//      ASSERT_TRUE(1 == 1);
//    }
// }

TEST(noiziv, defaultConstructor) {
    int a = rand()%100+1;
    int b = rand()%100+1;
    Matrix M(a,b);
    ASSERT_TRUE(M.num_rows() == a && M.num_cols() == b);
}

TEST(noiziv, defaultWithPointerConstructor) {
    int a = rand()%100+1;
    int b = rand()%100+1;
    Matrix *Mptr = new Matrix(a,b);
    ASSERT_TRUE(Mptr->num_rows() == a && Mptr->num_cols() == b);
    // Add boost smart pointers?
    delete Mptr;
}

// TODO: This is incomplete
TEST(noiziv, canConstructWithString) {
    string s = "1 2 3 4 5 6 7 8 9";
    int rows = 3;
    int cols = 3;
    Matrix M_str(s, rows, cols);
    ASSERT_TRUE(M_str.num_rows() == rows);
    ASSERT_TRUE(M_str.num_cols() == cols);
    ASSERT_TRUE(M_str(1,2) == 6);
}

TEST(noiziv, canCopyMatrix) {
    Matrix M_to_be_copied(10);
    M_to_be_copied.to_zero();
    M_to_be_copied(5,3) = 10;
    M_to_be_copied(4,8) = 5;
    Matrix M_copied(M_to_be_copied); // copy constructor
    M_to_be_copied(1,1) = 2;
    ASSERT_TRUE(M_copied.num_rows() == 10 && M_copied.num_cols() == 10 && M_copied(1,1) == 0 && M_copied(5,3)==10 && M_copied(4,8) == 5);
}

TEST(noiziv, canAssignMatrices) {
    Matrix M_to_be_copied(10);
    M_to_be_copied(1,1) = 0;
    Matrix M_copy_2 = M_to_be_copied;
    ASSERT_TRUE(M_copy_2.num_rows() == 10); 
    ASSERT_TRUE(M_copy_2.num_cols() == 10);
    ASSERT_TRUE(M_copy_2(1,1) == 0);
    ASSERT_TRUE(M_copy_2(5,3)==10); 
    ASSERT_TRUE(M_copy_2(4,8) == 5);
}

TEST(noiziv, canAssignMatricesWithDifferentSize) {
    Matrix M_to_be_copied(10);
    M_to_be_copied(1,1) = 0;
    Matrix M_copy_3(2); //originally a different size
    M_copy_3 = M_to_be_copied;
    M_to_be_copied(1,1) = 2;
    ASSERT_TRUE(M_copy_3.num_rows() == 10); 
    ASSERT_TRUE(M_copy_3.num_cols() == 10); 
    ASSERT_TRUE(M_copy_3(1,1) == 0 && M_copy_3(5,3)==10 && M_copy_3(4,8) == 5); 
}

TEST(noiziv, canTransposeInPlace) {
    Matrix M1; 
    Matrix M1t;
    int rows;
    int cols;
    for(int counter = 0; counter < nTestsEach; counter++){
        rows = rand()%100+1;
        cols = rand()%100+1;
        double *arr = new double[rows*cols];
        for(int i=0; i<rows*cols; i++)
            arr[i] = rand()%1000;
        M1 = Matrix(arr, rows, cols);
        M1t = M1; // copy with copy-constructor
        M1t.t();

        bool passed = true;
        for(int i=0; i<M1.num_rows(); i++) {
            for(int j=0; j<M1.num_cols(); j++){
                ASSERT_TRUE(M1(i,j) != M1t(j,i));
            }
        }
    }
}

TEST(noiziv, canGetSize) {
    int a;
    int b;
    for(int counter = 0; counter<nTestsEach; counter++){
        a = rand()%100+1;
        b = rand()%100+1;
        Matrix M(a,b);
        pair<int, int> p = M.size();
        ASSERT_TRUE(p.first == a && p.second == b && M.length() == max(a,b));
    }
}

TEST(noiziv, canGenerateRandomMatrix) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+50;
        int b = rand()%100+50;
        int n = a*b;
        Matrix M = randM(a,b);
        double tot = 0;
        for(int i=0; i<a; i++) for(int j=0; j<b; j++){
            tot+=M(i,j);
        }
        double evar = 111*sqrt(n)/80; // all but one in a million trials will have mean between n/2-evar and n/2+evar. We assume this is always true. In extremely rare cases, tests may fail for correct code.
        //double evar = 121*sqrt(n)/80; // all but one in ten million trials will have mean between n/2-evar and n/2+evar.
        ASSERT_TRUE(M.num_rows() == a && M.num_cols() == b && tot > n/2 - evar && tot < n/2 + evar);
    }
}

// unary + -
// TODO: These should be separated into two tests
TEST(noiziv, canUseUninaryOperators) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        int b = rand()%100+1;
        Matrix M = randM(a,b);
        Matrix M1 = +M;
        Matrix M2 = -M;

        bool passed = true; //innocent until proven guilty
        for(int i=0; i<M.num_rows(); i++)
            for(int j=0; j<M.num_cols(); j++){
                ASSERT_TRUE(M1(i,j) != M(i,j) || M2(i,j) != -M(i,j));
            }
    }
}

// TODO: There are two tests here
// increment prefix and postfix
TEST(noiziv, canPrefixMatrices) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        int b = rand()%100+1;

        Matrix M = randM(a,b)+10;
        Matrix M1 = M;                      // M1 and M agree
        ++M;                                // M is now one larger than M1
        --M1;                               // M is now two larger than M1
        Matrix M2 = M1++;                   // M is now one larger than M1, but two larger than M2
        Matrix M3 = --M;                    // M is now equal to M1 and M3, but one larger than M2

        bool passed = true; //innocent until proven guilty
        for(int i=0; i<M.num_rows(); i++)
            for(int j=0; j<M.num_cols(); j++){
                ASSERT_TRUE(M(i,j) != M1(i,j) || M(i,j) != M3(i,j) || M(i,j) != M2(i,j)+1);
            }
    }
}

// test binary operator+ with two matrices
TEST(noiziv, canANDTwoMatrices) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        int b = rand()%100+1;

        Matrix M1 = randM(a,b)+10;
        Matrix M2 = randM(a,b)+10;
        Matrix M3 = M1 + M2;

        bool passed = true; //innocent until proven guilty
        for(int i=0; i<a; i++) {
            for(int j=0; j<b; j++){
                ASSERT_TRUE(abs(M3(i,j) - (M1(i,j) + M2(i,j))) > 1e-14);
            }
        }
    }
}

// test binary operator+ with a matrix and a scalar
TEST(noiziv, canANDMatrixAndScalar) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        int b = rand()%100+1;

        Matrix M = randM(a,b)+10;
        double c = rand()%100;

        Matrix M1 = M + c;
        bool passed = true; //innocent until proven guilty
        for(int i=0; i<a; i++) {
            for(int j=0; j<b; j++){
                ASSERT_TRUE(abs(M1(i,j) - (M(i,j) + c)) > 1e-14);
            }
        }
    }
}

// test binary operator- with two matrices
TEST(noiziv, canUseBinaryNegWithTwoMatrices) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        int b = rand()%100+1;

        Matrix M1 = randM(a,b)+10;
        Matrix M2 = randM(a,b)+10;
        Matrix M3 = M1 - M2;

        bool passed = true; //innocent until proven guilty
        for(int i=0; i<a; i++) {
            for(int j=0; j<b; j++){
                ASSERT_TRUE(abs(M3(i,j) - (M1(i,j) - M2(i,j))) > 1e-14);
            }
        }
    }
}

// test binary operator- with a matrix and a scalar
TEST(noiziv, canUseBinaryNegWithMatrixAndScalar) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        int b = rand()%100+1;

        Matrix M = randM(a,b)+10;
        double c = rand()%100;

        Matrix M1 = M - c;

        bool passed = true; //innocent until proven guilty
        for(int i=0; i<a; i++) {
            for(int j=0; j<b; j++){
                ASSERT_TRUE(abs(M1(i,j) - (M(i,j) - c)) > 1e-14);
            }
        }
    }
}

// test operator==
TEST(noiziv, canCompareEqualityOfMatrices) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        int b = rand()%100+1;

        Matrix M1 = randM(a,b)+10;
        Matrix M2 = M1;
        Matrix M3 = M1 + 0.0001;

        ASSERT_TRUE(M1 == M2 && !(M1 == M3) && M1!=M3 && !(M1!=M2));
    }
}

// test operator > and operator <
TEST(noiziv, canCompareGreaterMatrices) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        int b = rand()%100+1;

        Matrix M1 = randM(a,b)+10;
        Matrix M2 = M1 - 0.0001;
        Matrix M3 = M1 + 0.0001;
        Matrix M4 = randM(a,b)+10;

        ASSERT_TRUE(M1 > M2 && !(M2>M1) && M3>M1 && !(M1>M3) && M2<M1 && !(M1<M2) && M1<M3 && !(M3<M1) && !(M1<M4) && !(M1>M4));
    }
}

// test binary operator* with two matrices
TEST(noiziv, canCompareLessThanMatrices) {
    for(int counter = 0; counter<nTestsEach; counter++){
        // we'll test multiplication by testing the distributive property of multiplication
        int a = rand()%100+1;
        int b = rand()%100+1;
        int c = rand()%100+1;

        Matrix M1 = randM(a,b)+10;
        Matrix M2 = randM(b,c)+10;
        Matrix M2p = randM(b,c)+10;
        Matrix M3 = M1 * M2;
        Matrix M4 = M1 * M3;
        Matrix M5 = M3 + M4;
        Matrix M6 = M1 * (M2 + M3);

        // if the distributive property holds, then M6 = M5
        ASSERT_TRUE(M5==M6);
    }
}

// TEST(noiziv, canORTwoMatrices) {
// 
// }

// TEST(noiziv, canORScalarAndMatrix) {
// 
// }

TEST(noiziv, canCreateZerosMatrix) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        int b = rand()%100+1;

        Matrix M = zeros(a,b);

        ASSERT_TRUE(a==M.num_rows() && b == M.num_cols());
        for(int i =0; i<a; i++){
            for(int j = 0; j<b; j++) {
                ASSERT_TRUE(M(i,j) == 0);
            }
        }

    }
}

TEST(noiziv, canCreateOnesMatrix) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        Matrix M = eye(a);

        bool passed = (a==M.num_rows() && a == M.num_cols());
        for(int i =0; i<a; i++){
            if(!passed) break;
            for(int j = 0; j<a; j++)
                if((i!=j && M(i,j)!=0) || (i==j && M(i,j)!=1))
                {
                    passed = false; break;
                }
        }
        // TODO: not sure about the logic here
        ASSERT_TRUE(passed);
    }
}

// we test trace of known matrices
// for an nxn matrix, its the sum of the elements of the main diagonal
TEST(noiziv, trace) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;

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

        ASSERT_TRUE(t1 == a && t2 == a && t3 == 0 && t4 == 3*a && t6 == 4*t5);
    }
}

// Test transpose
TEST(noiziv, canFindTranspose) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        int b = rand()%100+1;

        Matrix M = randM(a,b)*100+200;
        Matrix Mt = transpose(M);

        bool passed = (a == M.num_rows() && b == M.num_cols() && a == Mt.num_cols() && b == Mt.num_rows());

        for(int i=0; i<a; i++){
            if(!passed)break;
            for(int j=0; j<b; j++) if(M(i,j) != Mt(j,i)){
                passed = false;
                break;
            }
        }

        ASSERT_TRUE(passed);
    }
}

// Test Map from outside matrix [ both inplace and other version]
TEST(noiziv, canORTwoMatrices) {
    for(int counter = 0; counter<nTestsEach; counter++){
        int a = rand()%100+1;
        int b = rand()%100+1;

        Matrix M = randM(a,b);
        Matrix sinM = Map(sin, M);                  // apply sin to ALL entries in M, place results in sinM
        Matrix cosM = Map(cos, M);                  // apply cos to ALL entries in M, place results in cosM
        Matrix M2 = M;                              // copy of the original
        M2.Map(exp);                                // apply exp to ALL entries in M2, in-place

        bool passed = (a == sinM.num_rows() && a == cosM.num_rows() && a == M2.num_rows() && b == sinM.num_cols() && b == cosM.num_cols() && b == M2.num_cols());

        for(int i=0; i<a; i++){
            if(!passed)break;
            for(int j=0; j<b; j++) if(abs(sinM(i,j) - sin(M(i,j))) >1e-14 || abs(cosM(i,j) - cos(M(i,j)))>1e-14 || abs(M2(i,j) - exp(M(i,j))) > 1e-14)
            {
                passed = false;
                break;
            }
        }

        ASSERT_TRUE(passed);
    }
}
