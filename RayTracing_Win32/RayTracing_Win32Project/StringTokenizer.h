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
	size_t countTokens(); //token 갯수
	bool hasMoreTokens(); //token 존재 확인
	string nextToken();  //다음 token
	void split();   //string을 seperator로 나눠서 vector에 저장
};