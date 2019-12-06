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
    else if(trim[0] == '\\')
    {
        //For \exp and \ln
        if(trim.size() <= 3)
        {
            cout<<"Syntax Error: the function need to be ln, exp or tau"<<endl;
            return;
        }
        else if(trim[1] == 'l' && trim[2] == 'n' && trim[3] == '(')
        {
            //For \ln
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
                    
                    ln.push_back(operand);
                    return;
                }
                else index++;
            }
        }
        else if(trim.size() == 4 && trim[1] == 't' && trim[2] == 'a' && trim[3] == 'u')
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
        else if(trim.size() <= 4)
        {
            cout<<"Syntax Error: the function need to be ln, exp or tau"<<endl;
            return;
        }
        else if(trim[1] == 'e' && trim[2] == 'x' && trim[3] == 'p' && trim[4] == '(')
        {
            //For \exp
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
        else
        {
            cout<<"Syntax Error: the function need to be ln, exp or tau"<<endl;
            return;
        }
    }
    else
    {
        //For positive integer
        if(trim[0] != '0' && std::isdigit(trim[0]))
        {
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

        // For addition and other thing
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
            
            depth = 0;
            orderType = 0;
            for(unsigned int i = 0; i < operand_latex_list.size() ; i++)
            {
                Addition* operand = new Addition(operand_latex_list[i]);
                
                operand->mother = this;
                operand->motherType = 3;
                
                mother = nullptr;
                motherType = 0;
                
                if(operand->depth +1 > depth) depth = operand->depth +1;
                
                if(orderType == 0)
                {
                    //initialization
                    orderType = operand->orderType;
                    
                    if(operand->orderType == 2) order = operand->order;
                    else order = 0;
                }
                else if(orderType == 1)
                {
                    orderType = operand->orderType;
                    if(operand->orderType == 2) order = operand->order;
                }
                else if(orderType == 2)
                {
                    if(operand->orderType == 2)
                    {
                        if(order != operand->order)
                        {
                            orderType = 3;
                            order = 0;
                        }
                    }
                    else if(operand->orderType == 3)
                    {
                        orderType = 3;
                        order = 0;
                    }
                }
                
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
        }
        else
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
    else if(compositeType == 2) ln.push_back(operand);
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
    while(true)
    {
        bool isErase = eraseParenthesis(output);
        if(!isErase) break;
    }
    
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
