#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>
#include <TH1D.h>
#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TMultiGraph.h>
#include <TAxis.h>
#include <math.h>
#include <string>
using namespace std;

void efferr(float nsig, float ntotal, float factor = 1)
{
    float eff = nsig / ntotal;
    float err = sqrt((1 - eff) * eff / ntotal);
    cout << "efficiency = " << eff * factor << " +- " << err * factor << endl;
}

float mean_value(const vector<float> &ttmpvector)
{
    float sum_value = 0.0;
    for (float x : ttmpvector)
    {
        sum_value += x;
    }
    return (sum_value / ttmpvector.size());
}


void xPlot_signalEfficiency(string inputtxtFilename = "inputListSignal.txt", string outputtxtFilename = "signalEff.root")//, TString outputfile)
{

    Double_t lumi = 41500.0; // integrated luminosity; unit: pb^{-1}
    Double_t XS_DYincl = 6077.22; // unit: pb
    Double_t XS_DY1Jets = 1112.14; // unit: pb
    Double_t XS_DY2Jets = 344.987; // unit: pb
    Double_t XS_DY3Jets = 102.468; // unit: pb
    Double_t XS_DY4Jets = 48.8496; // unit: pb

    // For Initial
    TH1D* h_total_mcweight = new TH1D("h_total_mcweight", "", 5, 0, 5);
    h_total_mcweight->Sumw2();

    TH1D* h_before_Zpt_signal = new TH1D("h_before_Zpt_signal", "", 5, 0, 5);
    h_before_Zpt_signal->Sumw2();

    TH1D* h_after_Zpt50_signal = new TH1D("h_after_Zpt50_signal", "", 5, 0, 5);
    h_after_Zpt50_signal->Sumw2();

    TH1D* h_after_Zpt60_signal = new TH1D("h_after_Zpt60_signal", "", 5, 0, 5);
    h_after_Zpt60_signal->Sumw2();

    TH1D* h_after_Zpt100_signal = new TH1D("h_after_Zpt100_signal", "", 5, 0, 5);
    h_after_Zpt100_signal->Sumw2(); 

    TH1D* h_after_Zpt_scan_signal = new TH1D("h_after_Zpt_scan_signal", "", 100, 0, 1000);
    h_after_Zpt_scan_signal->Sumw2();

    //histograms for signal efficiency
    TH1D* h_eff_mchi2_150_ctau0p1_ZptOnly_Scan = new TH1D("h_eff_mchi2_150_ctau0p1_ZptOnly_Scan", "", 100, 0, 1000);
    h_eff_mchi2_150_ctau0p1_ZptOnly_Scan->Sumw2();

    TH1D* h_eff_mchi2_150_ctau1p0_ZptOnly_Scan = new TH1D("h_eff_mchi2_150_ctau1p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_eff_mchi2_150_ctau1p0_ZptOnly_Scan->Sumw2();

    TH1D* h_eff_mchi2_150_ctau10p0_ZptOnly_Scan = new TH1D("h_eff_mchi2_150_ctau10p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_eff_mchi2_150_ctau10p0_ZptOnly_Scan->Sumw2();

    TH1D* h_eff_mchi2_150_ctau100p0_ZptOnly_Scan = new TH1D("h_eff_mchi2_150_ctau100p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_eff_mchi2_150_ctau100p0_ZptOnly_Scan->Sumw2();

    TH1D* h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan = new TH1D("h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan->Sumw2();

    TH1D* h_eff_mchi2_1_ctau0p1_ZptOnly_Scan = new TH1D("h_eff_mchi2_1_ctau0p1_ZptOnly_Scan", "", 100, 0, 1000);
    h_eff_mchi2_1_ctau0p1_ZptOnly_Scan->Sumw2();

    TH1D* h_eff_mchi2_1_ctau1p0_ZptOnly_Scan = new TH1D("h_eff_mchi2_1_ctau1p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_eff_mchi2_1_ctau1p0_ZptOnly_Scan->Sumw2();

    TH1D* h_eff_mchi2_1_ctau10p0_ZptOnly_Scan = new TH1D("h_eff_mchi2_1_ctau10p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_eff_mchi2_1_ctau10p0_ZptOnly_Scan->Sumw2();

    TH1D* h_eff_mchi2_1_ctau100p0_ZptOnly_Scan = new TH1D("h_eff_mchi2_1_ctau100p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_eff_mchi2_1_ctau100p0_ZptOnly_Scan->Sumw2();

    TH1D* h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan = new TH1D("h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan->Sumw2();


    Double_t eff_mchi2_150_ctau0p1_ZptOnly[3];
    Double_t eff_mchi2_150_ctau1p0_ZptOnly[3];
    Double_t eff_mchi2_150_ctau10p0_ZptOnly[3];
    Double_t eff_mchi2_150_ctau100p0_ZptOnly[3];
    Double_t eff_mchi2_150_ctau1000p0_ZptOnly[3];
    Double_t eff_mchi2_1_ctau0p1_ZptOnly[3];
    Double_t eff_mchi2_1_ctau1p0_ZptOnly[3];
    Double_t eff_mchi2_1_ctau10p0_ZptOnly[3];
    Double_t eff_mchi2_1_ctau100p0_ZptOnly[3];
    Double_t eff_mchi2_1_ctau1000p0_ZptOnly[3];

    Double_t eff_mchi2_150_ctau0p1_Total[3];
    Double_t eff_mchi2_150_ctau1p0_Total[3];
    Double_t eff_mchi2_150_ctau10p0_Total[3];
    Double_t eff_mchi2_150_ctau100p0_Total[3];
    Double_t eff_mchi2_150_ctau1000p0_Total[3];
    Double_t eff_mchi2_1_ctau0p1_Total[3];
    Double_t eff_mchi2_1_ctau1p0_Total[3];
    Double_t eff_mchi2_1_ctau10p0_Total[3];
    Double_t eff_mchi2_1_ctau100p0_Total[3];
    Double_t eff_mchi2_1_ctau1000p0_Total[3];

    Double_t eff_mchi2_150_ctau0p1_ZptOnly_Scan[100];
    Double_t eff_mchi2_150_ctau1p0_ZptOnly_Scan[100];
    Double_t eff_mchi2_150_ctau10p0_ZptOnly_Scan[100];
    Double_t eff_mchi2_150_ctau100p0_ZptOnly_Scan[100];
    Double_t eff_mchi2_150_ctau1000p0_ZptOnly_Scan[100];
    Double_t eff_mchi2_1_ctau0p1_ZptOnly_Scan[100];
    Double_t eff_mchi2_1_ctau1p0_ZptOnly_Scan[100];
    Double_t eff_mchi2_1_ctau10p0_ZptOnly_Scan[100];
    Double_t eff_mchi2_1_ctau100p0_ZptOnly_Scan[100];
    Double_t eff_mchi2_1_ctau1000p0_ZptOnly_Scan[100];

    cout << "inputtxtFilename = " << inputtxtFilename << endl;
    ifstream flist(inputtxtFilename.data());
    string inputFile;
    while (getline(flist, inputFile))
    {
        h_total_mcweight->Reset();
        h_before_Zpt_signal->Reset();
        h_after_Zpt50_signal->Reset();
        h_after_Zpt60_signal->Reset();
        h_after_Zpt100_signal->Reset();
        h_after_Zpt_scan_signal->Reset();
        
        
        TString myFile = inputFile;
        cout << "\n" << myFile << endl;

        TFile* file = TFile::Open(myFile);
        //cout << "successfully opened" << endl;

        //TH1F* h_totevent = static_cast<TH1F*>(file->Get("Event_Variable/h_totevent"));
        h_total_mcweight = static_cast<TH1D*>(file->Get("h_total_mcweight"));
        h_before_Zpt_signal = static_cast<TH1D*>(file->Get("h_before_Zpt_signal"));
        h_after_Zpt50_signal = static_cast<TH1D*>(file->Get("h_after_Zpt50_signal"));
        h_after_Zpt60_signal = static_cast<TH1D*>(file->Get("h_after_Zpt60_signal"));
        h_after_Zpt100_signal = static_cast<TH1D*>(file->Get("h_after_Zpt100_signal"));
        h_after_Zpt_scan_signal = static_cast<TH1D*>(file->Get("h_after_Zpt_scan_signal"));

        Double_t nTotalBeforePreselection = h_total_mcweight->Integral();
        Double_t nBeforeZpt = h_before_Zpt_signal->Integral();
        Double_t nAfterZpt50 = h_after_Zpt50_signal->Integral();
        Double_t nAfterZpt60 = h_after_Zpt60_signal->Integral();
        Double_t nAfterZpt100 = h_after_Zpt100_signal->Integral();


        //Calculate signal efficiency
        Double_t effZpt50_ZptOnly = nAfterZpt50/nBeforeZpt;
        cout << "effZpt50_ZptOnly: " << effZpt50_ZptOnly << endl;
        Double_t effZpt60_ZptOnly = nAfterZpt60/nBeforeZpt;
        cout << "effZpt60_ZptOnly: " << effZpt60_ZptOnly << endl;
        Double_t effZpt100_ZptOnly = nAfterZpt100/nBeforeZpt;
        cout << "effZpt100_ZptOnly: " << effZpt100_ZptOnly << endl;
        Double_t effZpt50_Total = nAfterZpt50/nTotalBeforePreselection;
        cout << "effZpt50_Total: " << effZpt50_Total << endl;
        Double_t effZpt60_Total = nAfterZpt60/nTotalBeforePreselection;
        cout << "effZpt60_Total: " << effZpt60_Total << endl;
        Double_t effZpt100_Total = nAfterZpt100/nTotalBeforePreselection;
        cout << "effZpt60_Total: " << effZpt60_Total << endl;

        //h_effZptScan_ZptOnly = (TH1D*)h_after_Zpt_scan_signal->Clone("h_effZptScan_ZptOnly");
        //h_effZptScan_ZptOnly->Scale(1/nBeforeZpt);

        Double_t nAfterZptCut;
        Double_t effZptCut_ZptOnly;
        for (int i = 1; i <= 100; i++)
        {
            nAfterZptCut = h_after_Zpt_scan_signal->GetBinContent(i+1);
            effZptCut_ZptOnly = nAfterZptCut/nBeforeZpt;

            if (inputFile.find("Mchi2-150_Mchi1-1_ctau0p1") != string::npos)
            {
                cout << "Processing " << inputFile << endl;

                eff_mchi2_150_ctau0p1_ZptOnly_Scan[i-1] = effZptCut_ZptOnly;
            }

            if (inputFile.find("Mchi2-150_Mchi1-1_ctau1000p0") != string::npos)
            {
                cout << "Processing " << inputFile << endl;

                eff_mchi2_150_ctau1000p0_ZptOnly_Scan[i-1] = effZptCut_ZptOnly;
            }
            
            if (inputFile.find("Mchi2-150_Mchi1-1_ctau100p0") != string::npos)
            {
                cout << "Processing " << inputFile << endl;
                
                eff_mchi2_150_ctau100p0_ZptOnly_Scan[i-1] = effZptCut_ZptOnly;
            }

            if (inputFile.find("Mchi2-150_Mchi1-1_ctau10p0") != string::npos)
            {
                cout << "Processing " << inputFile << endl;
                
                eff_mchi2_150_ctau10p0_ZptOnly_Scan[i-1] = effZptCut_ZptOnly;
            }

            if (inputFile.find("Mchi2-150_Mchi1-1_ctau1p0") != string::npos)
            {
                cout << "Processing " << inputFile << endl;
                
                eff_mchi2_150_ctau1p0_ZptOnly_Scan[i-1] = effZptCut_ZptOnly;
            }

            if (inputFile.find("Mchi2-1_Mchi1-0p1_ctau0p1") != string::npos)
            {
                cout << "Processing " << inputFile << endl;

                eff_mchi2_1_ctau0p1_ZptOnly_Scan[i-1] = effZptCut_ZptOnly;              
            }

            if (inputFile.find("Mchi2-1_Mchi1-0p1_ctau1000p0") != string::npos)
            {
                cout << "Processing " << inputFile << endl;

                eff_mchi2_1_ctau1000p0_ZptOnly_Scan[i-1] = effZptCut_ZptOnly;
            }

            if (inputFile.find("Mchi2-1_Mchi1-0p1_ctau100p0") != string::npos)
            {
                cout << "Processing " << inputFile << endl;

                eff_mchi2_1_ctau100p0_ZptOnly_Scan[i-1] = effZptCut_ZptOnly;
            }

            if (inputFile.find("Mchi2-1_Mchi1-0p1_ctau10p0") != string::npos)
            {
                cout << "Processing " << inputFile << endl;

                eff_mchi2_1_ctau10p0_ZptOnly_Scan[i-1] = effZptCut_ZptOnly;
            }

            if (inputFile.find("Mchi2-1_Mchi1-0p1_ctau1p0") != string::npos)
            {
                cout << "Processing " << inputFile << endl;

                eff_mchi2_1_ctau1p0_ZptOnly_Scan[i-1] = effZptCut_ZptOnly;
            }
        }

        //get the numerator and kinematics
        /*for (int evt = 0; evt < Ttree->GetEntries(); evt++)
        {
            Ttree->GetEntry(evt);

            Int_t mcWeight = I_weight;
            Double_t eventWeight = mcWeight;
            float* MET = v_met->data();

            if (I_nThinJets < 2)
                continue;
            //if (*MET < 140.0)
            //    continue;
            h_Numerator->Fill(1, eventWeight);
            for (int ij = 0; ij < I_nThinJets; ij++)
            {
                h_ZbosonPt->Fill(f_ZbosonPt, eventWeight);
                h_ZbosonEta->Fill(f_ZbosonEta, eventWeight);
                h_jetPt->Fill((*v_passJetPt)[ij], eventWeight);
                h_jetEta->Fill((*v_passJetEta)[ij], eventWeight);
            }
        } //end of getEntries*/

        if (inputFile.find("Mchi2-150_Mchi1-1_ctau0p1") != string::npos)
        {
            cout << "Processing " << inputFile << endl;

            eff_mchi2_150_ctau0p1_ZptOnly[0] = effZpt50_ZptOnly;
            eff_mchi2_150_ctau0p1_ZptOnly[1] = effZpt60_ZptOnly;
            eff_mchi2_150_ctau0p1_ZptOnly[2] = effZpt100_ZptOnly;
            //declare this new but for 100 array > do it for all signals

            eff_mchi2_150_ctau0p1_Total[0] = effZpt50_Total;
            eff_mchi2_150_ctau0p1_Total[1] = effZpt60_Total;
            eff_mchi2_150_ctau0p1_Total[2] = effZpt100_Total;

            h_eff_mchi2_150_ctau0p1_ZptOnly_Scan = (TH1D*)h_after_Zpt_scan_signal->Clone("h_eff_mchi2_150_ctau0p1_ZptOnly_Scan");
            h_eff_mchi2_150_ctau0p1_ZptOnly_Scan->Scale(1/nBeforeZpt);
        }

        if (inputFile.find("Mchi2-150_Mchi1-1_ctau1000p0") != string::npos)
        {
            cout << "Processing " << inputFile << endl;

            eff_mchi2_150_ctau1000p0_ZptOnly[0] = effZpt50_ZptOnly;
            eff_mchi2_150_ctau1000p0_ZptOnly[1] = effZpt60_ZptOnly;
            eff_mchi2_150_ctau1000p0_ZptOnly[2] = effZpt100_ZptOnly;

            eff_mchi2_150_ctau1000p0_Total[0] = effZpt50_Total;
            eff_mchi2_150_ctau1000p0_Total[1] = effZpt60_Total;
            eff_mchi2_150_ctau1000p0_Total[2] = effZpt100_Total;

            h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan = (TH1D*)h_after_Zpt_scan_signal->Clone("h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan");
            h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan->Scale(1/nBeforeZpt);
        }

        if (inputFile.find("Mchi2-150_Mchi1-1_ctau100p0") != string::npos)
        {
            cout << "Processing " << inputFile << endl;

            eff_mchi2_150_ctau100p0_ZptOnly[0] = effZpt50_ZptOnly;
            eff_mchi2_150_ctau100p0_ZptOnly[1] = effZpt60_ZptOnly;
            eff_mchi2_150_ctau100p0_ZptOnly[2] = effZpt100_ZptOnly;

            eff_mchi2_150_ctau100p0_Total[0] = effZpt50_Total;
            eff_mchi2_150_ctau100p0_Total[1] = effZpt60_Total;
            eff_mchi2_150_ctau100p0_Total[2] = effZpt100_Total;

            h_eff_mchi2_150_ctau100p0_ZptOnly_Scan = (TH1D*)h_after_Zpt_scan_signal->Clone("h_eff_mchi2_150_ctau100p0_ZptOnly_Scan");
            h_eff_mchi2_150_ctau100p0_ZptOnly_Scan->Scale(1/nBeforeZpt);
        }

        if (inputFile.find("Mchi2-150_Mchi1-1_ctau10p0") != string::npos)
        {
            cout << "Processing " << inputFile << endl;

            eff_mchi2_150_ctau10p0_ZptOnly[0] = effZpt50_ZptOnly;
            eff_mchi2_150_ctau10p0_ZptOnly[1] = effZpt60_ZptOnly;
            eff_mchi2_150_ctau10p0_ZptOnly[2] = effZpt100_ZptOnly;

            eff_mchi2_150_ctau10p0_Total[0] = effZpt50_Total;
            eff_mchi2_150_ctau10p0_Total[1] = effZpt60_Total;
            eff_mchi2_150_ctau10p0_Total[2] = effZpt100_Total;

            h_eff_mchi2_150_ctau10p0_ZptOnly_Scan = (TH1D*)h_after_Zpt_scan_signal->Clone("h_eff_mchi2_150_ctau10p0_ZptOnly_Scan");
            h_eff_mchi2_150_ctau10p0_ZptOnly_Scan->Scale(1/nBeforeZpt);
        }

        if (inputFile.find("Mchi2-150_Mchi1-1_ctau1p0") != string::npos)
        {
            cout << "Processing " << inputFile << endl;

            eff_mchi2_150_ctau1p0_ZptOnly[0] = effZpt50_ZptOnly;
            eff_mchi2_150_ctau1p0_ZptOnly[1] = effZpt60_ZptOnly;
            eff_mchi2_150_ctau1p0_ZptOnly[2] = effZpt100_ZptOnly;

            eff_mchi2_150_ctau1p0_Total[0] = effZpt50_Total;
            eff_mchi2_150_ctau1p0_Total[1] = effZpt60_Total;
            eff_mchi2_150_ctau1p0_Total[2] = effZpt100_Total;

            h_eff_mchi2_150_ctau1p0_ZptOnly_Scan = (TH1D*)h_after_Zpt_scan_signal->Clone("h_eff_mchi2_150_ctau1p0_ZptOnly_Scan");
            h_eff_mchi2_150_ctau1p0_ZptOnly_Scan->Scale(1/nBeforeZpt);
        }

        if (inputFile.find("Mchi2-1_Mchi1-0p1_ctau0p1") != string::npos)
        {
            cout << "Processing " << inputFile << endl;

            eff_mchi2_1_ctau0p1_ZptOnly[0] = effZpt50_ZptOnly;
            eff_mchi2_1_ctau0p1_ZptOnly[1] = effZpt60_ZptOnly;
            eff_mchi2_1_ctau0p1_ZptOnly[2] = effZpt100_ZptOnly;

            eff_mchi2_1_ctau0p1_Total[0] = effZpt50_Total;
            eff_mchi2_1_ctau0p1_Total[1] = effZpt60_Total;
            eff_mchi2_1_ctau0p1_Total[2] = effZpt100_Total;

            h_eff_mchi2_1_ctau0p1_ZptOnly_Scan = (TH1D*)h_after_Zpt_scan_signal->Clone("h_eff_mchi2_1_ctau0p1_ZptOnly_Scan");
            h_eff_mchi2_1_ctau0p1_ZptOnly_Scan->Scale(1/nBeforeZpt);
        }

        if (inputFile.find("Mchi2-1_Mchi1-0p1_ctau1000p0") != string::npos)
        {
            cout << "Processing " << inputFile << endl;

            eff_mchi2_1_ctau1000p0_ZptOnly[0] = effZpt50_ZptOnly;
            eff_mchi2_1_ctau1000p0_ZptOnly[1] = effZpt60_ZptOnly;
            eff_mchi2_1_ctau1000p0_ZptOnly[2] = effZpt100_ZptOnly;

            eff_mchi2_1_ctau1000p0_Total[0] = effZpt50_Total;
            eff_mchi2_1_ctau1000p0_Total[1] = effZpt60_Total;
            eff_mchi2_1_ctau1000p0_Total[2] = effZpt100_Total;

            h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan = (TH1D*)h_after_Zpt_scan_signal->Clone("h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan");
            h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan->Scale(1/nBeforeZpt);
        }

        if (inputFile.find("Mchi2-1_Mchi1-0p1_ctau100p0") != string::npos)
        {
            cout << "Processing " << inputFile << endl;

            eff_mchi2_1_ctau100p0_ZptOnly[0] = effZpt50_ZptOnly;
            eff_mchi2_1_ctau100p0_ZptOnly[1] = effZpt60_ZptOnly;
            eff_mchi2_1_ctau100p0_ZptOnly[2] = effZpt100_ZptOnly;

            eff_mchi2_1_ctau100p0_Total[0] = effZpt50_Total;
            eff_mchi2_1_ctau100p0_Total[1] = effZpt60_Total;
            eff_mchi2_1_ctau100p0_Total[2] = effZpt100_Total;

            h_eff_mchi2_1_ctau100p0_ZptOnly_Scan = (TH1D*)h_after_Zpt_scan_signal->Clone("h_eff_mchi2_1_ctau100p0_ZptOnly_Scan");
            h_eff_mchi2_1_ctau100p0_ZptOnly_Scan->Scale(1/nBeforeZpt);
        }

        if (inputFile.find("Mchi2-1_Mchi1-0p1_ctau10p0") != string::npos)
        {
            cout << "Processing " << inputFile << endl;

            eff_mchi2_1_ctau10p0_ZptOnly[0] = effZpt50_ZptOnly;
            eff_mchi2_1_ctau10p0_ZptOnly[1] = effZpt60_ZptOnly;
            eff_mchi2_1_ctau10p0_ZptOnly[2] = effZpt100_ZptOnly;

            eff_mchi2_1_ctau10p0_Total[0] = effZpt50_Total;
            eff_mchi2_1_ctau10p0_Total[1] = effZpt60_Total;
            eff_mchi2_1_ctau10p0_Total[2] = effZpt100_Total;

            h_eff_mchi2_1_ctau10p0_ZptOnly_Scan = (TH1D*)h_after_Zpt_scan_signal->Clone("h_eff_mchi2_1_ctau10p0_ZptOnly_Scan");
            h_eff_mchi2_1_ctau10p0_ZptOnly_Scan->Scale(1/nBeforeZpt);
        }

        if (inputFile.find("Mchi2-1_Mchi1-0p1_ctau1p0") != string::npos)
        {
            cout << "Processing " << inputFile << endl;

            eff_mchi2_1_ctau1p0_ZptOnly[0] = effZpt50_ZptOnly;
            eff_mchi2_1_ctau1p0_ZptOnly[1] = effZpt60_ZptOnly;
            eff_mchi2_1_ctau1p0_ZptOnly[2] = effZpt100_ZptOnly;

            eff_mchi2_1_ctau1p0_Total[0] = effZpt50_Total;
            eff_mchi2_1_ctau1p0_Total[1] = effZpt60_Total;
            eff_mchi2_1_ctau1p0_Total[2] = effZpt100_Total;

            h_eff_mchi2_1_ctau1p0_ZptOnly_Scan = (TH1D*)h_after_Zpt_scan_signal->Clone("h_eff_mchi2_1_ctau1p0_ZptOnly_Scan");
            h_eff_mchi2_1_ctau1p0_ZptOnly_Scan->Scale(1/nBeforeZpt);
        }

    }// end of flist
    
    
    TString outputfile(outputtxtFilename);

    TFile *outFile = TFile::Open(outputfile, "RECREATE");
    outFile->cd();
    h_eff_mchi2_150_ctau0p1_ZptOnly_Scan->Write();
    h_eff_mchi2_150_ctau1p0_ZptOnly_Scan->Write();
    h_eff_mchi2_150_ctau10p0_ZptOnly_Scan->Write();
    h_eff_mchi2_150_ctau100p0_ZptOnly_Scan->Write();
    h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan->Write();
    h_eff_mchi2_1_ctau0p1_ZptOnly_Scan->Write();
    h_eff_mchi2_1_ctau1p0_ZptOnly_Scan->Write();
    h_eff_mchi2_1_ctau10p0_ZptOnly_Scan->Write();
    h_eff_mchi2_1_ctau100p0_ZptOnly_Scan->Write();
    h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan->Write();
    outFile->Close();


    //canvas
    gStyle->SetOptStat(0);
    TCanvas* c1 = new TCanvas("c1","c1",900,700); //width-height

    //legend
    auto leg = new TLegend(0.44,0.65,0.75,0.85); //x1,y1,x2,y2
    leg->SetBorderSize(0);
    leg->SetTextSize(0.027);

    const Int_t nPoints = 3;
    Double_t grXaxis[] = {50, 60, 100};

    //Input to TGraph (efficiency with respect to before Zpt)
    cout << "\nCreating graph..." << endl;

    TGraph *gr_mchi2_150_ctau0p1_ZptOnly = new TGraph(nPoints, grXaxis, eff_mchi2_150_ctau0p1_ZptOnly);

    TGraph *gr_mchi2_150_ctau1000p0_ZptOnly = new TGraph(nPoints, grXaxis, eff_mchi2_150_ctau1000p0_ZptOnly);

    TGraph *gr_mchi2_150_ctau100p0_ZptOnly = new TGraph(nPoints, grXaxis, eff_mchi2_150_ctau100p0_ZptOnly);

    TGraph *gr_mchi2_150_ctau10p0_ZptOnly = new TGraph(nPoints, grXaxis, eff_mchi2_150_ctau10p0_ZptOnly);

    TGraph *gr_mchi2_150_ctau1p0_ZptOnly = new TGraph(nPoints, grXaxis, eff_mchi2_150_ctau1p0_ZptOnly);

    
    TGraph *gr_mchi2_1_ctau0p1_ZptOnly = new TGraph(3, grXaxis, eff_mchi2_1_ctau0p1_ZptOnly);

    TGraph *gr_mchi2_1_ctau1000p0_ZptOnly = new TGraph(3, grXaxis, eff_mchi2_1_ctau1000p0_ZptOnly);

    TGraph *gr_mchi2_1_ctau100p0_ZptOnly = new TGraph(3, grXaxis, eff_mchi2_1_ctau100p0_ZptOnly);

    TGraph *gr_mchi2_1_ctau10p0_ZptOnly = new TGraph(3, grXaxis, eff_mchi2_1_ctau10p0_ZptOnly);

    TGraph *gr_mchi2_1_ctau1p0_ZptOnly = new TGraph(3, grXaxis, eff_mchi2_1_ctau1p0_ZptOnly);


    //Input to TGraph (efficiency with respect to n Total before any preselection)
    TGraph *gr_mchi2_150_ctau0p1_Total = new TGraph(3, grXaxis, eff_mchi2_150_ctau0p1_Total);

    TGraph *gr_mchi2_150_ctau1000p0_Total = new TGraph(3, grXaxis, eff_mchi2_150_ctau1000p0_Total);

    TGraph *gr_mchi2_150_ctau100p0_Total = new TGraph(3, grXaxis, eff_mchi2_150_ctau100p0_Total);

    TGraph *gr_mchi2_150_ctau10p0_Total = new TGraph(3, grXaxis, eff_mchi2_150_ctau10p0_Total);

    TGraph *gr_mchi2_150_ctau1p0_Total = new TGraph(3, grXaxis, eff_mchi2_150_ctau1p0_Total);

    
    TGraph *gr_mchi2_1_ctau0p1_Total = new TGraph(3, grXaxis, eff_mchi2_1_ctau0p1_Total);

    TGraph *gr_mchi2_1_ctau1000p0_Total = new TGraph(3, grXaxis, eff_mchi2_1_ctau1000p0_Total);

    TGraph *gr_mchi2_1_ctau100p0_Total = new TGraph(3, grXaxis, eff_mchi2_1_ctau100p0_Total);

    TGraph *gr_mchi2_1_ctau10p0_Total = new TGraph(3, grXaxis, eff_mchi2_1_ctau10p0_Total);

    TGraph *gr_mchi2_1_ctau1p0_Total = new TGraph(3, grXaxis, eff_mchi2_1_ctau1p0_Total);


    //create TGraph for 100 arrays or points
    const Int_t nPoints_scan = 100;
    Double_t grXaxis_scan[100];

    int cutvalue = 10;
    int discrepancy = 10;
    int point = 0;

    while (cutvalue<=1000)
    {
        grXaxis_scan[point] = cutvalue;
        //grXaxis_scan[bin] = bin;
        cutvalue+=discrepancy;
        point+=1;
    }

    //Input to TGraph with 100 points (efficiency with respect to before Zpt)
    TGraph *gr_mchi2_150_ctau0p1_ZptOnly_Scan = new TGraph(nPoints_scan, grXaxis_scan, eff_mchi2_150_ctau0p1_ZptOnly_Scan);
    TGraph *gr_mchi2_150_ctau1000p0_ZptOnly_Scan = new TGraph(nPoints_scan, grXaxis_scan, eff_mchi2_150_ctau1000p0_ZptOnly_Scan);
    TGraph *gr_mchi2_150_ctau100p0_ZptOnly_Scan = new TGraph(nPoints_scan, grXaxis_scan, eff_mchi2_150_ctau100p0_ZptOnly_Scan);
    TGraph *gr_mchi2_150_ctau10p0_ZptOnly_Scan = new TGraph(nPoints_scan, grXaxis_scan, eff_mchi2_150_ctau10p0_ZptOnly_Scan);
    TGraph *gr_mchi2_150_ctau1p0_ZptOnly_Scan = new TGraph(nPoints_scan, grXaxis_scan, eff_mchi2_150_ctau1p0_ZptOnly_Scan);

    TGraph *gr_mchi2_1_ctau0p1_ZptOnly_Scan = new TGraph(nPoints_scan, grXaxis_scan, eff_mchi2_1_ctau0p1_ZptOnly_Scan);
    TGraph *gr_mchi2_1_ctau1000p0_ZptOnly_Scan = new TGraph(nPoints_scan, grXaxis_scan, eff_mchi2_1_ctau1000p0_ZptOnly_Scan);
    TGraph *gr_mchi2_1_ctau100p0_ZptOnly_Scan = new TGraph(nPoints_scan, grXaxis_scan, eff_mchi2_1_ctau100p0_ZptOnly_Scan);
    TGraph *gr_mchi2_1_ctau10p0_ZptOnly_Scan = new TGraph(nPoints_scan, grXaxis_scan, eff_mchi2_1_ctau10p0_ZptOnly_Scan);
    TGraph *gr_mchi2_1_ctau1p0_ZptOnly_Scan = new TGraph(nPoints_scan, grXaxis_scan, eff_mchi2_1_ctau1p0_ZptOnly_Scan);



    //Draw and Save Signal Efficiency Plots of Mchi2-150_Mchi1-1 with respect to number of events before Z pT cut
    gr_mchi2_150_ctau0p1_ZptOnly->SetLineWidth(2);
    gr_mchi2_150_ctau0p1_ZptOnly->SetMarkerStyle(21);
    gr_mchi2_150_ctau0p1_ZptOnly->SetLineColor(800);
    gr_mchi2_150_ctau0p1_ZptOnly->SetTitle("Mchi2-150_Mchi1-1_ctau0p1");

    gr_mchi2_150_ctau1p0_ZptOnly->SetLineWidth(2);
    gr_mchi2_150_ctau1p0_ZptOnly->SetMarkerStyle(21);
    gr_mchi2_150_ctau1p0_ZptOnly->SetLineColor(801);
    gr_mchi2_150_ctau1p0_ZptOnly->SetTitle("Mchi2-150_Mchi1-1_ctau1p0");

    gr_mchi2_150_ctau10p0_ZptOnly->SetLineWidth(2);
    gr_mchi2_150_ctau10p0_ZptOnly->SetMarkerStyle(21);
    gr_mchi2_150_ctau10p0_ZptOnly->SetLineColor(805);
    gr_mchi2_150_ctau10p0_ZptOnly->SetTitle("Mchi2-150_Mchi1-1_ctau10p0");

    gr_mchi2_150_ctau100p0_ZptOnly->SetLineWidth(2);
    gr_mchi2_150_ctau100p0_ZptOnly->SetMarkerStyle(21);
    gr_mchi2_150_ctau100p0_ZptOnly->SetLineColor(804);
    gr_mchi2_150_ctau100p0_ZptOnly->SetTitle("Mchi2-150_Mchi1-1_ctau100p0");

    gr_mchi2_150_ctau1000p0_ZptOnly->SetLineWidth(2);
    gr_mchi2_150_ctau1000p0_ZptOnly->SetMarkerStyle(21);
    gr_mchi2_150_ctau1000p0_ZptOnly->SetLineColor(803);
    gr_mchi2_150_ctau1000p0_ZptOnly->SetTitle("Mchi2-150_Mchi1-1_ctau1000p0");

    /*gr_mchi2_150_ctau0p1_ZptOnly->Print();
    gr_mchi2_150_ctau1p0_ZptOnly->Print();
    gr_mchi2_150_ctau10p0_ZptOnly->Print();
    gr_mchi2_150_ctau100p0_ZptOnly->Print();
    gr_mchi2_150_ctau1000p0_ZptOnly->Print();*/


    cout << "\nCreating Multi Graph..." << endl;

    TMultiGraph *mg = new TMultiGraph("mg","");
    mg->SetTitle("Signal Efficiency (Mchi2-150_Mchi1-1); Z pT (GeV); Efficiency");
    mg->Add(gr_mchi2_150_ctau0p1_ZptOnly);
    mg->Add(gr_mchi2_150_ctau1p0_ZptOnly);
    mg->Add(gr_mchi2_150_ctau10p0_ZptOnly);
    mg->Add(gr_mchi2_150_ctau100p0_ZptOnly);
    mg->Add(gr_mchi2_150_ctau1000p0_ZptOnly);
    mg->Draw("ALP");

    gPad->Modified();
    gPad->Update();
    
    c1->BuildLegend();
    //c1->SaveAs("SignalEfficiency_Mchi2-150_ZptOnly.pdf");


    //Draw and Save Signal Efficiency Plots of Mchi2-1_Mchi1-0p1 with respect to number of events before Z pT cut
    gr_mchi2_1_ctau0p1_ZptOnly->SetLineWidth(2);
    gr_mchi2_1_ctau0p1_ZptOnly->SetMarkerStyle(21);
    gr_mchi2_1_ctau0p1_ZptOnly->SetLineColor(829);
    gr_mchi2_1_ctau0p1_ZptOnly->SetTitle("Mchi2-1_Mchi1-0p1_ctau0p1");

    gr_mchi2_1_ctau1p0_ZptOnly->SetLineWidth(2);
    gr_mchi2_1_ctau1p0_ZptOnly->SetMarkerStyle(21);
    gr_mchi2_1_ctau1p0_ZptOnly->SetLineColor(824);
    gr_mchi2_1_ctau1p0_ZptOnly->SetTitle("Mchi2-1_Mchi1-0p1_ctau1p0");

    gr_mchi2_1_ctau10p0_ZptOnly->SetLineWidth(2);
    gr_mchi2_1_ctau10p0_ZptOnly->SetMarkerStyle(21);
    gr_mchi2_1_ctau10p0_ZptOnly->SetLineColor(418);
    gr_mchi2_1_ctau10p0_ZptOnly->SetTitle("Mchi2-1_Mchi1-0p1_ctau10p0");

    gr_mchi2_1_ctau100p0_ZptOnly->SetLineWidth(2);
    gr_mchi2_1_ctau100p0_ZptOnly->SetMarkerStyle(21);
    gr_mchi2_1_ctau100p0_ZptOnly->SetLineColor(419);
    gr_mchi2_1_ctau100p0_ZptOnly->SetTitle("Mchi2-1_Mchi1-0p1_ctau100p0");

    gr_mchi2_1_ctau1000p0_ZptOnly->SetLineWidth(2);
    gr_mchi2_1_ctau1000p0_ZptOnly->SetMarkerStyle(21);
    gr_mchi2_1_ctau1000p0_ZptOnly->SetLineColor(420);
    gr_mchi2_1_ctau1000p0_ZptOnly->SetTitle("Mchi2-1_Mchi1-0p1_ctau1000p0");

    /*gr_mchi2_150_ctau0p1_ZptOnly->Print();
    gr_mchi2_150_ctau1p0_ZptOnly->Print();
    gr_mchi2_150_ctau10p0_ZptOnly->Print();
    gr_mchi2_150_ctau100p0_ZptOnly->Print();
    gr_mchi2_150_ctau1000p0_ZptOnly->Print();*/


    cout << "\nCreating Multi Graph 2..." << endl;

    TMultiGraph *mg2 = new TMultiGraph("mg2","");
    mg2->SetTitle("Signal Efficiency (Mchi2-1_Mchi1-0p1); Z pT (GeV); Efficiency");
    mg2->Add(gr_mchi2_1_ctau0p1_ZptOnly);
    mg2->Add(gr_mchi2_1_ctau1p0_ZptOnly);
    mg2->Add(gr_mchi2_1_ctau10p0_ZptOnly);
    mg2->Add(gr_mchi2_1_ctau100p0_ZptOnly);
    mg2->Add(gr_mchi2_1_ctau1000p0_ZptOnly);
    mg2->Draw("ALP");

    gPad->Modified();
    gPad->Update();
    
    c1->BuildLegend();
    //c1->SaveAs("SignalEfficiency_Mchi2-1_ZptOnly.pdf");


    //Draw and Save Signal Efficiency Plots of Mchi2-150_Mchi1-1 with respect to number of events before any preselection
    gr_mchi2_150_ctau0p1_Total->SetLineWidth(2);
    gr_mchi2_150_ctau0p1_Total->SetMarkerStyle(21);
    gr_mchi2_150_ctau0p1_Total->SetLineColor(856);
    gr_mchi2_150_ctau0p1_Total->SetTitle("Mchi2-150_Mchi1-1_ctau0p1");

    gr_mchi2_150_ctau1p0_Total->SetLineWidth(2);
    gr_mchi2_150_ctau1p0_Total->SetMarkerStyle(21);
    gr_mchi2_150_ctau1p0_Total->SetLineColor(857);
    gr_mchi2_150_ctau1p0_Total->SetTitle("Mchi2-150_Mchi1-1_ctau1p0");

    gr_mchi2_150_ctau10p0_Total->SetLineWidth(2);
    gr_mchi2_150_ctau10p0_Total->SetMarkerStyle(21);
    gr_mchi2_150_ctau10p0_Total->SetLineColor(860);
    gr_mchi2_150_ctau10p0_Total->SetTitle("Mchi2-150_Mchi1-1_ctau10p0");

    gr_mchi2_150_ctau100p0_Total->SetLineWidth(2);
    gr_mchi2_150_ctau100p0_Total->SetMarkerStyle(21);
    gr_mchi2_150_ctau100p0_Total->SetLineColor(601);
    gr_mchi2_150_ctau100p0_Total->SetTitle("Mchi2-150_Mchi1-1_ctau100p0");

    gr_mchi2_150_ctau1000p0_Total->SetLineWidth(2);
    gr_mchi2_150_ctau1000p0_Total->SetMarkerStyle(21);
    gr_mchi2_150_ctau1000p0_Total->SetLineColor(1);
    gr_mchi2_150_ctau1000p0_Total->SetTitle("Mchi2-150_Mchi1-1_ctau1000p0");

    /*gr_mchi2_150_ctau0p1_Total->Print();
    gr_mchi2_150_ctau1p0_Total->Print();
    gr_mchi2_150_ctau10p0_Total->Print();
    gr_mchi2_150_ctau100p0_Total->Print();
    gr_mchi2_150_ctau1000p0_Total->Print();*/


    cout << "\nCreating Multi Graph 3..." << endl;

    TMultiGraph *mg3 = new TMultiGraph("mg3","");
    mg3->SetTitle("Signal Efficiency (Mchi2-150_Mchi1-1); Z pT (GeV); Efficiency");
    mg3->Add(gr_mchi2_150_ctau0p1_Total);
    mg3->Add(gr_mchi2_150_ctau1p0_Total);
    mg3->Add(gr_mchi2_150_ctau10p0_Total);
    mg3->Add(gr_mchi2_150_ctau100p0_Total);
    mg3->Add(gr_mchi2_150_ctau1000p0_Total);
    mg3->Draw("ALP");

    gPad->Modified();
    gPad->Update();
    
    c1->BuildLegend();
    //c1->SaveAs("SignalEfficiency_Mchi2-150_Total.pdf");
    

    //Draw and Save Signal Efficiency Plots of Mchi2-1_Mchi1-0p1 with respect to number of events before any preselection
    gr_mchi2_1_ctau0p1_Total->SetLineWidth(2);
    gr_mchi2_1_ctau0p1_Total->SetMarkerStyle(21);
    gr_mchi2_1_ctau0p1_Total->SetLineColor(623);
    gr_mchi2_1_ctau0p1_Total->SetTitle("Mchi2-1_Mchi1-0p1_ctau0p1");

    gr_mchi2_1_ctau1p0_Total->SetLineWidth(2);
    gr_mchi2_1_ctau1p0_Total->SetMarkerStyle(21);
    gr_mchi2_1_ctau1p0_Total->SetLineColor(628);
    gr_mchi2_1_ctau1p0_Total->SetTitle("Mchi2-1_Mchi1-0p1_ctau1p0");

    gr_mchi2_1_ctau10p0_Total->SetLineWidth(2);
    gr_mchi2_1_ctau10p0_Total->SetMarkerStyle(21);
    gr_mchi2_1_ctau10p0_Total->SetLineColor(632);
    gr_mchi2_1_ctau10p0_Total->SetTitle("Mchi2-1_Mchi1-0p1_ctau10p0");

    gr_mchi2_1_ctau100p0_Total->SetLineWidth(2);
    gr_mchi2_1_ctau100p0_Total->SetMarkerStyle(21);
    gr_mchi2_1_ctau100p0_Total->SetLineColor(634);
    gr_mchi2_1_ctau100p0_Total->SetTitle("Mchi2-1_Mchi1-0p1_ctau100p0");

    gr_mchi2_1_ctau1000p0_Total->SetLineWidth(2);
    gr_mchi2_1_ctau1000p0_Total->SetMarkerStyle(21);
    gr_mchi2_1_ctau1000p0_Total->SetLineColor(636);
    gr_mchi2_1_ctau1000p0_Total->SetTitle("Mchi2-1_Mchi1-0p1_ctau1000p0");

    /*gr_mchi2_150_ctau0p1_Total->Print();
    gr_mchi2_150_ctau1p0_Total->Print();
    gr_mchi2_150_ctau10p0_Total->Print();
    gr_mchi2_150_ctau100p0_Total->Print();
    gr_mchi2_150_ctau1000p0_Total->Print();*/


    cout << "\nCreating Multi Graph 4..." << endl;

    TMultiGraph *mg4 = new TMultiGraph("mg4","");
    mg4->SetTitle("Signal Efficiency (Mchi2-1_Mchi1-0p1); Z pT (GeV); Efficiency");
    mg4->Add(gr_mchi2_1_ctau0p1_Total);
    mg4->Add(gr_mchi2_1_ctau1p0_Total);
    mg4->Add(gr_mchi2_1_ctau10p0_Total);
    mg4->Add(gr_mchi2_1_ctau100p0_Total);
    mg4->Add(gr_mchi2_1_ctau1000p0_Total);
    mg4->Draw("ALP");

    gPad->Modified();
    gPad->Update();
    
    c1->BuildLegend();
    //c1->SaveAs("SignalEfficiency_Mchi2-1_Total.pdf");

    //test
    /*int cutvalue = 0;
    int discrepancy = 10;
    int bin = 0;

    while (cutvalue<1000)
    {
        cutvalue+=discrepancy;
        cout << "\ncut value: " << cutvalue << endl;
        bin+=1;
        cout << "bin: " << bin << endl;
    }*/

    //Draw and Save Signal Efficiency Plots (For Scanning Step) of Mphi-500_Mchi2-150_Mchi1-1 with respect to number of events before Z pT cut
    gr_mchi2_150_ctau0p1_ZptOnly_Scan->SetLineWidth(2);
    gr_mchi2_150_ctau0p1_ZptOnly_Scan->SetMarkerStyle(20);
    gr_mchi2_150_ctau0p1_ZptOnly_Scan->SetMarkerSize(0.5);
    gr_mchi2_150_ctau0p1_ZptOnly_Scan->SetLineColor(800);
    gr_mchi2_150_ctau0p1_ZptOnly_Scan->SetTitle("Mphi-500_Mchi2-150_Mchi1-1_ctau0p1");

    gr_mchi2_150_ctau1p0_ZptOnly_Scan->SetLineWidth(2);
    gr_mchi2_150_ctau1p0_ZptOnly_Scan->SetMarkerStyle(20);
    gr_mchi2_150_ctau1p0_ZptOnly_Scan->SetMarkerSize(0.5);
    gr_mchi2_150_ctau1p0_ZptOnly_Scan->SetLineColor(801);
    gr_mchi2_150_ctau1p0_ZptOnly_Scan->SetTitle("Mphi-500_Mchi2-150_Mchi1-1_ctau1p0");

    gr_mchi2_150_ctau10p0_ZptOnly_Scan->SetLineWidth(2);
    gr_mchi2_150_ctau10p0_ZptOnly_Scan->SetMarkerStyle(20);
    gr_mchi2_150_ctau10p0_ZptOnly_Scan->SetMarkerSize(0.5);
    gr_mchi2_150_ctau10p0_ZptOnly_Scan->SetLineColor(805);
    gr_mchi2_150_ctau10p0_ZptOnly_Scan->SetTitle("Mphi-500_Mchi2-150_Mchi1-1_ctau10p0");

    gr_mchi2_150_ctau100p0_ZptOnly_Scan->SetLineWidth(2);
    gr_mchi2_150_ctau100p0_ZptOnly_Scan->SetMarkerStyle(20);
    gr_mchi2_150_ctau100p0_ZptOnly_Scan->SetMarkerSize(0.5);
    gr_mchi2_150_ctau100p0_ZptOnly_Scan->SetLineColor(804);
    gr_mchi2_150_ctau100p0_ZptOnly_Scan->SetTitle("Mphi-500_Mchi2-150_Mchi1-1_ctau100p0");

    gr_mchi2_150_ctau1000p0_ZptOnly_Scan->SetLineWidth(2);
    gr_mchi2_150_ctau1000p0_ZptOnly_Scan->SetMarkerStyle(20);
    gr_mchi2_150_ctau1000p0_ZptOnly_Scan->SetMarkerSize(0.5);
    gr_mchi2_150_ctau1000p0_ZptOnly_Scan->SetLineColor(803);
    gr_mchi2_150_ctau1000p0_ZptOnly_Scan->SetTitle("Mphi-500_Mchi2-150_Mchi1-1_ctau1000p0");

    /*gr_mchi2_150_ctau0p1_ZptOnly_Scan->Print();
    gr_mchi2_150_ctau1p0_ZptOnly_Scan->Print();
    gr_mchi2_150_ctau10p0_ZptOnly_Scan->Print();
    gr_mchi2_150_ctau100p0_ZptOnly_Scan->Print();
    gr_mchi2_150_ctau1000p0_ZptOnly_Scan->Print();*/


    cout << "\nCreating Multi Graph 5..." << endl;

    TMultiGraph *mg5 = new TMultiGraph("mg5","");
    mg5->SetTitle("Signal Efficiency (Mphi-500_Mchi2-150_Mchi1-1); Z pT (GeV); Efficiency");
    mg5->Add(gr_mchi2_150_ctau0p1_ZptOnly_Scan);
    mg5->Add(gr_mchi2_150_ctau1p0_ZptOnly_Scan);
    mg5->Add(gr_mchi2_150_ctau10p0_ZptOnly_Scan);
    mg5->Add(gr_mchi2_150_ctau100p0_ZptOnly_Scan);
    mg5->Add(gr_mchi2_150_ctau1000p0_ZptOnly_Scan);
    mg5->Draw("ACP");

    gPad->Modified();
    gPad->Update();
    
    c1->BuildLegend();
    //c1->SaveAs("SignalEfficiency_Mphi-500_Mchi2-150_ZptOnly_Scan.pdf");
    //c1->SaveAs("SignalEfficiency_Mphi-500_Mchi2-150_ZptOnly_Scan.png");


    //Draw and Save Signal Efficiency Plots of Mchi2-1_Mchi1-0p1 with respect to number of events before Z pT cut
    gr_mchi2_1_ctau0p1_ZptOnly_Scan->SetLineWidth(2);
    gr_mchi2_1_ctau0p1_ZptOnly_Scan->SetMarkerStyle(20);
    gr_mchi2_1_ctau0p1_ZptOnly_Scan->SetMarkerSize(0.5);
    gr_mchi2_1_ctau0p1_ZptOnly_Scan->SetLineColor(829);
    gr_mchi2_1_ctau0p1_ZptOnly_Scan->SetTitle("Mphi-500_Mchi2-1_Mchi1-0p1_ctau0p1");

    gr_mchi2_1_ctau1p0_ZptOnly_Scan->SetLineWidth(2);
    gr_mchi2_1_ctau1p0_ZptOnly_Scan->SetMarkerStyle(20);
    gr_mchi2_1_ctau1p0_ZptOnly_Scan->SetMarkerSize(0.5);
    gr_mchi2_1_ctau1p0_ZptOnly_Scan->SetLineColor(824);
    gr_mchi2_1_ctau1p0_ZptOnly_Scan->SetTitle("Mphi-500_Mchi2-1_Mchi1-0p1_ctau1p0");

    gr_mchi2_1_ctau10p0_ZptOnly_Scan->SetLineWidth(2);
    gr_mchi2_1_ctau10p0_ZptOnly_Scan->SetMarkerStyle(20);
    gr_mchi2_1_ctau10p0_ZptOnly_Scan->SetMarkerSize(0.5);
    gr_mchi2_1_ctau10p0_ZptOnly_Scan->SetLineColor(418);
    gr_mchi2_1_ctau10p0_ZptOnly_Scan->SetTitle("Mphi-500_Mchi2-1_Mchi1-0p1_ctau10p0");

    gr_mchi2_1_ctau100p0_ZptOnly_Scan->SetLineWidth(2);
    gr_mchi2_1_ctau100p0_ZptOnly_Scan->SetMarkerStyle(20);
    gr_mchi2_1_ctau100p0_ZptOnly_Scan->SetMarkerSize(0.5);
    gr_mchi2_1_ctau100p0_ZptOnly_Scan->SetLineColor(419);
    gr_mchi2_1_ctau100p0_ZptOnly_Scan->SetTitle("Mphi-500_Mchi2-1_Mchi1-0p1_ctau100p0");

    gr_mchi2_1_ctau1000p0_ZptOnly_Scan->SetLineWidth(2);
    gr_mchi2_1_ctau1000p0_ZptOnly_Scan->SetMarkerStyle(20);
    gr_mchi2_1_ctau1000p0_ZptOnly_Scan->SetMarkerSize(0.5);
    gr_mchi2_1_ctau1000p0_ZptOnly_Scan->SetLineColor(420);
    gr_mchi2_1_ctau1000p0_ZptOnly_Scan->SetTitle("Mphi-500_Mchi2-1_Mchi1-0p1_ctau1000p0");


    cout << "\nCreating Multi Graph 6..." << endl;

    TMultiGraph *mg6 = new TMultiGraph("mg6","");
    mg6->SetTitle("Signal Efficiency (Mphi-500_Mchi2-1_Mchi1-0p1); Z pT (GeV); Efficiency");
    mg6->Add(gr_mchi2_1_ctau0p1_ZptOnly_Scan);
    mg6->Add(gr_mchi2_1_ctau1p0_ZptOnly_Scan);
    mg6->Add(gr_mchi2_1_ctau10p0_ZptOnly_Scan);
    mg6->Add(gr_mchi2_1_ctau100p0_ZptOnly_Scan);
    mg6->Add(gr_mchi2_1_ctau1000p0_ZptOnly_Scan);
    mg6->Draw("ACP");

    gPad->Modified();
    gPad->Update();
    
    c1->BuildLegend();
    //c1->SaveAs("SignalEfficiency_Mphi-500_Mchi2-1_ZptOnly_Scan.pdf");
    //c1->SaveAs("SignalEfficiency_Mphi-500_Mchi2-1_ZptOnly_Scan.png");


    //overlay and draw signal efficiency in histograms
    cout << "\nCreating Overlaid Histo 1..." << endl;
    leg->Clear();
    h_eff_mchi2_150_ctau0p1_ZptOnly_Scan->SetTitle("Signal Efficiency (Mphi-500_Mchi2-150_Mchi1-1)");
    h_eff_mchi2_150_ctau0p1_ZptOnly_Scan->SetLineColor(800); //brown or orange
    h_eff_mchi2_150_ctau0p1_ZptOnly_Scan->SetLineWidth(2);
    h_eff_mchi2_150_ctau0p1_ZptOnly_Scan->GetYaxis()->SetTitle("Efficiency");
    h_eff_mchi2_150_ctau0p1_ZptOnly_Scan->GetXaxis()->SetTitle("Z pT (GeV)");
    leg->AddEntry(h_eff_mchi2_150_ctau0p1_ZptOnly_Scan, "Mphi-500_Mchi2-150_Mchi1-1_ctau0p1", "l");
    
    h_eff_mchi2_150_ctau1p0_ZptOnly_Scan->SetLineColor(801);
    h_eff_mchi2_150_ctau1p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_eff_mchi2_150_ctau1p0_ZptOnly_Scan, "Mphi-500_Mchi2-150_Mchi1-1_ctau1p0", "l");

    h_eff_mchi2_150_ctau10p0_ZptOnly_Scan->SetLineColor(805);
    h_eff_mchi2_150_ctau10p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_eff_mchi2_150_ctau10p0_ZptOnly_Scan, "Mphi-500_Mchi2-150_Mchi1-1_ctau10p0", "l");
    
    h_eff_mchi2_150_ctau100p0_ZptOnly_Scan->SetLineColor(804);
    h_eff_mchi2_150_ctau100p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_eff_mchi2_150_ctau100p0_ZptOnly_Scan, "Mphi-500_Mchi2-150_Mchi1-1_ctau100p0", "l");

    h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan->SetLineColor(803);
    h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan, "Mphi-500_Mchi2-150_Mchi1-1_ctau1000p0", "l");

    h_eff_mchi2_150_ctau0p1_ZptOnly_Scan->Draw("hist");
    h_eff_mchi2_150_ctau1p0_ZptOnly_Scan->Draw("histsame");
    h_eff_mchi2_150_ctau10p0_ZptOnly_Scan->Draw("histsame");
    h_eff_mchi2_150_ctau100p0_ZptOnly_Scan->Draw("histsame");
    h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan->Draw("histsame");
    leg->Draw();
    c1->Print("Histo_SignalEfficiency_Mphi-500_Mchi2-150_ZptOnly_Scan.pdf");
    //c1->Print("Histo_SignalEfficiency_Mphi-500_Mchi2-150_ZptOnly_Scan.png");


    //overlay and draw signal efficiency in histograms
    cout << "\nCreating Overlaid Histo 2..." << endl;
    leg->Clear();
    h_eff_mchi2_1_ctau0p1_ZptOnly_Scan->SetTitle("Signal Efficiency (Mphi-500_Mchi2-1_Mchi1-0p1)");
    h_eff_mchi2_1_ctau0p1_ZptOnly_Scan->SetLineColor(829); //green
    h_eff_mchi2_1_ctau0p1_ZptOnly_Scan->SetLineWidth(2);
    h_eff_mchi2_1_ctau0p1_ZptOnly_Scan->GetYaxis()->SetTitle("Efficiency");
    h_eff_mchi2_1_ctau0p1_ZptOnly_Scan->GetXaxis()->SetTitle("Z pT (GeV)");
    leg->AddEntry(h_eff_mchi2_1_ctau0p1_ZptOnly_Scan, "Mphi-500_Mchi2-1_Mchi1-0p1_ctau0p1", "l");
    
    h_eff_mchi2_1_ctau1p0_ZptOnly_Scan->SetLineColor(824);
    h_eff_mchi2_1_ctau1p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_eff_mchi2_1_ctau1p0_ZptOnly_Scan, "Mphi-500_Mchi2-1_Mchi1-0p1_ctau1p0", "l");

    h_eff_mchi2_1_ctau10p0_ZptOnly_Scan->SetLineColor(418);
    h_eff_mchi2_1_ctau10p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_eff_mchi2_1_ctau10p0_ZptOnly_Scan, "Mphi-500_Mchi2-1_Mchi1-0p1_ctau10p0", "l");
    
    h_eff_mchi2_1_ctau100p0_ZptOnly_Scan->SetLineColor(419);
    h_eff_mchi2_1_ctau100p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_eff_mchi2_1_ctau100p0_ZptOnly_Scan, "Mphi-500_Mchi2-1_Mchi1-0p1_ctau100p0", "l");

    h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan->SetLineColor(420);
    h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan, "Mphi-500_Mchi2-1_Mchi1-0p1_ctau1000p0", "l");

    h_eff_mchi2_1_ctau0p1_ZptOnly_Scan->Draw("hist");
    h_eff_mchi2_1_ctau1p0_ZptOnly_Scan->Draw("histsame");
    h_eff_mchi2_1_ctau10p0_ZptOnly_Scan->Draw("histsame");
    h_eff_mchi2_1_ctau100p0_ZptOnly_Scan->Draw("histsame");
    h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan->Draw("histsame");
    leg->Draw();
    c1->Print("Histo_SignalEfficiency_Mphi-500_Mchi2-1_ZptOnly_Scan.pdf");
    //c1->Print("Histo_SignalEfficiency_Mphi-500_Mchi2-1_ZptOnly_Scan.png");

} // end of main loop
