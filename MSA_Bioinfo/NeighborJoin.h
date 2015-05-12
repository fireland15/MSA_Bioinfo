#pragma once

#include "NWAlign.h"
#include <thread>
#include <future>
#include <fstream>
#include <sstream>

class NeighborJoin {
private:
	std::vector<Sequence> Sequences;
public:
	NeighborJoin();
	~NeighborJoin();

	void AddSequence(Sequence& S);
	Sequence AlignSequences();

	Sequence Align   (const Sequence& s1, const Sequence& s2);
	double   Distance(const Sequence& s1, const Sequence& s2);

private:
	std::vector<std::vector<double>> GenDistanceMatrix();
	double Q(std::vector<std::vector<double>>& Distmat, int u, int v, int n);
	double d(std::vector<std::vector<double>>& DistMat, int k, int f, int g);
};