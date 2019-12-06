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
    Addition* test = new Addition("1 + 1");
    test->print();
    delete test;
    
    /*
    Addition* Test_Additive_Identity = new Addition();
    Test_Additive_Identity->positveInterger = 1;
    
    //Test_Additive_Identity->print();
    
    delete Test_Additive_Identity;
     */
    
    return 0;
}
