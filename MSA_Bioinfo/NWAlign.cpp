#include "NWAlign.h"

NWAlign::NWAlign() {

}

NWAlign::~NWAlign() {

}

Sequence NWAlign::AlignSequences(const Sequence& S1, const Sequence& S2) {
	std::stack<std::vector<char>> AlignStack;

	int rows = S1.GetLength();
	int cols = S2.GetLength();
	int x = S1.GetNumSequences();
	int y = S2.GetNumSequences();
	int alignmentScore = 0;
	int choice[3];

	InitScoreMatrix(rows, cols);
	InitTraceMatrix(rows, cols);

	// Scoring
	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= cols; j++) {
			std::vector<char> ci = S1[i - 1];
			std::vector<char> cj = S2[j - 1];

			int pairScore = 0;

			for (int k = 0; k < x; k++) {
				for (int l = 0; l < y; l++) {
					if (ci[k] == cj[l]) {
						pairScore += match;
					}
					else {
						pairScore += mismatch;
					}
				}
			}

			choice[0] = score[i - 1][j - 1] + pairScore;
			choice[1] = score[i - 1][j] + (gap * y);
			choice[2] = score[i][j - 1] + (gap * x);
			score[i][j] = max(choice[0], choice[1], choice[2]);

			if (score[i][j] == choice[0])
				trace[i][j] = 0;
			else if (score[i][j] == choice[1])
				trace[i][j] = 1;
			else
				trace[i][j] = -1;
		}
	}

	// Traceback
	int m = rows;
	int n = cols;
	while (m > 0 || n > 0) {
		if (trace[m][n] == 0) {
			std::vector<char> c = S1[m - 1];
			std::vector<char> d = S2[n - 1];

			for (int k = 0; k < y; k++) {
				c.push_back(d[k]);
			}

			AlignStack.push(c);
			m--;
			n--;
		}

		else if (trace[m][n] == 1) {
			std::vector<char> c = S1[m - 1];
			std::vector<char> d(y, '-');

			for (int k = 0; k < y; k++) {
				c.push_back(d[k]);
			}
			
			AlignStack.push(c);
			m--;
		}

		else if (trace[m][n] == -1) {
			std::vector<char> c(x, '-');
			std::vector<char> d = S2[n - 1];

			for (int k = 0; k < y; k++) {
				c.push_back(d[k]);
			}

			AlignStack.push(c);
			n--;
		}
	}

	Sequence S;
	while (!AlignStack.empty()){
		S.push(AlignStack.top());
		AlignStack.pop();
	}

	return S;
}

double NWAlign::CalculateDistance(const Sequence& S1, const Sequence& S2) {
	Sequence s = AlignSequences(S1, S2);

	std::vector<std::vector<int>> mat;

	mat.resize(s.GetLength());
	for (unsigned int i = 0; i < s.GetLength(); i++) {
		mat[i].resize(s.GetLength());
	}

	for (int i = 0; i < s.GetLength(); i++) {
		mat[i][0] = i;
		mat[0][i] = i;
	}

	char v1, v2;

	for (int i = 1; i < s.GetLength(); i++) {
		for (int j = 1; j < s.GetLength(); j++) {
			v1 = s[i - 1][0];
			v2 = s[j - 1][1];
			if (v1 == v2) {
				mat[i][j] = mat[i - 1][j - 1];
			}
			else {
				mat[i][j] = min(mat[i - 1][j] + 1, mat[i][j - 1] + 1, mat[i - 1][j - 1] + 1);
			}
		}
	}

	return mat[s.GetLength() - 1][s.GetLength() - 1];
}

int NWAlign::max(int A, int B, int C) {
	return std::max(A, std::max(B, C));
}

int NWAlign::min(int A, int B, int C) {
	return std::min(A, std::min(B, C));
}

void NWAlign::InitScoreMatrix(int a, int b) {
	if (a == 0 || b == 0)
		return;
	
	a++;
	b++;

	score.resize(a);
	for (int i = 0; i < a; i++) {
		score[i].resize(b);
		for (int j = 0; j < b; j++) {
			score[i][j] = 0;
		}
	}

	for (int i = 0; i < a; i++) {
		score[i][0] = mismatch * i;
	}

	for (int j = 0; j < b; j++) {
		score[0][j] = mismatch * j;
	}
}

void NWAlign::InitTraceMatrix(int a, int b) {
	if (a == 0 || b == 0)
		return;

	a++;
	b++;

	trace.resize(a);
	for (int i = 0; i < a; i++) {
		trace[i].resize(b);
		for (int j = 0; j < b; j++) {
			trace[i][j] = 0;
		}
	}

	for (int i = 0; i < a; i++) {
		trace[i][0] = 1;
	}

	for (int j = 0; j < b; j++) {
		trace[0][j] = -1;
	}
}