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

bool initialise();
int firstGenerationSize(bool isLength);
int numberOfGenerationsToRun();
void chooseFirstGeneration(size_t size);
void chooseFirstGeneration2d(size_t size, int height);
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
bool loadFile();
void saveFile();
int getRuleIndex(bool first, bool second, bool third);
void gameOfLife(vector<vector<bool>> board, int ticks);
bool gameOfLifeDecision(bool tl, bool tm, bool tr, bool l, bool itself, bool r, bool bl, bool bm, bool br);

int main() {
    if (!initialise()) {
    	return 0;
    }

    int generations = numberOfGenerationsToRun();
  	bool wrap = false;
  	string option;

    cout << "Do you want to wrap? [Y/n]: ";
    cin >> option;
    if (cin.fail()) {
    	cin.clear();
    	option = "";
    }
    if (option == "Y" || option == "y") {
    	wrap = true;
    }

    if (is1d) {
    	printGeneration1d();
    } else {
    	printGeneration2d(currGen2d);
    }
    for (int i = 0; i < generations; i++) {
    	if (is1d) {
        	performGeneration1d(wrap);
        } else {
        	performGeneration2d(wrap);
        }
    }

    saveFile();
    return 0;
}

bool initialise() {
    string option;

    cout << "Do you want to play Conway's Game of Life? [Y/n]: ";
    cin >> option;
    if (cin.fail()) {
    	cout << "fail" << endl;
    	cin.clear();
    	option = "";
    }
    if (option == "Y" || option == "y") {
		vector<vector<bool>> board;
    	size_t size = firstGenerationSize(true);
    	int height = firstGenerationSize(false);
		string firstGeneration;
	    for (int i = 0; i < height; i++) {
		    cout << "Enter one row of generation of size " << size << ". Dots (.) represent empty values, zeros (0) represent full values (" << i+1 << "/" << height << "): ";
		    cin >> firstGeneration;
		    while (firstGeneration.find_first_not_of(".0") != string::npos || firstGeneration.length() != size || cin.fail()) {
		    	cin.clear();
		        cout << "Illegal characters encountered or wrong generation size. Try again: ";
		        cin >> firstGeneration;
			}
    		board.push_back(stringToBinary(firstGeneration));
    	}
    	gameOfLife(board, 5);
    	return false;
	}

    cout << "Load from file? [Y/n]: ";
    cin >> option;
    if (cin.fail()) {
    	cin.clear();
    	option = "";
    }
    bool fileResult = false;
    if (option == "Y" || option == "y") {
        fileResult = loadFile();
    } 
    if ((option != "Y" && option != "y") || !fileResult) {
    	cout << "Do you want generation to be 1D? [Y/n]: ";
    	cin >> option;
	    if (cin.fail()) {
	    	cin.clear();
	    	option = "";
	    }
    	if (option == "Y" || option == "y") {
    		is1d = true;
      	  chooseFirstGeneration(firstGenerationSize(true));
    	} else {
    		is1d = false;
    		int length = firstGenerationSize(true);
    		int height = firstGenerationSize(false);
    		chooseFirstGeneration2d(length, height);
    	}
    }

    if (ruleSet.size() != 8) {
        cout << "Ruleset is not defined yet. Apply a random ruleset? [Y/n]: ";
        cin >> option;
	    if (cin.fail()) {
	    	cin.clear();
	    	option = "";
	    }
        if (option == "Y" || option == "y") {
            randomRuleSet();
        } else {
            chooseRuleSet();
        }
    }

    cout << "Ruleset " << binaryToDecimal(ruleSet) << " applied" << endl;
    return true;
}

int firstGenerationSize(bool isLength) {
    int generationLength;
    cout << "Enter the " << (isLength ? "length" : "height") << " of the first generation: ";
    cin >> generationLength;
    while (generationLength <= 3 || cin.fail()) {
    	cin.clear();
        cout << "Error, input cannot be below 3. Enter a new number: ";
        cin >> generationLength;
    }
    return generationLength;
}

int numberOfGenerationsToRun() {
    int numberOfGenerations;
    cout << "Input the number of generations to run: ";
    cin >> numberOfGenerations;
    while (numberOfGenerations <= 0 || cin.fail()) {
    	cin.clear();
        cout << "Error, input cannot be below 0. Try again: ";
        cin >> numberOfGenerations;
    }
    return numberOfGenerations;
}

void chooseFirstGeneration(size_t size) {
    string firstGeneration;
    cout << "Enter first generation of size " << size << ". Dots (.) represent empty values, zeros (0) represent full values: ";
    cin >> firstGeneration;
    while (firstGeneration.find_first_not_of(".0") != string::npos || firstGeneration.length() != size || cin.fail()) {
    	cin.clear();
        cout << "Illegal characters encountered or wrong generation size. Try again: ";
        cin >> firstGeneration;
    }
    currGen = stringToBinary(firstGeneration);
    prevGen = currGen;
}

void chooseFirstGeneration2d(size_t size, int height) {
    string firstGeneration;
    for (int i = 0; i < height; i++) {
	    cout << "Enter one row of generation of size " << size << ". Dots (.) represent empty values, zeros (0) represent full values (" << i+1 << "/" << height << "): ";
	    cin >> firstGeneration;
	    while (firstGeneration.find_first_not_of(".0") != string::npos || firstGeneration.length() != size || cin.fail()) {
	    	cin.clear();
	        cout << "Illegal characters encountered or wrong generation size. Try again: ";
	        cin >> firstGeneration;
	    }
    	currGen2d.push_back(stringToBinary(firstGeneration));
	}
    prevGen2d = currGen2d;
}

void chooseRuleSet() {
    int rulesetDecimal;
    cout << "Enter a ruleset between 0 and 255: ";
    cin >> rulesetDecimal;
    if (rulesetDecimal < 0 || rulesetDecimal > 255 || cin.fail()) {
    	cin.clear();
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
    for (size_t i = 1; i < currGen.size() - 1; i++) {
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
        for (size_t i = 1; i < currGen2d[0].size() - 1; i++) {
            currGen2d[0][i] = ruleSet[getRuleIndex(prevGen2d[currGen2d.size() - 1][i - 1], prevGen2d[currGen2d.size() - 1][i], prevGen2d[currGen2d.size() - 1][i + 1])];
        }
        currGen2d[0][currGen2d[0].size() - 1] = ruleSet[getRuleIndex(prevGen2d[0][currGen2d[0].size() - 2], prevGen2d[0][currGen2d[0].size() - 1], prevGen2d[0][0])];
    } else {
        currGen2d[0][0] = ruleSet[getRuleIndex(false, false, false)];
        for (size_t i = 1; i < currGen2d[0].size() - 1; i++) {
            currGen2d[0][i] = ruleSet[getRuleIndex(false, false, false)];
        }
        currGen2d[0][currGen2d[0].size() - 1] = ruleSet[getRuleIndex(false, false, false)];
    }

    //other rows
    for (size_t i = 1; i < currGen2d.size(); i++) {
        currGen2d[i][0] = ruleSet[getRuleIndex(wrap ? prevGen2d[i - 1][currGen2d[0].size() - 1] : false, prevGen2d[i - 1][0], prevGen2d[i - 1][1])];
        for (size_t j = 1; j < currGen2d[0].size() - 1; j++) {
            currGen2d[i][j] = ruleSet[getRuleIndex(prevGen2d[i - 1][j - 1], prevGen2d[i - 1][j], prevGen2d[i - 1][j + 1])];
        }
        currGen2d[i][currGen2d[0].size() - 1] = ruleSet[getRuleIndex(prevGen2d[i - 1][currGen2d[0].size() - 2], prevGen2d[i - 1][currGen2d[0].size() - 1], wrap ? prevGen2d[i - 1][0] : false)];
    }

    prevGen2d = currGen2d;
    cout << endl;
    printGeneration2d(currGen2d);
}

void gameOfLife(vector<vector<bool>> board, int ticks) {
	vector<vector<bool>> prevBoard;
	cout << endl;
	printGeneration2d(board);

	for (int i = 0; i < ticks; i++) {
		prevBoard = board;

		//top
		board[0][0] = gameOfLifeDecision(false, false, false, false, prevBoard[0][0], prevBoard[0][1], false, prevBoard[1][0], prevBoard[1][1]);
		for (size_t j = 1; j < board[0].size() - 1; j++) {
			board[0][j] = gameOfLifeDecision(false, false, false, prevBoard[0][j - 1], prevBoard[0][j], prevBoard[0][j + 1], prevBoard[1][j - 1], prevBoard[1][j], prevBoard[1][j + 1]);
		}
		board[0][board[0].size() - 1] = gameOfLifeDecision(false, false, false, prevBoard[0][board[0].size() - 2], prevBoard[0][board[0].size() - 1], false, prevBoard[1][board[0].size() - 2], prevBoard[1][board[0].size() - 1], false);

		//middle rows
		for (size_t k = 1; k < board.size() - 1; k++) {
			board[k][0] = gameOfLifeDecision(false, prevBoard[k - 1][0], prevBoard[k - 1][1], false, prevBoard[k][0], prevBoard[k][1], false, prevBoard[k + 1][0], prevBoard[k + 1][1]);
			for (size_t j = 1; j < board[0].size() - 1; j++) {
				board[k][j] = gameOfLifeDecision(prevBoard[k - 1][j - 1], prevBoard[k - 1][j], prevBoard[k - 1][j + 1], prevBoard[k][j - 1], prevBoard[k][j], prevBoard[k][j + 1], prevBoard[k + 1][j - 1], prevBoard[k + 1][j], prevBoard[k + 1][j + 1]);
			}
			board[k][board[0].size() - 1] = gameOfLifeDecision(prevBoard[k - 1][board[0].size() - 2], prevBoard[k - 1][board[0].size() - 1], false, prevBoard[k][board[0].size() - 2], prevBoard[k][board[0].size() - 1], false, prevBoard[k + 1][board[0].size() - 2], prevBoard[k + 1][board[0].size() - 1], false);
		}

		//bottom row
		board[board.size() - 1][0] = gameOfLifeDecision(false, prevBoard[board.size() - 1 - 1][0], prevBoard[board.size() - 1 - 1][1], false, prevBoard[board.size() - 1][0], prevBoard[board.size() - 1][1], false, false, false);
		for (size_t j = 1; j < board[0].size() - 1; j++) {
			board[board.size() - 1][j] = gameOfLifeDecision(prevBoard[board.size() - 1 - 1][j - 1], prevBoard[board.size() - 1 - 1][j], prevBoard[board.size() - 1 - 1][j + 1], prevBoard[board.size() - 1][j - 1], prevBoard[board.size() - 1][j], prevBoard[board.size() - 1][j + 1], false, false, false);
		}
		board[board.size() - 1][board[0].size() - 1] = gameOfLifeDecision(prevBoard[board.size() - 1 - 1][board[0].size() - 2], prevBoard[board.size() - 1 - 1][board[0].size() - 1], false, prevBoard[board.size() - 1][board[0].size() - 2], prevBoard[board.size() - 1][board[0].size() - 1], false, false, false, false);

		cout << endl;
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
	if (liveNeighbours == 3) {
		return true;
	}
	return false;
}

void printGeneration1d() {
    cout << binaryToString(currGen) << endl;
}

void printGeneration2d(vector<vector<bool>> generation) {
	for (size_t i = 0; i < generation.size(); i++) {
    	cout << binaryToString(generation[i]) << endl;
	}
}

vector < bool > stringToBinary(string input) {
    vector < bool > output;
    for (size_t i = 0; i < input.length(); i++) {
        (input[i] == '.') ? output.push_back(0): output.push_back(1);
    }
    return output;
}

string binaryToString(vector < bool > input) {
    string outputString;
    for (size_t i = 0; i < input.size(); i++) {
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

bool loadFile() {
    ifstream fi(inputFileName);
    if (!fi.is_open()) {
    	cout << "File reading is unsuccessful. " << endl;
    	return false;
    }
    int decimalRuleSet;
    fi >> is1d;
    fi >> decimalRuleSet;
    ruleSet = decimalToBinary(decimalRuleSet);
    fi >> generationsPerformed;
    string currGenString;
    fi >> currGenString;
    currGen = stringToBinary(currGenString);
    prevGen = currGen;
    fi.close();
    return true;
}

void saveFile() {
    string option;
    cout << "Save results to a file? [Y/n]: ";
    cin >> option;
    if (cin.fail()) {
    	cin.clear();
    	option = "";
    }
    if (option == "Y" || option == "y") {
        ofstream fr(outputFileName);
        fr << is1d << endl;
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
