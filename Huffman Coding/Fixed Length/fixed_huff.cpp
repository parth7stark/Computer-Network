#include <iostream>
#include <bits/stdc++.h>
using namespace std;
string addBinary(string a, string b) ;

int main()
{
    int n,i;
    cout<<"\n Enter no of characters: ";
    cin>>n;

    char C[n];

    for(i=0;i<n;i++)
    {
        cout<<"\n Enter character "<<i+1<<": ";
        cin>>C[i];
    }

    //no of bits
     double n_bits = log(n)/log(2) ;
     n_bits = ceil(n_bits);

    string str;
    for(i=0;i<n_bits;i++)       //creating string of n_bits zeros 000....00  
        str += '0';

    unordered_map<char,string> m;
    m[C[0]] = str;
    string b = "1";
    for(i=1;i<n;i++)
    {
        m[C[i]] = addBinary(m[C[i-1]], b);
    }
    
    //print lookup table
    for(auto x:m)
    {
        cout<<x.first<<" "<<x.second;
        cout<<endl;
    }    

    //encode message
    string msg,en_msg=" ";
    cout<<"\n Enter message to be encoded: ";
    cin>>msg;

    for(i=0;i<msg.size();i++)       //take a character and replace
    {
        en_msg += m[msg[i]]; 
    }
    cout<<"encoded message: "<<en_msg;

    //decode message
    string msg1, de_msg=" ";
    cout<<"\n Enter message to be decoded: ";
    cin>>msg1;

    int j = n_bits;
    string sub;
    for(i=0;i<msg1.size();)
    {
        sub = msg1.substr(i,j); //make group
        for(auto x:m)
        {
            if(x.second == sub)
                de_msg += x.first;
        }
        i+=j;
        sub.clear();
    }

    cout<<"\n decoded message: "<<de_msg;



    return 0;
}


string addBinary(string a, string b) 
{ 
    string result = ""; // Initialize result 
    int s = 0;          // Initialize digit sum 
  
    // Traverse both strings starting from last 
    // characters 
    int i = a.size() - 1, j = b.size() - 1; 
    while (i >= 0 || j >= 0 || s == 1) 
    { 
        // Comput sum of last digits and carry 
        s += ((i >= 0)? a[i] - '0': 0); 
        s += ((j >= 0)? b[j] - '0': 0); 
  
        // If current digit sum is 1 or 3, add 1 to result 
        result = char(s % 2 + '0') + result; 
  
        // Compute carry 
        s /= 2; 
  
        // Move to next digits 
        i--; j--; 
    } 
    return result; 
} 