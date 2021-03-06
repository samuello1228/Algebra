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
    //test integer
    for(int i=1; i<=10000; i++)
    //for(int i=15; i<=15; i++)
    {
        Addition* test1 = new Addition(1,i);
        if(i%1000 == 0) cout<<"i = "<<i<<":"<<endl;
        //test1->print();
        
        //Addition* test2 = new Addition(1,test1);
        //test2->print(true);
        
        Addition* explnn1 = new Addition("\\exp(\\ln(-1))");
        explnn1->exp[0]->ln_2.push_back(test1);
        test1->mother = explnn1->exp[0];
        test1->motherType = 2;
        test1 = explnn1;
        test1->print();
     
        
        SemiInterger x = test1->isSemiInterger();
        
        if(x.type != -1)
        {
            if(x.type == 0)
            {
                if(x.integer != i)
                {
                    cout<<"Error: The value of integer is wrong."<<endl;
                    break;
                }
            }
            else
            {
                cout<<"Error: It not an interger."<<endl;
                break;
            }
        }
        else
        {
            cout<<"Error: It not a semi-interger."<<endl;
            break;
        }
        delete test1;
        //cout<<endl;
    }
    */
    
    /*
    {
        int max = 5000;
        int count = 1;
        Addition* x = new Addition("1");
        while(true)
        {
            cout<<"x: "<<count<<endl;
            x->simplification();
            cout<<"y: "<<count<<endl;
            Addition* y = new Addition(1,count);
            if(!Addition::isSame(x,y))
            {
                cout<<"Error: Different integer: "<<count<<endl;
                x->print();
                y->print();
                
                delete x;
                delete y;
                break;
            }
            else
            {
                //cout<<"Smae integer: "<<count<<endl;
                //x->print();
                //y->print();
            }
            
            delete y;
            if(count == max)
            {
                delete x;
                break;
            }
            else
            {
                Addition* one = new Addition("1");
                x->add.push_back(one);
                one->mother = x;
                one->motherType = 3;
                
                count++;
            }
        }
    }
    */
    
    /*
    {
        int max = 1000;
        int index = 0;
        int product = 1;
        Addition* x = new Addition("1");
        while(true)
        {
            cout<<"x: 2^"<<index<<endl;
            x->simplification();
            cout<<"y: 2^"<<index<<endl;
            //Addition* y = new Addition(1,product);
            Addition* y = Addition::PowerOf2(index);
            
            if(!Addition::isSame(x,y))
            {
                cout<<"Error: Different integer: 2^"<<index<<endl;
                x->print();
                y->print();
                
                delete x;
                delete y;
                break;
            }
            else
            {
                cout<<"Smae integer: 2^"<<index<<endl;
                x->print();
                y->print();
            }
            
            delete y;
            if(index == max)
            {
                delete x;
                break;
            }
            else
            {
                //exp(ln(2)+ln(x))
                Addition* lnx = new Addition(2,x);
                Addition* explnx = new Addition(1,lnx);
                
                Addition* two = new Addition("2");
                explnx->exp[0]->ln_c.push_back(two);
                two->mother = explnx->exp[0];
                two->motherType = 2;
                
                x = explnx;
                index++;
                product *= 2;
            }
        }
    }
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
    /test classifyln
    Addition* test1 = new Addition("\\ln(\\tau)");
    test1->ln[0]->nTau = false;
    
    //test1->ln[0]->nNegative = true;
    //test1->ln[0]->positveInterger = 2;
    test1->ln[0]->nComplex = true;
    
    test1->print();
    test1->simplification();
    delete test1;
    */
    
    /*
    //test cleanAdd and explnCancellation
    //Addition* test1 = new Addition("\\exp(\\ln( \\exp(x) + x ))");
    Addition* test1 = new Addition("\\exp( \\ln(\\exp(x)) + x )");
    test1->print();
    test1->simplification();
    delete test1;
    */
    
    //*
    //test constant calculation
    Addition* test1 = new Addition("\\ln((-\\infty)+x)");
    //Addition* test1 = new Addition("\\exp(\\ln(0) + x)");
    //Addition* test1 = new Addition("\\exp(0)");
    test1->print();
    test1->simplification();
    delete test1;
    //*/
    
    /*
    //test ln_1nic
    //Addition* test = new Addition("\\exp( \\ln(-1) + \\ln(2) ) + \\exp( \\ln(-1) + \\ln(2) )");
    //Addition* test = new Addition("\\exp( \\ln(1) + \\ln(-1) + \\ln(-1))");
    Addition* test = new Addition("\\exp( \\ln(2) + \\ln(i) + \\ln(i) + \\ln(i)) + \\exp( \\ln(-1) + \\ln(-1) + \\ln(2) + \\ln(2) )");
    //Addition* test = new Addition("\\exp( \\ln(1) + \\ln(-1) + \\ln(-1)) + \\exp( \\ln(i) + \\ln(i) + \\ln(i) + \\ln(i) ) + \\ln(x)");
    
    test->print();
    test->simplification();
    delete test;
    */
    
    /*
    //test expand
    Addition* test = new Addition("\\exp(\\ln(1 + i) + \\ln(1 + i) )");
    //Addition* test = new Addition("\\exp(\\ln(1 + i) + \\ln(1 + i) ) + \\exp(\\ln(\\tau + x) + \\ln(\\tau + x) )");
    //Addition* test = new Addition("\\exp( \\ln(\\exp(x) + \\ln(y)) + \\ln(\\exp(x) + \\ln(y)) )");
    //Addition* test = new Addition("\\exp( \\ln(\\exp(x) + \\exp(y)) + \\ln(\\exp(x) + \\exp(y)) )");
    
    test->print();
    test->simplification();
    delete test;
    */
    
    /*
    //test addCommonTerm
    //Addition* test = new Addition("0+0");
    //Addition* test = new Addition("\\ln(2) + \\ln(2)");
    Addition* test = new Addition("\\exp(\\exp(\\ln(2) + \\ln(\\ln(2))) + \\ln(2) + \\ln(2))");
    //Addition* test = new Addition("(-1)+(-1)+(-1)+(-1)");
    //Addition* test = new Addition("\\exp(\\ln(-1) + \\ln(2))");
    //Addition* test = new Addition("0+2+1 + (-1)");
    //Addition* test = new Addition("\\exp(\\ln(-1) + \\ln(2)) + \\exp(\\ln(-1) + \\ln(2)) + (-1) +3");
    //Addition* test = new Addition("x + \\exp(\\ln(2) + \\ln(x))");
    
    test->print();
    test->simplification();
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
