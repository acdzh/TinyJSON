#include <string>
#include <iostream>
#include "json.hpp"
using namespace std;
using namespace tiny;



int main(){
    
    string aa = "[{\"age\": 18, \"name\": \"�����\"}, {\"age\": 17, \"name\": \"�Ŷ���\" }, [{\"age\": 18, \"name\": \"������\"}, {\"age\": 14, \"name\": \"������\"}]]";
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

Json ErrCode


*/