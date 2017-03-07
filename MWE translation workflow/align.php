<?php

//Input files
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