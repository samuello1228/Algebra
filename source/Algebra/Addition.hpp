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
    vector<Addition*> ln_n1; //List: ln(-1) + ln(-1) + ...
    vector<Addition*> ln_c; //List: ln(c1) + ln(c2) + ...
    vector<Addition*> ln_i; //List: ln(i) + ln(i) + ...
    vector<Addition*> ln; //List of other: ln(x1) + ln(x2) + ...
    vector<Addition*> add; //List: x1 + x2
    
    //Constructor
    Addition();
    Addition(string latex);
    Addition(int fundamentalType, unsigned int x); //fundamentalType: 1 = positveInterger, 2 = the nth variable
    Addition(int compositeType, Addition* operand); //compositeType: 1 = exp, 2 = ln, 3 = add
    Addition(Addition* operand1, Addition* operand2); // addition
    ~Addition();
    
    string getLatex();
    void print();
    
    Addition* getCopy();
    Addition* getTopmost();
    void fillDepthOrder();
    
    bool isEmpty();
    bool haveOnlyZero();
    bool haveOnlyNegativeOne();
    bool haveOnlyPositveInterger();
    bool haveOnlyComplex();
    bool haveOnlyInf();
    bool haveOnlyOneItem();
    
    void basicArithmetic();
    void cleanAddOld();
    
    void cleanAdd();
    void explnCancellation();
    
    bool lnInf();
    void ln0();
    void addInf();
    void expInf();
    void exp0();
    void ln_1nic();
    
    void expand();
    
    void addCommonTerm();
    void simplification();
};

#endif /* Addition_hpp */
