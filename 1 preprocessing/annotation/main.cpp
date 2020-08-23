#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
using namespace std;

string ans = "";

int main() {
    string s;
    cin >> s; // start
    getchar(); //start后面的换行符
    while(getline(cin, s)) {
        if(s=="end") { break;}
        ans += s;
        ans += "\n";
    }

    int i=0, j=0, flag=0; //flag标记/*
    int len = ans.length();
    while(i+1<len) {
        if(ans[i]=='/' && ans[i+1]=='/') { //处理注释//
            j = i+2;
            while(j<len) {
                if(ans[j]=='\n') {
                    ans = ans.substr(0, i) + ans.substr(j); //去掉注释行
                    break; //处理完//
                }
                else j++;
            }
        }
        else if(ans[i]=='/' && ans[i+1]=='*' && !flag) {
            flag=1;
            j=i+2;
            while(j+1<len) {
                if(ans[j]=='*' && ans[j+1]=='/') {
                    flag=0;
                    ans = ans.substr(0, i) + ans.substr(j+2);
                    break; // 一对/**/处理完成
                }
                else j++;
            }
            if(flag) {
                ans = ans.substr(0,i) + "\n";
                break;//已经到字符串尾还没有找到*/，可以直接报错啦
            }
        }
        else i++;
    }
    cout << "start\n" << ans;
    if(flag) {
        cout << "error";
    }
    else cout << "end";
    return 0;
}


