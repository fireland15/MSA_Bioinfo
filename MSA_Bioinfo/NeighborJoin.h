#pragma once

#include "NWAlign.h"
#include <thread>
#include <future>
#include <fstream>
#include <sstream>

struct ThreadData {
	Sequence* s1;
	Sequence* s2;
	bool started;
	bool done;
	double result;
	int i, j;
	bool killthread;

	ThreadData(bool _started = false, bool _done = true, bool _killthread = false) : started(_started), done(_done), killthread(_killthread), i(-1), j(-1) {

	}
};

class NeighborJoin {
private:
	std::vector<Sequence> Sequences;
public:
	NeighborJoin();
	~NeighborJoin();

	void AddSequence(Sequence& S);
	Sequence AlignSequences();

	Sequence Align   (const Sequence& s1, const Sequence& s2);
	void     Distance(ThreadData* data);

private:
	std::vector<std::vector<double>> GenDistanceMatrix();
	double Q(std::vector<std::vector<double>>& Distmat, int u, int v, int n);
	double d(std::vector<std::vector<double>>& DistMat, int k, int f, int g);
};