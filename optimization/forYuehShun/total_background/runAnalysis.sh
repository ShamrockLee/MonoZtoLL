#!/bin/bash
#### FRAMEWORK SANDBOX SETUP ####
# Load cmssw_setup function
#export SCRAM_ARCH=slc7_amd64_gcc700
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh
#Proxy
export X509_USER_PROXY=$3
voms-proxy-info -all
voms-proxy-info -all -file $3

echo "1 is $1"
#dirpath="/eos/user/f/fkhuzaim/"
inputfile=`echo $1|rev |cut -d '/' -f 1|rev`
root -b -q ./xAna_pre_Zpt_optimization_bkg.C++\(\"${inputfile}\",\"$2\"\)
#root -b -q ./xAna_pre_Zpt_optimization_DYincl.C++\(\"${inputfile}\",\"$2\"\)
echo "finish the code"
if [ -e "$2" ]; then
  #until xrdcp -f "$2" root://se01.grid.nchc.org.tw//dpm/grid.nchc.org.tw/home/cms/store/user/fkhuzaim/ZpT_Optimization/"$2"; do
  until xrdcp -f "$2" /eos/user/y/yuehshun/reproduction_bkg/"$2"; do
  #until xrdcp -f "$2" root://se01.grid.nchc.org.tw//dpm/grid.nchc.org.tw/home/cms/store/user/fkhuzaim/test/"$2"; do
    sleep 60
    echo "Retrying"  
  done
fi
echo "output file has been transfered"

if [ ! -e "$2" ]; then
  echo "Error: The python script failed, could not create the output file."
  
fi

