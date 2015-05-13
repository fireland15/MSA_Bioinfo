#pragma once

#include <iostream>
#include <vector>
#include <string>

class Sequence {
private:
	std::vector<int> seqID;
	std::vector<char> sequence;
	unsigned int numSequences;
public:
	Sequence();
	Sequence(std::string s);
	Sequence(const Sequence& s);
	~Sequence();

	std::vector<char> operator[](int index);
	std::vector<char> operator[](int index) const;

	void push(std::vector<char>& C);

	int  GetNumSequences()       { return numSequences; }
	int  GetNumSequences() const { return numSequences; }

	int  GetLength()       { return (int)(sequence.size() / numSequences); }
	int  GetLength() const { return (int)(sequence.size() / numSequences); }
};