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
    
    /*
    //test basicArithmetic
    Addition* test = new Addition("0");
    test->nNegative = true;
    test->positveInterger = 1;
    test->print();
    test->basicArithmetic();
    delete test;
    */
    
    /*
    //test cleanAdd
    Addition* test1 = new Addition("1"); Addition* test2 = new Addition("((-1) + (-1)) + ((-1) + (0))");
    //Addition* test1 = new Addition("\\tau"); Addition* test2 = new Addition("((\\tau) + (\\tau)) + ((\\tau) + (0))");
    //Addition* test1 = new Addition("i"); Addition* test2 = new Addition("((i) + (i)) + ((i) + (0))");
    //Addition* test1 = new Addition("-\\infty"); Addition* test2 = new Addition("((-\\infty) + (-\\infty)) + ((-\\infty) + (x))");
    //Addition* test1 = new Addition("y"); Addition* test2 = new Addition("((x) + (z) + (z) + (x) + (z))");
    //Addition* test1 = new Addition("\\exp(x)"); Addition* test2 = new Addition("(\\exp(x) + \\exp(z)) + (\\exp(z) + \\exp(x) + \\exp(z))");
    //Addition* test1 = new Addition("\\ln(x)"); Addition* test2 = new Addition("(\\ln(x) + \\ln(z)) + (\\ln(z) + \\ln(x) + \\ln(z))");
    test1->add.push_back(test2);
    test2->mother = test1;
    test2->motherType = 3;
    test1->print();
    test1->cleanAdd();
    delete test1;
    */
    
    /*
    //test explnCancellation
    Addition* test1 = new Addition("\\exp(\\ln( (x) + \\exp(x) ))");
    //Addition* test1 = new Addition("\\exp(\\ln(\\exp(x+1)) + \\ln(\\exp(x+1)) + y)");
    test1->print();
    test1->simplification();
    delete test1;
    */
    
    /*
    //test constant calculation
    Addition* test1 = new Addition("\\exp(\\ln(0) + x)");
    //Addition* test1 = new Addition("\\exp(0)");
    test1->print();
    test1->simplification();
    delete test1;
    */
    
    //*
    //test ln_1nic
    //Addition* test = new Addition("\\exp( \\ln(-1) + \\ln(2) ) + \\exp( \\ln(-1) + \\ln(2) )");
    //Addition* test = new Addition("\\exp( \\ln(1) + \\ln(-1) + \\ln(-1))");
    //Addition* test = new Addition("\\exp( \\ln(2) + \\ln(i) + \\ln(i) + \\ln(i)) + \\exp( \\ln(-1) + \\ln(-1) + \\ln(2) + \\ln(2) )");
    Addition* test = new Addition("\\exp( \\ln(1) + \\ln(-1) + \\ln(-1)) + \\exp( \\ln(i) + \\ln(i) + \\ln(i) + \\ln(i) ) + \\ln(x)");
    
    test->print();
    test->simplification();
    delete test;
    //*/
    
    /*
    //test addCommonTerm
    Addition* test = new Addition("\\exp(\\ln(2) + \\ln(-1)) + \\exp(\\ln(2) + \\ln(-1))");
    //Addition* test = new Addition("x + \\exp(\\ln(2) + \\ln(x))");
    
    test->print();
    test->addCommonTerm();
    delete test;
    */
    
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
