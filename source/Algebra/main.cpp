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
    Addition* Test_Additive_Identity = new Addition(x->getCopy(),zero->getCopy());
    Test_Additive_Identity->print();
    delete Test_Additive_Identity;
    
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
