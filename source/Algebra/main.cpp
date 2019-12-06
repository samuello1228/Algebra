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
    Addition* test1 = new Addition("1 + 1");
    test1->print();
    
    Addition* test2 = new Addition(2,test1);
    test2->print();
    
    Addition* test3 = new Addition(test1,test2);
    test3->print();
    
    //delete test3;
    
    /*
    Addition* Test_Additive_Identity = new Addition();
    Test_Additive_Identity->positveInterger = 1;
    
    //Test_Additive_Identity->print();
    
    delete Test_Additive_Identity;
     */
    
    return 0;
}
