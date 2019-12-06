//
//  Addition.hpp
//  Algebra
//
//  Created by Samuel Lo on 6/12/2019.
//  Copyright © 2019 Samuel Lo. All rights reserved.
//

#ifndef Addition_hpp
#define Addition_hpp

#include<vector>
using namespace std;

class Addition
{
public:
    //link
    Addition* mother;
    unsigned int motherType; //1 = exp, 2 = In, 3 = add
    
    //depth and order
    unsigned int depth;
    unsigned int orderType;
    // constant = 1
    // normal order = 2
    // undefined order = 3
    int order;
    // order = (number of exp) - (number of In)
    
    //Fundamental
    bool nZero; //number of 0
    bool nNegative; //number of -1
    unsigned int positveInterger; // 1 or 2 or ...
    bool nTau; //number of tau = pi/2
    bool nComplex; //number of i
    bool nInfinity; //number of -Infinity
    vector<bool> variable; //number of x, number of y, ...
    
    //Composite
    vector<Addition*> exp; //List: exp(x1) + exp(x2) + ...
    vector<Addition*> ln; //List: In(x1) + In(x2) + ...
    vector<Addition*> add; //List: x1 + x2
    
    //Constructor
    Addition();
    Addition(string latex);
    Addition(int compositeType, Addition* operand); //compositeType: 1 = exp, 2 = ln
    Addition(Addition* operand1, Addition* operand2); // addition
    ~Addition();
    
    string getLatex();
    void print();
};

#endif /* Addition_hpp */
