#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "json.hpp"
using namespace std;
using namespace tiny;



int main(){
    map<string, Value> map1;
    map1.insert(make_pair("name", Value("�����")));
    map1.insert(make_pair("sex", Value("��")));
    map1.insert(make_pair("age", Value(18)));
    Json js1(map1);
    
    cout << "����: " << js1.toString() << endl;
    map<string, Value> map2;
    map2.insert(make_pair("name", Value("������")));
    map2.insert(make_pair("sex", Value("Ů")));
    map2.insert(make_pair("age", Value(14)));
    Json js2(map2);

    cout << "����: " << js1.toString() << endl;
    vector<Value> vec;
    vec.push_back(Value(&js1));
    vec.push_back(Value(&js2));
    Json js(vec);
    js += Value(js);
    cout << "�������˿�: " << js.toString() << endl;
    vector<Value> vec1;
    vec1.push_back(Value("����ɽСѧ"));
    vec1.push_back(Value("����һ��"));
    Json j = Json(vec1);
    Value a (&j);
    js1.insert("school", &j);
    js.deleteById(0);
    
    cout << "����ͬѧ�����˸���: " << js1.toString() << endl;
    cout << "�µ�������Сѧ: " << Json(map2).toString() << endl;


}

/*
ErrCode
-2: ��δ����
-1: �Զ������ʹ���
0: �޴���
10: ���ô���
11: �����ڵ�ֵ
12: ��������
20: ���������
21: ���������, ������id����{}
22: ���������, ������key����[]
30: �����������
31: �����������Ͳ�ƥ�� i j
32: �����������Ͳ�ƥ�� j i
33: ��������ǰ���Ǵ�������
34: ������������Ǵ�������



*/