#include <iostream>
#include "StringTokenizer.h"

using namespace std;

CStringTokenizer::CStringTokenizer(const string& inputstring, const string& seperator)
: _input(inputstring), _delimiter(seperator)
{
	split();
}

void CStringTokenizer::Init(const string& inputstring, const string& seperator)
{
	string(inputstring).swap(_input);
	string(seperator).swap(_delimiter);

	split();
}

size_t CStringTokenizer::countTokens()
{
	return token.size();
}

bool CStringTokenizer::hasMoreTokens()
{
	return (m_index < token.size());
}

string CStringTokenizer::nextToken()
{
	if (m_index < token.size())
	{
		return token[m_index++];
	}

	return "";
}

void CStringTokenizer::split()
{
	string::size_type lastPos = _input.find_first_not_of(_delimiter, 0); //구분자가 나타나지 않는 위치
	string::size_type Pos = _input.find_first_of(_delimiter, lastPos); //구분자가 나타나는 위치

	while(string::npos!=Pos || string::npos!=lastPos)
	{
		token.push_back(_input.substr(lastPos, Pos-lastPos));
		lastPos = _input.find_first_not_of(_delimiter, Pos); //구분자가 나타나지 않는 위치
		Pos = _input.find_first_of(_delimiter, lastPos); //구분자가 나타나는 위치
	}

	m_index = 0;
}