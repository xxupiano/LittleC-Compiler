#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include "def.h"
using namespace std;

int row = 0; //记录行数
string lines; //按行读文件


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

    ///词法分析
    string s;
    while(getline(ifs, lines)) {
        row++;
        istringstream str(lines);
        while(str >> s) { //s按空格分隔读入
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
                        cout << "第" << row << "行：" + tmp + " 词法错误" << endl;
                        return 0;
                    }
                }

                //处理分隔符和运算符
                if(!isso(s,i)) {
                    cout << "第" << row << "行：" + s + " 词法错误" << endl;
                    return 0;
                }
                j = i; //从分隔符下一个位置开始处理

            }

            if(flag) { //s没有分隔符和运算符
                if(!istoken(s.substr(j))) {
                    cout << "第" << row << "行：" + s.substr(j) + " 词法错误" << endl; //不是合法的5种token，直接报错
                    return 0;
                }
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
    cout << "词法分析完成，正确" << endl;



    ///语法分析
    BLOCK(getNext());
    cout << "语法分析完成，正确" << endl;

    return 0;
}
