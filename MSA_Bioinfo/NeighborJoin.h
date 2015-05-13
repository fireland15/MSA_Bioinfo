#pragma once

#include "NWAlign.h"
#include <thread>
#include <mutex>
#include <future>
#include <fstream>
#include <sstream>
#include "Timer.h"

class NeighborJoin {
private:
	std::vector<Sequence> Sequences;
	int maxThreads;
	int memLimit;
public:
	NeighborJoin(int _maxThreads, int _memLimit);
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