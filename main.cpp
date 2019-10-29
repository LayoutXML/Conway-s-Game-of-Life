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
int binaryToDecimal(vector<bool> binary);
vector<bool> decimalToBinary(int decimal);
void firstGenerationSize();
int numberOfGenerationsToRun();

int main() {
    int size = firstGenerationSize();
    numberOfGenerationsToRun();
	for (int i = 0; i < size; i++) {
		currGen.push_back(0);
	}
	currGen.push_back(1);
	for (int i = 0; i < size; i++) {
		currGen.push_back(0);
	}
	prevGen = currGen;
	printGeneration2d();
	for (int i = 0; i < 15; i++) {
		performGeneration2d();
		printGeneration2d();
	}

	vector<bool> binary = decimalToBinary(22);
	cout << binaryToDecimal(binary) << endl;
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
	cout << binaryToString(currGen) << endl;
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

int binaryToDecimal(vector<bool> binary) {
	int i = 1;
	int element;
	int decimal = 0;
	while (binary.size() != 0) {
		element = binary.back();
		binary.pop_back();
		decimal += element * i;
		i *= 2;
	}
	return decimal;
}

vector<bool> decimalToBinary(int decimal) {
	vector<bool> binary;
	while (decimal != 1) {
		binary.insert(binary.begin(), decimal%2);
		decimal /= 2;
	}
	binary.insert(binary.begin(), 1);
	return binary;
}

int firstGenerationSize() {
    int generationLenght;
    cin >> generationLenght;
    if (generationLenght <= 0) {
        cout << "Error, input cannot be below 0." << endl;
    }
    return generationLenght;
}

int numberOfGenerationsToRun() {
    int numberOfGenerations;
    cin >> numberOfGenerations;
    if (numberOfGenerations <= 0) {
        cout << "Error, input cannot be below 0." << endl;
    }
    return numberOfGenerations;
}
