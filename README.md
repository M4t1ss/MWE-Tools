# Multiword-Expression-Tools
A set of useful tools for use with multiword expression extraction from parallel corpora for Moses statistical machine translation system

## Tools included
### C++ programs
* Multiword Expression XML file converter to MPFormat
  - One parameter - input XML file from MWE toolkit with sentances containing MWEs
* Phrase counter from XML
  - One parameter - input XML file from MWE toolkit with MWE frequencies
* Jaccard Index calculator
  - Three parameters - MWE file counts for target and source languages and MWE file count of source and target language pairs
  
#### Files in each tools
  * CPP source file
  * Makefile
  * Test data file(s)

#### Requirements
  * XmlInspector (included)
  * boost/bimap
  * clang++

### Shell scripts
* Converter from MP aligner format to the moses training data format
  - Three parameters - input MPFormat file and output source and target language files
* Converter from MP aligner format to the moses translation table format
  - Two parameters - input MPFormat file and output translation table file
* MWE Translation Workflow
  - A complete workflow for extracting a set of parallel multiword-expressions from parallel corpora
