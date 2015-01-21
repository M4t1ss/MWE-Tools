#!/bin/sh

# get words from second column
nawk -F"\\t" '{print $2}' $0 > $1

# get words from fourth column
nawk -F"\\t" '{print $4}' $0 > $2