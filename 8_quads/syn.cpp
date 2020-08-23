#include "def.h"

/**
    所有语法分析函数实现
**/

int cnt2 = 0; //取ans中的token
int ti = 1; //ti中i计数
stack<string> num; //操作数栈
stack<string> ope; //运算符栈
vector<quad> results; //存放所有四元式
int rescnt = 1;     //对产生式计数，便于确定goto位置

///运算符优先级比较
int cmpop(string a, string b) {
    // 2:a>b, 1:a<b, 0:a=b
    if(b=="(") return 2; //(优先级最低
    if((a=="*"|| a=="/") && (b=="+"|| b=="-")) return 2; //>
    if((a=="+"|| a=="-") && (b=="*"|| b=="/")) return 1; //<
    if((a=="+"|| a=="-") && (b=="+"|| b=="-")) return 0; //=
    if((a=="*"|| a=="/") && (b=="*"|| b=="/")) return 0; //=
}

///生成算术表达式四元式
string geneari(string op, string arg1, string arg2) {
    string tmp = "t" + to_string(ti++);
    results.push_back(quad{op, arg1, arg2, tmp});
    rescnt++;
    return tmp; //返回算术表达式的result
}

//获取下一个token
token getNext() {
    if(cnt2<ans.size()) {
        return ans[cnt2++];
    }
    token t;
    t.val = "$";
    t.type = "$";
    t.num = 0;
    cnt2++;
    return t;
}

void ROP(token t) {///<ROP> -> > | >= | < | <= | == | !=
//    cout << t.val << endl;
    if(t.val==">" || t.val==">=" || t.val=="<" || t.val=="<=" ||
       t.val=="==" || t.val=="!=") return;
    cout << "Error!" << endl;
    exit(0);
}

void BOOL(token t) {
//    cout << t.val << endl;
    EXPR(t);
    while(!ope.empty()) {
        string a1 = num.top(); num.pop();
        string a2 = num.top(); num.pop();
        string o = ope.top(); ope.pop();
        string tmp = geneari(o, a2, a1);
        num.push(tmp);
    }
    string tmp1 = num.top();        //取EXPR结果
    num.pop();
    token c = getNext();
    ROP(c);
    EXPR(getNext());
    while(!ope.empty()) {
        string a1 = num.top(); num.pop();
        string a2 = num.top(); num.pop();
        string o = ope.top(); ope.pop();
        string tmp = geneari(o, a2, a1);
        num.push(tmp);
    }
    string tmp2 = num.top();
    num.pop();
    results.push_back(quad{"j"+c.val, tmp1, tmp2, to_string(rescnt+2)});
    rescnt++;
}

void FACTOR(token t) {
//    cout << t.val << endl;
    if(t.type=="id" || t.type=="number") {
        num.push(t.val);
        return;
    }
    else if(t.val=="(") {
        ope.push("(");
        EXPR(getNext());
        t = getNext();
//        cout << t.val << endl;
        if(t.val!=")") {
            cout << "Error!" << endl;
            exit(0);
        }
        //token为右括号
        while(ope.top()!="(") {
            string a1 = num.top(); num.pop();
            string a2 = num.top(); num.pop();
            string o = ope.top(); ope.pop();
            string tmp = geneari(o, a2, a1);
            num.push(tmp);
        }
        ope.pop(); //(出栈
    }
    else {
        cout << "Error!" << endl;
        exit(0);
    }
}

void MUL(token t) {
//    cout << t.val << endl;
    if(t.val=="*" || t.val=="/") {
        if(ope.empty()) { ///运算符栈空，直接入栈
            ope.push(t.val);
        }
        else { ///运算符栈不空
            if(cmpop(t.val, ope.top())==2) {///优先级>栈顶
                ope.push(t.val);
            }
            else {///优先级<=栈顶
                while(!ope.empty()&&(cmpop(t.val, ope.top())!=2)) {
                    string a1 = num.top(); num.pop();
                    string a2 = num.top(); num.pop();
                    string o = ope.top(); ope.pop();
                    string tmp = geneari(o, a2, a1);
                    num.push(tmp);
                }
                ope.push(t.val);
            }
        }
        return;
    }
    else {
        cout << "Error!" << endl;
        exit(0);
    }
}

void TERM1(token t) {
//    cout << t.val << endl;
    if(t.val=="+" || t.val=="-" || t.val==";" || t.val==")" ||
       t.val==">" || t.val==">=" || t.val=="<" || t.val=="<=" ||
       t.val=="==" || t.val=="!=") {
        cnt2--;
        return;
    }
    MUL(t);
    FACTOR(getNext());
    TERM1(getNext());
}
void TERM(token t) {
//    cout << t.val << endl;
    FACTOR(t);
    TERM1(getNext());
}

void ADD(token t) {
//    cout << t.val << endl;
    if(t.val=="+" || t.val=="-") {
        if(ope.empty()) {
            ope.push(t.val);
        }
        else {
            if(cmpop(t.val, ope.top())==2)
                ope.push(t.val);
            else {
                while(!ope.empty()&&(cmpop(t.val, ope.top())!=2)) {
                    string a1 = num.top(); num.pop();
                    string a2 = num.top(); num.pop();
                    string o = ope.top(); ope.pop();
                    string tmp = geneari(o, a2, a1);
                    num.push(tmp);
                }
                ope.push(t.val);
            }
        }
        return;
    }
    else {
        cout << "Error!" << endl;
        exit(0);
    }
}

void EXPR1(token t) {
//    cout << t.val;
    //EXPR1的FOLLOW
    if(t.val==";" || t.val==")" || t.val==">" || t.val==">=" ||
       t.val=="<" || t.val=="<=" || t.val=="==" || t.val=="!=") {
        cnt2--;
        return;
    }
    ADD(t);
    TERM(getNext());
    EXPR1(getNext());
}

void EXPR(token t) {
//    cout << t.val << endl;
    TERM(t);
    EXPR1(getNext());
}

void STMT(token t) {
//    cout << t.val << endl;
    token c;
    if(t.type=="id") {  ///id = <EXPR>;
        c = getNext();
//        cout << t.val << endl;
        if(c.val!="=") {
            cout << "Error!" << endl;
            exit(0);
        }
        EXPR(getNext());
        c = getNext();
//        cout << t.val << endl;
        if(c.val!=";") {
            cout << "Error!" << endl;
            exit(0);
        }
        ///对EXPR最后的处理
        while(!ope.empty()) {
            string a1 = num.top(); num.pop();
            string a2 = num.top(); num.pop();
            string o = ope.top(); ope.pop();
            string tmp = geneari(o, a2, a1);
            num.push(tmp);
        }
        ///语义动作，产生四元式(=, ti, _, id)
        results.push_back(quad{"=", num.top(), "_", t.val});
        num.pop();
        rescnt++;
    }
    else if(t.val=="if") {  ///<STMT>->if(<BOOL>)<STMT1>
        t = getNext();
        if(t.val!="(") {
            cout << "Error!" << endl;
            exit(0);
        }
        BOOL(getNext());
        t = getNext();
        if(t.val!=")") {
            cout << "Error!" << endl;
            exit(0);
        }
        results.push_back(quad{"jmp","_", "_","BOOL.FALSE"});
        rescnt++;
        int BOOL_FALSE = rescnt-2;

        STMT(getNext());

        results.push_back(quad{"jmp","_", "_","STMT.NEXT"});
        rescnt++;
        int STMT_NEXT = rescnt-2;


        t = getNext();
        if(t.val!="else") {
            results.erase(results.begin()+STMT_NEXT);
            rescnt--;
            results[BOOL_FALSE].res = to_string(rescnt);
            cnt2--;
            return;
        }
        else {               ///<STMT> -> if(<BOOL>) <STMT1> else <STMT2>
            results[BOOL_FALSE].res = to_string(rescnt);     //回填STMT.FALSE
            STMT(getNext());
        }
        results[STMT_NEXT].res = to_string(rescnt);
    }
    else if(t.val=="while") {///<STMT> -> while(<BOOL>) <STMT>
        int WHILE_BEGIN = rescnt;
        t = getNext();
//        cout << t.val << endl;
        if(t.val!="(") {
            cout << "Error!" << endl;
            exit(0);
        }
        BOOL(getNext());
        t = getNext();
//        cout << t.val << endl;
        if(t.val!=")") {
            cout << "Error!" << endl;
            exit(0);
        }
        results.push_back(quad{"jmp","_", "_","BOOL.FALSE"});
        rescnt++;
        int BOOL_FALSE = rescnt-2;

        STMT(getNext());
        results.push_back(quad{"jmp","_","_",to_string(WHILE_BEGIN)});
        rescnt++;

        results[BOOL_FALSE].res = to_string(rescnt);
    }
    else {BLOCK(t);}
}

void STMTS1(token t) {
//    cout << t.val << endl;
    if(t.val=="}") { //STMTS1的FOLLOW，规约为STMTS1->empty
        cnt2--;
        return;
    }
    STMT(t);
    STMTS1(getNext());
}

void STMTS(token t) {
//    cout << t.val << endl;
    STMTS1(t);
}

void NAME(token t) {
//    cout << t.val << endl;
    if(t.type!="id") {
        cout << "Error!" << endl;
        exit(0);
    }
    results.push_back(quad{"int", "_", "_", t.val});
    rescnt++;
}

void TYPE(token t) {
//    cout << t.val << endl;
    if(t.val!="int") {
        cout << "Error!" << endl;
        exit(0);
    }
}

void NAMES1(token t) {
//    cout << t.val << endl;
    if(t.val==";" || t.val==".") {
        cnt2--;
        return;
    }
    if(t.val!=",") {
        cout << "Error!" << endl;
        exit(0);
    }
    t = getNext();
    NAME(t);
    NAMES1(getNext());
}

void NAMES(token t) {
//    cout << t.val << endl;
    NAME(t);
    NAMES1(getNext());
}

void DECL(token t) {
//    cout << t.val << endl;
    TYPE(t);
    NAMES(getNext());
    t = getNext();
//    cout << t.val << endl;
    if(t.val!=";") {
        cout << "Error!" << endl;
        exit(0);
    }
}

void DECLS1(token t) {
    if(t.type=="id" || t.val=="if" || t.val=="while" ||
       t.val=="{" || t.val=="}") { //DECLS1的FOLLOW，规约为DECLS1->empty
        cnt2--;
        return;
    }
    DECL(t);
    DECLS1(getNext());
}

void DECLS(token t) {
//    cout << t.val << endl;
    DECLS1(t);
}

void BLOCK(token t) {
//    cout << t.val;
    if(t.val!="{") {
        cout << "Error!" << endl;
        exit(0);
    }
    DECLS(getNext());
    STMTS(getNext());
    t = getNext();
//    cout << t.val << endl;
    if(t.val!="}") {
        cout << "Error!" << endl;
        exit(0);
    }
}
