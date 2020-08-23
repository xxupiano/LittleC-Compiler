#include <bits/stdc++.h>
using namespace std;
//设定输入算术表达式只有一行

string s;
int cnt = 0;
vector<char> in;    //中缀表达式
vector<char> post;  //后缀表达式
stack<char> help;
map<int, char> ans;


void F(char token);
void E(char token);

char getNext() {
    while(s[cnt]==' ' && cnt<s.length()) cnt++; //去掉空格
    if(cnt==s.length()) return '$';       //字符串结束
    return s[cnt++];
}

void A(char token) {
//    cout << token << endl;
    if(token=='+' | token=='-') return;
    else {
        cout << "Error!" << endl;
        exit(0);
    }
}

void M(char token) {
   if(token=='*' | token=='/') return;
    else {
        cout << "Error!" << endl;
        exit(0);
    }
}

void F(char token) {
//    cout << token << endl;
    if(isalnum(token)) return;  // id或number
    else if(token=='(') {
        char c = getNext();
        if(c=='$') {
            cout << "Error!" << endl;
            exit(0);
        }
        E(c);
        c = getNext();
        if(c!=')') {
            cout << "Error!" << endl;
            exit(0);
        }
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
        char c = getNext();
        if(c=='$') {                    //M的FOLLOW没有$
            cout << "Error!" << endl;
            exit(0);
        }
        F(c);
        c = getNext();
        if(c=='$') return;              //F的FOLLOW有$
        T1(c);
    }
}

void T(char token) {
//    cout << token << endl;
    F(token);
    char c = getNext();
    if(c=='$') return;
    T1(c);
}

void E1(char token) {
//    cout << token << endl;
    if(token==')' || token=='$') {  //FOLLOW集中有)和$
        cnt--;
        return;
    }
    else {
        A(token);
        char c = getNext();
        if(c=='$') {
            cout << "Error!" << endl;
            exit(0);
        }
        T(c);
        c = getNext();
        if(c=='$') return;
        E1(c);
    }
}

void E(char token) {
//    cout << token << endl;
    T(token);
    char c = getNext();
    if(c=='$') return;      //T的FOLLOW还有$
    E1(c);
}

int cmp(char a, char b) {
    // 2:a>b, 1:a<b, 0:a=b
    if(b=='(') return 2; //(优先级最低
    if((a=='*'|| a=='/') && (b=='+'|| b=='-')) return 2; //>
    if((a=='+'|| a=='-') && (b=='*'|| b=='/')) return 1; //<
    if((a=='+'|| a=='-') && (b=='+'|| b=='-')) return 0; //=
    if((a=='*'|| a=='/') && (b=='*'|| b=='/')) return 0; //=
}

///中缀表达式转后缀表达式
void in2post() {
    for(int i=0; i<s.length(); i++) {
//        cout << s[i] << endl;
        if(s[i]==' ') continue; //去空格
        if(isalnum(s[i])) { //id或number直接输出
            in.push_back(s[i]);
            post.push_back(s[i]);
        }
        else if(s[i]=='(') { //（入栈，优先级变最低
            help.push(s[i]);
        }
        else if(s[i]==')') {
            while(help.top()!='(') { //(前输出
//                cout << help.top()<<endl;
                post.push_back(help.top());
                help.pop();
            }
            help.pop(); //'('出栈
        }
        else if(help.empty()) { //遇到运算符但是栈空，直接入栈
            in.push_back(s[i]);
            help.push(s[i]);
        }
        else if (cmp(s[i], help.top())==2) { //优先级大于栈顶
            in.push_back(s[i]);
//            cout << help.top()<<endl;
            help.push(s[i]);
        }
        else { //优先级小于等于栈顶
            in.push_back(s[i]);
            while(!help.empty() && (cmp(help.top(), s[i])==2 || cmp(help.top(), s[i])==0)) {
//                cout << help.top()<<endl;
                post.push_back(help.top());
                help.pop();
            }
            help.push(s[i]);
        }
    }
    while(!help.empty()) {
//        cout << help.top()<<endl;
        post.push_back(help.top());
        help.pop();
    }
}

///中后转层
void level(int root, int start, int end, int index) {
    if(start>end) return;
    int i = start;
    while(i<end && in[i]!=post[root]) i++; //找到中序中的root
    ans[index] = post[root];
    level(root-(end-i+1), start, i-1, 2*index+1);
    level(root-1, i+1, end, 2*index+2);
}

int main()
{
    getline(cin, s);

    ///判断输出串是否符合文法
    E(getNext());
    cout <<"Right."<<endl;

    ///构建语法树

    ///1 中缀转后缀
    in2post();
    ///2 中序后序转层序
    int n = post.size();
    level(n-1, 0, n-1, 0);

    cout << "Inorder: ";
    for(auto it=in.begin(); it!=in.end(); it++) cout << *it;
    cout << endl;
    cout << "Postorder: ";
    for(auto it=post.begin(); it!=post.end(); it++) cout << *it;
    cout << endl;

    ///3 输出层序
    cout << "Levelorder: ";
    for(auto it=ans.begin(); it!=ans.end(); it++) {
        cout << it->second;
    }

    return 0;
}
