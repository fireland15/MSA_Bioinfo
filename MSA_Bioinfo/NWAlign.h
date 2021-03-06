#pragma once

#include "Sequence.h"

#include <algorithm>
#include <stack>

class NWAlign {
private:
	std::vector<std::vector<short>> score;
	std::vector<std::vector<char>> trace;

	int mismatch = -1;
	int match = 2;
	int gap = -3;
public:
	NWAlign();
	~NWAlign();

	Sequence  AlignSequences(const Sequence& S1, const Sequence& S2);
	double    CalculateDistance(const Sequence& S1, const Sequence& S2);

private:
	int  max(int A, int B, int C);
	int  min(int A, int B, int C);
	void InitScoreMatrix(int a, int b);
	void InitTraceMatrix(int a, int b);
};