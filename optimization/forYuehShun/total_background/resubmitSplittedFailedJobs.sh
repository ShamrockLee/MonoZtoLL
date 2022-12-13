#!/bin/sh
#resubmit Failed jobs where the input files have been splitted into only 5 files/job

dateAndtime=`date +'%Y-%m-%d-%H-%M-%S'`
stringSearch="written"
 
splited_listdir="tempSplittedSubmitFilelists_2022-12-12-11-30-13" #change this to be the folder of input files that you have submitted and you want to resubmit

resubmittxt="resubmit_tempSplitted_2017Bkg_DY-Inclusive_${dateAndtime}.txt"
touch $resubmittxt

logfile="logresubmit_tempSplitted_2017Bkg_DY-Inclusive_${dateAndtime}.log"
touch $logfile

firstjobId="2659423" #change this to be the FIRST job Id of your oldSubmitDir; you can check your logsubmit.txt
lastjobId="2659550"  #change this to be the LAST job Id of your oldSubmitDir; you can check your logsubmit.txt

nFail=0

#for file in `ls error`; do
for file in `ls output`; do
    if ! grep -q $stringSearch output/$file; then
    #if grep -q $stringSearch error/$file; then
        jobid=`echo $file | cut -d '.' -f 2-3`
        jobcluster=`echo $file | cut -d '.' -f 2`
        if [[ "$jobcluster" -ge $firstjobId && "$jobcluster" -le $lastjobId ]]; then
            #submitdate=`echo $oldSubmitDir | cut -d '_' -f 2 | cut -d '-' -f 1-3`
            #if grep $submitdate log/condor.$jobid.log; then
                (( nFail = nFail + 1 ))
                failedTxt=`grep -hnr "inputtxtFilename" output/condor.$jobid.out | rev | cut -d ' ' -f 1 | rev`
                echo " "
                echo $file $failedTxt
                prefix=`echo $failedTxt | rev | cut -d '_' -f 1 --complement | rev`
                #echo $prefix
                echo $splited_listdir/$prefix/$failedTxt >> $resubmittxt
            #fi
        fi
    fi
done

echo " "
echo "nFail: $nFail"

while read -r line; do
    #inputsample=$line
    outputsample=`echo $line | cut -d '/' -f 3`
    outputname=`echo $outputsample | cut -d '.' -f 1`
    echo " "
    echo $line
    echo $outputname
    cp submit_multi.sub submit_multi_temp.sub
    sed -i "/listFile = /c listFile = ${line}" submit_multi_temp.sub
    sed -i "/outputname = /c outputname = ${outputname}" submit_multi_temp.sub
    condor_submit submit_multi_temp.sub >> $logfile
    #root -b -q ./xAna_Zpt_optimization_bkg2.C++\(\"${inputsample}\",\"${outputname}\"\)
done < $resubmittxt
