//
//  Addition.cpp
//  Algebra
//
//  Created by Samuel Lo on 6/12/2019.
//  Copyright © 2019 Samuel Lo. All rights reserved.
//

#include <iostream>
#include "Addition.hpp"
using namespace std;

bool compare(unsigned int depth1, unsigned int orderType1, int order1, unsigned int depth2, unsigned int orderType2, int order2)
{
    if(orderType1 < orderType2) return true;
    else if(orderType1 > orderType2) return false;
    
    if(orderType1 == 2)
    {
        if(order1 < order2) return true;
        else if(order1 > order2) return false;
    }
    
    if(depth1 < depth2) return true;
    else return false;
}

bool eraseParenthesis(string& latex)
{
    if(latex[0] != '(' || latex[latex.size()-1] != ')') return false;
    
    bool isErase = false;
    unsigned int index = 0;
    int parenthesisLevel = 0;
    while(true)
    {
        if(latex[index] == '(') parenthesisLevel++;
        else if(latex[index] == ')')
        {
            parenthesisLevel--;
            if(parenthesisLevel == 0)
            {
                if(index+1 == latex.size()) isErase = true;
                else isErase = false;
                
                break;
            }
        }
        
        if(index+1 == latex.size())
        {
            cout<<"Error: Impossible case"<<endl;
            break;
        }
        else index++;
    }
    
    if(isErase)
    {
        latex.erase(0,1);
        latex.erase(latex.size()-1,1);
    }
    
    return isErase;
}

void eraseEmptyElement(vector<Addition*>& list)
{
    int index = 0;
    while(true)
    {
        if(index >= list.size()) break;
        
        if(list[index]->isEmpty())
        {
            delete list[index];
            list.erase(list.begin()+index);
        }
        else index++;
    }
}

void updateDepthOrder(unsigned int& depth1, unsigned int& orderType1, int& order1,
                      unsigned int depth2, unsigned int orderType2, int order2)
{
    if(depth2 > depth1) depth1 = depth2;
    
    // |1 2 3
    //-+------
    //1|1 2 3
    //2|2 ? 3
    //3|3 3 3
    
    if(orderType1 == 1)
    {
        orderType1 = orderType2;
        if(orderType2 == 2) order1 = order2;
    }
    else if(orderType1 == 2)
    {
        if(orderType2 == 2)
        {
            if(order1 != order2)
            {
                orderType1 = 3;
                order1 = 0;
            }
        }
        else if(orderType2 == 3)
        {
            orderType1 = 3;
            order1 = 0;
        }
    }
}

bool isSameList(vector<Addition*>& x1, vector<Addition*>& x2)
{
    if(x1.size() != x2.size()) return false;
    std::vector<bool> isMatch(x1.size(), false);
    
    for(unsigned int i = 0; i < x1.size() ; i++)
    {
        bool isFoundMatch = false;
        for(unsigned int j = 0; j < x2.size() ; j++)
        {
            if(!isMatch[j] && Addition::isSame(x1[i],x2[j]))
            {
                isMatch[j] = true;
                isFoundMatch = true;
                break;
            }
        }
        
        if(!isFoundMatch) return false;
    }
    
    return true;
}

bool Addition::isSame(Addition* x1,Addition* x2)
{
    if(x1->nZero != x2->nZero) return false;
    if(x1->nNegative != x2->nNegative) return false;
    if(x1->nOne != x2->nOne) return false;
    if(x1->nTwo != x2->nTwo) return false;
    if(x1->nTau != x2->nTau) return false;
    if(x1->nComplex != x2->nComplex) return false;
    if(x1->nInfinity != x2->nInfinity) return false;
    
    if(x1->variable.size() != x2->variable.size()) return false;
    for(unsigned int i = 0; i < x1->variable.size() ; i++)
    {
        if(x1->variable[i] != x2->variable[i]) return false;
    }
    
    if(!isSameList(x1->exp,x2->exp)) return false;
    if(!isSameList(x1->ln_n1,x2->ln_n1)) return false;
    if(!isSameList(x1->ln_2,x2->ln_2)) return false;
    if(!isSameList(x1->ln_i,x2->ln_i)) return false;
    if(!isSameList(x1->ln,x2->ln)) return false;
    if(!isSameList(x1->add,x2->add)) return false;
    
    return true;
}

Addition::Addition()
{
    mother = nullptr;
    motherType = 0;
    
    depth = 0;
    orderType = 0;
    order = 0;
    
    nZero = false;
    nNegative = false;
    nOne = false;
    nTwo = false;
    nTau = false;
    nComplex = false;
    nInfinity = false;
}

Addition::Addition(string latex)
{
    //trimming space
    string trim;
    for(unsigned int i = 0; i < latex.size() ; i++)
    {
        if(latex[i] == ' ') continue;
        else trim += latex[i];
    }
    
    if(trim.size() == 0)
    {
        cout<<"Syntax Error: Constructor for latex: the expression is empty"<<endl;
        return;
    }
    
    //erase "(" and ")"
    while(true)
    {
        bool isErase = eraseParenthesis(trim);
        if(!isErase) break;
    }
    
    {
        // For addition
        if(trim[0] == '+' || trim[trim.size()-1] == '+')
        {
            cout<<"Syntax Error: the expression has + sign at the beginning or the end"<<endl;
            return;
        }
        
        //split by "+" sign
        unsigned int index = 0;
        vector<string> operand_latex_list;
        string operand_latex;
        int parenthesisLevel = 0;
        while(true)
        {
            if(trim[index] == '+' && parenthesisLevel == 0)
            {
                operand_latex_list.push_back(operand_latex);
                operand_latex.clear();
            }
            else
            {
                operand_latex += trim[index];
                if(trim[index] == '(') parenthesisLevel++;
                else if(trim[index] == ')') parenthesisLevel--;
            }
            
            if(index+1 == trim.size())
            {
                operand_latex_list.push_back(operand_latex);
                operand_latex.clear();
                break;
            }
            else index++;
        }
        
        if(operand_latex_list.size() >= 2)
        {
            //For f(x) + g(x)
            
            depth = 1;
            orderType = 1;
            order = 0;
            for(unsigned int i = 0; i < operand_latex_list.size() ; i++)
            {
                Addition* operand = new Addition(operand_latex_list[i]);
                
                operand->mother = this;
                operand->motherType = 3;
                
                mother = nullptr;
                motherType = 0;
                
                updateDepthOrder(depth, orderType, order, operand->depth +1, operand->orderType, operand->order);
                
                nZero = false;
                nNegative = false;
                nOne = false;
                nTwo = false;
                nTau = false;
                nComplex = false;
                nInfinity = false;
                
                add.push_back(operand);
            }
            
            //sort(add.begin(), add.end(), [](Addition* a, Addition* b)->bool{return compare(a->depth,a->orderType,a->order,b->depth,b->orderType,b->order);});
            
            return;
        }
        
    }
    
    //For \exp
    if(trim.size() >= 5 && trim[0] == '\\' && trim[1] == 'e' && trim[2] == 'x' && trim[3] == 'p' && trim[4] == '(')
    {
        if(trim.size() <= 6)
        {
            cout<<"Syntax Error: the exp function does not have operand"<<endl;
            return;
        }
        
        unsigned int index = 5;
        string operand_latex;
        int parenthesisLevel = 0;
        while(true)
        {
            operand_latex += trim[index];
            if(trim[index] == '(') parenthesisLevel++;
            else if(trim[index] == ')') parenthesisLevel--;
            
            if(index+1 == trim.size())
            {
                cout<<"Syntax Error: the exp function does not have right parenthesis"<<endl;
                return;
            }
            else if(trim[index+1] == ')' && parenthesisLevel == 0)
            {
                if(trim.size() >= index+3)
                {
                    cout<<"Syntax Error: the exp function may have some left missing parenthesis"<<endl;
                    return;
                }
                
                //For exp(f(x))
                Addition* operand = new Addition(operand_latex);
                
                operand->mother = this;
                operand->motherType = 1;
                
                mother = nullptr;
                motherType = 0;
                
                depth = operand->depth +1;
                orderType = operand->orderType;
                
                if(orderType == 2) order = operand->order +1;
                else order = 0;
                
                nZero = false;
                nNegative = false;
                nOne = false;
                nTwo = false;
                nTau = false;
                nComplex = false;
                nInfinity = false;
                
                exp.push_back(operand);
                return;
            }
            else index++;
        }
    }
    
    //For \ln
    if(trim.size() >= 4 && trim[0] == '\\' && trim[1] == 'l' && trim[2] == 'n' && trim[3] == '(')
    {
        if(trim.size() <= 5)
        {
            cout<<"Syntax Error: the ln function does not have operand"<<endl;
            return;
        }
        
        unsigned int index = 4;
        string operand_latex;
        int parenthesisLevel = 0;
        while(true)
        {
            operand_latex += trim[index];
            if(trim[index] == '(') parenthesisLevel++;
            else if(trim[index] == ')') parenthesisLevel--;
            
            if(index+1 == trim.size())
            {
                cout<<"Syntax Error: the ln function does not have right parenthesis"<<endl;
                return;
            }
            else if(trim[index+1] == ')' && parenthesisLevel == 0)
            {
                if(trim.size() >= index+3)
                {
                    cout<<"Syntax Error: the ln function may have some left missing parenthesis"<<endl;
                    return;
                }
                
                //For ln(f(x))
                Addition* operand = new Addition(operand_latex);
                
                operand->mother = this;
                operand->motherType = 2;
                
                mother = nullptr;
                motherType = 0;
                
                depth = operand->depth +1;
                orderType = operand->orderType;
                
                if(orderType == 2) order = operand->order -1;
                else order = 0;
                
                nZero = false;
                nNegative = false;
                nOne = false;
                nTwo = false;
                nTau = false;
                nComplex = false;
                nInfinity = false;
                
                if(operand->haveOnlyNegativeOne()) ln_n1.push_back(operand);
                else if(operand->haveOnlyOne() || operand->haveOnlyTwo()) ln_2.push_back(operand);
                else if(operand->haveOnlyComplex()) ln_i.push_back(operand);
                else ln.push_back(operand);
                
                return;
            }
            else index++;
        }
    }
    
    //For constant and variable
    if(trim.size() == 1 && trim[0] == '0')
    {
        //For 0
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 1;
        order = 0;
        
        nZero = true;
        nNegative = false;
        nOne = false;
        nTwo = false;
        nTau = false;
        nComplex = false;
        nInfinity = false;
        return;
    }
    else if(trim.size() == 1 && trim[0] == '1')
    {
        //For 1
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 1;
        order = 0;
        
        nZero = false;
        nNegative = false;
        nOne = true;
        nTwo = false;
        nTau = false;
        nComplex = false;
        nInfinity = false;
        return;
    }
    else if(trim.size() == 1 && trim[0] == '2')
    {
        //For 2
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 1;
        order = 0;
        
        nZero = false;
        nNegative = false;
        nOne = false;
        nTwo = true;
        nTau = false;
        nComplex = false;
        nInfinity = false;
        return;
    }
    else if(trim.size() == 1 && trim[0] == 'i')
    {
        //For i
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 1;
        order = 0;
        
        nZero = false;
        nNegative = false;
        nOne = false;
        nTwo = false;
        nTau = false;
        nComplex = true;
        nInfinity = false;
        return;
    }
    else if(trim.size() == 1 && trim[0] == 'x')
    {
        //For x
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 2;
        order = 0;
        
        nZero = false;
        nNegative = false;
        nOne = false;
        nTwo = false;
        nTau = false;
        nComplex = false;
        nInfinity = false;
        variable.push_back(true);
        return;
    }
    else if(trim.size() == 1 && trim[0] == 'y')
    {
        //For y
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 2;
        order = 0;
        
        nZero = false;
        nNegative = false;
        nOne = false;
        nTwo = false;
        nTau = false;
        nComplex = false;
        nInfinity = false;
        variable.push_back(false);
        variable.push_back(true);
        return;
    }
    else if(trim.size() == 1 && trim[0] == 'z')
    {
        //For z
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 2;
        order = 0;
        
        nZero = false;
        nNegative = false;
        nOne = false;
        nTwo = false;
        nTau = false;
        nComplex = false;
        nInfinity = false;
        variable.push_back(false);
        variable.push_back(false);
        variable.push_back(true);
        return;
    }
    else if(trim.size() == 2 && trim[0] == '-' && trim[1] == '1')
    {
        //For -1
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 1;
        order = 0;
        
        nZero = false;
        nNegative = true;
        nOne = false;
        nTwo = false;
        nTau = false;
        nComplex = false;
        nInfinity = false;
        return;
    }
    else if(trim.size() == 4 && trim[0] == '\\' && trim[1] == 't' && trim[2] == 'a' && trim[3] == 'u')
    {
        //For tau
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 1;
        order = 0;
        
        nZero = false;
        nNegative = false;
        nOne = false;
        nTwo = false;
        nTau = true;
        nComplex = false;
        nInfinity = false;
        return;
    }
    else if(trim.size() == 7 && trim[0] == '-' && trim[1] == '\\' && trim[2] == 'i' && trim[3] == 'n' && trim[4] == 'f' && trim[5] == 't' && trim[6] == 'y')
    {
        //For -\infty
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 1;
        order = 0;
        
        nZero = false;
        nNegative = false;
        nOne = false;
        nTwo = false;
        nTau = false;
        nComplex = false;
        nInfinity = true;
        return;
    }
    else if(trim[0] != '0' && std::isdigit(trim[0]))
    {
        //For positive integer
        unsigned int index = 0;
        string positive_integer;
        while(true)
        {
            positive_integer += trim[index];
            
            if(index+1 == trim.size())
            {
                //For positive integer
                Addition* c = new Addition(1, std::stoi(positive_integer));
                
                mother = nullptr;
                motherType = 0;
                
                depth = c->depth +1;
                orderType = 1;
                order = 0;
                
                nZero = false;
                nNegative = false;
                nOne = false;
                nTwo = false;
                nTau = false;
                nComplex = false;
                nInfinity = false;
                
                add.push_back(c);
                c->mother = this;
                c->motherType = 3;
                
                return;
            }
            else if(std::isdigit(trim[index+1]))
            {
                index++;
            }
            else
            {
                break;
            }
        }
    }
    
    {
        cout<<"Syntax Error: the expression cannot be processed: "<<trim<<endl;
        
        mother = nullptr;
        motherType = 0;
        
        depth = 0;
        orderType = 0;
        order = 0;
        
        nZero = false;
        nNegative = false;
        nOne = false;
        nTwo = false;
        nTau = false;
        nComplex = false;
        nInfinity = false;
        
        return;
    }
}

Addition* Addition::lnPowerOf2(unsigned int x)
{
    //y = ln(2^x)
    
    //  ln(2^7)
    //= ln( 2^(2^0 + 2^1 + 2^2) )
    //= ln( 2^(2^0) * 2^(2^1) * 2^(2^2) )
    //= ln(2^(2^0)) + ln(2^(2^1)) + ln(2^(2^2))
    //= ln(2) + exp(ln(ln(2)) + ln(2^1)) + exp(ln(ln(2)) + ln(2^2))
    
    Addition* y = new Addition();
    
    y->mother = nullptr;
    y->motherType = 0;
    
    y->orderType = 1;
    y->order = 0;
    
    y->nZero = false;
    y->nNegative = false;
    y->nOne = false;
    y->nTwo = false;
    y->nTau = false;
    y->nComplex = false;
    y->nInfinity = false;
    
    unsigned int index = 0;
    unsigned int product = 1;
    while(true)
    {
        //bitwise AND
        unsigned int bit = x & product;
        
        if(bit > 0)
        {
            //cout<<"2^"<<index<<" = "<<bit<<endl;
            
            if(index == 0)
            {
                //y += ln(2)
                y->depth = 2;
                
                Addition* two = new Addition("2");
                y->ln_2.push_back(two);
                two->mother = y;
                two->motherType = 2;
            }
            else
            {
                //y += exp(ln(ln(2)) + ln(2^index))
                
                //minimum depth = 4
                y->depth = 4;
                
                //ln(n) = ln(2^index)
                Addition* lnn = Addition::lnPowerOf2(index);
                
                //ln(2)
                Addition* ln2 = new Addition("\\ln(2)");
                
                //ln(ln(2)) + ln(2^index)
                lnn->ln.push_back(ln2);
                ln2->mother = lnn;
                ln2->motherType = 2;
                
                //lnn = ln(ln(2)) + ln(2^index)
                //lnn->print();
                
                y->exp.push_back(lnn);
                lnn->mother = y;
                lnn->motherType = 1;
                
                if(lnn->depth +1 > y->depth) y->depth = lnn->depth +1;
            }
            
            //x = x - bit, use bitwise XOR
            x = x ^ bit;
        }
        
        if(x==0) break;
        else
        {
            //product *= 2
            index++;
            product = product << 1;
        }
    }
    
    //reverse order
    reverse(y->exp.begin(),y->exp.end());
    
    return y;
}

Addition* Addition::PowerOf2(unsigned int x)
{
    //return 2^x
    if(x==0)
    {
        Addition* y = new Addition("1");
        return y;
    }
    else if(x==1)
    {
        Addition* y = new Addition("2");
        return y;
    }
    else
    {
        Addition* lny = Addition::lnPowerOf2(x);
        Addition* y = new Addition(1,lny);
        return y;
    }
}

Addition::Addition(int fundamentalType,unsigned int x)
{
    if(fundamentalType == 1)
    {
        //For positive integer
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 1;
        order = 0;
        
        nZero = false;
        nNegative = false;
        nOne = false;
        nTwo = false;
        nTau = false;
        nComplex = false;
        nInfinity = false;
        
        depth = 1;
        unsigned int index = 0;
        unsigned int product = 1;
        while(true)
        {
            //bitwise AND
            unsigned int bit = x & product;
            
            if(bit > 0)
            {
                //cout<<"2^"<<index<<" = "<<bit<<endl;
                
                if(index == 0)
                {
                    nOne = true;
                }
                else if(index == 1)
                {
                    nTwo = true;
                }
                else
                {
                    //minimum depth = 5
                    if (depth < 5) depth = 5;
                    
                    //y = 2^n = exp(ln(2^n))
                    Addition* lny = Addition::lnPowerOf2(index);
                    
                    exp.push_back(lny);
                    lny->mother = this;
                    lny->motherType = 1;
                    
                    if(lny->depth +1 > depth) depth = lny->depth +1;
                }
                
                //x = x - bit, use bitwise XOR
                x = x ^ bit;
            }
            
            if(x==0) break;
            else
            {
                //product *= 2
                index++;
                product = product << 1;
            }
        }
        
        //reverse order
        reverse(exp.begin(),exp.end());
        
        return;
    }
    if(fundamentalType == 2)
    {
        //For the nth variable
        mother = nullptr;
        motherType = 0;
        
        depth = 1;
        orderType = 2;
        order = 0;
        
        nZero = false;
        nNegative = false;
        nOne = false;
        nTwo = false;
        nTau = false;
        nComplex = false;
        nInfinity = false;
        
        for(int i = 0; i < x ; i++)
        {
            variable.push_back(false);
        }
        variable.push_back(true);
        
        return;
    }
}

Addition::Addition(int compositeType, Addition* operand)
{
    operand->mother = this;
    operand->motherType = compositeType;
    
    mother = nullptr;
    motherType = 0;
    
    depth = operand->depth +1;
    orderType = operand->orderType;
    
    if(orderType == 2)
    {
        if(compositeType == 1) order = operand->order +1;
        else if(compositeType == 2) order = operand->order -1;
        else if(compositeType == 3) order = operand->order;
    }
    else order = 0;
    
    nZero = false;
    nNegative = false;
    nOne = false;
    nTwo = false;
    nTau = false;
    nComplex = false;
    nInfinity = false;
    
    if(compositeType == 1) exp.push_back(operand);
    else if(compositeType == 2)
    {
        if(operand->haveOnlyNegativeOne()) ln_n1.push_back(operand);
        else if(operand->haveOnlyOne() || operand->haveOnlyTwo()) ln_2.push_back(operand);
        else if(operand->haveOnlyComplex()) ln_i.push_back(operand);
        else ln.push_back(operand);
    }
    else if(compositeType == 3) add.push_back(operand);
}

Addition::Addition(Addition* operand1, Addition* operand2)
{
    operand1->mother = this;
    operand1->motherType = 3;
    operand2->mother = this;
    operand2->motherType = 3;
    
    mother = nullptr;
    motherType = 0;
    
    if(operand1->depth > operand2->depth) depth = operand1->depth +1;
    else depth = operand2->depth +1;
    
    // |1 2 3
    //-+------
    //1|1 2 3
    //2|2 ? 3
    //3|3 3 3
    
    if(operand1->orderType == 3 || operand2->orderType == 3)
    {
        orderType = 3;
        order = 0;
    }
    else if(operand1->orderType == 1 && operand2->orderType == 1)
    {
        orderType = 1;
        order = 0;
    }
    else if(operand1->orderType == 2 && operand2->orderType == 1)
    {
        orderType = 2;
        order = operand1->order;
    }
    else if(operand1->orderType == 2 && operand2->orderType == 1)
    {
        orderType = 2;
        order = operand2->order;
    }
    else if(operand1->orderType == 2 && operand2->orderType == 2)
    {
        if(operand1->order == operand2->order)
        {
            orderType = 2;
            order = operand1->order;
        }
        else
        {
            orderType = 3;
            order = 0;
        }
    }
    else
    {
        cout<<"Error: Impossible case"<<endl;
    }
    
    nZero = false;
    nNegative = false;
    nOne = false;
    nTwo = false;
    nTau = false;
    nComplex = false;
    nInfinity = false;
    
    add.push_back(operand1);
    add.push_back(operand2);
    
    //sort(add.begin(), add.end(), [](Addition* a, Addition* b)->bool{return compare(a->depth,a->orderType,a->order,b->depth,b->orderType,b->order);});
}

Addition::~Addition()
{
    for(unsigned int i = 0; i < exp.size() ; i++)
    {
        delete exp[i];
    }
    
    for(unsigned int i = 0; i < ln_n1.size() ; i++)
    {
        delete ln_n1[i];
    }
    
    for(unsigned int i = 0; i < ln_2.size() ; i++)
    {
        delete ln_2[i];
    }
    
    for(unsigned int i = 0; i < ln_i.size() ; i++)
    {
        delete ln_i[i];
    }
    
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        delete ln[i];
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        delete add[i];
    }
}

string Addition::getLatex(bool isPrintInteger)
{
    string output;
    
    if(isPrintInteger)
    {
        SemiInterger temp = isSemiInterger();
        if(temp.type == 0)
        {
            output = std::to_string(temp.integer);
            return output;
        }
    }
    
    if(nZero)
    {
        output += " + 0";
    }
    
    if(nNegative)
    {
        output += " + (-1)";
    }
    
    if(nOne)
    {
        output += " + 1";
    }
    
    if(nTwo)
    {
        output += " + 2";
    }
    
    if(nTau)
    {
        output += " + \\\\tau";
    }
    
    if(nComplex)
    {
        output += " + i";
    }
    
    if(nInfinity)
    {
        output += " + (-\\\\infty)";
    }
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i])
        {
            if(i==0)
            {
                output += " + x";
            }
            else if(i==1)
            {
                output += " + y";
            }
            else if(i==2)
            {
                output += " + z";
            }
        }
    }
    
    for(unsigned int i = 0; i < exp.size() ; i++)
    {
        output += " + \\\\exp(";
        output += exp[i]->getLatex(isPrintInteger);
        output += ")";
        
        if(exp[i]->mother != this)
        {
            cout<<"Error: exp["<<i<<"]: the mother link is wrong"<<endl;
        }
        if(exp[i]->motherType != 1)
        {
            cout<<"Error: exp["<<i<<"]: the motherType is wrong"<<endl;
        }
    }
    
    for(unsigned int i = 0; i < ln_n1.size() ; i++)
    {
        output += " + \\\\ln(";
        output += ln_n1[i]->getLatex(isPrintInteger);
        output += ")";
        
        if(ln_n1[i]->mother != this)
        {
            cout<<"Error: ln_n1["<<i<<"]: the mother link is wrong"<<endl;
        }
        if(ln_n1[i]->motherType != 2)
        {
            cout<<"Error: ln_n1["<<i<<"]: the motherType is wrong"<<endl;
        }
    }
    
    for(unsigned int i = 0; i < ln_2.size() ; i++)
    {
        output += " + \\\\ln(";
        output += ln_2[i]->getLatex(isPrintInteger);
        output += ")";
        
        if(ln_2[i]->mother != this)
        {
            cout<<"Error: ln_c["<<i<<"]: the mother link is wrong"<<endl;
        }
        if(ln_2[i]->motherType != 2)
        {
            cout<<"Error: ln_c["<<i<<"]: the motherType is wrong"<<endl;
        }
    }
    
    for(unsigned int i = 0; i < ln_i.size() ; i++)
    {
        output += " + \\\\ln(";
        output += ln_i[i]->getLatex(isPrintInteger);
        output += ")";
        
        if(ln_i[i]->mother != this)
        {
            cout<<"Error: ln_i["<<i<<"]: the mother link is wrong"<<endl;
        }
        if(ln_i[i]->motherType != 2)
        {
            cout<<"Error: ln_i["<<i<<"]: the motherType is wrong"<<endl;
        }
    }
    
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        output += " + \\\\ln(";
        output += ln[i]->getLatex(isPrintInteger);
        output += ")";
        
        if(ln[i]->mother != this)
        {
            cout<<"Error: ln["<<i<<"]: the mother link is wrong"<<endl;
        }
        if(ln[i]->motherType != 2)
        {
            cout<<"Error: ln["<<i<<"]: the motherType is wrong"<<endl;
        }
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        string operand_latex = add[i]->getLatex(isPrintInteger);
        output += " + (";
        output += operand_latex;
        output += ")";
        
        if(add[i]->mother != this)
        {
            cout<<"Error: add["<<i<<"]: the mother link is wrong"<<endl;
        }
        if(add[i]->motherType != 3)
        {
            cout<<"Error: add["<<i<<"]: the motherType is wrong"<<endl;
        }
    }
    
    //erase " + "
    if(output.size() == 0)
    {
        cout<<"Error: getLatex: the expression is empty"<<endl;
    }
    else output.erase(0,3);
    
    //erase "(" and ")"
    if(output == "(-1)") output = "-1";
    else if(output == "(-\\\\infty)") output = "-\\\\infty";
    
    return output;
}

void Addition::print(bool isPrintInteger)
{
    cout<<getLatex(isPrintInteger)<<endl;
}

Addition* Addition::getCopy()
{
    Addition* copy = new Addition();
    
    copy->mother = nullptr;
    copy->motherType = 0;
    
    copy->depth = depth;
    copy->orderType = orderType;
    copy->order = order;
    
    copy->nZero = nZero;
    copy->nNegative = nNegative;
    copy->nOne = nOne;
    copy->nTwo = nTwo;
    copy->nTau = nTau;
    copy->nComplex = nComplex;
    copy->nInfinity = nInfinity;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        copy->variable.push_back(variable[i]);
    }
    
    for(unsigned int i = 0; i < exp.size() ; i++)
    {
        Addition* element = exp[i]->getCopy();
        element->mother = copy;
        element->motherType = 1;
        
        copy->exp.push_back(element);
    }
    
    for(unsigned int i = 0; i < ln_n1.size() ; i++)
    {
        Addition* element = ln_n1[i]->getCopy();
        element->mother = copy;
        element->motherType = 2;
        
        copy->ln_n1.push_back(element);
    }
    
    for(unsigned int i = 0; i < ln_2.size() ; i++)
    {
        Addition* element = ln_2[i]->getCopy();
        element->mother = copy;
        element->motherType = 2;
        
        copy->ln_2.push_back(element);
    }
    
    for(unsigned int i = 0; i < ln_i.size() ; i++)
    {
        Addition* element = ln_i[i]->getCopy();
        element->mother = copy;
        element->motherType = 2;
        
        copy->ln_i.push_back(element);
    }
    
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        Addition* element = ln[i]->getCopy();
        element->mother = copy;
        element->motherType = 2;
        
        copy->ln.push_back(element);
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        Addition* element = add[i]->getCopy();
        element->mother = copy;
        element->motherType = 3;
        
        copy->add.push_back(element);
    }
    
    return copy;
}

Addition* Addition::getTopmost()
{
    Addition* son = this;
    while(true)
    {
        if(son->mother == nullptr) return son;
        else son = son->mother;
    }
}

void Addition::fillDepthOrder()
{
    if(isEmpty())
    {
        cout<<"Error: fillDepthOrder: the expression is empty"<<endl;
        return;
    }
    
    depth = 1;
    orderType = 1;
    order = 0;
    
    bool existVariable = false;
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) existVariable = true;
    }
    
    if(existVariable) updateDepthOrder(depth,orderType,order,1,2,0);
    
    for(unsigned int i = 0; i < exp.size() ; i++)
    {
        exp[i]->fillDepthOrder();
        updateDepthOrder(depth,orderType,order, exp[i]->depth +1, exp[i]->orderType, exp[i]->order +1);
    }
    
    for(unsigned int i = 0; i < ln_n1.size() ; i++)
    {
        ln_n1[i]->fillDepthOrder();
        updateDepthOrder(depth,orderType,order, ln_n1[i]->depth +1, ln_n1[i]->orderType, ln_n1[i]->order -1);
    }
    
    for(unsigned int i = 0; i < ln_2.size() ; i++)
    {
        ln_2[i]->fillDepthOrder();
        updateDepthOrder(depth,orderType,order, ln_2[i]->depth +1, ln_2[i]->orderType, ln_2[i]->order -1);
    }
    
    for(unsigned int i = 0; i < ln_i.size() ; i++)
    {
        ln_i[i]->fillDepthOrder();
        updateDepthOrder(depth,orderType,order, ln_i[i]->depth +1, ln_i[i]->orderType, ln_i[i]->order -1);
    }
    
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        ln[i]->fillDepthOrder();
        updateDepthOrder(depth,orderType,order, ln[i]->depth +1, ln[i]->orderType, ln[i]->order -1);
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        add[i]->fillDepthOrder();
        updateDepthOrder(depth,orderType,order, add[i]->depth +1, add[i]->orderType, add[i]->order);
    }
}

void Addition::classifyln()
{
    int index = 0;
    while(true)
    {
        if(index >= ln.size()) break;
        
        if(ln[index]->haveOnlyNegativeOne())
        {
            ln_n1.push_back(ln[index]);
            ln.erase(ln.begin()+index);
        }
        else if(ln[index]->haveOnlyOne() || ln[index]->haveOnlyTwo())
        {
            ln_2.push_back(ln[index]);
            ln.erase(ln.begin()+index);
        }
        else if(ln[index]->haveOnlyComplex())
        {
            ln_i.push_back(ln[index]);
            ln.erase(ln.begin()+index);
        }
        else index++;
    }
}

bool Addition::isEmpty()
{
    if(nZero) return false;
    if(nNegative) return false;
    if(nOne) return false;
    if(nTwo) return false;
    if(nTau) return false;
    if(nComplex) return false;
    if(nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_2.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyZero()
{
    if(!nZero) return false;
    if(nNegative) return false;
    if(nOne) return false;
    if(nTwo) return false;
    if(nTau) return false;
    if(nComplex) return false;
    if(nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_2.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyNegativeOne()
{
    if(nZero) return false;
    if(!nNegative) return false;
    if(nOne) return false;
    if(nTwo) return false;
    if(nTau) return false;
    if(nComplex) return false;
    if(nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_2.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyOne()
{
    if(nZero) return false;
    if(nNegative) return false;
    if(!nOne) return false;
    if(nTwo) return false;
    if(nTau) return false;
    if(nComplex) return false;
    if(nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_2.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyTwo()
{
    if(nZero) return false;
    if(nNegative) return false;
    if(nOne) return false;
    if(!nTwo) return false;
    if(nTau) return false;
    if(nComplex) return false;
    if(nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_2.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyComplex()
{
    if(nZero) return false;
    if(nNegative) return false;
    if(nOne) return false;
    if(nTwo) return false;
    if(nTau) return false;
    if(!nComplex) return false;
    if(nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_2.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyInf()
{
    if(nZero) return false;
    if(nNegative) return false;
    if(nOne) return false;
    if(nTwo) return false;
    if(nTau) return false;
    if(nComplex) return false;
    if(!nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_2.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyOneItem()
{
    int sum = 0;
    
    if(nZero) sum++;
    if(nNegative) sum++;
    if(nOne) sum++;
    if(nTwo) sum++;
    if(nTau) sum++;
    if(nComplex) sum++;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) sum++;
    }
    
    sum += exp.size();
    sum += ln_n1.size();
    sum += ln_2.size();
    sum += ln_i.size();
    sum += ln.size();
    sum += add.size();
    
    if(sum == 1) return true;
    else return false;
}

SemiInterger Addition::isSemiInterger()
{
    //check whether it can be converted to the form: c, ln(c), ln(ln(c))
    //where c is positive integer
    vector<SemiInterger> input;
    SemiInterger output;
    
    for(unsigned int i = 0; i < exp.size() ; i++)
    {
        SemiInterger temp = exp[i]->isSemiInterger();
        if(temp.type == -1 || temp.type == 0) {output.type = -1; return output;}
        else
        {
            temp.type --;
            input.push_back(temp);
        }
    }
    
    for(unsigned int i = 0; i < ln_2.size() ; i++)
    {
        SemiInterger temp = ln_2[i]->isSemiInterger();
        if(temp.type != 0)
        {
            cout<<"Error: ln_2 contains non-interger."<<endl;
            output.type = -1;
            return output;
        }
        else
        {
            temp.type = 1;
            input.push_back(temp);
        }
    }
    
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        SemiInterger temp = ln[i]->isSemiInterger();
        if(temp.type == -1 || temp.type == 2) {output.type = -1; return output;}
        else if(temp.type == 0)
        {
            cout<<"Error: ln contains interger."<<endl;
            temp.type = 1;
            input.push_back(temp);
        }
        else if(temp.type == 1)
        {
            temp.type = 2;
            if(temp.integer != 2) cout<<"Error: ln(ln(c)) with c != 2."<<endl;
            input.push_back(temp);
        }
    }
    
    /*
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        SemiInterger temp = add[i]->isSemiInterger();
        if(temp.type == -1) {output.type = -1; return output;}
        else input.push_back(temp);
    }
    */
    
    //cout<<"start integer reduction:"<<endl;
    //print(true);
    
    if(nNegative) {output.type = -1; return output;}
    if(nTau) {output.type = -1; return output;}
    if(nComplex) {output.type = -1; return output;}
    if(nInfinity) {output.type = -1; return output;}
    if(ln_n1.size() != 0) {output.type = -1; return output;}
    if(ln_i.size() != 0) {output.type = -1; return output;}
    
    unsigned int nc = 0;
    unsigned int nlnc = 0;
    unsigned int nlnln2 = 0;
    for(unsigned int i = 0; i < input.size() ; i++)
    {
        if(input[i].type == 0) nc++;
        else if(input[i].type == 1) nlnc++;
        else if(input[i].type == 2) nlnln2++;
    }
    
    if(nlnln2 >= 2)
    {
        cout<<"Error: more than one ln(ln(2))."<<endl;
        output.type = -1;
        return output;
    }
    
    //Case 1: 1+2+4
    //Case 2: ln(ln(2)) + ln(n_1) + ln(n_2) + ... = ln(ln(2^(n_1*n_2*...)))
    if(nlnc == 0 && nlnln2 == 0)
    {
        //Case 1: 1+2+4
        output.type = 0;
        output.integer = 0;
        
        if(nOne) output.integer += 1;
        if(nTwo) output.integer += 2;
        
        for(unsigned int i = 0; i < input.size() ; i++)
        {
            output.integer += input[i].integer;
        }
        
        if(output.integer != 2)
        {
            //cout<<"isSemiInterger: 1+2+4+..."<<endl;
            //getTopmost()->print(true);
        }
        return output;
    }
    else if(nc == 0 && (nlnc + nlnln2) >= 1)
    {
        //Case 2: ln(ln(2)) + ln(n_1) + ln(n_2) + ... = ln(ln(2^(n_1*n_2*...)))
        if(nOne) {output.type = -1; return output;}
        if(nTwo) {output.type = -1; return output;}
        
        //ln(n_1) + ln(n_2) = ln(n_1*n_2)
        output.integer = 1;
        for(unsigned int i = 0; i < input.size() ; i++)
        {
            if(input[i].type == 1) output.integer *= input[i].integer;
        }
        
        if(nlnc >= 2)
        {
            //cout<<"isSemiInterger: ln(n_1) + ln(n_2) = ln(n_1*n_2)"<<endl;
            //getTopmost()->print(true);
        }
        
        if(nlnln2 == 1)
        {
            //ln(ln(2)) + ln(n) = ln(ln(2^n))
            if(nlnc >= 1)
            {
                if(output.integer <= 1) {output.type = -1; return output;}
                
                //calculate ln(ln(2^n))
                output.integer = 1 << output.integer;
                
                //cout<<"isSemiInterger: ln(ln(2)) + ln(n) = ln(ln(2^n))"<<endl;
                //getTopmost()->print(true);
            }
            else
            {
                output.integer = 2;
            }
            
            output.type = 2;
        }
        else
        {
            output.type = 1;
        }
        
        return output;
    }
    else {output.type = -1; return output;}
}

void Addition::cleanAdd(bool isPrintInteger)
{
    bool isChanged = false;
    
    //exp
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        for(unsigned int j = 0; j < add[i]->exp.size() ; j++)
        {
            exp.push_back(add[i]->exp[j]);
            add[i]->exp[j]->mother = this;
            isChanged = true;
        }
        add[i]->exp.clear();
    }
    
    if(isChanged)
    {
        eraseEmptyElement(add);
        
        cout<<"cleanAdd: exp"<<endl;
        if(isPrintInteger) getTopmost()->print(true);
        else getTopmost()->print();
    }
    isChanged = false;
    
    //ln
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        for(unsigned int j = 0; j < add[i]->ln_n1.size() ; j++)
        {
            ln_n1.push_back(add[i]->ln_n1[j]);
            add[i]->ln_n1[j]->mother = this;
            isChanged = true;
        }
        add[i]->ln_n1.clear();
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        for(unsigned int j = 0; j < add[i]->ln_2.size() ; j++)
        {
            ln_2.push_back(add[i]->ln_2[j]);
            add[i]->ln_2[j]->mother = this;
            isChanged = true;
        }
        add[i]->ln_2.clear();
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        for(unsigned int j = 0; j < add[i]->ln_i.size() ; j++)
        {
            ln_i.push_back(add[i]->ln_i[j]);
            add[i]->ln_i[j]->mother = this;
            isChanged = true;
        }
        add[i]->ln_i.clear();
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        for(unsigned int j = 0; j < add[i]->ln.size() ; j++)
        {
            ln.push_back(add[i]->ln[j]);
            add[i]->ln[j]->mother = this;
            isChanged = true;
        }
        add[i]->ln.clear();
    }
    
    if(isChanged)
    {
        eraseEmptyElement(add);
        
        cout<<"cleanAdd: ln"<<endl;
        if(isPrintInteger) getTopmost()->print(true);
        else getTopmost()->print();
    }
    isChanged = false;
    
    //add
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        for(unsigned int j = 0; j < add[i]->add.size() ; j++)
        {
            //add[i]->add[j]->exp.size must be 0
            //add[i]->add[j]->ln.size must be 0
            //because the cleanAdd is done for lower depth
            //Hence, after add.push_back(add[i]->add[j])
            //For all k, add[k]->exp.size must be 0 and add[k]->ln.size must be 0
            
            add.push_back(add[i]->add[j]);
            add[i]->add[j]->mother = this;
            isChanged = true;
        }
        add[i]->add.clear();
    }
    
    if(isChanged)
    {
        eraseEmptyElement(add);
        
        cout<<"cleanAdd: add"<<endl;
        if(isPrintInteger) getTopmost()->print(true);
        else getTopmost()->print();
    }
    isChanged = false;
}

void Addition::explnCancellation(bool isPrintInteger)
{
    //\exp(\ln(x_1)) + \exp(\ln(x_2)) + y = x_1 + x_2 + y
    bool isChanged = false;
    int index = 0;
    while(true)
    {
        if(index >= exp.size()) break;
        
        if(exp[index]->nZero) {index++; continue;}
        if(exp[index]->nNegative) {index++; continue;}
        if(exp[index]->nOne) {index++; continue;}
        if(exp[index]->nTwo) {index++; continue;}
        if(exp[index]->nTau) {index++; continue;}
        if(exp[index]->nComplex) {index++; continue;}
        if(exp[index]->nInfinity) {index++; continue;}
        
        bool nVariable = false;
        for(unsigned int j = 0; j < exp[index]->variable.size() ; j++)
        {
            if(exp[index]->variable[j]) nVariable = true;
        }
        if(nVariable) {index++; continue;}
        
        if(exp[index]->exp.size() != 0) {index++; continue;}
        if(exp[index]->add.size() != 0) {index++; continue;}
        
        if(exp[index]->ln_n1.size() +
           exp[index]->ln_2.size()  +
           exp[index]->ln_i.size()  +
           exp[index]->ln.size() != 1) {index++; continue;}
        
        //move x to add
        vector<Addition*>* ln_x = nullptr;
        if     (exp[index]->ln_n1.size() == 1) ln_x = &(exp[index]->ln_n1);
        else if(exp[index]->ln_2.size() == 1)  ln_x = &(exp[index]->ln_2);
        else if(exp[index]->ln_i.size() == 1)  ln_x = &(exp[index]->ln_i);
        else if(exp[index]->ln.size() == 1)    ln_x = &(exp[index]->ln);
        
        add.push_back((*ln_x)[0]);
        (*ln_x)[0]->mother = this;
        (*ln_x)[0]->motherType = 3;
        
        (*ln_x).clear();
        delete exp[index];
        exp.erase(exp.begin()+index);
        
        isChanged = true;
    }
    
    if(isChanged)
    {
        //not empty because add.push_back
        cout<<"explnCancellation: \\exp(\\ln(x)) = x"<<endl;
        if(isPrintInteger) getTopmost()->print(true);
        else getTopmost()->print();
    }
    isChanged = false;
    
    //For \exp(\ln(\exp(x_1)) + \ln(\exp(x_2)) + y) = \exp(x_1 + x_2 + y)
    if(mother == nullptr) return;
    if(motherType != 1) return;
    
    index = 0;
    while(true)
    {
        if(index >= ln.size()) break;
        
        if(ln[index]->nZero) {index++; continue;}
        if(ln[index]->nNegative) {index++; continue;}
        if(ln[index]->nOne) {index++; continue;}
        if(ln[index]->nTwo) {index++; continue;}
        if(ln[index]->nTau) {index++; continue;}
        if(ln[index]->nComplex) {index++; continue;}
        if(ln[index]->nInfinity) {index++; continue;}
        
        bool nVariable = false;
        for(unsigned int j = 0; j < ln[index]->variable.size() ; j++)
        {
            if(ln[index]->variable[j]) nVariable = true;
        }
        if(nVariable) {index++; continue;}
        
        if(ln[index]->ln_n1.size() != 0) {index++; continue;}
        if(ln[index]->ln_2.size() != 0) {index++; continue;}
        if(ln[index]->ln_i.size() != 0) {index++; continue;}
        if(ln[index]->ln.size() != 0) {index++; continue;}
        if(ln[index]->add.size() != 0) {index++; continue;}
        
        if(ln[index]->exp.size() != 1) {index++; continue;}
        
        //move x to add
        add.push_back(ln[index]->exp[0]);
        ln[index]->exp[0]->mother = this;
        ln[index]->exp[0]->motherType = 3;
        
        ln[index]->exp.clear();
        delete ln[index];
        ln.erase(ln.begin()+index);
        
        isChanged = true;
    }
    
    if(isChanged)
    {
        //not empty because add.push_back
        cout<<"explnCancellation: \\ln(\\exp(x)) = x"<<endl;
        if(isPrintInteger) getTopmost()->print(true);
        else getTopmost()->print();
    }
    isChanged = false;
    
    //no need to do explnCancellation for this->add
    //because the cleanAdd is done for this->add[i]
    //this->add[i]->exp.size must be 0, this->add[i]->ln.size must be 0
}

bool Addition::lnInf()
{
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        if(ln[i]->haveOnlyInf()) return true;
    }
    
    return false;
}

void Addition::ln0()
{
    //EXP[ ln(0) + x ]  = EXP[ -inf + x ]
    bool isChanged = false;
    int index = 0;
    while(true)
    {
        if(index >= ln.size()) break;
        
        if(ln[index]->haveOnlyZero())
        {
            delete ln[index];
            ln.erase(ln.begin()+index);
            
            Addition* inf = new Addition("-\\infty");
            
            add.push_back(inf);
            inf->mother = this;
            inf->motherType = 3;
            
            isChanged = true;
        }
        else index++;
    }
    
    if(isChanged)
    {
        cout<<"ln0: ln(0) = -inf"<<endl;
        getTopmost()->print();
    }
}

void Addition::addInf()
{
    if(haveOnlyInf()) return;
    
    bool existInfinity = nInfinity;
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        if(add[i]->nInfinity) existInfinity = true;
    }
    
    //erase all elements in add
    if(existInfinity)
    {
        nZero = false;
        nNegative = false;
        nOne = false;
        nTwo = false;
        nTau = false;
        nComplex = false;
        nInfinity = true;
        
        for(unsigned int i = 0; i < variable.size() ; i++)
        {
            variable[i] = false;
        }
        
        for(unsigned int i = 0; i < exp.size() ; i++)
        {
            delete exp[i];
        }
        
        for(unsigned int i = 0; i < ln_n1.size() ; i++)
        {
            delete ln_n1[i];
        }
        
        for(unsigned int i = 0; i < ln_2.size() ; i++)
        {
            delete ln_2[i];
        }
        
        for(unsigned int i = 0; i < ln_i.size() ; i++)
        {
            delete ln_i[i];
        }
        
        for(unsigned int i = 0; i < ln.size() ; i++)
        {
            delete ln[i];
        }
        
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            delete add[i];
        }
        
        exp.clear();
        ln_n1.clear();
        ln_2.clear();
        ln_i.clear();
        ln.clear();
        add.clear();
        
        cout<<"addInf: -inf + x = -inf"<<endl;
        getTopmost()->print();
        return;
    }
}

void Addition::expInf()
{
    bool isChanged = false;
    int index = 0;
    while(true)
    {
        if(index >= exp.size()) break;
        
        if(exp[index]->nInfinity)
        {
            delete exp[index];
            exp.erase(exp.begin()+index);
            isChanged = true;
        }
        else index++;
    }
    
    if(isChanged)
    {
        if(isEmpty()) nZero = true;
        cout<<"expInf: exp(-inf) = 0"<<endl;
        getTopmost()->print();
    }
}

void Addition::exp0()
{
    bool isChanged = false;
    int index = 0;
    while(true)
    {
        if(index >= exp.size()) break;
        
        if(exp[index]->haveOnlyZero())
        {
            delete exp[index];
            exp.erase(exp.begin()+index);
            
            Addition* one = new Addition(1,1);
            
            add.push_back(one);
            one->mother = this;
            one->motherType = 3;
            
            isChanged = true;
        }
        else index++;
    }
    
    if(isChanged)
    {
        cout<<"exp0: exp(0) = 1"<<endl;
        getTopmost()->print();
    }
}

void Addition::ln_1nic()
{
    if(mother == nullptr) return;
    else if(motherType != 1) return;
    
    bool isChanged = false;
    int product = 1;
    
    //For ln_i
    if(ln_i.size() >= 2)
    {
        unsigned long n;
        if(ln_i.size() % 2 == 0)
        {
            n = ln_i.size();
        }
        else
        {
            n = ln_i.size() -1;
        }
        
        for(unsigned int i = 0; i < n ; i++)
        {
            delete ln_i[0];
            ln_i.erase(ln_i.begin());
        }
        
        if(n/2 % 2 == 1) product *= -1;
        
        isChanged = true;
    }
    
    //For ln_n1
    if(ln_n1.size() >= 2)
    {
        unsigned long n;
        if(ln_n1.size() % 2 == 0)
        {
            n = ln_n1.size();
        }
        else
        {
            n = ln_n1.size() -1;
        }
        
        for(unsigned int i = 0; i < n ; i++)
        {
            delete ln_n1[0];
            ln_n1.erase(ln_n1.begin());
        }
        
        isChanged = true;
    }
    
    if(product == -1)
    {
        if(ln_n1.size() == 1)
        {
            delete ln_n1[0];
            ln_n1.erase(ln_n1.begin());
        }
        else if(ln_n1.size() == 0)
        {
            Addition* n1 = new Addition("-1");
            
            ln_n1.push_back(n1);
            n1->mother = this;
            n1->motherType = 2;
        }
        
        product = 1;
    }
    
    //check whether the expression need to be simplified
    {
        int count_one = 0;
        int count_c = 0;
        for(unsigned int j = 0; j < ln_2.size() ; j++)
        {
            if(ln_2[j]->nOne)
            {
                count_one++;
            }
            else
            {
                count_c++;
            }
        }
        
        if(count_one >= 1) isChanged = true;
    }
    
    //For ln_c
    if(isChanged)
    {
        int index = 0;
        while(true)
        {
            if(index >= ln_2.size()) break;
            
            if(ln_2[index]->nOne)
            {
                delete ln_2[index];
                ln_2.erase(ln_2.begin()+index);
            }
            else index++;
        }
    }
    
    if(isChanged)
    {
        if(isEmpty()) nZero = true;
        cout<<"log formula"<<endl;
        getTopmost()->print();
    }
}

void Addition::expand()
{
    //exp(x + ln(y_1 + y_2 + ...)) = exp(x + ln(y_1)) + exp(x + ln(y_2)) + ...
    
    unsigned int index1 = 0;
    while(true)
    {
        if(index1 >= exp.size()) break;
        
        //check whether each ln contains only one item
        bool isOnlyOne = true;
        int lnIndex = 0;
        for(unsigned int i = 0; i < exp[index1]->ln.size() ; i++)
        {
            if(!exp[index1]->ln[i]->haveOnlyOneItem())
            {
                isOnlyOne = false;
                lnIndex = i ;
                break;
            }
        }
        
        if(isOnlyOne) index1++;
        else
        {
            Addition* oldExpItem = exp[index1];
            Addition* oldExplnItem = exp[index1]->ln[lnIndex];
            Addition* newExpItem = exp[index1]->getCopy();
            delete newExpItem->ln[lnIndex];
            
            if(oldExplnItem->nZero)
            {
                newExpItem->ln[lnIndex] = new Addition("0");
                
                oldExplnItem->nZero = false;
            }
            else if(oldExplnItem->nNegative)
            {
                newExpItem->ln[lnIndex] = new Addition("-1");
                
                oldExplnItem->nNegative = false;
            }
            else if(oldExplnItem->nOne)
            {
                newExpItem->ln[lnIndex] = new Addition("1");
                
                oldExplnItem->nOne = false;
            }
            else if(oldExplnItem->nTwo)
            {
                newExpItem->ln[lnIndex] = new Addition("2");
                
                oldExplnItem->nTwo = false;
            }
            else if(oldExplnItem->nTau)
            {
                newExpItem->ln[lnIndex] = new Addition("\\tau");
                
                oldExplnItem->nTau = false;
            }
            else if(oldExplnItem->nComplex)
            {
                newExpItem->ln[lnIndex] = new Addition("i");
                
                oldExplnItem->nComplex = false;
            }
            else if(oldExplnItem->variable.size() >= 1)
            {
                for(unsigned int i = 0; i < oldExplnItem->variable.size() ; i++)
                {
                    if(oldExplnItem->variable[i])
                    {
                        newExpItem->ln[lnIndex] = new Addition(2,i);
                        
                        oldExplnItem->variable[i] = false;
                        
                        break;
                    }
                }
            }
            else if(oldExplnItem->exp.size() >= 1)
            {
                newExpItem->ln[lnIndex] = new Addition(1,oldExplnItem->exp[0]);
                
                oldExplnItem->exp.erase(oldExplnItem->exp.begin());
            }
            else if(oldExplnItem->ln_n1.size() >= 1)
            {
                newExpItem->ln[lnIndex] = new Addition(2,oldExplnItem->ln_n1[0]);
                
                oldExplnItem->ln_n1.erase(oldExplnItem->ln_n1.begin());
            }
            else if(oldExplnItem->ln_2.size() >= 1)
            {
                newExpItem->ln[lnIndex] = new Addition(2,oldExplnItem->ln_2[0]);
                
                oldExplnItem->ln_2.erase(oldExplnItem->ln_2.begin());
            }
            else if(oldExplnItem->ln_i.size() >= 1)
            {
                newExpItem->ln[lnIndex] = new Addition(2,oldExplnItem->ln_i[0]);
                
                oldExplnItem->ln_i.erase(oldExplnItem->ln_i.begin());
            }
            else if(oldExplnItem->ln.size() >= 1)
            {
                newExpItem->ln[lnIndex] = new Addition(2,oldExplnItem->ln[0]);
                
                oldExplnItem->ln.erase(oldExplnItem->ln.begin());
            }
            else if(oldExplnItem->add.size() >= 1)
            {
                newExpItem->ln[lnIndex] = new Addition(3,oldExplnItem->add[0]);
                
                oldExplnItem->add.erase(oldExplnItem->add.begin());
            }
            
            newExpItem->ln[lnIndex]->mother = newExpItem;
            newExpItem->ln[lnIndex]->motherType = 2;
            
            exp.insert(exp.begin()+index1,newExpItem);
            newExpItem->mother = this;
            newExpItem->motherType = 1;
            
            cout<<"Expand:"<<endl;
            getTopmost()->print();
            
            newExpItem->simplification();
            oldExpItem->simplification();
        }
    }
}

void Addition::addCommonTerm()
{
    //Type 1a:
    //0 + x = x
    //1 + 1 = 2
    //2 + 2 = \exp(\exp(\ln(2) + \ln(\ln(2))))
    //(-1) + (-1) = \exp(\ln(2) + \ln(-1))
    //1 + (-1) = 0
    //2 + (-1) = 1
    
    //Type 1b: x = tau, i, x, y, z, ln(x)
    //x + x = \exp(\ln(2) + \ln(x))
    
    //Type 1c: x = positiveInteger, (-1), tau, i, x, y, z, ln(x)
    //x + \exp(\ln(-1)          + \ln(x)) = 0
    //x + \exp(          \ln(c) + \ln(x)) = \exp(          \ln(c+1) + \ln(x))
    //x + \exp(\ln(-1) + \ln(c) + \ln(x)) = \exp(\ln(-1) + \ln(c-1) + \ln(x))
    
    //Type 2: x = 1 + 2 + (-1) + tau + i + x + y + z + exp(x) + exp(y) + ... + ln(x) + ln(y) + ...
    //\exp(                     x) + \exp(                     x) = \exp(          \ln(2)       + x)
    //\exp(                     x) + \exp(\ln(-1)            + x) = 0
    //\exp(                     x) + \exp(          \ln(c)   + x) = \exp(          \ln(c+1)     + x)
    //\exp(                     x) + \exp(\ln(-1) + \ln(c)   + x) = \exp(\ln(-1) + \ln(c-1)     + x)
    
    //\exp(\ln(-1)            + x) + \exp(\ln(-1)            + x) = \exp(\ln(-1) + \ln(2)       + x)
    //\exp(\ln(-1)            + x) + \exp(          \ln(c)   + x) = \exp(          \ln(c-1)     + x)
    //\exp(\ln(-1)            + x) + \exp(\ln(-1) + \ln(c)   + x) = \exp(\ln(-1) + \ln(c+1)     + x)
    
    //\exp(          \ln(c_1) + x) + \exp(          \ln(c_2) + x) = \exp(          \ln(c_1+c_2) + x)
    //\exp(          \ln(c_1) + x) + \exp(\ln(-1) + \ln(c_2) + x) = \exp(          \ln(c_1-c_2) + x)
    //                                                              \exp(\ln(-1) + \ln(c_2-c_1) + x)
    
    //\exp(\ln(-1) + \ln(c_1) + x) + \exp(\ln(-1) + \ln(c_2) + x) = \exp(\ln(-1) + \ln(c_1+c_2) + x)
    
    {
        //For zero
        bool isChanged = false;
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            if(add[i]->nZero)
            {
                add[i]->nZero = false;
                isChanged = true;
            }
        }
        
        if(isChanged)
        {
            eraseEmptyElement(add);
            if(isEmpty()) nZero = true;
        }
        
        if(nZero && !haveOnlyZero())
        {
            nZero = false;
            isChanged = true;
        }
        
        if(isChanged)
        {
            cout<<"addCommonTerm: 0 + x = x"<<endl;
            getTopmost()->print();
        }
        isChanged = false;
        
        //For 1
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            if(add[i]->nOne)
            {
                add[i]->nOne = false;
                
                if(nOne)
                {
                    nOne = false;
                    
                    Addition* two = new Addition("2");
                    add.push_back(two);
                    two->mother = this;
                    two->motherType = 3;
                }
                else
                {
                    nOne = true;
                }
                isChanged = true;
            }
        }
        
        if(isChanged)
        {
            eraseEmptyElement(add);
            if(isEmpty()) nZero = true;
        }
        
        if(isChanged)
        {
            cout<<"addCommonTerm: one"<<endl;
            getTopmost()->print();
        }
        isChanged = false;
        
        //For 2
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            if(add[i]->nTwo)
            {
                add[i]->nTwo = false;
                
                if(nTwo)
                {
                    nTwo = false;
                    
                    Addition* four = new Addition("\\exp(\\ln(\\ln(2)))");
                    
                    Addition* two = new Addition("2");
                    four->exp[0]->ln_2.push_back(two);
                    two->mother = four->exp[0];
                    two->motherType = 2;
                    
                    exp.push_back(four);
                    four->mother = this;
                    four->motherType = 1;
                }
                else
                {
                    nTwo = true;
                }
                isChanged = true;
            }
        }
        
        if(isChanged)
        {
            eraseEmptyElement(add);
            if(isEmpty()) nZero = true;
        }
        
        if(isChanged)
        {
            cout<<"addCommonTerm: two"<<endl;
            getTopmost()->print();
        }
        isChanged = false;
        
        //For -1
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            if(add[i]->nNegative)
            {
                add[i]->nNegative = false;
                
                if(nNegative)
                {
                    nNegative = false;
                    
                    Addition* ln2 = new Addition("\\ln(2)");
                    Addition* n1 = new Addition("-1");
                    
                    ln2->ln_n1.push_back(n1);
                    n1->mother = ln2;
                    n1->motherType = 2;
                    
                    exp.push_back(ln2);
                    ln2->mother = this;
                    ln2->motherType = 1;
                }
                else
                {
                    nNegative = true;
                }
                isChanged = true;
            }
        }
        
        if(isChanged)
        {
            eraseEmptyElement(add);
            if(isEmpty()) nZero = true;
        }
        
        if(isChanged)
        {
            cout<<"addCommonTerm: -1"<<endl;
            getTopmost()->print();
        }
        isChanged = false;
    }
    
    //For Type 1b
    //ln(x) + ln(x) = exp(ln(2) + ln(ln(x)))
    while(true)
    {
        bool isFound = false;
        for(unsigned int i = 0; i < ln_2.size() ; i++)
        {
            for(unsigned int j = i+1; j < ln_2.size() ; j++)
            {
                if(isSame(ln_2[i],ln_2[j]))
                {
                    isFound = true;
                    
                    //delete ln_c[j]
                    delete ln_2[j];
                    ln_2.erase(ln_2.begin()+j);
                    
                    //create ln(2)
                    Addition* ln2 = new Addition("\\ln(2)");
                    
                    //move ln_c[i]
                    Addition* lnx = new Addition(2,ln_2[i]);
                    //erase ln_c[i]
                    ln_2.erase(ln_2.begin()+i);
                    
                    //create ln(2) + ln(ln(x))
                    ln2->ln.push_back(lnx);
                    lnx->mother = ln2;
                    lnx->motherType = 2;
                    
                    //create exp(ln(2) + ln(ln(x)))
                    exp.push_back(ln2);
                    ln2->mother = this;
                    ln2->motherType = 1;
                    
                    cout<<"addCommonTerm: ln(x) + ln(x) = exp(ln(2) + ln(ln(x)))"<<endl;
                    getTopmost()->print();
                    
                    break;
                }
            }
            
            if(isFound) break;
        }
        
        if(!isFound) break;
    }
    
    //For Type 1c
    
    //For Type 2
    //exp(x) + exp(x) = exp(ln(2) + x)
    while(true)
    {
        bool isFound = false;
        for(unsigned int i = 0; i < exp.size() ; i++)
        {
            for(unsigned int j = i+1; j < exp.size() ; j++)
            {
                if(isSame(exp[i],exp[j]))
                {
                    isFound = true;
                    
                    //delete exp[j]
                    delete exp[j];
                    exp.erase(exp.begin()+j);
                    
                    //modift exp[i]
                    Addition* two = new Addition("2");
                    
                    exp[i]->ln_2.push_back(two);
                    two->mother = exp[i];
                    two->motherType = 2;
                    
                    cout<<"addCommonTerm: exp(x) + exp(x) = exp(ln(2) + x)"<<endl;
                    getTopmost()->print();
                    
                    exp[i]->addCommonTerm();
                    
                    break;
                }
            }
            
            if(isFound) break;
        }
        
        if(!isFound) break;
    }
}

void Addition::simplification()
{
    for(unsigned int i = 0; i < exp.size() ; i++)
    {
        exp[i]->simplification();
    }
    
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        ln[i]->simplification();
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        add[i]->simplification();
    }
    
    //cout<<"start simplifying:"<<endl;
    //print();
    
    //implicit done in the following functions
    //x + (0) = x
    
    //----------------------
    //For example1:
    //  exp(ln( exp(x) + (x) ))
    //= ( exp(x) + (x) )           (explnCancellation)
    //= exp(x) + (x)               (cleanAdd)
    
    //----------------------
    //For example2:
    //  EXP[ (ln(exp(x))) + (x) ]
    //= EXP[ ln(exp(x)) + (x) ]      (cleanAdd)
    //= EXP[ (x) + (x) ]             (explnCancellation)
    
    //----------------------
    //(exp(x_1)+exp(x_2)+...) + (exp(y_1)+exp(y_2)+...) + ... = exp(x_1)+exp(x_2)+... + exp(y_1)+exp(y_2)+... + ...
    //( ln(x_1)+ ln(x_2)+...) + ( ln(y_1)+ ln(y_2)+...) + ... =  ln(x_1)+ ln(x_2)+... +  ln(y_1)+ ln(y_2)+... + ...
    //(0) + (2) + ((-1)) + (tau) + (i) + ((-inf)) + (x) + (y) + (z) = 0 + 2 + (-1) + tau + i + (-inf) + x + y + z
    cleanAdd();
    
    //----------------------
    //exp(ln(x)) + y = (x) + y
    //EXP[ ln(exp(x)) + y ] = EXP[ (x) + y ]
    explnCancellation();
    cleanAdd();
    //better search for EXP[]
    //----------------------
    
    //----------------------
    //check whether ln(-inf) exist
    if(lnInf())
    {
        cout<<"Error: In(-inf) exist"<<endl;
        return;
    }
    
    //----------------------
    //For example:
    //EXP[ ln(0) + x ]  = EXP[ -inf + x ]
    //                  = EXP[ -inf ]
    
    //----------------------
    //EXP[ ln(0) + x ]  = EXP[ -inf + x ]
    ln0();
    
    //----------------------
    //-inf + x = -inf
    addInf();
    
    //----------------------
    //exp(-inf) = 0
    expInf();
    //exp(0) = 1
    exp0();
    
    //----------------------
    //log formula:
    //EXP[ ln(i) + ln(i) + x ] = //EXP[ ln(-1) + x ]
    //EXP[ ln(-1) + ln(-1) + x ] = //EXP[ ln(1) + x ]
    //EXP[ ln(1) + x ] = EXP[ x ]
    //need classifyln
    classifyln();
    ln_1nic();
    
    //----------------------
    //For example:
    //  EXP[ exp(ln(2) + ln(tau) + ln(i)) + exp(ln(-1) + ln(tau + x) + ln(i)) + y ]
    //= EXP[ exp(ln(2) + ln(tau) + ln(i)) + exp(ln(-1) + ln(tau) + ln(i)) + exp(ln(-1) + ln(x) + ln(i)) + y ] (Expand)
    //= EXP[ exp(ln(tau) + ln(i)) + exp(ln(-1) + ln(x) + ln(i)) + y ]                                         (addCommonTerm)
    //= EXP[ ln(i) + exp( ln(-1) + ln(x) + ln(i)) + y ]                                                       (Euler formula)
    
    //----------------------
    //For example:
    //  exp( ln(1+i) + ln(1+i) )
    //= exp( ln(1) + ln(1+i) ) + exp( ln(i) + ln(x+y) )                    (expand)
    //= exp( ln(1+i) ) + exp( ln(i) + ln(1+i) )                            (ln_1nic for lower depth)
    //= exp(ln(1)) + exp(ln(i)) + exp( ln(i) + ln(1+i) )                   (expand)
    //= exp(0) + exp(ln(i)) + exp( ln(i) + ln(1+i) )                       (ln(1) for lower depth)
    //= exp(0) + exp(ln(i)) + exp( ln(i) + ln(1) ) + exp( ln(i) + ln(i) )  (ln(1) for lower depth)
    //= exp(0) + exp(ln(i)) + exp(ln(i)) + exp(ln(-1))                     (ln_1nic for lower depth)
    //= exp(0) + (i) + (i) + (-1)                                          (explnCancellation)
    //= (1) + (i) + (i) + (-1)                                             (exp0)
    //= exp(ln(2) + ln(i))                                                 (addCommonTerm)
    
    //----------------------
    //Expand:
    //exp(x + ln(y + z)) = exp(x + ln(y)) + exp(x + ln(z))
    //need ln_1nic before expand, to ensure ln_n1.size() <= 1, etc
    expand();
    explnCancellation();
    cleanAdd();
    exp0();
    
    //----------------------
    //c >= 2
    //Type 0: x = positiveInteger, (-1)
    //x_1 + x_2 + ... = \exp(\ln(-1) + \ln(c))
    
    //Type 1: x = tau, i, x, y, z, ln(x)
    //x + x = \exp(\ln(2) + \ln(x))
    
    //Type 2: x = positiveInteger, (-1), tau, i, x, y, z, ln(x)
    //x + \exp(\ln(-1)          + \ln(x)) = 0
    //x + \exp(          \ln(c) + \ln(x)) = \exp(          \ln(c+1) + \ln(x))
    //x + \exp(\ln(-1) + \ln(c) + \ln(x)) = \exp(\ln(-1) + \ln(c-1) + \ln(x))
    
    //Type 3: x = positiveInteger + (-1) + tau + i + x + y + z + exp(x) + exp(y) + ... + ln(x) + ln(y) + ...
    //\exp(                     x) + \exp(                     x) = \exp(          \ln(2)       + x)
    //\exp(                     x) + \exp(\ln(-1)            + x) = 0
    //\exp(                     x) + \exp(          \ln(c)   + x) = \exp(          \ln(c+1)     + x)
    //\exp(                     x) + \exp(\ln(-1) + \ln(c)   + x) = \exp(\ln(-1) + \ln(c-1)     + x)
    
    //\exp(\ln(-1)            + x) + \exp(\ln(-1)            + x) = \exp(\ln(-1) + \ln(2)       + x)
    //\exp(\ln(-1)            + x) + \exp(          \ln(c)   + x) = \exp(          \ln(c-1)     + x)
    //\exp(\ln(-1)            + x) + \exp(\ln(-1) + \ln(c)   + x) = \exp(\ln(-1) + \ln(c+1)     + x)
    
    //\exp(          \ln(c_1) + x) + \exp(          \ln(c_2) + x) = \exp(          \ln(c_1+c_2) + x)
    //\exp(          \ln(c_1) + x) + \exp(\ln(-1) + \ln(c_2) + x) = \exp(          \ln(c_1-c_2) + x)
    //                                                              \exp(\ln(-1) + \ln(c_2-c_1) + x)
    
    //\exp(\ln(-1) + \ln(c_1) + x) + \exp(\ln(-1) + \ln(c_2) + x) = \exp(\ln(-1) + \ln(c_1+c_2) + x)
    //need classifyln
    for(unsigned int i = 0; i < exp.size() ; i++)
    {
        exp[i]->classifyln();
    }
    addCommonTerm();
    
    //----------------------
    //Euler formula:
    //EXP[ exp(                 ln(tau) + ln(i)) + x ] =X= EXP[                + x ]
    //EXP[ exp(ln(-1)         + ln(tau) + ln(i)) + x ] =X= EXP[          ln(i) + x ]
    //EXP[ exp(         ln(c) + ln(tau) + ln(i)) + x ] =X= EXP[ ln(-1)         + x ]
    //EXP[ exp(ln(-1) + ln(c) + ln(tau) + ln(i)) + x ] =X= EXP[ ln(-1) + ln(i) + x ]
    euler();
}
