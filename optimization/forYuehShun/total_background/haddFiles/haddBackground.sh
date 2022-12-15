#!/bin/bash
inputdir="/eos/user/f/fkhuzaim/MET_Optimization/newRootFile_20221213"
inputhaddname="haddName.txt"

outputdir="/eos/user/f/fkhuzaim/MET_Optimization/merge_output"
mkdir $outputdir

arrFile=()

while read -r line; do
    unset arrFile
    for file in `ls $inputdir`; do
        samplename=`echo $file | rev | cut -d '_' -f 1-3 --complement | rev`
        if [ $samplename == "$line" ]; then
            arrFile+=("$inputdir/$file") 
        fi
    done

    #echo ""
    #echo ${arrFile[@]}
    #echo ""
    #echo $outputdir/$line.root
    
    theFiles=`echo ${arrFile[@]}`
    hadd $outputdir/$line.root $theFiles

done < $inputhaddname
