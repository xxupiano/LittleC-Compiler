#ifndef DEF_H_INCLUDED
#define DEF_H_INCLUDED

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <regex>
#include <fstream>
#include <stack>
#include <iomanip>
using namespace std;

#define NUMBER 0
#define ID     22

struct token {
    string val;     //单词值
    string type;    //单词类型, keyword, separator, operator, id, number
    int num;        //单词种别码
};

struct quad {
    string op, arg1, arg2, res;
};

extern vector<token> ans;
extern int cnt1; //记录id数量
extern int cnt2; //控制从ans取token
extern vector<quad> results; //存放所有四元式
extern int rescnt;


///词法分析
extern unordered_map<string, int> keywords;
extern unordered_map<char, int> separators;
extern unordered_map<char, int> operators1;
extern unordered_map<string, int> operators2;

bool alldigit(string s);
bool isid(string s);
bool istoken(string s);
bool isso(string s, int& i);


///语法分析
token getNext();
int cmpop(string a, string b);
void ROP(token t);
void BOOL(token t);
void FACTOR(token t);
void MUL(token t);
void TERM(token t);
void ADD(token t);
void TERM1(token t);
void EXPR1(token t);
void EXPR(token t);
void STMT(token t);
void STMTS1(token t);
void STMTS(token t);
void NAME(token t);
void TYPE(token t);
void NAMES1(token t);
void NAMES(token t);
void DECL(token t);
void DECLS1(token t);
void DECLS1(token t);
void BLOCK(token t);

///四元式相关
string geneari(string op, string arg1, string arg2);

#endif // DEF_H_INCLUDED
