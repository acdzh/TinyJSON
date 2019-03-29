#include <string>
#include <iostream>
#include "json.hpp"
using namespace std;
using namespace tiny;



int main(){
    
    string aa = "[{\"age\": 18, \"name\": \"李二狗\"}, {\"age\": 17, \"name\": \"张二楞\" }, [{\"age\": 18, \"name\": \"王二花\"}, {\"age\": 14, \"name\": \"刘旺财\"}]]";
    //cout << "input str: ";
    //getline(cin, aa);
    Json j = new Json();
    j.parse(aa);
    cout << "str : " << aa << endl;
    //cout << "Json: " << j.toString() << endl;
    //Value qqq = j.getValueById(1);
    //Json jj = *(qqq.getJson());
    //cout << jj.toString();
}

/*
Value ErrCode
-2: 尚未分类
-1: 自定义类型错误
0: 无错误
10: 引用错误
11: 不存在的值
12: 索引超限
20: 错误的引用
21: 错误的引用, 尝试用id引用{}
22: 错误的引用, 尝试用key引用[]
30: 代数运算出错
31: 代数运算类型不匹配 i j
32: 代数运算类型不匹配 j i
33: 代数运算前者是错误类型
34: 代数运算后者是错误类型

Json ErrCode


*/