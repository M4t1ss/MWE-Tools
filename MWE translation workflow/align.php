<?php

//	Usage:
//	1) Source and target '.taas.tagged' files that are produced by XMLConverter/converter from MWE Tools
//	2) Remove '<TENAME>' and '</TENAME>' xml tags and everything that's between them (sed can be used)
//	3) Filter out duplicate sentences so that only uniqe ones are left
//	4) Feed the files with uniqe source and target sentences (where each line starts with the sentence ID) as inputs for this script

$source 	= $argv[1];
$target		= $argv[2];

$inSRC = fopen($source, "r") or die("Can't open input file!");
$inTRG = fopen($target, "r") or die("Can't open input file!");
$outSRC = fopen($source.".out.txt", "a") or die("Can't create output file!");
$outTRG = fopen($target.".out.txt", "a") or die("Can't create output file!");

if ($inSRC && $inTRG) {
    while (($sourceSentence = fgets($inSRC)) !== false) {
		$sourceParts = explode("\t",$sourceSentence);
		$sources[$sourceParts[0]] = $sourceParts[1];
	}
    while (($targetSentence = fgets($inTRG)) !== false) {
		$targetParts = explode("\t",$targetSentence);
		$targets[$targetParts[0]] = $targetParts[1];
	}
	
	foreach($sources as $srcKey => $srcSent){
		if(isset($targets[$srcKey]) && count($targets[$srcKey]) > 0){
			fwrite($outSRC, $sources[$srcKey]);
			fwrite($outTRG, $targets[$srcKey]);
		}
	}
}

fclose($outSRC);
fclose($outTRG);