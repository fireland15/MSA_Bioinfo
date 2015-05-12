#include "Sequence.h"

Sequence::Sequence() {
	numSequences = 1;
}

Sequence::Sequence(std::string s) {
	for (unsigned int i = 0; i < s.length(); i++) {
		sequence.push_back(s[i]);
	}
	numSequences = 1;
}

Sequence::Sequence(const Sequence& s) {
	numSequences = s.numSequences;
	sequence = s.sequence;
}

Sequence::~Sequence() {

}

std::vector<char> Sequence::operator[](int index) {
	std::vector<char> C;
	for (unsigned int i = numSequences * index; i < numSequences * (index + 1); i++) {
		C.push_back(sequence[i]);
	}
	return C;
}

std::vector<char> Sequence::operator[](int index) const {
	std::vector<char> C;
	for (unsigned int i = numSequences * index; i < numSequences * (index + 1); i++) {
		C.push_back(sequence[i]);
	}
	return C;
}

void Sequence::push(std::vector<char>& C) {
	if (numSequences == 1) {
		numSequences = C.size();
	}
	if (numSequences == C.size()) {
		for (unsigned int i = 0; i < C.size(); i++) {
			sequence.push_back(C[i]);
		}
		return;
	}
	else {
		std::cout << "Error: Can't push incorrect amount of characters." << std::endl;
	}
	return;
}
