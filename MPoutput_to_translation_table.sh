#!/bin/sh

# get phrase table
# Source-phrase ||| target-phrase ||| inverse-phrase-translation probability inverse-lexical-weighting direct-phrase-translation-probability direct-lexical-weighting phrase-penalty
nawk -F"\\t" '{print $2,"|||", $4, "|||", $7, "1", $7, "1 2.718"}' $0 > $1