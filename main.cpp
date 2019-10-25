#include <iostream>
#include <vector>

using namespace std;

bool ruleSet[8] = {0, 0, 0, 1, 1, 1, 1, 0};
vector<bool> prevGen;
vector<bool> currGen;

void performGeneration2d();
int getRuleIndex(bool first, bool second, bool third);
void printGeneration2d();
vector<bool> stringToBinary(string input);
string binaryToString(vector<bool> input);

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

	//Testing code
	// string input = ".00..0.0..";
	// stringToBinary(input);
	// vector<bool> input2; 
    // input2.push_back(1);
    // input2.push_back(1);
    // input2.push_back(1); 
    // input2.push_back(0);
    // input2.push_back(1);
	// binaryToString(input2);
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
	binaryToString(currGen);
	cout << endl;
}

vector<bool> stringToBinary(string input) {
    vector<bool> output;
    for (int i = 0; i < input.length(); i++) {
        (input[i] == '.') ? output.push_back(0) : output.push_back(1);
	}
    return output;
}

string binaryToString(vector<bool> input) {
	string outputString;


    for (int i = 0; i < input.size(); i++) {
        (input[i] == 0) ? outputString+="." : outputString+="0";
	}
    return outputString;
}
