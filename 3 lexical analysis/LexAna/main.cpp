#include <bits/stdc++.h>
#define NUMBER 0
#define ID     27
using namespace std;

///工作模块
struct token {
    string val;     //单词值
    string type;    //单词类型, keyword, separator, operator, id, number
    int num;        //单词种别码
};

unordered_map<string, int> keywords = {{"int",1}, {"if",2}, {"else",3},
                                       {"while", 4}, {"main", 5}, {"return", 6}};
unordered_map<char, int> separators = {{'{',7}, {'}',8}, {';', 9}, {'(', 10}, {')', 11},
                                       {'\\', 12}, {'"', 13}, {',', 14}};
unordered_map<char, int> operators1 = {{'=', 15}, {'+', 16}, {'-', 17}, {'*', 18},
                                        {'/', 19}, {'>', 20}, {'<', 21}, {'%', 22}};
unordered_map<string, int> operators2 = {{">=", 23}, {"<=", 24}, {"==", 25}, {"!=", 26}};
vector<string> ids; //所有标识符
vector<token> ans;  //符号表
int cnt = 0; //记录id数量

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
    regex e("[a-zA-Z_][a-z0-9A-Z_]{0,18}", regex_constants::icase);
    return regex_match(s, e);
}

//处理除运算符和分隔符以外的
bool istoken(string s) {
//    cout << s << endl;
    if(keywords[s]!=0) { // keyword
        ans.push_back({s, "keyword", keywords[s]});
    }
    else if(alldigit(s)) { //是number
        ans.push_back({s, "number", 0});
    }
    else if(isid(s)) { //id
        if(find(ids.begin(), ids.end(), s)!=ids.end()) { //new id
            ans.push_back({s, "id", ID+(cnt++)});
        }
        else {  //existing id
            int tmp = find(ids.begin(), ids.end(), s) - ids.begin(); //找到id已有种别码
            ans.push_back({s, "id", ID + tmp});
        }
    }
    else {return false;}
    return true;
}

bool isso(string s, int& i) {
//    cout << s[i] << s[i+1] << endl;
    if(separators[s[i]]!=0) { //分隔符
        string tmp;
        tmp.push_back(s[i]);
//        cout << tmp << endl;
        ans.push_back({tmp, "separator", separators[s[i]]});
        i++;
    }
    else if(operators2[s.substr(i,2)]!=0) { //双字符运算符，一定要先处理双字符！！！
        ans.push_back({s.substr(i,2), "operator", operators2[s.substr(i,2)]});
        i += 2;
    }
    else if(operators1[s[i]]!=0) { //单字符运算符
        string tmp;
        tmp.push_back(s[i]);
//        cout << tmp << endl;
        ans.push_back({tmp, "operator", operators1[s[i]]});
        i++;
    }
    else {return false;}
    return true;
}


///驱动模块
int main()
{
    string fn;
    cout << "Please input the filename: ";
    cin >> fn;
    ifstream ifs(fn, ios::in);
    if(!ifs) {
        cout << "Failed to open the file." << endl;
        return 0;
    }

    string s;
    while(ifs >> s) { //string可以按空格分隔字符串读入
//        cout << s << endl;
        int i=0,j=0; //标记每个分隔符位置，和待处理子串位置
        int flag = 0;
        //以分隔符和运算符为界限来分离字符串，处理分隔符之前的部分
        while(i<s.length()) {
            while(i<s.length() && separators[s[i]]==0 && operators1[s[i]]==0) {//寻找分隔符和运算符
                if(s.substr(i,2)=="!=") break;
                i++; //不是分隔符
            }
            if(i==s.length()) {
                flag = 1;
                break;//没有找到
            }

            if(j!=i) { //两个符号之间有内容，处理中间部分
                string tmp = s.substr(j, i-j);
//                cout << tmp << endl;
                if(!istoken(tmp)) {
                    cout << "Error!" << endl;
                    return 0;
                }
            }

            //处理分隔符和运算符
            if(!isso(s,i)) {
                cout << "Error!" << endl;
                return 0;
            }
            j = i; //从分隔符下一个位置开始处理

        }

        if(flag) { //s没有分隔符和运算符
            if(!istoken(s.substr(j))) {
                cout << "Error!" << endl; //不是合法的5种token，直接报错
                return 0;
            }
        }
    }
    ofstream ofs("Word_List.txt", ios::out);
    if(!ofs) {
        cout << "Failed to create the output file." << endl;
        return 0;
    }

    ofs.setf(ios::left);
    for(int i=0; i<ans.size(); i++) {
        ofs << setw(15) << i+1
            << setw(20) << ans[i].type
            << setw(20) << ans[i].val
            << setw(15) << ans[i].num << endl;
    }
    ifs.close();
    ofs.close();
    return 0;
}
