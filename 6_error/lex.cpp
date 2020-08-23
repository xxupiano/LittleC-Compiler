#include "def.h"
/**
    所有词法分析函数实现
**/

unordered_map<string, int> keywords = {{"int",1}, {"if",2}, {"else",3},
                                       {"while", 4}};
unordered_map<char, int> separators = {{'{',5}, {'}',6}, {';', 7}, {'(', 8}, {')', 9},
                                       {',', 10}};
unordered_map<char, int> operators1 = {{'=', 11}, {'+', 12}, {'-', 13}, {'*', 14},
                                        {'/', 15}, {'>', 16}, {'<', 17}};
unordered_map<string, int> operators2 = {{">=", 18}, {"<=", 19}, {"==", 20}, {"!=", 21}};

vector<string> ids; //所有标识符
vector<token> ans;  //符号表
int cnt1 = 0;

//判断整个字符串是否为的number
bool alldigit(string s) {
    if(s[0]=='-') s = s.substr(1); //负数去掉负号
    for(int i=0; i<s.length(); i++) {
        if(!isdigit(s[i])) return false;
    }
    if(s.length()>15) return false;
    return true;
}

//判断是否为合法的字符串
bool isid(string s) {
    regex e("[a-zA-Z_][a-z0-9A-Z_]{0,19}", regex_constants::icase);
    return regex_match(s, e);
}

//处理除运算符和分隔符以外的
bool istoken(string s) {
//    cout << s << endl;
    if(keywords[s]!=0) { // keyword
        ans.push_back({s, "keyword", keywords[s], row});
    }
    else if(alldigit(s)) { //是number
        ans.push_back({s, "number", 0, row});
    }
    else if(isid(s)) { //id
        if(find(ids.begin(), ids.end(), s)!=ids.end()) { //new id
            ans.push_back({s, "id", ID+(cnt1++), row});
        }
        else {  //existing id
            int tmp = find(ids.begin(), ids.end(), s) - ids.begin(); //找到id已有种别码
            ans.push_back({s, "id", ID + tmp, row});
        }
    }
    else {return false;}
    return true;
}

//处理分隔符或运算符
bool isso(string s, int& i) {
//    cout << s[i] << s[i+1] << endl;
    if(separators[s[i]]!=0) { //分隔符
        string tmp;
        tmp.push_back(s[i]);
//        cout << tmp << endl;
        ans.push_back({tmp, "separator", separators[s[i]], row});
        i++;
    }
    else if(operators2[s.substr(i,2)]!=0) { //双字符运算符，一定要先处理双字符！！！
        ans.push_back({s.substr(i,2), "operator", operators2[s.substr(i,2)], row});
        i += 2;
    }
    else if(operators1[s[i]]!=0) { //单字符运算符
        string tmp;
        tmp.push_back(s[i]);
//        cout << tmp << endl;
        ans.push_back({tmp, "operator", operators1[s[i]], row});
        i++;
    }
    else {return false;}
    return true;
}
