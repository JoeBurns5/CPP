#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

//generate base name
string getBase_Name(string& base, int num);

//get # of states
int getNumStates(int numberStates, vector<string>& trans);

//get input alphabet 
vector<unsigned int> getInputAlpha(vector<unsigned int>& alpha, vector<string>& trans);

//get tape alphabet 
vector<unsigned int> getTapeAlpha(vector<unsigned int>& alpha, vector<string>& trans);

int main()
{
	string baseName;
	string machDesc = "";
	string outfileName = "";

	vector <unsigned int> inAlpha;
	vector <unsigned int> tapeAlpha;

	int numStates = 0;

	for (int i = 0; i < 100; i++)
	{
		getBase_Name(baseName, i);

		//mcahine description
		machDesc = baseName + ".tm";

		//accept strings
		outfileName = baseName + ".txt";


		cout << machDesc << endl;
		cout << outfileName << endl;
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

		int acceptState = 254;
		int rejectState = 255;

		//build a vector of all transitions from file
		vector<string> transitions;

		while (getline(machineDesc, line))
		{
			transitions.push_back(line);
		}

		//get number of states
		numStates = getNumStates(numStates, transitions);

		/*cout << "Number of States: " << numStates << endl;
		cout << endl;*/

		//get tape and input alphabet
		inAlpha = getInputAlpha(inAlpha, transitions);
		tapeAlpha = getTapeAlpha(tapeAlpha, transitions);

		for (int i = 0; i < inAlpha.size(); i++)
		{
			cout << inAlpha.at(i) << " ";
		}
		cout << endl;

		for (int i = 0; i < tapeAlpha.size(); i++)
		{
			cout << tapeAlpha.at(i) << " ";
		}
		cout << endl;

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

//get # of states
int getNumStates(int numberStates, vector<string>& trans)
{
	string sT = "";
	vector<unsigned int> St;
	numberStates = 0;

	for (int i = 0; i < trans.size(); i++)
	{
		vector<string> thisTrans;

		string whole = trans.at(i);
		whole += ',';

		string fromSt = "";
		string readSym = "";
		string toSt = "";
		string writeSym = "";
		char headDir;
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
		unsigned int startS = 0;
		sS >> startS;

		stringstream inP(thisTrans.at(1));
		unsigned int readIn = 0;
		inP >> readIn;

		stringstream gT(thisTrans.at(2));
		unsigned int goToS = 0;
		gT >> goToS;

		stringstream wrT(thisTrans.at(3));
		unsigned int writeOut = 0;
		wrT >> writeOut;

		string headMov = thisTrans.at(4);
		headDir = headMov[0];


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

//get input alphabet 
vector<unsigned int> getInputAlpha(vector<unsigned int>& alpha, vector<string>& trans)
{
	string inAlpha = "";

	for (int i = 0; i < trans.size(); i++)
	{
		vector<string> thisTrans;

		string whole = trans.at(i);
		whole += ',';

		string fromSt = "";
		unsigned int readSym = 0;
		string toSt = "";
		string writeSym = "";
		char headDir;
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
		unsigned int startS = 0;
		sS >> startS;

		stringstream inP(thisTrans.at(1));
		unsigned int readIn = 0;
		inP >> readIn;

		stringstream gT(thisTrans.at(2));
		unsigned int goToS = 0;
		gT >> goToS;

		stringstream wrT(thisTrans.at(3));
		unsigned int writeOut = 0;
		wrT >> writeOut;

		string headMov = thisTrans.at(4);
		headDir = headMov[0];

		readSym = stoul(thisTrans.at(1));

		if (find(alpha.begin(), alpha.end(), readSym) == alpha.end())
		{
			inAlpha += ", ";
			inAlpha += thisTrans.at(1);
			alpha.push_back(readSym);
		}
		return alpha;

	}

	cout << inAlpha << endl;
}

//get tape alphabet 
vector<unsigned int> getTapeAlpha(vector<unsigned int>& alpha, vector<string>& trans)
{
	string tapeAlpha = "";

	for (int i = 0; i < trans.size(); i++)
	{
		vector<string> thisTrans;

		string whole = trans.at(i);
		whole += ',';

		string fromSt = "";
		string readSym = "";
		string toSt = "";
		string writeSym = "";
		char headDir;
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
		unsigned int startS = 0;
		sS >> startS;

		stringstream inP(thisTrans.at(1));
		unsigned int readIn = 0;
		inP >> readIn;

		stringstream gT(thisTrans.at(2));
		unsigned int goToS = 0;
		gT >> goToS;

		stringstream wrT(thisTrans.at(3));
		unsigned int writeOut = 0;
		wrT >> writeOut;

		string headMov = thisTrans.at(4);
		headDir = headMov[0];


		if (find(alpha.begin(), alpha.end(), writeOut) == alpha.end())
		{
			tapeAlpha += ", ";
			tapeAlpha += thisTrans.at(3);
			alpha.push_back(writeOut);
		}
		return alpha;

	}
	cout << tapeAlpha << endl;
}