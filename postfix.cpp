#include <iostream>
#include <string>
#include <cctype>
#include <stack>
#include <map>

using namespace std;

string popStack(stack<char> *s, char c, string str, map<char, int> m)
{  
    char aux = s->top();

    while(m[aux] >= m[c])
    {
        //cout 
        str += aux;
        cout << str << endl;
        s->pop();
        cout << "sale " << aux << endl;
        if(s->size())
            aux = s->top();
        else    
            break;
    }
    s->push(c);
    cout << "entra " << s -> top() << endl;
    
    return str;
}

string postfix_regular_expression(string input)
{
    map<char, int> procedence;
    procedence['*'] = 3;
    procedence['?'] = 2;
    procedence['+'] = 1;

    //variables
    string pfString;
    int i, inputSize, pCounter = 0;
    char aux;
    //data structures
    stack<char> pfStack;
    
    inputSize = input.size();

    for(i = 0 ; i < inputSize ; i++)
    {
        if(isalpha(input[i]) || isdigit(input[i]) || input[i] == '-' || input[i] == '_')
        {
            pfString += input[i];
            cout << pfString << endl;
        }
        else
        {
            if(input[i] == '(')
            {
                pfStack.push('(');
                pCounter++;
            }
            else if(input[i] == ')')
            {
                if(pfStack.empty())
                {
                    cout << "Error: unbalanced parenthesis.";
                    pfString = "\0";
                    return pfString;
                }
                else
                {
                    pCounter--;
                    do
                    {
                        aux = pfStack.top();
                        pfStack.pop();
                        cout << "sale " << aux << endl;
                        if(aux != '(')
                        {
                            pfString += aux;
                        }
                        cout << pfString << endl; 
                    } while(!pfStack.empty() && aux != '(');
                      
                }
            }
            else
            {   
                if(pfStack.empty())
                {
                    pfStack.push(input[i]);
                    cout << "entra " << input[i] << endl;
                    //cout << pfStack.size() << endl;
                }
                else
                {
                    pfString = popStack(&pfStack, input[i], pfString, procedence);
                    //cout << pfStack.size() << endl;
                }
            }
        }   
        
    }

    if(pCounter)
    {
        cout << "Error: unbalanced parenthesis.";
        pfString = "\0";
        return pfString;
    }

    while(!pfStack.empty())
    {
        pfString += pfStack.top();
        pfStack.pop();
    }

    return pfString;
}

int main()
{
    string input, output;

    cout << "Ingresa una cadena:";
    cin >> input;

    output = postfix_regular_expression(input);

    cout << output;

}