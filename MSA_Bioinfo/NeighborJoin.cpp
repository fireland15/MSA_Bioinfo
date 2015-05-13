#include "NeighborJoin.h"

NeighborJoin::NeighborJoin(int _maxThreads, int _memLimit ) { // 1GB memory default
	maxThreads = _maxThreads;
	memLimit = _memLimit;
}

NeighborJoin::~NeighborJoin() {

}

void NeighborJoin::AddSequence(Sequence& S) {
	Sequences.push_back(S);
}

Sequence NeighborJoin::AlignSequences() {
	std::vector<std::vector<double>> DistMat;

	DistMat = GenDistanceMatrix();
	
#ifdef DISTMAT
	std::ofstream file("distmat.txt");
	file << std::endl;
	int x = Sequences.size();
	for (unsigned int i = 0; i < x; i++) {
		for (unsigned int j = 0; j < x; j++) {
			file << DistMat[i][j] << "\t";
		}
		file << std::endl;
	}
	file.close();
#endif

	std::vector<std::vector<double>> Qmat;
	int x = 0;
	int count = 0;

	while (Sequences.size() > 2) {
		x = (int)Sequences.size();
		count++;

		Qmat.resize(x);
		for (int i = 0; i < x; i++) {
			Qmat[i].resize(x);
		}

		double minQ = (double)INFINITY;
		int sI, sJ;

		for (unsigned int i = 0; i < Qmat.size(); i++) {
			for (unsigned int j = 0; j < Qmat.size(); j++) {
				if (i != j){
					Qmat[i][j] = Q(DistMat, i, j, x);
				}
				else {
					Qmat[i][j] = 0;
				}

				if (Qmat[i][j] < minQ) {
					sI = i;
					sJ = j;
					minQ = Qmat[i][j];
				}
			}
		}

		Sequence s1 = Sequences[sI];
		Sequence s2 = Sequences[sJ];

		std::vector<Sequence>::iterator begin = Sequences.begin();
		if (sI > sJ) {
			Sequences.erase(begin + sI);
			Sequences.erase(begin + sJ);
		}
		else {
			Sequences.erase(begin + sJ);
			Sequences.erase(begin + sI);
		}

		Sequences.push_back(Align(s1, s2));

		x = (int)Sequences.size();

		std::vector<std::vector<double>> newDistMat;
		newDistMat.resize(x);
		for (int i = 0; i < x; i++)
			newDistMat[i].resize(x);

		int oI = 0;
		int oJ = 0;

		for (int i = 0; i < x - 1; i++) {
			if (sI == oI || sJ == oI) {
				oI++;
			}
			if (sI == oI || sJ == oI) {
				oI++;
			}

			oJ = 0;

			for (int j = 0; j < x - 1; j++) {
				if (sI == oJ || sJ == oJ) {
					oJ++;
				}
				if (sI == oJ || sJ == oJ) {
					oJ++;
				}

				newDistMat[i][j] = DistMat[oI][oJ];
				oJ++;
			}
			oI++;
		}

		oI = 0;
		for (int i = 0; i < x - 1; i++) {
			if (sI == oI || sJ == oJ) {
				oI++;
			}
			if (sI == oI || sJ == oI) {
				oI++;
			}
			newDistMat[i][x - 1] = d(DistMat, oI, sI, sJ);
			oI++;
		}

		oJ = 0;
		for (int i = 0; i < x - 1; i++) {
			if (sI == oJ || sJ == oJ) {
				oJ++;
			}
			if (sI == oJ || sJ == oJ) {
				oJ++;
			}
			newDistMat[x - 1][i] = d(DistMat, oJ, sI, sJ);
			oJ++;
		}

		newDistMat[x - 1][x - 1] = 0;
		DistMat.swap(newDistMat);
	}

	Sequence final = Align(Sequences[0], Sequences[1]);

	std::vector<Sequence>::iterator begin = Sequences.begin();

	Sequences.erase(begin + 1);
	Sequences.erase(begin);

	return final;
}

Sequence NeighborJoin::Align(const Sequence& s1, const Sequence& s2) {
	NWAlign align;
	return align.AlignSequences(s1, s2);
}

void NeighborJoin::Distance(int id, int& job_id, std::stack<std::pair<std::pair<Sequence, int>, std::pair<Sequence, int>>>& taskList, std::vector<std::vector<double>>& Distmat) {
	fli::Timer clock;
	clock.Start();
	while (!taskList.empty()) {
		int job;
		mutex_taskList.lock();
		std::pair<Sequence, int> p = taskList.top().first;
		std::pair<Sequence, int> q = taskList.top().second;
		taskList.pop();
		job = job_id++;
		mutex_taskList.unlock();

		NWAlign align;
		double result = align.CalculateDistance(p.first, q.first);
		clock.Stop();
		double time = clock.GetElapsedTimeSec();
		std::stringstream ss;
		ss << "Job: " << job_id << " - Thread " << id << " - " << time << " sec" << std::endl;
		std::cout << ss.str();
		clock.Start();

		mutex_DistMat.lock();
		Distmat[p.second][q.second] = Distmat[q.second][p.second] = result;
		mutex_DistMat.unlock();
	}
}

std::vector<std::vector<double>> NeighborJoin::GenDistanceMatrix() {
	std::vector<std::vector<double>> DistMat;

	unsigned int x = (unsigned int)Sequences.size();
	DistMat.resize(x);
	for (unsigned int i = 0; i < x; i++) {
		DistMat[i].resize(x);
	}

	std::stack<std::pair<std::pair<Sequence, int>, std::pair<Sequence, int>>> taskList;

	for (unsigned int i = 0; i < x; i++) {
		for (unsigned int j = 0; j < x - (i + 1); j++) {
			taskList.push(std::pair<std::pair<Sequence, int>, std::pair<Sequence, int>>(std::pair<Sequence, int>(Sequences[i], i), std::pair<Sequence, int>(Sequences[j], j)));
		}
	}

	int maxNumThreads;
	int maxHardwareThreads = std::thread::hardware_concurrency();
	if (maxThreads > maxHardwareThreads) {
		maxNumThreads = maxHardwareThreads;
	}
	else {
		maxNumThreads = maxThreads;
	}

	long int estimatedMem = (3 * Sequences[0].GetLength() * Sequences[0].GetLength()) / (1024 * 1024);
	std::cout << Sequences[0].GetLength() * Sequences[0].GetLength() << std::endl;
	std::cout << estimatedMem << std::endl;
	if (estimatedMem > 0) {
		if (memLimit / (estimatedMem) < maxNumThreads) {
			std::cout << memLimit / (estimatedMem) << std::endl;
			maxNumThreads = memLimit / (estimatedMem);
		}
	}

	std::cout << "Max Threads: " << maxNumThreads << std::endl;
	std::cout << "Total jobs: " << x * (x - 1) / 2 << std::endl;

	std::vector<std::thread> threadPool;
	int job_id = 0;

	for (int i = 0; i < maxNumThreads; i++) {
		threadPool.push_back(std::thread(&NeighborJoin::Distance, *this, i, std::ref(job_id), std::ref(taskList), std::ref(DistMat)));
		//std::stringstream ss;
		//ss << "Thread " << i << " started.\n";
		//std::cout << ss.str();
	}

	while (!taskList.empty()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	for (int i = 0; i < maxNumThreads; i++) {
		threadPool[i].join();
		std::cout << "Thread " << i << " killed." << std::endl;
	}
	return DistMat;
}

double NeighborJoin::Q(std::vector<std::vector<double>>& Distmat, int u, int v, int n) {
	double sumik = 0;
	double sumjk = 0;

	for (unsigned int k = 0; k < Distmat.size(); k++) {
		sumik += Distmat[u][k];
	}

	for (unsigned int k = 0; k < Distmat.size(); k++) {
		sumjk += Distmat[v][k];
	}

	return (n - 2) * Distmat[u][v] - sumik - sumjk;
}

double NeighborJoin::d(std::vector<std::vector<double>>& DistMat, int k, int f, int g) {
	return 0.5 * (DistMat[f][k] + DistMat[g][k] - DistMat[f][g]);
}