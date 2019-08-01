#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

//generate base name
string getBase_Name(string& base, int num);

//get alphabet 
string getAlpha(string& alpha, vector<string>& trans);

//get # of states
int getNumStates(int numberStates, vector<string>& trans);

//fill machine table of transitions
vector < vector<int> > fillTable(vector< vector<int>>& table, vector<string>& trans);

//build NFA/DFA
	//read machine desc line by line after first line to get transitions
	//build multidimensional vector for transitions based on states and language

//Validate NFA/DFA

//read input file
	//run each line into machine
		//if machine accepts, store the accepted string into accepted string file for machine

//create log file
	//write validity
	//write number of states
	//write the alphabet
	//write the number of accepted strings out of total strings


int main()
{
	string baseName;
	string machDesc = "";
	string accStrings = "";
	string logFile = "";

	string alphabet;
	int numStates = 0;

	for (int i = 0; i < 100; i++)
	{
		getBase_Name(baseName, i);

		//mcahine description
		machDesc = baseName + ".fa";

		//accept strings
		accStrings = baseName + ".txt";

		//log file
		logFile = baseName + ".log";

		cout << machDesc << endl;
		cout << accStrings << endl;
		cout << logFile << endl;
		cout << endl;

		//open machine description file
		ifstream machineDesc;
		machineDesc.open(machDesc);

		if (!machineDesc)
		{
			cerr << "Can't open file: " << machDesc << endl;
			cout << "Terminating..." << endl;
			break;
		}

		string line;
		getline(machineDesc, line);				//reads first line of machine description file
		vector<string> acceptStates;

		for (int i = 0; i < line.length(); i++)	//makes the line delimited by commas, to make parsing easier
		{
			if (line[i] == '{' || line[i] == '}')
				line[i] = ',';
		}

		string aState = "";

		//build vector of accept states
		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] != ',')
			{
				aState = aState + line[i];
			}
			else if (aState.length() == 0)
			{
				continue;
			}
			else
			{
				acceptStates.push_back(aState);
				aState = "";
				continue;
			}
		}

		vector<string> transitions;

		while (getline(machineDesc, line))	//build a vector of transitions
		{
			transitions.push_back(line);
		}

		//get alphabet
		alphabet = getAlpha(alphabet, transitions);

		/*cout << "Alphabet: " << alphabet << endl;
		cout << "Alphabet size: " << alphabet.length() << endl;
		cout << endl;*/

		//get number of states
		numStates = getNumStates(numStates, transitions);

		/*cout << "Number of States: " << numStates << endl;
		cout << endl;*/

		//build machine
		int possStates = 256;
		int possAlpha = 95;
		int trapState = 255;

		vector<int> states(possStates, trapState);
		vector< vector<int> > table(possAlpha, states);	//stes up table for input of transitions for each machine description

		//fill table for machine
		fillTable(table, transitions);

		/*for (const vector<int> &v : table)
		{
			for (int x : v)
			{
				cout << x << " ";
			}

			cout << endl;
		}*/

		//Validate Machine
		char eps = '`';
		int epsTrs = ((int)eps) - 32;
		string validity = "DFA";

		for (int i = 0; i < table[epsTrs].size(); i++)
		{
			if (table[epsTrs][i] != trapState)
			{
				validity = "NFA";
				break;
			}
		}

		cout << "Epsilon transition: " << eps << ", in ASCII: " << epsTrs << endl;
		cout << "This machine is a " << validity << "." << endl;

		//read input file and test strings on table, putting any accepted strings in an output file
		string inFileName = "input.txt";
		ifstream inFile;
		inFile.open(inFileName);

		string outFileName = accStrings;
		ofstream outFile;
		outFile.open(outFileName);

		if (!inFile)
			cerr << "Can't open file: " << inFileName << endl;

		string inString;
		double total = 0;
		double accepted = 0;

		while (getline(inFile, inString))
		{
			int currState = 0;

			string isAccept = "";

			for (int i = 0; i < inString.length(); i++)
			{
				char inChar = inString[i];
				int chAscii = ((int)inChar) - 32;

				currState = table[chAscii][currState];
			}

			for (int j = 0; j < acceptStates.size(); j++)
			{
				stringstream check(acceptStates.at(j));
				int checkState;
				check >> checkState;

				if (currState == checkState)
				{
					isAccept = "\" is in the language.";
					accepted++;
					outFile << inString << endl;
				}
				else
					isAccept = "\" is not in the language.";
			}
		}

		cout << "Number of accepted Strings: " << accepted << endl;
		inFile.close();
		outFile.close();

		//build and fill log file
		double totalStrings = 0;
		inFile.open(inFileName);
		while (getline(inFile, inString))
		{
			totalStrings++;
		}
		inFile.close();

		string logFileName = logFile;
		ofstream log;
		log.open(logFileName);

		log << "Valid: " << validity << endl;
		log << endl;
		log << "States: " << numStates << endl;
		log << endl;
		log << "Alphabet: " << alphabet << endl;
		log << endl;
		log << "Accepted Strings: " << accepted << " / " << totalStrings << endl;
	}

	return 0;
}

//generate base name
string getBase_Name(string& base, int num)
{
	//basename = "mxx" xx = double digit number starting at 00
	base = "m";
	if (num > -1 && num < 10)
	{
		base += "0";
		base += to_string(num);
	}
	else
	{
		base += to_string(num);
	}

	return base;
}

//get the alphabet
string getAlpha(string& alpha, vector<string>& trans)
{
	alpha = "";

	for (int i = 0; i < trans.size(); i++)
	{
		/*cout << trans.at(i) << endl;*/

		vector<string> thisTrans;

		string whole = trans.at(i);
		whole += ',';

		string startT = "";
		char input;
		string endT = "";
		string toAdd = "";

		if (whole.length() < 5)
		{
			continue;
		}

		for (int j = 0; j < whole.length(); j++)
		{
			if (whole[j] != ',')
			{
				toAdd += whole[j];
			}
			else if (toAdd.length() == 0)
				continue;
			else
			{
				thisTrans.push_back(toAdd);
				toAdd = "";
				continue;
			}
		}

		stringstream sS(thisTrans.at(0));
		int startS = 0;
		sS >> startS;

		string inP = thisTrans.at(1);
		input = inP[0];
		int inAs = ((int)input) - 32;

		stringstream gT(thisTrans.at(2));
		int goToS = 0;
		gT >> goToS;

		if (alpha.find_first_of(input) == -1)
			alpha += input;

	}
	return alpha;
}

//get # of states
int getNumStates(int numberStates, vector<string>& trans)
{
	string sT = "";
	vector<int> St;
	numberStates = 0;

	for (int i = 0; i < trans.size(); i++)
	{
		/*cout << trans.at(i) << endl;*/

		vector<string> thisTrans;

		string whole = trans.at(i);
		whole += ',';

		string startT = "";
		char input;
		string endT = "";
		string toAdd = "";

		if (whole.length() < 5)
		{
			continue;
		}

		for (int j = 0; j < whole.length(); j++)
		{
			if (whole[j] != ',')
			{
				toAdd += whole[j];
			}
			else if (toAdd.length() == 0)
				continue;
			else
			{
				thisTrans.push_back(toAdd);
				toAdd = "";
				continue;
			}
		}

		stringstream sS(thisTrans.at(0));
		int startS = 0;
		sS >> startS;

		string inP = thisTrans.at(1);
		input = inP[0];
		int inAs = ((int)input) - 32;

		stringstream gT(thisTrans.at(2));
		int goToS = 0;
		gT >> goToS;

		if (find(St.begin(), St.end(), startS) == St.end())
		{
			sT += ", ";
			sT += thisTrans.at(0);
			St.push_back(startS);
		}
		if (find(St.begin(), St.end(), goToS) == St.end())
		{
			sT += ", ";
			sT += thisTrans.at(2);
			St.push_back(goToS);
		}
		numberStates = St.size();
	}


	return numberStates;
}

//fill machine table of transitions
vector < vector<int> > fillTable(vector< vector<int> >& table, vector<string>& trans)
{
	string sT = "";
	vector<int> St;
	int numberStates = 0;
	int trapState = 255;

	for (int i = 0; i < trans.size(); i++)
	{
		/*cout << trans.at(i) << endl;*/

		vector<string> thisTrans;

		string whole = trans.at(i);
		whole += ',';

		string startT = "";
		char input;
		string endT = "";
		string toAdd = "";

		if (whole.length() < 5)
		{
			continue;
		}

		for (int j = 0; j < whole.length(); j++)
		{
			if (whole[j] != ',')
			{
				toAdd += whole[j];
			}
			else if (toAdd.length() == 0)
				continue;
			else
			{
				thisTrans.push_back(toAdd);
				toAdd = "";
				continue;
			}
		}

		stringstream sS(thisTrans.at(0));
		int startS = 0;
		sS >> startS;

		string inP = thisTrans.at(1);
		input = inP[0];
		int inAs = ((int)input) - 32;

		stringstream gT(thisTrans.at(2));
		int goToS = 0;
		gT >> goToS;

		if (table[inAs][startS] == trapState)
		{
			table[inAs][startS] = goToS;
		}
	}

	return table;
}
