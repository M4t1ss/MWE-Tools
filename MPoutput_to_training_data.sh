#!/bin/sh

# get words from second column
nawk -F"\\t" '{print $2}' $1 > $2

# get words from fourth column
nawk -F"\\t" '{print $4}' $1 > $3
