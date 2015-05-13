#pragma once

#include "NWAlign.h"
#include <thread>
#include <mutex>
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
	NeighborJoin(const NeighborJoin& nj) {

	}
	~NeighborJoin();

	void AddSequence(Sequence& S);
	Sequence AlignSequences();

	Sequence Align   (const Sequence& s1, const Sequence& s2);
	void     Distance(int id, int& job_id, std::stack<std::pair<std::pair<Sequence, int>, std::pair<Sequence, int>>>& taskList, std::vector<std::vector<double>>& Distmat);

private:
	std::vector<std::vector<double>> GenDistanceMatrix();
	double Q(std::vector<std::vector<double>>& Distmat, int u, int v, int n);
	double d(std::vector<std::vector<double>>& DistMat, int k, int f, int g);

	std::mutex mutex_taskList;
	std::mutex mutex_DistMat;
};