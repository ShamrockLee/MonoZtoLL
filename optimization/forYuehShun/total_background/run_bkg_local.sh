#!/bin/bash

#inputfile="resubmit_Skimmed_2017Bkg_DY-HT_Top_Diboson_Triboson_2022-11-03-11-04-23/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/ZZTo4L_TuneCP5_13TeV_powheg_pythia8_9.txt"
inputfile="Skimmed_bkgtest.txt"

mkdir output_background

outputname="output_background/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_TEST2.root"

#while read -r line; do
    #echo "$line"
    #echo $outputname
    root -b -q ./xAna_pre_Zpt_optimization_bkg.C++\(\"${inputfile}\",\"${outputname}\"\)
    #root -b -q ./xAna_pre_Zpt_optimization_DYincl.C++\(\"${inputfile}\",\"${outputname}\"\)
#done < $inputfile    
