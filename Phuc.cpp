#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>
#include <fstream>
using namespace std;
string convert(string s)
{
    int foundsharp = s.find('#');
    if (foundsharp != string::npos) s.erase(s.begin()+foundsharp,s.end());

    while (s[0]==' ') s.erase(s.begin());
    int i=0;
    while (i<s.size())
    {
        if (s[i]==' '&&s[i+1]==' ') s.erase(s.begin()+i), i--;
        i++;
    }
    if (s[s.size()-1]==' ') s.erase(s.begin()+s.size()-1);
    for (int i=0; i<s.size(); ++i)
        if (s[i]==' ') s[i]=',';
    i=0;
    while (i<s.size())
    {
        if (s[i]==','&&s[i+1]==',') s.erase(s.begin()+i), i--;
        i++;
    }
    int foundtab = s.find('\t');
    if (foundtab != string::npos) s.erase(s.begin()+foundtab);
    return s;
}

int main()
{
    freopen("input.txt","r",stdin);
    string s; vector <string> Result;
    while (getline(cin,s))
        if (convert(s).size()!=0) Result.push_back(convert(s));
    for (string show:Result) cout << show << endl;
    return 0;
}
