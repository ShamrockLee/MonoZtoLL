# Calculate the Signal Efficiency and Obtain the Plots
The preselections used in this folder do not include the AK4jets preselctions.

## I. Apply the preselection to the signal samples
Firstly, apply the preselection in your local lxplus by running the command below. Don't forget to setup your proxy before you run the shell script.

```
voms-proxy-init --voms cms --valid 192:00 && cp -v /tmp/x509up_xxxxxxx /afs/cern.ch/user/usernameinitial/yourusername/private/x509up

. run_bkg.sh
```

## II. Produce the signal efficiency plots
To get the plots, please go to ```plots``` directory and then run the macro.

```
cd plots

root -l xPlot_signalEfficiency.C
```
