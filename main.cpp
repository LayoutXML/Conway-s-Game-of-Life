#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

const string outputFileName = "output.txt";
const string inputFileName = "input.txt";

vector <bool> ruleSet;
vector <bool> prevGen;
vector <bool> currGen;
vector <vector<bool>> prevGen2d; //[row][column]
vector <vector<bool>> currGen2d;
int generationsPerformed = 0;
bool is1d = true;

void initialise();
int firstGenerationSize();
int numberOfGenerationsToRun();
void chooseFirstGeneration(int size);
void chooseRuleSet();
void randomRuleSet();
void performGeneration1d(bool wrap);
void performGeneration2d(bool wrap);
void printGeneration1d();
void printGeneration2d(vector<vector<bool>> generation);
vector < bool > stringToBinary(string input);
string binaryToString(vector < bool > input);
vector < bool > decimalToBinary(int decimal);
int binaryToDecimal(vector < bool > binary);
void loadFile();
void saveFile();
int getRuleIndex(bool first, bool second, bool third);
void gameOfLife(vector<vector<bool>> board, int ticks);
bool gameOfLifeDecision(bool tl, bool tm, bool tr, bool l, bool itself, bool r, bool bl, bool bm, bool br);

int main() {
    initialise();

    int generations = numberOfGenerationsToRun();
    printGeneration1d();
    for (int i = 0; i < generations; i++) {
        performGeneration1d(false);
    }

    saveFile();
    return 0;
}

void initialise() {
    char option;

    cout << "Load from file? [Y/n]: ";
    cin >> option;

    if (option == 'Y' || option == 'y') {
        loadFile();
    } else {
        chooseFirstGeneration(firstGenerationSize());
    }

    if (ruleSet.size() != 8) {
        cout << "Ruleset is not defined yet. Apply a random ruleset? [Y/n]: ";
        cin >> option;

        if (option == 'Y' || option == 'y') {
            randomRuleSet();
        } else {
            chooseRuleSet();
        }
    }

    cout << "Ruleset " << binaryToDecimal(ruleSet) << " applied" << endl;
}

int firstGenerationSize() {
    int generationLength;
    cout << "Enter the length of the first generation: ";
    cin >> generationLength;
    while (generationLength <= 3) {
        cout << "Error, input cannot be below 3. Enter a new number: ";
        cin >> generationLength;
    }
    return generationLength;
}

int numberOfGenerationsToRun() {
    int numberOfGenerations;
    cout << "Input the number of generations to run: ";
    cin >> numberOfGenerations;
    while (numberOfGenerations <= 0) {
        cout << "Error, input cannot be below 0. Try again: ";
        cin >> numberOfGenerations;
    }
    return numberOfGenerations;
}

void chooseFirstGeneration(int size) {
    string firstGeneration;
    cout << "Enter first generation of size " << size << ". Dots (.) represent empty values, zeros (0) represent full values: ";
    cin >> firstGeneration;
    while (firstGeneration.find_first_not_of(".0") != string::npos || firstGeneration.length() != size) {
        cout << "Illegal characters encountered or wrong generation size. Try again: ";
        cin >> firstGeneration;
    }
    currGen = stringToBinary(firstGeneration);
    prevGen = currGen;
}

void chooseRuleSet() {
    int rulesetDecimal;
    cout << "Enter a ruleset between 0 and 255: ";
    cin >> rulesetDecimal;
    if (rulesetDecimal < 0 || rulesetDecimal > 255) {
        cout << "Incorrect value. Try again: ";
        cin >> rulesetDecimal;
    }
    ruleSet = decimalToBinary(rulesetDecimal);
}

void randomRuleSet() {
    srand(time(0));
    ruleSet.clear();
    for (int i = 0; i < 8; i++) {
        ruleSet.push_back(rand() % 2);
    }
}

void performGeneration1d(bool wrap) {
    generationsPerformed++;

    currGen[0] = ruleSet[getRuleIndex(wrap ? prevGen[currGen.size() - 1] : false, prevGen[0], prevGen[1])];
    for (int i = 1; i < currGen.size() - 1; i++) {
        currGen[i] = ruleSet[getRuleIndex(prevGen[i - 1], prevGen[i], prevGen[i + 1])];
    }
    currGen[currGen.size() - 1] = ruleSet[getRuleIndex(prevGen[currGen.size() - 2], prevGen[currGen.size() - 1], wrap ? prevGen[0] : false)];

    prevGen = currGen;
    printGeneration1d();
}

void performGeneration2d(bool wrap) {
    generationsPerformed++;

    //top row (done separately to allow wrapping)
    if (wrap) {
        currGen2d[0][0] = ruleSet[getRuleIndex(prevGen2d[currGen2d.size() - 1][currGen2d[0].size() - 1], prevGen2d[currGen2d.size() - 1][0], prevGen2d[currGen2d.size() - 1][1])];
        for (int i = 1; i < currGen2d[0].size() - 1; i++) {
            currGen2d[0][i] = ruleSet[getRuleIndex(prevGen2d[currGen2d.size() - 1][i - 1], prevGen2d[currGen2d.size() - 1][i], prevGen2d[currGen2d.size() - 1][i + 1])];
        }
        currGen2d[0][currGen2d[0].size() - 1] = ruleSet[getRuleIndex(prevGen2d[0][currGen2d[0].size() - 2], prevGen2d[0][currGen2d[0].size() - 1], prevGen2d[0][0])];
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
    printGeneration2d(currGen2d);
}

void gameOfLife(vector<vector<bool>> board, int ticks) {
	//board [row][column]

	// vector<vector<bool>> board;
	// vector<bool> emptyRow;
	// for (int i = 0; i < width; i++) {
	// 	emptyRow.push_back(false);
	// }
	// for (int i = 0; i < height; i++) {
	// 	board.push_back(emptyRow);
	// }

	vector<vector<bool>> prevBoard;

	for (int i = 0; i < ticks; i++) {
		prevBoard = board;

		//top
		board[0][0] = gameOfLifeDecision(false, false, false, false, prevBoard[0][0], prevBoard[0][1], false, prevBoard[1][0], prevBoard[1][1]);
		for (int j = 1; j < board[0].size() - 1; j++) {
			board[0][j] = gameOfLifeDecision(false, false, false, prevBoard[0][j - 1], prevBoard[0][j], prevBoard[0][j + 1], prevBoard[1][j - 1], prevBoard[1][j], prevBoard[1][j + 1]);
		}
		board[0][board[0].size() - 1] = gameOfLifeDecision(false, false, false, prevBoard[0][board[0].size() - 2], prevBoard[0][board[0].size() - 1], false, prevBoard[1][board[0].size() - 2], prevBoard[1][board[0].size() - 1], false);

		//middle rows
		for (int k = 1; k < board.size() - 1; k++) {
			board[k][0] = gameOfLifeDecision(false, prevBoard[k - 1][0], prevBoard[k - 1][1], false, prevBoard[k][0], prevBoard[k][1], false, prevBoard[k + 1][0], prevBoard[k + 1][1]);
			for (int j = 1; j < board[0].size() - 1; j++) {
				board[0][j] = gameOfLifeDecision(prevBoard[k - 2][j - 1], prevBoard[k - 1][j], prevBoard[k - 1][j + 1], prevBoard[k][j - 1], prevBoard[k][j], prevBoard[k][j + 1], prevBoard[k + 1][j - 1], prevBoard[k + 1][j], prevBoard[k + 1][j + 1]);
			}
			board[k][board[0].size() - 1] = gameOfLifeDecision(prevBoard[k - 1][board[0].size() - 2], prevBoard[k - 1][board[0].size() - 1], false, prevBoard[k][board[0].size() - 2], prevBoard[k][board[0].size() - 1], false, prevBoard[k + 1][board[0].size() - 2], prevBoard[k + 1][board[0].size() - 1], false);
		}

		//bottom row
		for (int k = 1; k < board.size() - 1; k++) {
			board[k][0] = gameOfLifeDecision(false, prevBoard[k - 1][0], prevBoard[k - 1][1], false, prevBoard[k][0], prevBoard[k][1], false, false, false);
			for (int j = 1; j < board[0].size() - 1; j++) {
				board[0][j] = gameOfLifeDecision(prevBoard[k - 2][j - 1], prevBoard[k - 1][j], prevBoard[k - 1][j + 1], prevBoard[k][j - 1], prevBoard[k][j], prevBoard[k][j + 1], false, false, false);
			}
			board[k][board[0].size() - 1] = gameOfLifeDecision(prevBoard[k - 1][board[0].size() - 2], prevBoard[k - 1][board[0].size() - 1], false, prevBoard[k][board[0].size() - 2], prevBoard[k][board[0].size() - 1], false, false, false, false);
		}

		printGeneration2d(board);
	}
}

bool gameOfLifeDecision(bool tl, bool tm, bool tr, bool l, bool itself, bool r, bool bl, bool bm, bool br) {
	int liveNeighbours = 0;
	if (tl) {
		liveNeighbours++;
	}
	if (tm) {
		liveNeighbours++;
	}
	if (tr) {
		liveNeighbours++;
	}
	if (l) {
		liveNeighbours++;
	}
	if (r) {
		liveNeighbours++;
	}
	if (bl) {
		liveNeighbours++;
	}
	if (bm) {
		liveNeighbours++;
	}
	if (br) {
		liveNeighbours++;
	}

	if (itself && liveNeighbours < 2) {
		return false;
	}
	if (itself && liveNeighbours < 4) {
		return true;
	} else if (itself) {
		return false;
	}
	return true;
}

void printGeneration1d() {
    cout << binaryToString(currGen) << endl;
}

void printGeneration2d(vector<vector<bool>> generation) {
    //empty method stub
    //TODO: write this method with generation form a parameter
}

vector < bool > stringToBinary(string input) {
    vector < bool > output;
    for (int i = 0; i < input.length(); i++) {
        (input[i] == '.') ? output.push_back(0): output.push_back(1);
    }
    return output;
}

string binaryToString(vector < bool > input) {
    string outputString;
    for (int i = 0; i < input.size(); i++) {
        (input[i] == 0) ? outputString += ".": outputString += "0";
    }
    return outputString;
}

vector < bool > decimalToBinary(int decimal) {
    vector < bool > binary;
    while (decimal != 1) {
        binary.insert(binary.begin(), decimal % 2);
        decimal /= 2;
    }
    binary.insert(binary.begin(), 1);
    while (binary.size() < 8) {
        binary.insert(binary.begin(), 0);
    }
    return binary;
}

int binaryToDecimal(vector < bool > binary) {
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

void loadFile() {
    //TODO: check if file exists
    ifstream fi(inputFileName);
    int decimalRuleSet;
    fi >> decimalRuleSet;
    ruleSet = decimalToBinary(decimalRuleSet);
    fi >> generationsPerformed;
    string currGenString;
    fi >> currGenString;
    currGen = stringToBinary(currGenString);
    prevGen = currGen;
    fi.close();
}

void saveFile() {
    char option;
    cout << "Save results to a file? [Y/n]: ";
    cin >> option;
    if (option == 'Y' || option == 'y') {
        ofstream fr(outputFileName);
        fr << binaryToDecimal(ruleSet) << endl;
        fr << generationsPerformed << endl;
        fr << binaryToString(currGen) << endl;
        fr.close();
    }
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
