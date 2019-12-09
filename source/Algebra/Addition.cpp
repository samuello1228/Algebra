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

Addition::Addition()
{
    mother = nullptr;
    motherType = 0;
    
    depth = 0;
    orderType = 0;
    order = 0;
    
    nZero = false;
    nNegative = false;
    positveInterger = 0;
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
        cout<<"Syntax Error: the expression is empty"<<endl;
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
                positveInterger = 0;
                nTau = false;
                nComplex = false;
                nInfinity = false;
                
                add.push_back(operand);
            }
            
            sort(add.begin(), add.end(), [](Addition* a, Addition* b)->bool{
                return compare(a->depth,a->orderType,a->order,b->depth,b->orderType,b->order);
            });
            
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
                positveInterger = 0;
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
                positveInterger = 0;
                nTau = false;
                nComplex = false;
                nInfinity = false;
                
                if(operand->haveOnlyNegativeOne()) ln_n1.push_back(operand);
                else if(operand->haveOnlyPositveInterger()) ln_c.push_back(operand);
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
        positveInterger = 0;
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
        positveInterger = 0;
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
        positveInterger = 0;
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
        positveInterger = 0;
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
        positveInterger = 0;
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
        positveInterger = 0;
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
        positveInterger = 0;
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
        positveInterger = 0;
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
                mother = nullptr;
                motherType = 0;
                
                depth = 1;
                orderType = 1;
                order = 0;
                
                nZero = false;
                nNegative = false;
                positveInterger = std::stoi(positive_integer);
                nTau = false;
                nComplex = false;
                nInfinity = false;
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
        cout<<"Syntax Error: the expression cannot be processed"<<endl;
        
        mother = nullptr;
        motherType = 0;
        
        depth = 0;
        orderType = 0;
        order = 0;
        
        nZero = false;
        nNegative = false;
        positveInterger = 0;
        nTau = false;
        nComplex = false;
        nInfinity = false;
        
        return;
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
        positveInterger = x;
        nTau = false;
        nComplex = false;
        nInfinity = false;
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
        positveInterger = 0;
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
    }
    else order = 0;
    
    nZero = false;
    nNegative = false;
    positveInterger = 0;
    nTau = false;
    nComplex = false;
    nInfinity = false;
    
    if(compositeType == 1) exp.push_back(operand);
    else if(compositeType == 2)
    {
        if(operand->haveOnlyNegativeOne()) ln_n1.push_back(operand);
        else if(operand->haveOnlyPositveInterger()) ln_c.push_back(operand);
        else if(operand->haveOnlyComplex()) ln_i.push_back(operand);
        else ln.push_back(operand);
    }
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
    positveInterger = 0;
    nTau = false;
    nComplex = false;
    nInfinity = false;
    
    add.push_back(operand1);
    add.push_back(operand2);
    
    sort(add.begin(), add.end(), [](Addition* a, Addition* b)->bool{
        return compare(a->depth,a->orderType,a->order,b->depth,b->orderType,b->order);
    });
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
    
    for(unsigned int i = 0; i < ln_c.size() ; i++)
    {
        delete ln_c[i];
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

string Addition::getLatex()
{
    string output;
    
    if(nZero)
    {
        output += " + 0";
    }
    
    if(nNegative)
    {
        output += " + (-1)";
    }
    
    if(positveInterger != 0)
    {
        output += " + ";
        output += std::to_string(positveInterger);
    }
    
    if(nTau)
    {
        output += " + \\tau";
    }
    
    if(nComplex)
    {
        output += " + i";
    }
    
    if(nInfinity)
    {
        output += " + (-\\infty)";
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
        output += " + \\exp(";
        output += exp[i]->getLatex();
        output += ")";
    }
    
    for(unsigned int i = 0; i < ln_n1.size() ; i++)
    {
        output += " + \\ln(";
        output += ln_n1[i]->getLatex();
        output += ")";
    }
    
    for(unsigned int i = 0; i < ln_c.size() ; i++)
    {
        output += " + \\ln(";
        output += ln_c[i]->getLatex();
        output += ")";
    }
    
    for(unsigned int i = 0; i < ln_i.size() ; i++)
    {
        output += " + \\ln(";
        output += ln_i[i]->getLatex();
        output += ")";
    }
    
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        output += " + \\ln(";
        output += ln[i]->getLatex();
        output += ")";
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        string operand_latex = add[i]->getLatex();
        output += " + (";
        output += operand_latex;
        output += ")";
    }
    
    //erase " + "
    if(output.size() == 0)
    {
        cout<<"Error: the expression is empty"<<endl;
    }
    else output.erase(0,3);
    
    //erase "(" and ")"
    if(output == "(-1)") output = "-1";
    else if(output == "(-\\infty)") output = "-\\infty";
    
    return output;
}

void Addition::print()
{
    cout<<getLatex()<<endl;
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
    copy->positveInterger = positveInterger;
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
        element->mother = this;
        element->motherType = 1;
        
        copy->exp.push_back(element);
    }
    
    for(unsigned int i = 0; i < ln_n1.size() ; i++)
    {
        Addition* element = ln_n1[i]->getCopy();
        element->mother = this;
        element->motherType = 2;
        
        copy->ln_n1.push_back(element);
    }
    
    for(unsigned int i = 0; i < ln_c.size() ; i++)
    {
        Addition* element = ln_c[i]->getCopy();
        element->mother = this;
        element->motherType = 2;
        
        copy->ln_c.push_back(element);
    }
    
    for(unsigned int i = 0; i < ln_i.size() ; i++)
    {
        Addition* element = ln_i[i]->getCopy();
        element->mother = this;
        element->motherType = 2;
        
        copy->ln_i.push_back(element);
    }
    
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        Addition* element = ln[i]->getCopy();
        element->mother = this;
        element->motherType = 2;
        
        copy->ln.push_back(element);
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        Addition* element = add[i]->getCopy();
        element->mother = this;
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
        cout<<"Error: the expression is empty"<<endl;
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
    
    for(unsigned int i = 0; i < ln_c.size() ; i++)
    {
        ln_c[i]->fillDepthOrder();
        updateDepthOrder(depth,orderType,order, ln_c[i]->depth +1, ln_c[i]->orderType, ln_c[i]->order -1);
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

bool Addition::isEmpty()
{
    if(nZero) return false;
    if(nNegative) return false;
    if(positveInterger != 0) return false;
    if(nTau) return false;
    if(nComplex) return false;
    if(nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_c.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyZero()
{
    if(!nZero) return false;
    if(nNegative) return false;
    if(positveInterger != 0) return false;
    if(nTau) return false;
    if(nComplex) return false;
    if(nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_c.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyNegativeOne()
{
    if(nZero) return false;
    if(!nNegative) return false;
    if(positveInterger != 0) return false;
    if(nTau) return false;
    if(nComplex) return false;
    if(nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_c.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyPositveInterger()
{
    if(nZero) return false;
    if(nNegative) return false;
    if(positveInterger == 0) return false;
    if(nTau) return false;
    if(nComplex) return false;
    if(nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_c.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyComplex()
{
    if(nZero) return false;
    if(nNegative) return false;
    if(positveInterger != 0) return false;
    if(nTau) return false;
    if(!nComplex) return false;
    if(nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_c.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

bool Addition::haveOnlyInf()
{
    if(nZero) return false;
    if(nNegative) return false;
    if(positveInterger != 0) return false;
    if(nTau) return false;
    if(nComplex) return false;
    if(!nInfinity) return false;
    
    for(unsigned int i = 0; i < variable.size() ; i++)
    {
        if(variable[i]) return false;
    }
    
    if(exp.size() != 0) return false;
    if(ln_n1.size() != 0) return false;
    if(ln_c.size() != 0) return false;
    if(ln_i.size() != 0) return false;
    if(ln.size() != 0) return false;
    if(add.size() != 0) return false;
    
    return true;
}

void Addition::basicArithmetic()
{
    for(unsigned int i = 0; i < exp.size() ; i++)
    {
        exp[i]->basicArithmetic();
    }
    
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        ln[i]->basicArithmetic();
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        add[i]->basicArithmetic();
    }
    
    if(nInfinity)
    {
        nInfinity = false;
        if(isEmpty())
        {
            nInfinity = true;
            return;
        }
        
        nZero = false;
        nNegative = false;
        positveInterger = 0;
        nTau = false;
        nComplex = false;
        nInfinity = true;
        
        for(unsigned int i = 0; i < exp.size() ; i++)
        {
            delete exp[i];
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
        ln.clear();
        add.clear();
        
        cout<<"basicArithmetic: -inf + x = -inf"<<endl;
        getTopmost()->print();
        return;
    }
    
    if(nZero)
    {
        nZero = false;
        if(isEmpty())
        {
            nZero = true;
            return;
        }
        else
        {
            cout<<"basicArithmetic: 0 + x = x"<<endl;
            getTopmost()->print();
        }
    }
    
    if(nNegative && positveInterger >= 1)
    {
        //because it is not empty, nZero must be false
        if(positveInterger == 1)
        {
            nNegative = false;
            positveInterger = 0;
            
            if(isEmpty())
            {
                nZero = true;
                cout<<"basicArithmetic: 1 + (-1) = 0"<<endl;
                getTopmost()->print();
                return;
            }
        }
        else
        {
            nNegative = false;
            positveInterger--;
            
            cout<<"basicArithmetic: x + (-1)"<<endl;
            getTopmost()->print();
            return;
        }
    }
}

void Addition::cleanAddOld()
{
    basicArithmetic();
    
    for(unsigned int i = 0; i < exp.size() ; i++)
    {
        exp[i]->cleanAddOld();
    }
    
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        ln[i]->cleanAddOld();
    }
    
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        add[i]->cleanAddOld();
    }
    
    //-inf
    bool existInfinity = false;
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        if(add[i]->nInfinity) existInfinity = true;
    }
    
    //erase all elements in add
    if(existInfinity)
    {
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            delete add[i];
        }
        add.clear();
        
        nInfinity = true;
        
        cout<<"cleanAdd: -inf"<<endl;
        getTopmost()->print();
        return;
    }
    
    //zero
    bool isChanged = false;
    for(unsigned int i = 0; i < add.size() ; i++)
    {
        if(add[i]->nZero)
        {
            //Because basicArithmetic is done, add[i] must be empty
            add[i]->nZero = false;
            isChanged = true;
        }
    }
    
    if(isChanged)
    {
        eraseEmptyElement(add);
        if(isEmpty())
        {
            nZero = true;
            cout<<"cleanAdd: zero"<<endl;
            getTopmost()->print();
            return;
        }
        else
        {
            cout<<"cleanAdd: zero"<<endl;
            getTopmost()->print();
        }
    }
    isChanged = false;
    
    //integer
    {
        //positveInterger
        int sum = positveInterger;
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            if(add[i]->positveInterger != 0)
            {
                sum += add[i]->positveInterger;
                add[i]->positveInterger = 0;
                isChanged = true;
            }
        }
        
        //negative
        if(nNegative) sum--;
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            if(add[i]->nNegative)
            {
                sum--;
                add[i]->nNegative = false;
                isChanged = true;
            }
        }
        
        if(isChanged)
        {
            if(sum>=0)
            {
                nNegative = false;
                positveInterger = sum;
            }
            else if(sum == -1)
            {
                nNegative = true;
                positveInterger = 0;
            }
            else
            {
                nNegative = false;
                positveInterger = 0;
                
                Addition* c = new Addition(1,-sum);
                Addition* ln_c = new Addition(2,c);
                Addition* n1 = new Addition("-1");
                
                ln_c->ln.push_back(n1);
                n1->mother = ln_c;
                n1->motherType = 2;
                
                exp.push_back(ln_c);
            }
        }
    }
    
    if(isChanged)
    {
        eraseEmptyElement(add);
        if(isEmpty())
        {
            nZero = true;
            cout<<"cleanAdd: interger"<<endl;
            getTopmost()->print();
            return;
        }
        else
        {
            cout<<"cleanAdd: interger"<<endl;
            getTopmost()->print();
        }
    }
    isChanged = false;
    
    //tau
    {
        unsigned int sum = 0;
        if(nTau) sum++;
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            if(add[i]->nTau)
            {
                sum++;
                add[i]->nTau = false;
                isChanged = true;
            }
        }
        
        if(isChanged)
        {
            if(sum == 1)
            {
                nTau = true;
            }
            else
            {
                nTau = false;
                
                Addition* c = new Addition(1,sum);
                Addition* ln_c = new Addition(2,c);
                Addition* tau = new Addition("\\tau");
                
                ln_c->ln.push_back(tau);
                tau->mother = ln_c;
                tau->motherType = 2;
                
                exp.push_back(ln_c);
            }
        }
    }
    
    if(isChanged)
    {
        eraseEmptyElement(add);
        cout<<"cleanAdd: tau"<<endl;
        getTopmost()->print();
    }
    isChanged = false;
    
    //i
    {
        unsigned int sum = 0;
        if(nComplex) sum++;
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            if(add[i]->nComplex)
            {
                sum++;
                add[i]->nComplex = false;
                isChanged = true;
            }
        }
        
        if(isChanged)
        {
            if(sum == 1)
            {
                nComplex = true;
            }
            else
            {
                nComplex = false;
                
                Addition* c = new Addition(1,sum);
                Addition* ln_c = new Addition(2,c);
                Addition* i = new Addition("i");
                
                ln_c->ln.push_back(i);
                i->mother = ln_c;
                i->motherType = 2;
                
                exp.push_back(ln_c);
            }
        }
    }
    
    if(isChanged)
    {
        eraseEmptyElement(add);
        cout<<"cleanAdd: i"<<endl;
        getTopmost()->print();
    }
    isChanged = false;
    
    //Variable
    {
        vector<unsigned int> sum;
        sum.resize(variable.size(),0);
        for(unsigned int i = 0; i < variable.size() ; i++)
        {
            if(variable[i]) sum[i]++;
        }
        
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            for(unsigned int j = 0; j < add[i]->variable.size() ; j++)
            {
                if(add[i]->variable[j])
                {
                    if(j >= sum.size())
                    {
                        //expand vector sum
                        unsigned long original_size = sum.size();
                        for(unsigned int k = 0; k < j-original_size ; k++)
                        {
                            sum.push_back(0);
                        }
                        sum.push_back(1);
                    }
                    else
                    {
                        sum[j]++;
                    }
                    
                    isChanged = true;
                }
            }
            
            add[i]->variable.clear();
        }
        
        if(isChanged)
        {
            for(unsigned int i = 0; i < sum.size() ; i++)
            {
                if(sum[i] == 0) {}
                else if(sum[i] == 1)
                {
                    if(i >= variable.size())
                    {
                        //expand vector sum
                        unsigned long original_size = variable.size();
                        for(int k = 0; k < i-original_size ; k++)
                        {
                            variable.push_back(false);
                        }
                        variable.push_back(true);
                    }
                    else
                    {
                        variable[i] = true;
                    }
                }
                else
                {
                    if(i < variable.size())
                    {
                        variable[i] = false;
                    }
                    
                    Addition* c = new Addition(1,sum[i]);
                    Addition* ln_c = new Addition(2,c);
                    Addition* x = new Addition(2,i);
                    
                    ln_c->ln.push_back(x);
                    x->mother = ln_c;
                    x->motherType = 2;
                    
                    exp.push_back(ln_c);
                }
            }
        }
    }
    
    if(isChanged)
    {
        eraseEmptyElement(add);
        cout<<"cleanAdd: variable"<<endl;
        getTopmost()->print();
    }
    isChanged = false;
    
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
        fillDepthOrder();
        
        sort(exp.begin(), exp.end(), [](Addition* a, Addition* b)->bool{
            return compare(a->depth,a->orderType,a->order,b->depth,b->orderType,b->order);
        });
        
        cout<<"cleanAdd: exp"<<endl;
        getTopmost()->print();
    }
    isChanged = false;
    
    //ln
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
        fillDepthOrder();
        
        sort(ln.begin(), ln.end(), [](Addition* a, Addition* b)->bool{
        return compare(a->depth,a->orderType,a->order,b->depth,b->orderType,b->order);
        });
        
        cout<<"cleanAdd: ln"<<endl;
        getTopmost()->print();
    }
    isChanged = false;
    
    if(add.size() != 0)
    {
        cout<<"Error: cannot clean add"<<endl;
    }
}

void Addition::cleanAdd()
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
        fillDepthOrder();
        
        sort(exp.begin(), exp.end(), [](Addition* a, Addition* b)->bool{
            return compare(a->depth,a->orderType,a->order,b->depth,b->orderType,b->order);
        });
        
        cout<<"cleanAdd: exp"<<endl;
        getTopmost()->print();
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
        for(unsigned int j = 0; j < add[i]->ln_c.size() ; j++)
        {
            ln_c.push_back(add[i]->ln_c[j]);
            add[i]->ln_c[j]->mother = this;
            isChanged = true;
        }
        add[i]->ln_c.clear();
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
        fillDepthOrder();
        
        sort(ln.begin(), ln.end(), [](Addition* a, Addition* b)->bool{
        return compare(a->depth,a->orderType,a->order,b->depth,b->orderType,b->order);
        });
        
        cout<<"cleanAdd: ln"<<endl;
        getTopmost()->print();
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
        fillDepthOrder();
        
        sort(add.begin(), add.end(), [](Addition* a, Addition* b)->bool{
        return compare(a->depth,a->orderType,a->order,b->depth,b->orderType,b->order);
        });
        
        cout<<"cleanAdd: add"<<endl;
        getTopmost()->print();
    }
    isChanged = false;
}

void Addition::explnCancellation()
{
    //\exp(\ln(x_1)) + \exp(\ln(x_2)) + y = x_1 + x_2 + y
    bool isChanged = false;
    int index = 0;
    while(true)
    {
        if(index >= exp.size()) break;
        
        if(exp[index]->nZero) {index++; continue;}
        if(exp[index]->nNegative) {index++; continue;}
        if(exp[index]->positveInterger != 0) {index++; continue;}
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
           exp[index]->ln_c.size()  +
           exp[index]->ln_i.size()  +
           exp[index]->ln.size() != 1) {index++; continue;}
        
        //move x to add
        vector<Addition*>* ln_x = nullptr;
        if     (exp[index]->ln_n1.size() == 1) ln_x = &(exp[index]->ln_n1);
        else if(exp[index]->ln_c.size() == 1)  ln_x = &(exp[index]->ln_c);
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
        getTopmost()->print();
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
        if(ln[index]->positveInterger != 0) {index++; continue;}
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
        if(ln[index]->ln_c.size() != 0) {index++; continue;}
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
        getTopmost()->print();
    }
    isChanged = false;
    
    //no need to do explnCancellation for this->add
    //because the cleanAdd is done for this->add[i]
    //this->add[i]->exp.size must be 0, this->add[i]->ln.size must be 0
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
        positveInterger = 0;
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
        
        for(unsigned int i = 0; i < ln.size() ; i++)
        {
            delete ln[i];
        }
        
        for(unsigned int i = 0; i < add.size() ; i++)
        {
            delete add[i];
        }
        
        exp.clear();
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
        for(unsigned int j = 0; j < ln_c.size() ; j++)
        {
            if(ln_c[j]->positveInterger == 1)
            {
                count_one++;
            }
            else
            {
                count_c++;
            }
        }
        
        if(count_one >= 1 && count_c >= 2) isChanged = true;
    }
    
    //For ln_c
    if(isChanged)
    {
        int index = 0;
        while(true)
        {
            if(index >= ln_c.size()) break;
            
            product *= ln_c[index]->positveInterger;
            
            delete ln_c[index];
            ln_c.erase(ln_c.begin()+index);
        }
        
        if(product >= 2)
        {
            Addition* c = new Addition(1,product);
            
            ln_c.push_back(c);
            c->mother = this;
            c->motherType = 2;
        }
    }
    
    if(isChanged)
    {
        if(isEmpty()) nZero = true;
        cout<<"log formula"<<endl;
        getTopmost()->print();
    }
}

void Addition::addCommonTerm()
{
    cleanAddOld();
    
    for(unsigned int i = 0; i < exp.size() ; i++)
    {
        exp[i]->addCommonTerm();
    }
    
    for(unsigned int i = 0; i < ln.size() ; i++)
    {
        ln[i]->addCommonTerm();
    }
    
    fillDepthOrder();
    
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
    //\exp(                     x) + \exp(                   + x) = \exp(          \ln(2)       + x)
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
    
    //For Type 1
    
    //For Type 2
    
    //For Type 3
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
    
    //implicit done in the following functions
    //x + (0) = x
    
    //----------------------
    //For example:
    //\exp(\ln( (x) + \exp(x) )) = ((x) + \exp(x))             (explnCancellation)
    //                           = (x) + \exp(x)               (cleanAdd)
    
    //exp(ln(x)) + y = (x) + y
    //EXP[ ln(exp(x)) + y ] = EXP[ (x) + y ]
    explnCancellation();
    
    //----------------------
    //(exp(x_1)+exp(x_2)+...) + (exp(y_1)+exp(y_2)+...) + ... = exp(x_1)+exp(x_2)+... + exp(y_1)+exp(y_2)+... + ...
    //( ln(x_1)+ ln(x_2)+...) + ( ln(y_1)+ ln(y_2)+...) + ... =  ln(x_1)+ ln(x_2)+... +  ln(y_1)+ ln(y_2)+... + ...
    //(0) + (2) + ((-1)) + (tau) + (i) + ((-inf)) + (x) + (y) + (z) = 0 + 2 + (-1) + tau + i + (-inf) + x + y + z
    cleanAdd();
    //better search for EXP[]
    //----------------------
    
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
    //For example:
    //  EXP[ ln(-1) + ln(-1) + x ]
    //= EXP[ ln(1) + x ]            (log formula)
    //= EXP[ x ]                    (ln(1) = 0)
    
    //----------------------
    //log formula:
    //EXP[ ln(i) + ln(i) + x ] = //EXP[ ln(-1) + x ]
    //EXP[ ln(-1) + ln(-1) + x ] = //EXP[ ln(1) + x ]
    //EXP[ ln(1) + x ] = EXP[ x ]
    //EXP[ ln(c_1) + ln(c_2) + x ] = //EXP[ ln(c_1*c_2) + x ]
    ln_1nic();
    
    //----------------------
    //For example:
    //  EXP[ exp(ln(2) + ln(tau) + ln(i)) + exp( ln(-1) + ln(tau) + ln(i)) + x ]
    //= EXP[ exp(ln(tau) + ln(i)) + x ]                                                    (addCommonTerm)
    //= EXP[ ln(i) + x ]                                                                   (Euler formula)
    
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
    //\exp(                     x) + \exp(                   + x) = \exp(          \ln(2)       + x)
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
    
    //----------------------
    //Euler formula:
    //EXP[ exp(                 ln(tau) + ln(i)) + x ] =X= EXP[                + x ]
    //EXP[ exp(ln(-1)         + ln(tau) + ln(i)) + x ] =X= EXP[          ln(i) + x ]
    //EXP[ exp(         ln(c) + ln(tau) + ln(i)) + x ] =X= EXP[ ln(-1)         + x ]
    //EXP[ exp(ln(-1) + ln(c) + ln(tau) + ln(i)) + x ] =X= EXP[ ln(-1) + ln(i) + x ]
    euler();
}
