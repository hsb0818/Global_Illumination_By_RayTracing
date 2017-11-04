#pragma once

#include <vector>
using namespace std;

class CStringTokenizer
{
public:
	CStringTokenizer() {}
	CStringTokenizer(const string& inputstring, const string& seperator);
	virtual ~CStringTokenizer() {};
private:
	CStringTokenizer(const CStringTokenizer& rhs);

private:
	string _input;
	string _delimiter;
	vector<string> token;
	size_t m_index;

public:
	void Init(const string& inputstring, const string& seperator);
	size_t countTokens(); //token ����
	bool hasMoreTokens(); //token ���� Ȯ��
	string nextToken();  //���� token
	void split();   //string�� seperator�� ������ vector�� ����
};