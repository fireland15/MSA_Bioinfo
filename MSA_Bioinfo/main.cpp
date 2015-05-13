#include "Sequence.h"
#include "NWAlign.h"
#include "NeighborJoin.h"

#include <string>
#include <thread>
#include <future>
#include <iomanip>
#include "Timer.h"

std::string parseFASFA(std::string fname) {
	std::ifstream fasta(fname);
	std::string line, s;
	std::vector<char> seq;
	if (fasta.is_open()) {
		getline(fasta, line);
		while (!fasta.eof()) {
			getline(fasta, line);
			s.append(line);
		}
		return s;
	}
	else {
		std::cout << "Error: Unable to open - " << fname << std::endl;
		return "";
	}
}

void WriteToFile(Sequence& s, std::string filename) {
	std::vector<std::string> out(s.GetNumSequences(), "");

	for (int i = 0; i < s.GetLength(); i++) {
		std::vector<char> c = s[i];
		for (int j = 0; j < s.GetNumSequences(); j++) {
			out[j] += (c[j]);
		}
	}

	std::ofstream output;
	int L = s.GetLength();
	int charPerLine = 100;
	output.open(filename);
	if (output.is_open()) {


		for (int k = 0; k < L / charPerLine; k++) {
			output << std::setw(5) << std::left << (k * charPerLine) + 1 << std::setw(charPerLine - 5) << std::right << ((k + 1) * charPerLine) << std::endl;
			for (int i = 0; i < s.GetNumSequences(); i++) {
				for (int j = (k * charPerLine); j < (k + 1) * charPerLine; j++) {
					output << out[i][j];
				}
				output << std::endl;
			}
			output << std::endl;
		}


		int k = L / charPerLine;
		output << std::setw(5) << std::left << (k * charPerLine) + 1 << std::setw(L % charPerLine - 5) << std::right << L << std::endl;
		for (int i = 0; i < s.GetNumSequences(); i++) {
			for (int j = ((L / charPerLine) * charPerLine); j < L; j++) {
				output << out[i][j];
			}
			output << std::endl;
		}
		output << std::endl;

		output.close();
	}
	else {
		std::cout << "Error: Unable to open - " << filename << std::endl;
	}
}

int main(int argc, char** argv) {
	std::ios_base::sync_with_stdio(false);
	

	std::string filename;

	std::cout << argc << std::endl;
	if (argc == 1){
		std::cout << "Enter the name of the input file: ";
		std::cin >> filename;
	}
	else {
		filename = argv[1];
	}
	int f = 6128;
	NeighborJoin NJ(4, f);
	std::ifstream input(filename);
	if (input.is_open()){
		while (!input.eof()) {
			std::string fname;
			std::getline(input, fname);
			std::cout << "Opening " << fname << std::endl;
			NJ.AddSequence(Sequence(parseFASFA(fname)));
		}
	}
	else{
		std::cout << "Error opening input file" << std::endl;
		return 0;
	}

	fli::Timer t;
	t.Start();

	Sequence MSA = NJ.AlignSequences();

	std::cout << t.GetElapsedTimeSec() << std::endl;

	WriteToFile(MSA, "MSA.txt");

	std::vector<std::string> out(MSA.GetNumSequences(), "");

	return 0;
}