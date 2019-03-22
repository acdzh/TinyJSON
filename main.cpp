#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "json.hpp"
using namespace std;
using namespace tiny;



int main(){
    map<string, Value> map1;
    map1.insert(make_pair("name", Value("李二狗")));
    map1.insert(make_pair("sex", Value("男")));
    map1.insert(make_pair("age", Value(18)));
    Json js1(map1);
    
    cout << "二狗: " << js1.toString() << endl;
    map<string, Value> map2;
    map2.insert(make_pair("name", Value("王二花")));
    map2.insert(make_pair("sex", Value("女")));
    map2.insert(make_pair("age", Value(14)));
    Json js2(map2);

    cout << "二花: " << js1.toString() << endl;
    vector<Value> vec;
    vec.push_back(Value(&js1));
    vec.push_back(Value(&js2));
    Json js(vec);
    js += Value(js);
    cout << "象牙村人口: " << js.toString() << endl;
    vector<Value> vec1;
    vec1.push_back(Value("象牙山小学"));
    vec1.push_back(Value("铁岭一中"));
    Json j = Json(vec1);
    Value a (&j);
    js1.insert("school", &j);
    js.deleteById(0);
    
    cout << "二狗同学考上了高中: " << js1.toString() << endl;
    cout << "新的象牙村小学: " << Json(map2).toString() << endl;


}

/*
ErrCode
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



*/