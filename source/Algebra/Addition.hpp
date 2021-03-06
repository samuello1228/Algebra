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

struct SemiInterger
{
    int type;
    //-1: not a semi-interger
    //0: integer
    //1: ln(c)
    //2: ln(ln(c))
    //c is positive integer
    
    unsigned int integer;
};

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
    bool nOne; //number of 1
    bool nTwo; //number of 2
    
    bool nTau; //number of tau = pi/2
    bool nComplex; //number of i
    bool nInfinity; //number of -Infinity
    
    vector<bool> constant; //number of c_1, c_2, ...
    vector<bool> variable; //number of x, number of y, ...
    
    //Composite
    vector<Addition*> exp_lnc; //List: exp(x), x = ln(2^n), n >= 2
    vector<Addition*> exp_lnlnc; //List: exp(x), x = ln(ln(2^n)), n >= 2
    vector<Addition*> exp; //List: exp(x1) + exp(x2) + ...
    
    vector<Addition*> ln_1; //List: ln(1)
    vector<Addition*> ln_2; //List: ln(2)
    vector<Addition*> ln_ln2; //List: ln(ln(2))
    vector<Addition*> ln_N; //List: ln(c), c is non-negative integer, c = 0 or c >= 3
    vector<Addition*> ln_n1; //List: ln(-1) + ln(-1) + ...
    vector<Addition*> ln_i; //List: ln(i) + ln(i) + ...
    //vector<Addition*> ln_c; //List: ln(c), c is constant
    vector<Addition*> ln; //List of other: ln(x1) + ln(x2) + ...
    
    vector<Addition*> add; //List: x1 + x2
    
    //Constructor
    Addition();
    Addition(string latex);
    static Addition* lnPowerOf2(unsigned int x); //return ln(2^x)
    //static Addition* PowerOf2(unsigned int x); //return 2^x
    Addition(int fundamentalType, unsigned int x); //fundamentalType: 1 = positveInterger, 2 = the nth variable
    Addition(int compositeType, Addition* operand); //compositeType: 1 = exp, 2 = ln, 3 = add
    Addition(Addition* operand1, Addition* operand2); // addition
    
    void deleteAllList();
    ~Addition();
    
    string getLatex(bool isPrintInteger = false);
    void print(bool isPrintInteger = false);
    
    Addition* getCopy();
    Addition* getTopmost();
    void fillDepthOrder();
    void classifyln();
    static bool isSame(Addition* x1,Addition* x2);
    
    bool AllListIsEmpty();
    bool isEmpty();
    bool haveOnlyZero();
    bool haveOnlyNegativeOne();
    bool haveOnlyOne();
    bool haveOnlyTwo();
    bool haveOnlyComplex();
    bool haveOnlyInf();
    bool haveOnlyOneItem();
    bool containFundamental();
    SemiInterger isSemiInterger();
    
    void basicArithmetic();
    void cleanAddOld();
    
    void cleanAdd(bool isPrint = true);
    void explnCancellation(bool isPrint = true);
    
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
