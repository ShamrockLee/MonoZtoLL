# I. Settings for submitting jobs to HTCondor

A set of scripts to submit jobs to HTCondor for applying preselections to background samples for Z pT optimization


## A. Setup environment

Download this folder ```total_background```

Create output, error, and log folders

```
mkdir output error log
```

Create output folder in your CERN EOS

```
mkdir /eos/user/userinitial/username/foldername
```


## B. Changes must be done in executing files

Change the ```Proxy_path``` in ```line 3``` of submit_multi.sub file to be your proxy path.

Change the output folder ```/eos/user/f/fkhuzaim/MET_Optimization/``` in ```line 20 ``` of runAnalysis.sh file to be the folder that you just created in the CERN EOS.


## C. Submit the condor jobs

### 1. If you run for ALL BACKGROUND samples EXCEPT DY-Inclusive sample

Please uncomment ```Line 15``` and comment ```Line 16``` of runAnalysis.sh file.

Please set ```Line 5``` of run_bkg.sh to be false ```is_DY_inclusive="false"```.


### 2. If you run for ONLY DY-Inclusive sample

Please comment ```Line 15``` and uncomment ```Line 16``` of runAnalysis.sh file.

Please set ```Line 5``` of run_bkg.sh to be true ```is_DY_inclusive="true"```.


#### Running

Set your proxy before submitting analysis to HTCondor

```
voms-proxy-init --voms cms --valid 192:00 && cp -v /tmp/x509up_xxxxxxx /afs/cern.ch/user/usernameinitial/yourusername/private/x509up

. run_bkg.sh
```

Condor Jobs will be submitted with this file.

Run the following command to see the status:

```condor_q yourusername```

#### Check the Failed Jobs

If you want to ONLY check the failed jobs without resubmitting them, you can change ```line 6``` of resubmitSplittedFailedJobs.sh to be true ```is_checkFailedJobs="true"```.

Change the folder name in ```line 11``` of resubmitSplittedFailedJobs.sh file to be your ```tempSplittedSubmitFilelists_YYYY-mm-dd-HH-MM-SS``` folder name that you have submitted and you want to resubmit

Change the FIRST job Id in ```line 13``` of resubmitSplittedFailedJobs.sh file to be your FIRST job Id of ```tempSplittedSubmitFilelists_YYYY-mm-dd-HH-MM-SS```; you can check the job id in your ```logsubmit_YYYY-MM-DD-HH-MM-SS.log```

Change the LAST job Id in ```line 14``` of resubmitSplittedFailedJobs.sh file to be your LAST job Id of ```tempSplittedSubmitFilelists_YYYY-mm-dd-HH-MM-SS```; you can check the job id in your ```logsubmit_YYYY-MM-DD-HH-MM-SS.log```

Then, run the script

```
. resubmitSplittedFailedJobs.sh
```


# II. Settings for REsubmitting jobs to HTCondor

Once your jobs have finished but you find there are number of jobs that are failed, you can resubmit the jobs by following the instructions below.

## A. Changes must be done in executing files for resubmission

Change ```line 7``` of resubmitSplittedFailedJobs.sh to be false ```is_checkFailedJobs="false"```

Change the folder name in ```line 11``` of resubmitSplittedFailedJobs.sh file to be your ```tempSplittedSubmitFilelists_YYYY-mm-dd-HH-MM-SS``` folder name that you have submitted and you want to resubmit

Change the FIRST job Id in ```line 13``` of resubmitSplittedFailedJobs.sh file to be your FIRST job Id of ```tempSplittedSubmitFilelists_YYYY-mm-dd-HH-MM-SS```; you can check the job id in your ```logsubmit_YYYY-MM-DD-HH-MM-SS.log```

Change the LAST job Id in ```line 14``` of resubmitSplittedFailedJobs.sh file to be your LAST job Id of ```tempSplittedSubmitFilelists_YYYY-mm-dd-HH-MM-SS```; you can check the job id in your ```logsubmit_YYYY-MM-DD-HH-MM-SS.log```


### 1. If you run for ALL BACKGROUND samples EXCEPT DY-Inclusive sample

Please change ```line 8``` of resubmitSplittedFailedJobs.sh to be false ```is_DY_inclusive="false"```

### 2. If you run for ONLY DY-Inclusive sample

Please change ```line 8``` of resubmitSplittedFailedJobs.sh to be true ```is_DY_inclusive="true"```


## B. Resubmit the failed jobs to HTCondor

Before resubmitting the failed jobs, do not forget to set your proxy first.

```
voms-proxy-init --voms cms --valid 192:00 && cp -v /tmp/x509up_xxxxxxx /afs/cern.ch/user/usernameinitial/yourusername/private/x509up

. resubmitSplittedFailedJobs.sh
```


# III. Merge the Output Root Files
Merge the output root files using hadd. Please follow the instructions or commands below to merge the root files.

```
cd haddFiles/
```

Then, change ```line 2``` of haddBackground.sh to be the folder path you located your output in your CERN EOS.

Run for merging
```
. haddBackground.sh
```
