# Settings for submitting jobs to HTCondor. 

A set of scripts to submit jobs to HTCondor for applying preselections to background samples for Z pT optimization


### Setup environment

Download this folder ```total_background```

Create output, error, and log folders

```
mkdir output error log
```

Create output folder in your CERN EOS

```
mkdir /eos/user/userinitial/username/foldername
```


### Changes to be done in executing files.

Change the ```Proxy_path``` in ```line 3``` of submit_multi.sub file to be your proxy path.

Change the output folder ```/eos/user/f/fkhuzaim/MET_Optimization/``` in ```line 20 ``` of runAnalysis.sh file to be the folder that you just created in the CERN EOS.


### Submit the condor jobs

#### If you run for ALL BACKGROUND samples EXCEPT DY-Inclusive sample

Please uncomment ```Line 15``` and comment ```Line 16``` of runAnalysis.sh file.

Please set ```Line 5``` of run_bkg.sh to be false ```is_DY_inclusive="false"```.


#### Running

Set your proxy before running by executing and then submit to condor jobs

```
voms-proxy-init --voms cms --valid 192:00 && cp -v /tmp/x509up_xxxxxxx /afs/cern.ch/user/usernameinitial/yourusername/private/x509up

. run_bkg.sh
```

Condor Jobs will be submitted with this file.

Run the following command to see the status:

```condor_q yourusername```




# Settings for REsubmitting jobs to HTCondor.

Once your jobs have finished but you find there are number of jobs that are failed, you can resubmit the jobs by following the instructions below.

### Changes to be done in executing files for resubmission

Change the folder name in ```line 7``` of ```resubmitSplittedFailedJobs.sh``` file to be your ```tempSplittedSubmitFilelists_YYYY-mm-dd-HH-MM-SS``` folder name that you have submitted and you want to resubmit

Change the FIRST job Id in ```line 15``` of ```resubmitSplittedFailedJobs.sh``` file to be your FIRST job Id of ```tempSplittedSubmitFilelists_YYYY-mm-dd-HH-MM-SS```; you can check the job id in your ```logsubmit.txt```

Change the LAST job Id in ```line 16``` of ```resubmitSplittedFailedJobs.sh``` file to be your LAST job Id of ```tempSplittedSubmitFilelists_YYYY-mm-dd-HH-MM-SS```; you can check the job id in your ```logsubmit.txt```


### Resubmit the failed jobs to HTCondor

Run

```

voms-proxy-init --voms cms --valid 192:00 && cp -v /tmp/x509up_xxxxxxx /afs/cern.ch/user/usernameinitial/yourusername/private/x509up

. resubmitSplittedFailedJobs.sh
```
