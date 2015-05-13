#include "NeighborJoin.h"

NeighborJoin::NeighborJoin() {

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
	std::cout << "Thread " << id << " started.\n";
	while (!taskList.empty()) {
		mutex_taskList.lock();
		std::pair<Sequence, int> p = taskList.top().first;
		std::pair<Sequence, int> q = taskList.top().second;
		taskList.pop();
		job_id = job_id + 1;
		//std::cout << "Thread " << id << " started job " << job_id << "\n";
		mutex_taskList.unlock();

		NWAlign align;
		double result = align.CalculateDistance(p.first, q.first);

		mutex_DistMat.lock();
		Distmat[p.second][q.second] = Distmat[q.second][p.second] = result;
		mutex_DistMat.unlock();
		//std::cout << "Thread " << id << " finished." << std::endl;
	}
}

std::vector<std::vector<double>> NeighborJoin::GenDistanceMatrix() {
	std::vector<std::vector<double>> DistMat;

	unsigned int x = Sequences.size();
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

	int maxNumThreads = std::thread::hardware_concurrency() - 1;
	std::cout << "Max Threads: " << maxNumThreads << std::endl;
	std::vector<std::thread> threadPool;
	int job_id = 0;

	for (int i = 0; i < maxNumThreads; i++) {
		threadPool.push_back(std::thread(&NeighborJoin::Distance, *this, i, std::ref(job_id), std::ref(taskList), std::ref(DistMat)));
	}

	while (!taskList.empty()) {
		_sleep(1);
		//std::cout << taskList.size() << std::endl;
	}

	for (int i = 0; i < maxNumThreads; i++) {
		threadPool[i].join();
		std::cout << "Thread " << i << " killed." << std::endl;
	}

	//std::cout << "Generating NeighborJoin Distance Matrix..." << std::endl;

	//int progress = 0; 
	//int load = x * (x - 1) / 2;

	//for (unsigned int i = 0; i < x; i++) {
	//	std::vector<std::future<double>> threads;
	//	for (unsigned int j = 0; j < x - (i + 1); j++) {
	//		//std::cout << i << " " << j + 1 + i << std::endl;
	//		threads.push_back(std::async(&NeighborJoin::Distance, this, Sequences[i], Sequences[j + (i + 1)]));
	//		//std::cout << "Thread " << j << " started." << std::endl;
	//	}

	//	for (unsigned int j = 0; j < x - (i + 1); j++) {
	//		DistMat[i][j + (i + 1)] = DistMat[j + (i + 1)][i] = threads[j].get();
	//		//std::cout << "Thread " << j << " returned." << std::endl;
	//	}
	//}

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