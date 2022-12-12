#!/bin/bash

inputfile="Skimmed_2017SignalList.txt"

mkdir output_signal_root

#nline=`awk 'END { print NR }' $inputfile`
#for line in `cat $inputfile`; do
#    inputname=`echo $line | cut -d ' ' -f 1`
#    outputname=`echo $line | cut -d ' ' -f 2`
#    echo $inputname $outputname
    #root -b -q ./xAna_Zpt_optimization3.C++\(\"${inputname}\",\"${outputname}\"\)
#done

while read -r line; do
    #echo -e "$line\n"
    inputname=`echo $line | cut -d ' ' -f 1`
    outputname=`echo $line | cut -d ' ' -f 2`
    echo $inputname
    echo $outputname
    root -b -q ./xAna_Zpt_optimization_signal.C++\(\"${inputname}\",\"${outputname}\"\)
done < $inputfile    
