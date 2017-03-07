# Workflow for obtaining a parallel set of MWEs from a parallel corpus for running in a cluster
# Required to set paths to the tools
# LV Tagger only required for Latvian

MOSES_SCRIPTS=
TREE_TAGGER=
LV_TAGGER=
MWE_TOOLKIT=
MONO=
MP_ALIGNER=
MWE_TOOLS=

# This is an example for English and Czech

# Czech
qsub -N job_cs_aa1 -cwd -b y "sed 's/$/\n<sentence>/' cspart.aa > cspart.br.aa"
qsub -N job_cs_aa2 -hold_jid job_cs_aa1 -cwd -b y "$TREE_TAGGER/cmd/tree-tagger-czech < ./cspart.br.aa > ./cspart.tt.aa"
qsub -N job_cs_aa3 -hold_jid job_cs_aa2 -cwd -b y "sed -i 's/<sentence>/.\tSENTENCE\t./g' ./cspart.tt.aa"
qsub -N job_cs_aa4 -hold_jid job_cs_aa3 -cwd -b y "python2 $MWE_TOOLKIT/bin/from_treetagger.py -s SENTENCE ./cspart.tt.aa > ./cspart.tagged.aa.xml"
qsub -N job_cs_aa5 -hold_jid job_cs_aa4 -cwd -b y "python2 $MWE_TOOLKIT/bin/candidates.py -p ~/scripts/patterns_cs.xml ./cspart.tagged.aa.xml > ./candidates.cs.aa.xml" 
qsub -N job_cs_aa6 -hold_jid job_cs_aa5 -cwd -b y "python2 $MWE_TOOLKIT/bin/annotate_mwe.py --candidates-from XML -c ./candidates.cs.aa.xml ./cspart.tagged.aa.xml > ./cspart.annotated.aa.xml" 
qsub -N job_cs_aa7 -hold_jid job_cs_aa6 -cwd -b y "$MWE_TOOLS/XMLConverter/converter ./cspart.annotated.aa.xml" 

# English
qsub -N job_en_aa1 -cwd -b y "sed 's/$/\n<sentence>/' enpart.aa > enpart.br.aa"
qsub -N job_en_aa2 -hold_jid job_en_aa1 -cwd -b y "$TREE_TAGGER/cmd/tree-tagger-czech < ./enpart.br.aa > ./enpart.tt.aa"
qsub -N job_en_aa3 -hold_jid job_en_aa2 -cwd -b y "sed -i 's/<sentence>/.\tSENTENCE\t./g' ./enpart.tt.aa"
qsub -N job_en_aa4 -hold_jid job_en_aa3 -cwd -b y "python2 $MWE_TOOLKIT/bin/from_treetagger.py -s SENTENCE ./enpart.tt.aa > ./enpart.tagged.aa.xml"
qsub -N job_en_aa5 -hold_jid job_en_aa4 -cwd -b y "python2 $MWE_TOOLKIT/bin/candidates.py -p ~/scripts/patterns_en.xml ./enpart.tagged.aa.xml > ./candidates.en.aa.xml" 
qsub -N job_en_aa6 -hold_jid job_en_aa5 -cwd -b y "python2 $MWE_TOOLKIT/bin/annotate_mwe.py --candidates-from XML -c ./candidates.en.aa.xml ./enpart.tagged.aa.xml > ./enpart.annotated.aa.xml" 
qsub -N job_en_aa7 -hold_jid job_en_aa6 -cwd -b y "$MWE_TOOLS/XMLConverter/converter ./enpart.annotated.aa.xml" 

# Run MPAligner to try to align the MWEs 
qsub -N job_encs_aa8 -hold_jid job_cs_aa7,job_en_aa7 -cwd -b y "$MONO/mono --runtime=v4.0 $MP_ALIGNER/ParallelCorpusMPAligner.exe -c $MP_ALIGNER/LinuxMPAlignerConfig.xml -si enpart.annotated.aa.xml.taas.tagged -ti cspart.annotated.aa.xml.taas.tagged -sl en -tl cs -o part.aa.MPOutput" 

# Convert MPOutput to normal phrase pairs with my tool 
qsub -N job_encs_aa9 -hold_jid job_encs_aa8 -cwd -b y "$MWE_TOOLS/MPoutput_to_training_data.sh part.aa.MPOutput enpart.aa.out cspart.aa.out"


# The final outputs should be an aligned set of parallel multiword expressions in files:
# "enpart.aa.out" and "cspart.aa.out"