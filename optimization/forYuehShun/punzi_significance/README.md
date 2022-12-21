# Calculate and Generate Plot of Punzi Significance
To calculate and get the plots of punzi significance, make sure that you have produced these two files inside their respective folders:

1. ```../signal_efficiency/plots/signalEff.root```
2. ```../total_background/plots/allBkg_wDYincl.root```

Then, get the plots in the form of pdf and root files by running below command:
```
root -l xPlot_punzisignificance.C
```

If you find your plots are cropped due to the maximum value of the y-axis, you can set up manually the maximum value of the y-axis by your self by change:

1. the number in ```line 260``` of xPlot_punzisignificance.C for the Mphi-500_Mchi2-150_Mchi1-1 plots (brown or orange), and
2. the number in ```line 297``` of xPlot_punzisignificance.C for the Mphi-500_Mchi2-1_Mchi1-0p1 plots (green)

If you want to know the detail value of the punzi significance of each sample, you can open the output root file ```punziSig_wDYincl.root``` or make your own script to get the value you need
