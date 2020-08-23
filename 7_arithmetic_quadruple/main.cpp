#include <bits/stdc++.h>
using namespace std;
//设定输入算术表达式只有一行

string s;
struct quad {
    char op;
    string arg1, arg2, res;
};
int cnt = 0;//取字符串中的字符
int ti = 1; //ti中的i计数

stack<string> num; //操作数栈
stack<char> ope; //运算符栈
vector<quad> ans;


void F(char token);
void E(char token);

///运算符优先级比较
int cmp(char a, char b) {
    // 2:a>b, 1:a<b, 0:a=b
    if(b=='(') return 2; //(优先级最低
    if((a=='*'|| a=='/') && (b=='+'|| b=='-')) return 2; //>
    if((a=='+'|| a=='-') && (b=='*'|| b=='/')) return 1; //<
    if((a=='+'|| a=='-') && (b=='+'|| b=='-')) return 0; //=
    if((a=='*'|| a=='/') && (b=='*'|| b=='/')) return 0; //=
}

///生成四元式
string generquad(char op, string arg1, string arg2) {
    string tmp = "t" + to_string(ti++);
    ans.push_back(quad{op, arg1, arg2, tmp});
    return tmp; //返回四元式的result
}

///获取表达式s下一个字符
char getNext() {
    while(s[cnt]==' ' && cnt<s.length()) cnt++; //去掉空格
    if(cnt==s.length()) {cnt++;return '$';}             //字符串结束
    return s[cnt++];
}

void A(char token) {
//    cout << token << endl;
    if(token=='+' | token=='-') {
        if(ope.empty()) { ///运算符栈空，直接入栈
            ope.push(token);
        }
        else { ///运算符栈不空
            if(cmp(token, ope.top())==2) {///优先级>栈顶
                ope.push(token);
            }
            else {///优先级<=栈顶
                while(!ope.empty()&&(cmp(token, ope.top())!=2)) {
                    string a1 = num.top(); num.pop();
                    string a2 = num.top(); num.pop();
                    char o = ope.top(); ope.pop();
                    string tmp = generquad(o, a2, a1);
                    num.push(tmp);
                }
                ope.push(token);
            }
        }
        return;
    }
    else {
        cout << "Error!" << endl;
        exit(0);
    }
}

void M(char token) {
   if(token=='*' | token=='/') {
        if(ope.empty()) { ///运算符栈空，直接入栈
            ope.push(token);
        }
        else { ///运算符栈不空
            if(cmp(token, ope.top())==2) {///优先级>栈顶
                ope.push(token);
            }
            else {///优先级<=栈顶
                while(!ope.empty()&&(cmp(token, ope.top())!=2)) {
                    string a1 = num.top(); num.pop();
                    string a2 = num.top(); num.pop();
                    char o = ope.top(); ope.pop();
                    string tmp = generquad(o, a2, a1);
                    num.push(tmp);
                }
                ope.push(token);
            }
        }
        return;
   }
    else {
        cout << "Error!" << endl;
        exit(0);
    }
}

void F(char token) {
//    cout << token << endl;
    if(isalnum(token)) {
        ///id和number入栈
        string tmp;
        tmp.push_back(token);
        num.push(tmp);
        return;  // id或number
    }
    else if(token=='(') {
        ope.push('(');
        E(getNext());
        char c = getNext();
        if(c!=')') {
            cout << "Error!" << endl;
            exit(0);
        }
        ///c为右括号
        while(ope.top()!='(') {
            string a1 = num.top(); num.pop();
            string a2 = num.top(); num.pop();
            char o = ope.top(); ope.pop();
            string tmp = generquad(o, a2, a1);
            num.push(tmp);
        }
        ope.pop(); //(出栈

    }
    else {
        cout << "Error!" << endl;
        exit(0);
    }
}


void T1(char token) {
//    cout << token << endl;
    if(token=='+' || token=='-' || token==')' || token=='$') {
        cnt--;
        return;  //FOLLOW中有+-)$
    }
    else {
        M(token);
        F(getNext());
        T1(getNext());
    }
}

void T(char token) {
//    cout << token << endl;
    F(token);
    T1(getNext());
}

void E1(char token) {
//    cout << token << endl;
    if(token==')' || token=='$') {  //FOLLOW集中有)和$
        cnt--;
        return;
    }
    else {
        A(token);
        T(getNext());
        E1(getNext());
    }
}

void E(char token) {
//    cout << token << endl;
    T(token);
    E1(getNext());
}


int main()
{
    getline(cin, s);

    ///判断输出串是否符合文法
    E(getNext());
    if(num.size()==1) {
        string tmp = num.top();
        ans.push_back(quad{'_', tmp, "_", "_"});
    }
    while(!ope.empty()) {
        string a1 = num.top(); num.pop();
        string a2 = num.top(); num.pop();
        char o = ope.top(); ope.pop();
        string tmp = generquad(o, a2, a1);
        num.push(tmp);
    }
    cout <<"语法分析正确，四元式如下："<<endl;
    for(int i=0; i<ans.size(); i++) {
        cout << setw(5) << ans[i].op
             << setw(5) << ans[i].arg1
             << setw(5) << ans[i].arg2
             << setw(5) << ans[i].res << endl;
    }
    return 0;
}
