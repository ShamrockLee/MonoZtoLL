#!/usr/bin/env bash

SECONDS=0

sendSpace="$1"
macroName="$2"
clusterId="$3"
inputListFile="$4"

export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

splittedSpace=ntuple_filelist_splitted.tmp
outputSpace=/eos/user/y/yuehshun/ntuple_filelist_output_2017_optimization.tmp
outputFile="$(dirname "$outputSpace${inputListFile:${#splittedSpace}}")/$(basename -s .txt "$inputListFile")_$clusterId.root"

export X509_USER_PROXY=/afs/cern.ch/user/y/yuehshun/private/x509up
echo "X509_USER_PROXY=$X509_USER_PROXY"

# # shellcheck disable=SC2046
# xrdcp -v --parallel 4 --retry 5 $(cat "$sendSpace/$inputListFile") "$infileDir/"

"./$macroName.o" ::: "$outputFile" $(cat "$sendSpace/$inputListFile")

duration="$SECONDS"
echo -e "RUN TIME: $((duration / 60)) minutes and $((duration % 60)) seconds"
echo "Done"
