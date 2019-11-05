#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<bool> ruleSet;
vector<bool> prevGen;
vector<bool> currGen;
vector<vector<bool>> prevGen2d; //[row][column]
vector<vector<bool>> currGen2d;
int generationsPerformed = 0;
string outputFileName = "output.txt";
string inputFileName = "input.txt";

void performGeneration1d(bool wrap);
int getRuleIndex(bool first, bool second, bool third);
void printGeneration1d();
vector<bool> stringToBinary(string input);
string binaryToString(vector<bool> input);
int binaryToDecimal(vector<bool> binary);
vector<bool> decimalToBinary(int decimal);
int firstGenerationSize();
int numberOfGenerationsToRun();
void saveFile();
void loadFile();

int main() {
  	// loadFile();

	ruleSet.push_back(0);
	ruleSet.push_back(0);
	ruleSet.push_back(0);
	ruleSet.push_back(1);
	ruleSet.push_back(1);
	ruleSet.push_back(1);
	ruleSet.push_back(1);
	ruleSet.push_back(0);
  
 //    int size = firstGenerationSize();
    int generations = numberOfGenerationsToRun();
	prevGen = currGen;
	printGeneration1d();
	for (int i = 0; i < generations; i++) {
		performGeneration1d(false);
		printGeneration1d();
	}

	// saveFile();
    return 0;
}

void performGeneration1d(bool wrap) {
	generationsPerformed++;

	currGen[0] = ruleSet[getRuleIndex(wrap ? prevGen[currGen.size() - 1] : false, prevGen[0], prevGen[1])];
	for (int i = 1; i < currGen.size() - 1; i++) {
		currGen[i] = ruleSet[getRuleIndex(prevGen[i - 1], prevGen[i], prevGen[i + 1])];
	}
	currGen[currGen.size() - 1] = ruleSet[getRuleIndex(prevGen[currGen.size() - 2], prevGen[currGen.size() - 1], wrap ? prevGen[0] : false)];

	prevGen = currGen;
}

void performGeneration2d(bool wrap) {
	generationsPerformed++;

	//top row (done separately to allow wrapping)
	if (wrap) {
		currGen2d[0][0] = ruleSet[getRuleIndex(prevGen2d[currGen2d.size() - 1][currGen2d[0].size()-1], prevGen2d[currGen2d.size() - 1][0], prevGen2d[currGen2d.size() - 1][1])];
		for (int i = 1; i < currGen2d[0].size() - 1; i++) {
			currGen2d[0][i] = ruleSet[getRuleIndex(prevGen2d[currGen2d.size() -1][i - 1], prevGen2d[currGen2d.size() -1][i], prevGen2d[currGen2d.size() -1][i + 1])];
		}
		currGen2d[0][currGen2d[0].size() - 1] = ruleSet[getRuleIndex(prevGen2d[0][currGen2d[0].size() - 2], prevGen2d[0][currGen2d[0].size() -1], prevGen2d[0][0])];
	} else {
		currGen2d[0][0] = ruleSet[getRuleIndex(false, false, false)];
		for (int i = 1; i < currGen2d[0].size() - 1; i++) {
			currGen2d[0][i] = ruleSet[getRuleIndex(false, false, false)];
		}
		currGen2d[0][currGen2d[0].size() - 1] = ruleSet[getRuleIndex(false, false, false)];
	}

	//other rows
	for (int i = 1; i < currGen2d.size(); i++) {
		currGen2d[i][0] = ruleSet[getRuleIndex(wrap ? prevGen2d[i - 1][currGen2d[0].size() - 1] : false, prevGen2d[i - 1][0], prevGen2d[i - 1][1])];
		for (int j = 1; j < currGen2d[0].size() - 1; j++) {
			currGen2d[i][j] = ruleSet[getRuleIndex(prevGen2d[i - 1][j - 1], prevGen2d[i - 1][j], prevGen2d[i - 1][j + 1])];
		}
		currGen2d[i][currGen2d[0].size() - 1] = ruleSet[getRuleIndex(prevGen2d[i - 1][currGen2d[0].size() - 2], prevGen2d[i - 1][currGen2d[0].size() - 1], wrap ? prevGen2d[i - 1][0] : false)];
	}

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

void printGeneration1d() {
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
	while(binary.size() < 8) {
		binary.insert(binary.begin(), 0);		
	}
	return binary;
}

int firstGenerationSize() {
    int generationLenght;
    cout << "Input the lenght of the first generation:" << endl;
    cin >> generationLenght;
    while (generationLenght <= 0) {
        cout << "Error, input cannot be below 0." << endl;
        cin >> generationLenght;
    }
    return generationLenght;
}

int numberOfGenerationsToRun() {
    int numberOfGenerations;
    cout << "Input the number of generations to run:" << endl;
    cin >> numberOfGenerations;
    while (numberOfGenerations <= 0) {
        cout << "Error, input cannot be below 0." << endl;
        cin >> numberOfGenerations;
    }
    return numberOfGenerations;
}

void saveFile() {
	ofstream fr(outputFileName);
	fr << binaryToDecimal(ruleSet) << endl;
	fr << generationsPerformed << endl;
	fr << binaryToString(currGen) << endl;
	fr.close();
}

void loadFile() {
	ifstream fi(inputFileName);
	int decimalRuleSet;
	fi >> decimalRuleSet;
	ruleSet = decimalToBinary(decimalRuleSet);
	fi >> generationsPerformed;
	string currGenString;
	fi >> currGenString;
	currGen = stringToBinary(currGenString);
	fi.close();
}
