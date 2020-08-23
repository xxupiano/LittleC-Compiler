#include "def.h"

/**
    所有语法分析函数实现
**/
int cnt2 = 0; //取ans中的token

//获取下一个token
token getNext() {
    if(cnt2<ans.size()) {
        return ans[cnt2++];
    }
    token t;
    t.val = "$";
    t.type = "$";
    t.num = 0;
    return t;
}

void ROP(token t) {
//    cout << t.val << endl;
    if(t.val==">" || t.val==">=" || t.val=="<" || t.val=="<=" ||
       t.val=="==" || t.val=="!=") return;
    cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
    exit(0);
}

void BOOL(token t) {
//    cout << t.val << endl;
    EXPR(t);
    ROP(getNext());
    EXPR(getNext());
}

void FACTOR(token t) {
//    cout << t.val << endl;
    if(t.type=="id") return;
    else if(t.type=="number") return;
    else if(t.val=="(") {
        t = getNext();
//        cout << t.val << endl;
        if(t.val=="$") {
            cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
            exit(0);
        }
        EXPR(t);
        t = getNext();
//        cout << t.val << endl;
        if(t.val!=")") {
            cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
            exit(0);
        }
    }
    else {
        cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
        exit(0);
    }
}

void MUL(token t) {
//    cout << t.val << endl;
    if(t.val=="*" || t.val=="/") return;
    cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
    exit(0);
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
    t = getNext();
//    cout << t.val << endl;
    FACTOR(t);
    TERM1(getNext());
}
void TERM(token t) {
//    cout << t.val << endl;
    FACTOR(t);
    TERM1(getNext());
}

void ADD(token t) {
//    cout << t.val << endl;
    if(t.val=="+" || t.val=="-") return;
    cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
    exit(0);
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
    if(t.type=="id") {
        t = getNext();
//        cout << t.val << endl;
        if(t.val!="=") {
            cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
            exit(0);
        }
        EXPR(getNext());
        t = getNext();
//        cout << t.val << endl;
        if(t.val!=";") {
            cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
            exit(0);
        }
    }
    else if(t.val=="if") {
        t = getNext();
//        cout << t.val << endl;
        if(t.val!="(") {
            cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
            exit(0);
        }
        BOOL(getNext());
        t = getNext();
//        cout << t.val << endl;
        if(t.val!=")") {
            cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
            exit(0);
        }
        STMT(getNext());
        t = getNext();
//        cout << t.val << endl;
        if(t.val!="else") {
            cnt2--;
            return;
        }
        else {
            STMT(getNext());
        }
    }
    else if(t.val=="while") {
        t = getNext();
//        cout << t.val << endl;
        if(t.val!="(") {
            cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
            exit(0);
        }
        BOOL(getNext());
        t = getNext();
//        cout << t.val << endl;
        if(t.val!=")") {
            cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
            exit(0);
        }
        STMT(getNext());
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
        cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
        exit(0);
    }
}

void TYPE(token t) {
//    cout << t.val << endl;
    if(t.val!="int") {
        cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
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
        cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
        exit(0);
    }
    NAME(getNext());
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
        cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
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
        cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
        exit(0);
    }
    DECLS(getNext());
    STMTS(getNext());
    t = getNext();
//    cout << t.val << endl;
    if(t.val!="}") {
        cout << "第" << t.line << "行：" + t.val + " 语法错误" << endl;
        exit(0);
    }
}
