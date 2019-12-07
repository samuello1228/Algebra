//
//  main.cpp
//  Algebra
//
//  Created by Samuel Lo on 6/12/2019.
//  Copyright © 2019 Samuel Lo. All rights reserved.
//

#include <iostream>
#include "Addition.hpp"
using namespace std;
int main()
{
    //initialiaztion
    Addition* zero = new Addition("0");
    Addition* one = new Addition("1");
    Addition* negative = new Addition("-1");
    Addition* tau = new Addition("\\tau");
    Addition* i = new Addition("i");
    Addition* inf = new Addition("-\\infty");
    
    Addition* x = new Addition("x");
    Addition* y = new Addition("y");
    Addition* z = new Addition("z");
    
    //test
    //Addition* test = new Addition("((2) + (-1)) + ((-1) + (0))");
    //Addition* test = new Addition("((-1) + (-1)) + ((-1) + (0))");
    //Addition* test = new Addition("((1) + (-1)) + ((0) + (0))");
    //Addition* test = new Addition("((\\tau) + (\\tau)) + ((\\tau) + (0))");
    //Addition* test = new Addition("((i) + (i)) + ((i) + (0))");
    //Addition* test = new Addition("((-\\infty) + (-\\infty)) + ((-\\infty) + (x))");
    //Addition* test = new Addition("((x) + (z) + (z) + (x) + (z))");
    //Addition* test = new Addition("(\\exp(x) + \\exp(z)) + (\\exp(z) + \\exp(x) + \\exp(z))");
    Addition* test = new Addition("(\\ln(x) + \\ln(z)) + (\\ln(z) + \\ln(x) + \\ln(z))");
    test->print();
    test->cleanAdd();
    delete test;
    
    //delete
    delete zero;
    delete one;
    delete negative;
    delete tau;
    delete i;
    delete inf;
    
    delete x;
    delete y;
    delete z;
    
    return 0;
}
