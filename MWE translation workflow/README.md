# Multiword-Expression Workflow
A workflow for extracting a parallel corpus of MWEs from parallel sentences.

### Included Scripts
  * Script for obtaining a parallel set of MWEs from a parallel corpus
  * Script for obtaining a parallel set of MWEs from a parallel corpus for running in a cluster
  * Script for obtaining a set of parallel sentences that include at least one MWE from a parallel corpus

#### Required Tools
  * [Moses scripts](https://github.com/moses-smt/mosesdecoder/tree/master/scripts)
  * [Tree Tagger](http://www.cis.uni-muenchen.de/~schmid/tools/TreeTagger/) (and for Latvian - [LVTagger](https://github.com/PeterisP/LVTagger))
  * [MWE Toolkit](mwetoolkit.sourceforge.net)
  * [Mono](http://www.mono-project.com/)
  * [MPAligner](https://github.com/pmarcis/mp-aligner)
  * Tools from the root directory of [MWE-Tools](https://github.com/M4t1ss/MWE-Tools)

#### Required Resources
  * A set of parallel sentences (many parallel corpora available in [OPUS](http://opus.lingfil.uu.se/))
  * MWE [patterns](http://mwetoolkit.sourceforge.net/PHITE.php?sitesig=MWE&page=MWE_020_Tutorials&subpage=MWE_030_Defining_Patterns) for each language
