#include <iostream>
#include <vector>

using namespace std;

bool ruleSet[8] = {0, 0, 0, 1, 1, 1, 1, 0};
vector<bool> prevGen;
vector<bool> currGen;

void performGeneration2d();
int getRuleIndex(bool first, bool second, bool third);
void printGeneration2d();

int main() {
	for (int i = 0; i < 15; i++) {
		currGen.push_back(0);
	}
	currGen.push_back(1);
	for (int i = 0; i < 15; i++) {
		currGen.push_back(0);
	}
	prevGen = currGen;
	printGeneration2d();
	for (int i = 0; i < 15; i++) {
		performGeneration2d();
		printGeneration2d();
	}
    return 0;
}

void performGeneration2d() {
	currGen[0] = ruleSet[getRuleIndex(false, prevGen[0], prevGen[1])];
	for (int i = 1; i < currGen.size() - 1; i++) {
		currGen[i] = ruleSet[getRuleIndex(prevGen[i - 1], prevGen[i], prevGen[i + 1])];
	}
	currGen[currGen.size() - 1] = ruleSet[getRuleIndex(prevGen[currGen.size() - 2], prevGen[currGen.size() - 1], false)];
	prevGen = currGen;
}

int getRuleIndex(bool first, bool second, bool third) {
	//111 110 101 100 011 010 001 000
	if (first) {
		if (second) {
			if (third) {
				return 0;
			}
			return 1;
		} else {
			if (third) {
				return 2;
			}
			return 3;
		}
	}
	if (second) {
		if (third) {
			return 4;
		}
		return 5;
	}
	if (third) {
		return 6;
	}
	return 7;
}

void printGeneration2d() {
	for (int i = 0; i < currGen.size(); i++) {
		cout << (currGen[i] ? "O" : ".");
	}
	cout << endl;
}
