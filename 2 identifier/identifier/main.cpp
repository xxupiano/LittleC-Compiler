#include <iostream>
#include <cstdio>
#include <string>
#include <regex>
#include <unordered_set>

using namespace std;

//32 keywords of C
unordered_set<string> keywords {"char", "double", "enum", "float", "int", "long", "short", "signed",
                                "struct", "union", "unsigned", "void", "for", "do", "while", "break",
                                "continue", "if", "else", "goto", "switch", "case", "default", "return",
                                "auto", "extern", "register", "static", "const", "sizeof", "typedef", "volatile"};
vector<string> ans;
vector<string> res;
smatch m;

int main()
{
    string s;

    ///preprocessing->ans
    while(getline(cin, s)) {
        if(s=="") continue; //eliminate blank lines
        if(s[0]=='#') continue; // eliminate #include
        for(int i=0; i<s.length(); i++) {  //preprocessing
            if(s[i]=='\'') { //eliminate constant characters, such as "char c = 'a'";
                s = s.substr(0, i) + s.substr(i+3);

            }
            if(s[i]=='[') {  //eliminate [macro definition]
                for(int j=i+1; j<s.length(); j++) {
                    if(s[j]==']') {
                        s = s.substr(0, i) + s.substr(j+1);
                        break;
                    }
                }
            }
            if(s[i]=='"') { //eliminate "string"
                for(int j=i+1; j<s.length(); j++) {
                    if(s[j]=='"') {
                        s = s.substr(0, i) + s.substr(j+1);
                        break;
                    }
                }
            }
        }
        ans.push_back(s);
    }

    ///regex
    regex e("[a-zA-Z_][a-z0-9A-Z_]{0,18}", regex_constants::icase);
    for(int i=0; i<ans.size(); i++) {
        while(regex_search(ans[i], m, e)) {
            for(auto x:m) res.push_back(x);
            ans[i] = m.suffix().str();
        }
    }

    //output the identifiers that are not the keywords
    for(int i=0; i<res.size(); i++) {
        if(keywords.find(res[i])!=keywords.end()) continue;
        cout << res[i] << endl;
    }
    return 0;
}

