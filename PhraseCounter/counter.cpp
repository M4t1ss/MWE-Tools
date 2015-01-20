#include "../XmlInspector/XmlInspector.hpp"
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
	std::cout << "Parsing multiword expressions..." << std::endl;

	std::string phrase, filename;

	// Read the xml file into a vector
	if (argc == 1){
		std::cout << "Please provide an input file name" << std::endl;
		return 0;
	}else{
		filename = argv[1];
	}
	
	Xml::Inspector<Xml::Encoding::Utf8Writer> inspector(filename);

	std::ofstream myfile;
	myfile.open(filename+".count.txt");
	myfile.clear();
	myfile.close();
	
	
	while (inspector.Inspect()){
		//if occurs starts
		if ((inspector.GetInspected() == Xml::Inspected::StartTag ||
			inspector.GetInspected() == Xml::Inspected::EmptyElementTag) &&
			inspector.GetName() == "occurs")
		{
			phrase = "";
		}

		//if word starts
		if ((inspector.GetInspected() == Xml::Inspected::StartTag ||
			inspector.GetInspected() == Xml::Inspected::EmptyElementTag) &&
			inspector.GetName() == "w")
		{
			phrase = phrase + " " + inspector.GetAttributeAt(0).Value;
		}

		//if freq
		if ((inspector.GetInspected() == Xml::Inspected::StartTag ||
			inspector.GetInspected() == Xml::Inspected::EmptyElementTag) &&
			inspector.GetName() == "freq")
		{
			// Print count
			myfile.open(filename+".count.txt", std::ios::app);
			myfile << phrase << "\t" << inspector.GetAttributeAt(1).Value << std::endl;
			myfile.close();
		}
	}
	std::cout << "All done!" << std::endl;
	return EXIT_SUCCESS;
}