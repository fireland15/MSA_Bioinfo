#pragma once

#include "NWAlign.h"

class NeighborJoin {
private:
	std::vector<Sequence> Sequences;
public:
	NeighborJoin();
	~NeighborJoin();

	void AddSequence(Sequence& S);

	Sequence Align   (const Sequence& s1, const Sequence& s2);
	double   Distance(const Sequence& s1, const Sequence& s2);
};