// JsonExample.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "json.hpp"
#include <fstream>
using namespace std;
int main()
{
	nlohmann::json json;
	json["id"] = 100;
	json["test"] = "1234";

	nlohmann::json jsonItem1;
	jsonItem1["item1"] = "test";

	nlohmann::json jsonObj;
	jsonObj["obj1"] = json;
	jsonObj["obj2"] = jsonItem1;
//	std::cout << jsonObj;

	std::ofstream os("test.json", ios::binary);
	os << jsonObj.dump(2) << endl;

	std::ifstream in("test.json");
	if (!in.is_open())
		return 0;
	in.seekg(0, std::ios::end);
	int length = in.tellg();
	in.seekg(0, std::ios::beg);
	char *pbuf = new char[length];
	in.read(pbuf, length);
	in.clear();
	in.close();

	std::string strjson(pbuf,length);
	delete[] pbuf;

	//cout << strjson << endl;

	nlohmann::json jsonp = nlohmann::json::parse(strjson);;

	nlohmann::json j = jsonp.at("obj1");
	cout << j.dump(4) << endl;
	int a = j.at("id").get<int>();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
