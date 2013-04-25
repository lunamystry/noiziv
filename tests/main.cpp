/**
 *   @file main.cpp
 *
 *   @version 03.08
 *   @date 03/08/2013
 *   @brief runs all the tests from the headers
 *
 **/

#include <iostream>
#include "noizivTests.h"
#include "gtest/gtest.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
