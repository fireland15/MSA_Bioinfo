#include "Sequence.h"
#include "NWAlign.h"

#include <string>
#include <thread>
#include <future>

Sequence alignment(const Sequence& s1, const Sequence& s2) {
	NWAlign align;
	return align.AlignSequences(s1, s2);
}

double distance(Sequence s1, Sequence s2) {
	return 0.01;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::string str = "abcdeijdvsdvsdvifg";
	Sequence s(str);

	std::string trs = "abcdsasceeg";
	Sequence r(trs);

	std::string rts = "abcdsascedg";
	Sequence q(rts);

	std::future<Sequence> t1 = std::async(&alignment, s, r);
	std::future<Sequence> t2 = std::async(&alignment, r, q);

	Sequence out1 = t1.get();
	Sequence out2 = t2.get();

	std::string top;
	std::string bot;

	for (unsigned int i = 0; i < out1.GetLength(); i++) {
		std::vector<char> c = out1[i];
		top += c[0];
		bot += c[1];
	}

	std::cout << top << std::endl;
	std::cout << bot << std::endl;

	top.clear();
	bot.clear();

	for (unsigned int i = 0; i < out2.GetLength(); i++) {
		std::vector<char> c = out2[i];
		top += c[0];
		bot += c[1];
	}

	std::cout << top << std::endl;
	std::cout << bot << std::endl;
	return 0;
}