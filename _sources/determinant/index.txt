Determinant class (Det)
=======================

Introduction
------------
This is the documentation for the Determinant class of the noiziV
computer vision library. There is two parts to this documentation,
there is the sphinx documentation and doxygen documentation. The
sphinx documentation is aimed to be read to explain how to the
determinant class was implemented. The doxygen documentation is
generated from code. The code is not fully commented though so a lot
may be missing from the doxygen documentation.
The doxygen documentation can be accessed from:
`doc/source/doxy/html/index.html`
This document will explain the determinant class. It briefly gives how
the code can be used and how it was implemented. The method of
compiling the code accompaniying this documentation is also given.
It may be worth while to read the file structure section which
explains the layout of the code.

Determining the determinant
---------------------------
The requirement was to create a class which can be used to calculate
the determinant for different types of matrices. The class is called
Det. The class takes a matrix, it will calculate the determinant using
LU Decomposition by default. The determinant of some matrices can be
calculated more efficiently. The diagonal matrix, Upper/Lower
triangular matrix for example have their matrices calculated by
multiplying their diagonal elements. The tridiagonal matrix's
determinant should also be calculated in a special way which reduces
the time taken in calculating it to O(n). This is not implemented
though as we could not get the algorithm could not be properly
implemented.

Using Det class
---------------
The Det class is used by creating a new Det object with a
noiziv::Matrix type. The calculate function can then be called to
calculate the determinant. Examples of using the Det class can be
found in `src/main.cpp` which came with this submission.

.. code-block:: c++
   :linenos:

    #include <iostream>
    #include <ctime>
    #include "noiziv.h"
    #include "Det.h"

    using namespace std;
    using namespace noiziv;

    int main() {
      srand(time(NULL));
      int a = rand()%10+1;
      int b = a;

      double *arr = new double[a*b];
      for(int i=0; i<a*b; i++) arr[i] = i;
      Matrix m = Matrix(arr, a, b);
      Matrix sing = Matrix("1 1 1 1", 2, 2);
      Matrix notsquare = Matrix("1 1 1 1 1 1", 2, 4);
      cout << m << endl;
      cout << sing << endl;
      Det determinant = Det(m);
      Det determinant2 = Det(sing);
      Det determinant3 = Det(notsquare);
      double det = determinant.calculate();
      double det2 = determinant2.calculate();
      try {
        double det2 = determinant3.calculate();
      } catch (exception &e) {
        cout << e.what() << endl;
      }
      cout << det << endl;
      cout << det2 << endl;

      return 0;
    }

Testing
-------
Mathematical operations on floating point numbers increase the round off error
probability when doing calculations and comparing det(LU) and det(L)*det(U). The
answers would at times be different by a single decimal point and the test would fail in
such an instant. Thus only the first 8 decimal points were compared to determine the
result of that single test.
The matrix dimensions are caped at 250*250 matrix. The reason for this is that it would
take a while to run 300 test of 10000*10000 matrix since finding the determinant of a
3square matrix uses the Gaussian Elimination process witch take order (n ). But a few
tests on big matrices (10000*10000) were done seperatly and they passed the tests.
In the test code the algorithms for Gaussian Elimination and tridiagonal matrix are not
briliently coded since other groups had been allocated the tasks of doing those functions.
300 test were done of which the 1st 100 were for random L and U square matrices to
make
A = LU matrix.
100 tests for random matrix (n*m) dimensions. These tests are expected to fail.
100 test for finding the det of a random tridiagonal matrix.
Thus 200/ 300 tests passed is considered a good result.
The code takes about 10 seconds to finish running.

File structure
--------------

The file structure is given below. The more important directories are
src, lib and tests. These contain the source code, the library used in
doing the LU Decomposition and the tests. The doc directory contains
the doxygen and sphinx documentation and the log directory is to
contain logs. The root directory also contains the waf and wscript
which are used to compile the code instead of make.

.. code-block:: bash
   :linenos:


    ├── build
    ├── doc
    │   ├── Doxyfile
    │   ├── Makefile
    │   ├── build
    │   ├── make.bat
    │   └── source
    ├── lib
    │   └── eigen
    ├── log
    │   └── noiziv_base.log
    ├── results.log
    ├── scripts
    │   ├── init.bat
    │   └── test_data_generator.py
    ├── src
    │   ├── Det.cpp
    │   ├── Det.h
    │   ├── Exceptions.h
    │   ├── main.cpp
    │   ├── noiziv.cpp
    │   └── noiziv.h
    ├── tests
    │   ├── detTest.cpp
    │   └── nzvtest.cpp
    ├── waf
    └── wscript

Compiling the code
------------------
Waf is a build tool written in python. It is an alternative to make,
cmake and ant. It requires python 2.7 to run. To compile the code
using waf the following command can be used on unix like systems:

`python2 waf build`

to run the test, the following can be used:

`python2 waf -t`

These commands have to be run in the root directory of the project.
That is, the directory containing the wscript.

An alternative is to use g++ directly. When using this way, one has to
also add the lib directory to the g++ include path.

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
