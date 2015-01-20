#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE 1
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

using namespace std;


int main(int argc, char* argv[])
{
	std::cout << "Counting multiword expressions..." << std::endl;
	std::string phrase, filename1, filename2, filename3;

	if (argc < 4){
		std::cout << "Please provide an input file names" << std::endl;
		return 0;
	}
	else{
		filename1 = argv[1];
		filename2 = argv[2];
		filename3 = argv[3];
	}


	ofstream myOutFile;
	myOutFile.open(filename1 + ".JCount.txt");
	myOutFile.clear();
	myOutFile.close();


	ifstream mwfile(filename1);
	ifstream sfile(filename2);
	ifstream tfile(filename3);


	std::string line, sline, tline;
	while (std::getline(mwfile, line))
	{
		std::istringstream iss(line);
		int prhaseCount;
		if (!(iss >> prhaseCount)) { continue; } // error

		string myTempString = iss.str().c_str();
		if (myTempString.find("\t") == string::npos) continue;
		char *cstr = new char[myTempString.length() + 1];
		strcpy(cstr, myTempString.c_str());

		char *p = strtok(cstr, "\t");
		std::string sourceString(p);
		sourceString = sourceString.erase(0, sourceString.find_first_not_of(' '));
		sourceString = sourceString.erase(sourceString.find_last_not_of(' ') + 1);

		int i = 0;
		while (isblank(sourceString[i])) i++;
		while (!isblank(sourceString[i])) i++;
		while (isblank(sourceString[i])) i++;
		if(i<=sourceString.length())sourceString = sourceString.substr(i);

		//cout << sourceString << " - ";

		//cout << p << endl;
		p = strtok(NULL, "\t");
		std::string translatedString(p);
		translatedString = translatedString.erase(0, translatedString.find_first_not_of(' '));
		translatedString = translatedString.erase(translatedString.find_last_not_of(' ') + 1);
		//cout << translatedString << " = " << prhaseCount << endl;

		//We have the sourceString, translatedString and prhaseCount. 
		//Now we need to find counts of each individual string  - sourceCount and translationCount

		//get the source count
		int sourceCount;
		while (getline(sfile, sline)) {
			if (sline.find(sourceString + "\t", 0) != string::npos) {
				//cout << "found: " << sourceString << " on line: " << sline << endl;
				std::istringstream iss_s(sline);
				string sTempString = iss_s.str().c_str();
				char *scstr = new char[sTempString.length() + 1];
				strcpy(scstr, sTempString.c_str());
				char *sp = strtok(scstr, "\t");
				sp = strtok(NULL, "\t");
				std::string sourceStringLine(sp);
				sourceCount = stoi(sourceStringLine);
				//cout << sourceCount << endl;
			}
		}
		sfile.clear();
		sfile.seekg(0, ios::beg);

		//get the translation count
		int translationCount;
		while (getline(tfile, tline)) {
			if (tline.find(translatedString + "\t", 0) != string::npos) {
				//cout << "found: " << translatedString << " on line: " << tline << endl;
				std::istringstream iss_s(tline);
				string tTempString = iss_s.str().c_str();
				char *tcstr = new char[tTempString.length() + 1];
				strcpy(tcstr, tTempString.c_str());
				char *tp = strtok(tcstr, "\t");
				tp = strtok(NULL, "\t");
				std::string translationStringLine(tp);
				translationCount = stoi(translationStringLine);
				//cout << translationCount << endl << endl;
			}
		}
		tfile.clear();
		tfile.seekg(0, ios::beg);

		//We have the sourceString, translatedString, prhaseCount, sourceCount and translationCount
		//Calculate the Jaccard Index
		float JaccardIndex;
		JaccardIndex = (float)prhaseCount / ((float)sourceCount + (float)translationCount - (float)prhaseCount);

		myOutFile.open(filename1 + ".JCount.txt", std::ios::app);
		myOutFile.precision(6);
		myOutFile << sourceString << " ||| " << translatedString << " ||| " << fixed << JaccardIndex << " 1 " << fixed << JaccardIndex << " 1 2.718" << endl;
		myOutFile.close();
	}

	std::cout << "All done!" << std::endl;
	return EXIT_SUCCESS;
}