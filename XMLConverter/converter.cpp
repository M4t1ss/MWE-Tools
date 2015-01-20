#include "../XmlInspector/XmlInspector.hpp"
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <boost/bimap.hpp>

int main(int argc, char* argv[])
{
	std::string word, postag, lemma, expressionWord, currentExpression, currentLemma, currentPosTag, filename, OUTfilename;
	int expressionNumber, expressionWordIndex, wordCounter, sentanceCounter, expI;

	std::map<int, std::pair<std::string, std::pair<std::string, std::string> > > wordMap;// word index, word, lemma, postag
	std::multimap<int, int> multiWordExpressionMap;// word index, expression index
	std::list<int> expressions;
	bool printed = false;

	// Read the xml file into a vector
	if (argc == 1){
		std::cout << "Please specify at least the input file name as the first argument and optionally the output file name as the second argument." << std::endl;
		return 0;
	}else{
		filename = argv[1];
	}
	// Name of the output file?
	if (argc < 3)
		OUTfilename = filename + ".taas.tagged";
	else
		OUTfilename = argv[2];

	Xml::Inspector<Xml::Encoding::Utf8Writer> inspector(filename);

	std::ofstream myfile;
	myfile.open(OUTfilename);
	myfile.clear();
	myfile.close();

	while (inspector.Inspect()){
		//if sentance starts
		if ((inspector.GetInspected() == Xml::Inspected::StartTag ||
			inspector.GetInspected() == Xml::Inspected::EmptyElementTag) &&
			inspector.GetName() == "s")
		{
			Xml::Inspector<Xml::Encoding::Utf8Writer>::SizeType i;
			for (i = 0; i < inspector.GetAttributesCount(); ++i){
				if (inspector.GetAttributeAt(i).Name == "s_id"){
					//got a new sentance
					sentanceCounter = std::stoi(inspector.GetAttributeAt(i).Value);
					wordMap.clear();
					multiWordExpressionMap.clear();
					wordCounter = 1;
					expI = 0;
					expressions.clear();
					break;
				}
			}
		}

		//if word starts
		if ((inspector.GetInspected() == Xml::Inspected::StartTag ||
			inspector.GetInspected() == Xml::Inspected::EmptyElementTag) &&
			inspector.GetName() == "w")
		{
			Xml::Inspector<Xml::Encoding::Utf8Writer>::SizeType i;
			for (i = 0; i < inspector.GetAttributesCount(); ++i){
				// get word attributes
				if (inspector.GetAttributeAt(i).Name == "surface"){
					word = inspector.GetAttributeAt(i).Value;
				}
				if (inspector.GetAttributeAt(i).Name == "pos"){
					postag = inspector.GetAttributeAt(i).Value;
				}
				if (inspector.GetAttributeAt(i).Name == "lemma"){
					lemma = inspector.GetAttributeAt(i).Value;
				}
			}
			wordMap.insert(std::make_pair(wordCounter, make_pair(word, make_pair(lemma, postag))));
			wordCounter++;
		}

		//if multiword expression part starts
		if ((inspector.GetInspected() == Xml::Inspected::StartTag ||
			inspector.GetInspected() == Xml::Inspected::EmptyElementTag) &&
			inspector.GetName() == "mwepart")
		{
			Xml::Inspector<Xml::Encoding::Utf8Writer>::SizeType i;
			for (i = 0; i < inspector.GetAttributesCount(); ++i){
				// get multiword expression attributes
				if (inspector.GetAttributeAt(i).Name == "index"){
					expressionWordIndex = std::stoi(inspector.GetAttributeAt(i).Value);
					expressionWord = inspector.GetValue();

					multiWordExpressionMap.insert(std::make_pair(expI, expressionWordIndex));
					expressions.push_back(expI);
					break;
				}
			}
		}

		//if multiword expression ends
		if ((inspector.GetInspected() == Xml::Inspected::EndTag ||
			inspector.GetInspected() == Xml::Inspected::EmptyElementTag) &&
			inspector.GetName() == "mweoccur")
		{
			expI++;
		}

		//if sentance ends
		if ((inspector.GetInspected() == Xml::Inspected::EndTag ||
			inspector.GetInspected() == Xml::Inspected::EmptyElementTag) &&
			inspector.GetName() == "s")
		{
			myfile.open(OUTfilename, std::ios::app);
			expressions.unique();
			for (std::list<int>::iterator Exit = expressions.begin(); Exit != expressions.end(); ++Exit){
				myfile << sentanceCounter << "\t";
				printed = false;
				currentExpression = currentLemma = currentPosTag = "";
				//get all words for current expression
				auto got = multiWordExpressionMap.find(*Exit);
				if (got != multiWordExpressionMap.end()){
					//print sentance
					for (auto itWords = wordMap.cbegin(); itWords != wordMap.cend(); ++itWords){
						//check if word is a part of the current expression
						if (itWords->first != got->second || printed){
							myfile << itWords->second.first << " ";
						}
						else{
							//print the expression...
							printed = true;
							currentExpression = currentExpression + wordMap[got->second].first;
							currentLemma = currentLemma + wordMap[got->second].second.first;
							currentPosTag = currentPosTag + wordMap[got->second].second.second;
							while (++got != multiWordExpressionMap.cend() && got->first == *Exit){
								currentExpression = currentExpression + " " + wordMap[got->second].first;
								currentLemma = currentLemma + " " + wordMap[got->second].second.first;
								currentPosTag = currentPosTag + " " + wordMap[got->second].second.second;
								itWords++;
							}
							if (got == multiWordExpressionMap.end() || got->first != *Exit)
								got--;
							myfile << "<TENAME SCORE=\"1.0\" ";
							myfile << "MSD=\"";
							myfile << currentPosTag;
							myfile << "\" LEMMA=\"";
							myfile << currentLemma;
							myfile << "\">";
							myfile << currentExpression;
							myfile << "</TENAME> ";
						}
					}
				}
				myfile << std::endl;
			}
			myfile.close();
		}
	}

	if (inspector.GetErrorCode() != Xml::ErrorCode::None)
	{
		std::cout << "Error: " << inspector.GetErrorMessage() <<
			" At row: " << inspector.GetRow() <<
			", column: " << inspector.GetColumn() << ".\n";
	}

	return EXIT_SUCCESS;
}