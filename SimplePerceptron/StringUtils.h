#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <regex>
#include <map>

using namespace std;

template <class T, class Y>
class VariableMultiListNode
{
public:

	T key;
	vector<Y> list;
};

template< class T, class Y >
class VariableSizedMultiList
{
public:
	VariableMultiListNode<T, Y>& operator[](T key)
	{
		for (int i = 0; i < m_storage.size(); i++)
		{
			if (m_storage[i].key == key){
				return m_storage[i];
			}
		}

		return m_storage[0];
	}

	int Insert(T key, Y val)
	{
		for (int i = 0; i < m_storage.size(); i++)
		{
			if (m_storage[i].key == key){

				m_storage[i].list.push_back(val);
				return i;
			}
		}

		// key not found, add new entry
		VariableMultiListNode<T, Y> node;
		node.key = key;
		node.list.push_back(val);
		m_storage.push_back(node);

		return m_storage.size() - 1;


	}

	int FindKey(T key)
	{
		for (int i = 0; i < m_storage.size(); i++)
		{
			if (m_storage[i].key == key){
				return i;
			}
		}
		return -1;
	}

	vector<VariableMultiListNode<T, Y>> m_storage;
};

std::vector<std::string> split(std::string str, std::string sep){
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> arr;
	current = strtok_s(cstr, sep.c_str(), 0);
	while (current != NULL){
		arr.push_back(current);
		current = strtok_s(NULL, sep.c_str(), 0);
	}
	return arr;
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}



using namespace std;
/*
vector<string> split2(string delim, string s){
regex r(delim); //regex matches whole words, (greedy, so no fragment words)
regex_iterator<string::iterator> rit(s.begin(), s.end(), r);
regex_iterator<string::iterator> rend; //iterators to iterate thru words
vector<string> result<regex_iterator>(rit, rend);
return result;  //iterates through the matches to fill the vector
}*/

std::vector<string> vsplit(const string str, const std::regex regex)
{
	using namespace std;

	std::vector<string> result;

	sregex_token_iterator it(str.begin(), str.end(), regex, -1);
	sregex_token_iterator reg_end;

	for (; it != reg_end; ++it) {
		if (!it->str().empty()) //token could be empty:check
			result.emplace_back(it->str());
	}

	return result;
}

string separate_commas(string s)
{
	string out = "";
	for (int i = 0; i < s.size(); i++)
	{
		out += s[i];
		if (i == s.size() - 1)break;
		out += ",";
	}

	return out;
}

vector<string> string_to_vector(string s)
{
	vector<string> out;
	int iter = 0;

	while (iter < s.size())
	{
		string s1 = "";
		while (s[iter] != ','){
			s1 += s[iter++];
			if (iter >= s.size())
				break;
		}
		out.push_back(s1); iter++;
		if (iter >= s.size())
			return out;
	}


	return out;
}


int string_to_number(string s)
{
	return atoi(s.c_str());
}

float string_to_float(string s)
{
	return atof(s.c_str());
}

bool isNumberC(const std::string& s)
{
	char* p;
	strtod(s.c_str(), &p);
	return *p == 0;
}

double string_to_double(string s)
{
	char* p;
	double val = strtod(s.c_str(), &p);
	if (*p == 0) return val;

	return 0.0;
}

// if its an integer, return a 1
bool GetType(double d)
{
	if ((d - floor(d)) != 0.0)
		return false;
	else return true; // it can be an integer
}




int indexOf(string findelement, string x)
{
	std::size_t found = x.find(findelement);
	if (found != std::string::npos)
		return 1;
	else return -1;

}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}


string vector_to_string_comma_split(vector<string> vs)
{
	string output = "";
	for (int i = 0; i < vs.size(); i++)
	{
		output += vs[i];
		if (i == vs.size() - 1)break;
		output += ",";
	}

	return output;
}


template < class ContainerT >
void tokenize(const std::string& str, vector<string>& tokens,
	const std::string& delimiters = " ", bool trimEmpty = false)
{
	std::string::size_type pos, lastPos = 0;

	using value_type = typename ContainerT::value_type;
	using size_type = typename ContainerT::size_type;

	while (true)
	{
		pos = str.find_first_of(delimiters, lastPos);
		if (pos == std::string::npos)
		{
			pos = str.length();

			if (pos != lastPos || !trimEmpty)
				tokens.push_back(value_type(str.data() + lastPos,
				(size_type)pos - lastPos));

			break;
		}
		else
		{
			if (pos != lastPos || !trimEmpty)
				tokens.push_back(value_type(str.data() + lastPos,
				(size_type)pos - lastPos));
		}

		lastPos = pos + 1;
	}
}
vector<string> tokenize2(const string& s, const string& delim, const bool keep_empty = true) {
	vector<string> result;
	if (delim.empty()) {
		result.push_back(s);
		return result;
	}
	string::const_iterator substart = s.begin(), subend;
	while (true) {
		subend = search(substart, s.end(), delim.begin(), delim.end());
		string temp(substart, subend);
		if (keep_empty || !temp.empty()) {
			result.push_back(temp);
		}
		if (subend == s.end()) {
			break;
		}
		substart = subend + delim.size();
	}
	return result;
}


template<typename T>
vector<T>
tokenize3(const T & str, const T & delimiters) {
	vector<T> v;
	T::size_type start = 0;
	auto pos = str.find_first_of(delimiters, start);
	while (pos != T::npos) {
		if (pos != start) // ignore empty tokens
			v.emplace_back(str, start, pos - start);
		start = pos + 1;
		pos = str.find_first_of(delimiters, start);
	}
	if (start < str.length()) // ignore trailing delimiter
		v.emplace_back(str, start, str.length() - start); // add what's left of the string
	return v;
}


template<typename Operator>
void tokenize4(Operator& op, const char* input, const char* delim)
{
	const char* s = input;
	const char* e = s;
	while (*e != 0) {
		e = s;
		while (*e != 0 && strchr(delim, *e) == 0)
		{
			++e;
		}
		if (*e == '(' || *e == ')' || *e == '{' || *e == '}' || *e == '+' || *e == '*' || *e == '/' ||
			*e == '%' || *e == '^' || *e == '!' || *e == '-' || *e == '=' || *e == '>' || *e == '<' || *e == ';')
		{
			if (e - s > 0)
			{
				op(s, e - s);
				op(e, 1);

			}
			else
				op(e, 1);

			s = e + 1;
		}
		else
		{
			if (e - s > 0) {
				op(s, e - s);
			}
			s = e + 1;
		}
	}
}

//using namespace MY_GLOBAL_HASH_SPACE;

class Appender : public std::vector<std::string> {
public:
	void operator() (const char* s, unsigned length) {
		this->push_back(std::string(s, length));
	}
};




namespace my {
	// Templated join which can be used on any combination of streams, iterators and base types //
	template <typename TStream, typename TIter, typename TSeperator>
	TStream& join(TStream& stream, TIter begin, TIter end, TSeperator seperator) {
		// A flag which, when true, has next iteration prepend our seperator to the stream //
		bool sep = false;
		// Begin iterating through our list //
		for (TIter i = begin; i != end; ++i) {
			// If we need to prepend a seperator, do it //
			if (sep) stream << seperator;
			// Stream the next value held by our iterator //
			stream << *i;
			// Flag that next loops needs a seperator //
			sep = true;
		}
		// As a convenience, we return a reference to the passed stream //
		return stream;
	}
}

#endif