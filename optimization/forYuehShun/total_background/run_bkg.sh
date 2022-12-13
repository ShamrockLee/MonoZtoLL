#!/bin/bash

dateAndtime=`date +'%Y-%m-%d-%H-%M-%S'`
linesPerFile=4
is_DY_inclusive="false"

inputdir="SkimmedFiles_2017Background_Lists"
if [ $is_DY_inclusive == "true" ]; then
    inputdir="SkimmedFiles_2017Background_DY-Inclusive"
fi


splited_listdir="tempSplittedSubmitFilelists_${dateAndtime}"
mkdir -p $splited_listdir

txtfile="listOf_${splited_listdir}.txt"
#txtfile="Skimmed_bkgtest.txt" 
touch $txtfile

touch logsubmit_${dateAndtime}.log

#nline=`awk 'END { print NR }' $inputfile`
#for line in `cat $inputfile`; do
#    inputname=`echo $line | cut -d ' ' -f 1`
#    outputname=`echo $line | cut -d ' ' -f 2`
#    echo $inputname $outputname
    #root -b -q ./xAna_Zpt_optimization3.C++\(\"${inputname}\",\"${outputname}\"\)
#done 

for file in `ls $inputdir`; do
    Nmax=`wc -l $inputdir/$file | cut -d ' ' -f 1`
    #echo $Nmax
    c1=1  # this will be used as line counter
    c2=$linesPerFile
    i=0
    prefix=`echo $file | cut -d '.' -f 1`
    #echo "adding new directory $splited_listdir/$prefix"
    mkdir -p $splited_listdir/$prefix
    #echo "start to do the split"
    while [ $c1 -le $Nmax ];do
        if [ $c2 -gt $Nmax ];then
            c2=$Nmax
        fi
        newFlist="${prefix}_${i}" 
        sed -n "${c1},${c2}p" $inputdir/$file > $splited_listdir/$prefix/${newFlist}.txt
        (( i = i + 1 ))
        (( c1 = c1 + $linesPerFile ))
        (( c2 = c2 + $linesPerFile ))
    done
    ls $splited_listdir/$prefix/* >> $txtfile
done


while read -r line; do
    #inputsample=`echo $line | cut -d ' ' -f 1`
    inputsample=$line
    #outputname=`echo $line | cut -d ' ' -f 2`
    outputsample=`echo $line | cut -d '/' -f 3`
    outputname=`echo $outputsample | cut -d '.' -f 1`
    echo " "
    echo $inputsample
    echo $outputname
    cp submit_multi.sub submit_multi_temp.sub
    sed -i "/listFile = /c listFile = ${inputsample}" submit_multi_temp.sub
    sed -i "/outputname = /c outputname = ${outputname}" submit_multi_temp.sub
    if [ $is_DY_inclusive == "true" ]; then
        sed -i '/transfer_input_files = /c transfer_input_files = runAnalysis.sh, xAna_pre_Zpt_optimization_DYincl.C, $(listFile), dummy.txt, untuplizer.h' submit_multi_temp.sub
    fi
    condor_submit submit_multi_temp.sub >> logsubmit_${dateAndtime}.log
    #root -b -q ./xAna_pre_Zpt_optimization_bkg.C++\(\"${inputsample}\",\"${outputname}\"\)
done < $txtfile
