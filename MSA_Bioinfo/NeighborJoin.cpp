#include "NeighborJoin.h"

NeighborJoin::NeighborJoin() {

}

NeighborJoin::~NeighborJoin() {

}

void NeighborJoin::AddSequence(Sequence& S) {
	Sequences.push_back(S);
}

Sequence NeighborJoin::Align(const Sequence& s1, const Sequence& s2) {
	NWAlign align;
	return align.AlignSequences(s1, s2);
}

double NeighborJoin::Distance(const Sequence& s1, const Sequence& s2) {
	NWAlign align;
	return align.CalculateDistance(s1, s2);
}