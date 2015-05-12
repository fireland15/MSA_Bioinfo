#pragma once

#include "Sequence.h"

#include <algorithm>
#include <stack>

class NWAlign {
private:
	std::vector<std::vector<int>> score;
	std::vector<std::vector<int>> trace;

	int mismatch = -1;
	int match = 2;
	int gap = -2;
public:
	NWAlign();
	~NWAlign();

	Sequence  AlignSequences(const Sequence& S1, const Sequence& S2);
	double    CalculateDistance(Sequence& S1, Sequence& S2);

private:
	int max(int A, int B, int C);
	void InitScoreMatrix(int a, int b);
	void InitTraceMatrix(int a, int b);
};