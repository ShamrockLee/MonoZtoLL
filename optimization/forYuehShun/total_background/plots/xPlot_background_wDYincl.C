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
#include "Cross_section.h"
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


void xPlot_background_wDYincl(string inputtxtFilename = "inputListBkg.txt", string outputtxtFilename = "allBkg_wDYincl.root")
{

    Double_t lumi = 41500.0; // integrated luminosity; unit: pb^{-1}
    Double_t XS_DYincl = 6077.22; // unit: pb

    // For Initial
    TH1D* h_total_mcweight = new TH1D("h_total_mcweight", "", 5, 0, 5);
    h_total_mcweight->Sumw2();

    TH1D* h_total_mcweight_new = new TH1D("h_total_mcweight_new", "", 5, 0, 5);
    h_total_mcweight_new->Sumw2();

    TH1D* h_before_Zpt_bkg = new TH1D("h_before_Zpt_bkg", "", 5, 0, 5);
    h_before_Zpt_bkg->Sumw2(); 

    TH1D* h_after_Zpt_scan_bkg = new TH1D("h_after_Zpt_scan_bkg", "", 100, 0, 1000);
    h_after_Zpt_scan_bkg->Sumw2();

    TH1D *h_HT_eventCount = new TH1D("h_HT_eventCount", "", 10, 0, 10);
    h_HT_eventCount->SetYTitle("N event");
    h_HT_eventCount->Sumw2();

    TH1D *h_HT_eventCount_new = new TH1D("h_HT_eventCount_new", "", 10, 0, 10);
    h_HT_eventCount_new->SetYTitle("N event");
    h_HT_eventCount_new->Sumw2();


    //DY-Inclusive
    //Initial
    TH1D *h_after_Zpt_scan_bkg_DYincl_HT0to70 = new TH1D("h_after_Zpt_scan_bkg_DYincl_HT0to70", "", 100, 0, 1000);
    h_after_Zpt_scan_bkg_DYincl_HT0to70->Sumw2();

    TH1D *h_after_Zpt_scan_bkg_DYincl_HT70to100 = new TH1D("h_after_Zpt_scan_bkg_DYincl_HT70to100", "", 100, 0, 1000);
    h_after_Zpt_scan_bkg_DYincl_HT70to100->Sumw2();

    TH1D *h_after_Zpt_scan_bkg_DYincl_HT100to200 = new TH1D("h_after_Zpt_scan_bkg_DYincl_HT100to200", "", 100, 0, 1000);
    h_after_Zpt_scan_bkg_DYincl_HT100to200->Sumw2();

    TH1D *h_after_Zpt_scan_bkg_DYincl_HT200to400 = new TH1D("h_after_Zpt_scan_bkg_DYincl_HT200to400", "", 100, 0, 1000);
    h_after_Zpt_scan_bkg_DYincl_HT200to400->Sumw2();

    TH1D *h_after_Zpt_scan_bkg_DYincl_HT400to600 = new TH1D("h_after_Zpt_scan_bkg_DYincl_HT400to600", "", 100, 0, 1000);
    h_after_Zpt_scan_bkg_DYincl_HT400to600->Sumw2();

    TH1D *h_after_Zpt_scan_bkg_DYincl_HT600to800 = new TH1D("h_after_Zpt_scan_bkg_DYincl_HT600to800", "", 100, 0, 1000);
    h_after_Zpt_scan_bkg_DYincl_HT600to800->Sumw2();

    TH1D *h_after_Zpt_scan_bkg_DYincl_HT800to1200 = new TH1D("h_after_Zpt_scan_bkg_DYincl_HT800to1200", "", 100, 0, 1000);
    h_after_Zpt_scan_bkg_DYincl_HT800to1200->Sumw2();

    TH1D *h_after_Zpt_scan_bkg_DYincl_HT1200to2500 = new TH1D("h_after_Zpt_scan_bkg_DYincl_HT1200to2500", "", 100, 0, 1000);
    h_after_Zpt_scan_bkg_DYincl_HT1200to2500->Sumw2();

    TH1D *h_after_Zpt_scan_bkg_DYincl_HT2500toInf = new TH1D("h_after_Zpt_scan_bkg_DYincl_HT2500toInf", "", 100, 0, 1000);
    h_after_Zpt_scan_bkg_DYincl_HT2500toInf->Sumw2();

    //After Normalization to Integrated Luminosity
    TH1D* h_after_Zpt_scan_xsWeighted_DYincl_HT0to70 = new TH1D("h_after_Zpt_scan_xsWeighted_DYincl_HT0to70", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DYincl_HT0to70->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DYincl_HT70to100 = new TH1D("h_after_Zpt_scan_xsWeighted_DYincl_HT70to100", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DYincl_HT70to100->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DYincl_HT100to200 = new TH1D("h_after_Zpt_scan_xsWeighted_DYincl_HT100to200", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DYincl_HT100to200->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DYincl_HT200to400 = new TH1D("h_after_Zpt_scan_xsWeighted_DYincl_HT200to400", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DYincl_HT200to400->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DYincl_HT400to600 = new TH1D("h_after_Zpt_scan_xsWeighted_DYincl_HT400to600", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DYincl_HT400to600->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DYincl_HT600to800 = new TH1D("h_after_Zpt_scan_xsWeighted_DYincl_HT600to800", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DYincl_HT600to800->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DYincl_HT800to1200 = new TH1D("h_after_Zpt_scan_xsWeighted_DYincl_HT800to1200", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DYincl_HT800to1200->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DYincl_HT1200to2500 = new TH1D("h_after_Zpt_scan_xsWeighted_DYincl_HT1200to2500", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DYincl_HT1200to2500->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DYincl_HT2500toInf = new TH1D("h_after_Zpt_scan_xsWeighted_DYincl_HT2500toInf", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DYincl_HT2500toInf->Sumw2();


    //DY-HT
    TH1D* h_after_Zpt_scan_xsWeighted_DY_HT70to100 = new TH1D("h_after_Zpt_scan_xsWeighted_DY_HT70to100", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DY_HT70to100->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DY_HT100to200 = new TH1D("h_after_Zpt_scan_xsWeighted_DY_HT100to200", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DY_HT100to200->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DY_HT200to400 = new TH1D("h_after_Zpt_scan_xsWeighted_DY_HT200to400", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DY_HT200to400->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DY_HT400to600 = new TH1D("h_after_Zpt_scan_xsWeighted_DY_HT400to600", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DY_HT400to600->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DY_HT600to800 = new TH1D("h_after_Zpt_scan_xsWeighted_DY_HT600to800", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DY_HT600to800->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DY_HT800to1200 = new TH1D("h_after_Zpt_scan_xsWeighted_DY_HT800to1200", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DY_HT800to1200->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DY_HT1200to2500 = new TH1D("h_after_Zpt_scan_xsWeighted_DY_HT1200to2500", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DY_HT1200to2500->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_DY_HT2500toInf = new TH1D("h_after_Zpt_scan_xsWeighted_DY_HT2500toInf", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_DY_HT2500toInf->Sumw2();

    //Top
    TH1D* h_after_Zpt_scan_xsWeighted_ST_tW_antitop = new TH1D("h_after_Zpt_scan_xsWeighted_ST_tW_antitop", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_ST_tW_antitop->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_ST_tW_top = new TH1D("h_after_Zpt_scan_xsWeighted_ST_tW_top", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_ST_tW_top->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_TTTo2L2Nu = new TH1D("h_after_Zpt_scan_xsWeighted_TTTo2L2Nu", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_TTTo2L2Nu->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_TTWJetsToLNu = new TH1D("h_after_Zpt_scan_xsWeighted_TTWJetsToLNu", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_TTWJetsToLNu->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_TTWJetsToQQ = new TH1D("h_after_Zpt_scan_xsWeighted_TTWJetsToQQ", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_TTWJetsToQQ->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_TTZToLLNuNu = new TH1D("h_after_Zpt_scan_xsWeighted_TTZToLLNuNu", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_TTZToLLNuNu->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_TTZToQQ = new TH1D("h_after_Zpt_scan_xsWeighted_TTZToQQ", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_TTZToQQ->Sumw2();


    //Diboson
    TH1D* h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2mu = new TH1D("h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2mu", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2mu->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2nu = new TH1D("h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2nu", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2nu->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2tau = new TH1D("h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2tau", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2tau->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2nu = new TH1D("h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2nu", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2nu->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2tau = new TH1D("h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2tau", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2tau->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_gg_ZZ_4e = new TH1D("h_after_Zpt_scan_xsWeighted_gg_ZZ_4e", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_gg_ZZ_4e->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_gg_ZZ_4mu = new TH1D("h_after_Zpt_scan_xsWeighted_gg_ZZ_4mu", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_gg_ZZ_4mu->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_gg_ZZ_4tau = new TH1D("h_after_Zpt_scan_xsWeighted_gg_ZZ_4tau", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_gg_ZZ_4tau->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_qq_WW_2L2Nu = new TH1D("h_after_Zpt_scan_xsWeighted_qq_WW_2L2Nu", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_qq_WW_2L2Nu->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_WZ_3LNu = new TH1D("h_after_Zpt_scan_xsWeighted_WZ_3LNu", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_WZ_3LNu->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_ZZ_2L2Nu = new TH1D("h_after_Zpt_scan_xsWeighted_ZZ_2L2Nu", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_ZZ_2L2Nu->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_ZZ_4L = new TH1D("h_after_Zpt_scan_xsWeighted_ZZ_4L", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_ZZ_4L->Sumw2();

    //Triboson
    TH1D* h_after_Zpt_scan_xsWeighted_WWZ = new TH1D("h_after_Zpt_scan_xsWeighted_WWZ", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_WWZ->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_WZZ = new TH1D("h_after_Zpt_scan_xsWeighted_WZZ", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_WZZ->Sumw2();

    TH1D* h_after_Zpt_scan_xsWeighted_ZZZ = new TH1D("h_after_Zpt_scan_xsWeighted_ZZZ", "", 100, 0, 1000);
    h_after_Zpt_scan_xsWeighted_ZZZ->Sumw2();

    //for saving to output root file
    TH1D* h_sum_after_Zpt_scan_xsWeighted = new TH1D("h_sum_after_Zpt_scan_xsWeighted", "Sum of all bkg after ZpT cut and XS weight", 100, 0, 1000);
    h_sum_after_Zpt_scan_xsWeighted->Sumw2();

    
    
    cout << "inputtxtFilename = " << inputtxtFilename << endl;
    ifstream flist(inputtxtFilename.data());
    string inputFile;
    while (getline(flist, inputFile))
    {
        //h_total_mcweight->Reset();
        h_total_mcweight_new->Reset();
        h_before_Zpt_bkg->Reset();
        h_after_Zpt_scan_bkg->Reset();

        TString myFile = inputFile;
        cout << "\n" << myFile << endl;

        TFile* file = TFile::Open(myFile);
        //cout << "successfully opened" << endl;

        //TH1F* h_totevent = static_cast<TH1F*>(file->Get("Event_Variable/h_totevent"));
        h_total_mcweight = static_cast<TH1D*>(file->Get("h_total_mcweight"));
        h_total_mcweight_new = static_cast<TH1D*>(file->Get("h_total_mcweight_new"));
        h_before_Zpt_bkg = static_cast<TH1D*>(file->Get("h_before_Zpt_bkg"));
        h_after_Zpt_scan_bkg = static_cast<TH1D*>(file->Get("h_after_Zpt_scan_bkg"));

        Double_t nTotalBeforePreselection = h_total_mcweight_new->Integral();

        Double_t nTotalBfrPreselection_HT0to70;
        Double_t nTotalBfrPreselection_HT70to100;
        Double_t nTotalBfrPreselection_HT100to200;
        Double_t nTotalBfrPreselection_HT200to400;
        Double_t nTotalBfrPreselection_HT400to600;
        Double_t nTotalBfrPreselection_HT600to800;
        Double_t nTotalBfrPreselection_HT800to1200;
        Double_t nTotalBfrPreselection_HT1200to2500;
        Double_t nTotalBfrPreselection_HT2500toInf;
        
        if (inputFile.find("DYJetsToLL_M-50_TuneCP5") != string::npos)
        {
            h_HT_eventCount = static_cast<TH1D*>(file->Get("h_HT_eventCount"));
            nTotalBfrPreselection_HT0to70 = h_HT_eventCount->GetBinContent(2);
            cout << "nTotalBfrPreselection_HT0to70: " << nTotalBfrPreselection_HT0to70 << endl; 
            nTotalBfrPreselection_HT70to100 = h_HT_eventCount->GetBinContent(3);
            cout << "nTotalBfrPreselection_HT70to100: " << nTotalBfrPreselection_HT70to100 << endl;
            nTotalBfrPreselection_HT100to200 = h_HT_eventCount->GetBinContent(4);
            nTotalBfrPreselection_HT200to400 = h_HT_eventCount->GetBinContent(5);
            nTotalBfrPreselection_HT400to600 = h_HT_eventCount->GetBinContent(6);
            nTotalBfrPreselection_HT600to800 = h_HT_eventCount->GetBinContent(7);
            nTotalBfrPreselection_HT800to1200 = h_HT_eventCount->GetBinContent(8);
            nTotalBfrPreselection_HT1200to2500 = h_HT_eventCount->GetBinContent(9);
            nTotalBfrPreselection_HT2500toInf = h_HT_eventCount->GetBinContent(10);

            h_after_Zpt_scan_bkg_DYincl_HT0to70 = static_cast<TH1D*>(file->Get("h_after_Zpt_scan_bkg_DYincl_HT0to70"));
            h_after_Zpt_scan_bkg_DYincl_HT70to100 = static_cast<TH1D*>(file->Get("h_after_Zpt_scan_bkg_DYincl_HT70to100"));
            h_after_Zpt_scan_bkg_DYincl_HT100to200 = static_cast<TH1D*>(file->Get("h_after_Zpt_scan_bkg_DYincl_HT100to200"));
            h_after_Zpt_scan_bkg_DYincl_HT200to400 = static_cast<TH1D*>(file->Get("h_after_Zpt_scan_bkg_DYincl_HT200to400"));
            h_after_Zpt_scan_bkg_DYincl_HT400to600 = static_cast<TH1D*>(file->Get("h_after_Zpt_scan_bkg_DYincl_HT400to600"));
            h_after_Zpt_scan_bkg_DYincl_HT600to800 = static_cast<TH1D*>(file->Get("h_after_Zpt_scan_bkg_DYincl_HT600to800"));
            h_after_Zpt_scan_bkg_DYincl_HT800to1200 = static_cast<TH1D*>(file->Get("h_after_Zpt_scan_bkg_DYincl_HT800to1200"));
            h_after_Zpt_scan_bkg_DYincl_HT1200to2500 = static_cast<TH1D*>(file->Get("h_after_Zpt_scan_bkg_DYincl_HT1200to2500"));
            h_after_Zpt_scan_bkg_DYincl_HT2500toInf = static_cast<TH1D*>(file->Get("h_after_Zpt_scan_bkg_DYincl_HT2500toInf"));

            //For DY Inclusive HT0to70
            Double_t HT0to70weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::HT0to70CS) / nTotalBfrPreselection_HT0to70;
            h_after_Zpt_scan_xsWeighted_DYincl_HT0to70 = (TH1D*)h_after_Zpt_scan_bkg_DYincl_HT0to70->Clone("h_after_Zpt_scan_xsWeighted_DYincl_HT0to70");
            h_after_Zpt_scan_xsWeighted_DYincl_HT0to70->Scale(HT0to70weight);
        }

        //likely, need to make 2 this kind of script: for DY and Top, Diboson, & Triboson
        //For DY, inside it-> have to sum make 2 if loop
        if (inputFile.find("HT-70to100") != string::npos)
        {
            Double_t HT70to100weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::HT70to100CS) / (nTotalBeforePreselection + nTotalBfrPreselection_HT70to100);
            h_after_Zpt_scan_xsWeighted_DY_HT70to100 = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_DY_HT70to100");
            h_after_Zpt_scan_xsWeighted_DY_HT70to100->Scale(HT70to100weight);

            //For DY Inclusive HT70to100
            h_after_Zpt_scan_xsWeighted_DYincl_HT70to100 = (TH1D*)h_after_Zpt_scan_bkg_DYincl_HT70to100->Clone("h_after_Zpt_scan_xsWeighted_DYincl_HT70to100");
            h_after_Zpt_scan_xsWeighted_DYincl_HT70to100->Scale(HT70to100weight);
        }

        if (inputFile.find("HT-100to200") != string::npos)
        {
            Double_t HT100to200weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::HT100to200CS) / (nTotalBeforePreselection + nTotalBfrPreselection_HT100to200);
            h_after_Zpt_scan_xsWeighted_DY_HT100to200 = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_DY_HT100to200");
            h_after_Zpt_scan_xsWeighted_DY_HT100to200->Scale(HT100to200weight);

            //For DY Inclusive HT100to200
            h_after_Zpt_scan_xsWeighted_DYincl_HT100to200 = (TH1D*)h_after_Zpt_scan_bkg_DYincl_HT100to200->Clone("h_after_Zpt_scan_xsWeighted_DYincl_HT100to200");
            h_after_Zpt_scan_xsWeighted_DYincl_HT100to200->Scale(HT100to200weight);
        }

        if (inputFile.find("HT-200to400") != string::npos)
        {
            Double_t HT200to400weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::HT200to400CS) / (nTotalBeforePreselection + nTotalBfrPreselection_HT200to400);
            h_after_Zpt_scan_xsWeighted_DY_HT200to400 = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_DY_HT200to400");
            h_after_Zpt_scan_xsWeighted_DY_HT200to400->Scale(HT200to400weight);

            //For DY Inclusive HT200to400
            h_after_Zpt_scan_xsWeighted_DYincl_HT200to400 = (TH1D*)h_after_Zpt_scan_bkg_DYincl_HT200to400->Clone("h_after_Zpt_scan_xsWeighted_DYincl_HT200to400");
            h_after_Zpt_scan_xsWeighted_DYincl_HT200to400->Scale(HT200to400weight);
        }

        if (inputFile.find("HT-400to600") != string::npos)
        {
            Double_t HT400to600weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::HT400to600CS) / (nTotalBeforePreselection + nTotalBfrPreselection_HT400to600);
            h_after_Zpt_scan_xsWeighted_DY_HT400to600 = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_DY_HT400to600");
            h_after_Zpt_scan_xsWeighted_DY_HT400to600->Scale(HT400to600weight);

            //For DY Inclusive HT400to600
            h_after_Zpt_scan_xsWeighted_DYincl_HT400to600 = (TH1D*)h_after_Zpt_scan_bkg_DYincl_HT400to600->Clone("h_after_Zpt_scan_xsWeighted_DYincl_HT400to600");
            h_after_Zpt_scan_xsWeighted_DYincl_HT400to600->Scale(HT400to600weight);
        }

        if (inputFile.find("HT-600to800") != string::npos)
        {
            Double_t HT600to800weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::HT600to800CS) / (nTotalBeforePreselection + nTotalBfrPreselection_HT600to800);
            h_after_Zpt_scan_xsWeighted_DY_HT600to800 = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_DY_HT600to800");
            h_after_Zpt_scan_xsWeighted_DY_HT600to800->Scale(HT600to800weight);

            //For DY Inclusive HT600to800
            h_after_Zpt_scan_xsWeighted_DYincl_HT600to800 = (TH1D*)h_after_Zpt_scan_bkg_DYincl_HT600to800->Clone("h_after_Zpt_scan_xsWeighted_DYincl_HT600to800");
            h_after_Zpt_scan_xsWeighted_DYincl_HT600to800->Scale(HT600to800weight);
        }
        
        if (inputFile.find("HT-800to1200") != string::npos)
        {
            Double_t HT800to1200weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::HT800to1200CS) / (nTotalBeforePreselection + nTotalBfrPreselection_HT800to1200);
            h_after_Zpt_scan_xsWeighted_DY_HT800to1200 = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_DY_HT800to1200");
            h_after_Zpt_scan_xsWeighted_DY_HT800to1200->Scale(HT800to1200weight);

            //For DY Inclusive HT800to1200
            h_after_Zpt_scan_xsWeighted_DYincl_HT800to1200 = (TH1D*)h_after_Zpt_scan_bkg_DYincl_HT800to1200->Clone("h_after_Zpt_scan_xsWeighted_DYincl_HT800to1200");
            h_after_Zpt_scan_xsWeighted_DYincl_HT800to1200->Scale(HT800to1200weight);
        }

        if (inputFile.find("HT-1200to2500") != string::npos)
        {
            Double_t HT1200to2500weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::HT1200to2500CS) / (nTotalBeforePreselection + nTotalBfrPreselection_HT1200to2500);
            h_after_Zpt_scan_xsWeighted_DY_HT1200to2500 = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_DY_HT1200to2500");
            h_after_Zpt_scan_xsWeighted_DY_HT1200to2500->Scale(HT1200to2500weight);

            //For DY Inclusive HT1200to2500
            h_after_Zpt_scan_xsWeighted_DYincl_HT1200to2500 = (TH1D*)h_after_Zpt_scan_bkg_DYincl_HT1200to2500->Clone("h_after_Zpt_scan_xsWeighted_DYincl_HT1200to2500");
            h_after_Zpt_scan_xsWeighted_DYincl_HT1200to2500->Scale(HT1200to2500weight);
        }

        if (inputFile.find("HT-2500toInf") != string::npos)
        {
            Double_t HT2500toInfweight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::HT2500toInfCS) / (nTotalBeforePreselection + nTotalBfrPreselection_HT2500toInf);
            h_after_Zpt_scan_xsWeighted_DY_HT2500toInf = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_DY_HT2500toInf");
            h_after_Zpt_scan_xsWeighted_DY_HT2500toInf->Scale(HT2500toInfweight);

            //For DY Inclusive HT2500toInf
            h_after_Zpt_scan_xsWeighted_DYincl_HT2500toInf = (TH1D*)h_after_Zpt_scan_bkg_DYincl_HT2500toInf->Clone("h_after_Zpt_scan_xsWeighted_DYincl_HT2500toInf");
            h_after_Zpt_scan_xsWeighted_DYincl_HT2500toInf->Scale(HT2500toInfweight);
        }

        //Top
        if (inputFile.find("ST_tW_antitop") != string::npos)
        {
            Double_t ST_tW_antitop_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::ST_tW_antitop_5f_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_ST_tW_antitop = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_ST_tW_antitop");
            h_after_Zpt_scan_xsWeighted_ST_tW_antitop->Scale(ST_tW_antitop_weight);
        }

        if (inputFile.find("ST_tW_top") != string::npos)
        {
            Double_t ST_tW_top_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::ST_tW_top_5f_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_ST_tW_top = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_ST_tW_top");
            h_after_Zpt_scan_xsWeighted_ST_tW_top->Scale(ST_tW_top_weight);
        }

        if (inputFile.find("TTTo2L2Nu") != string::npos)
        {
            Double_t TTTo2L2Nu_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::TTTo2L2Nu_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_TTTo2L2Nu = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_TTTo2L2Nu");
            h_after_Zpt_scan_xsWeighted_TTTo2L2Nu->Scale(TTTo2L2Nu_weight);
        }

        if (inputFile.find("TTWJetsToLNu") != string::npos)
        {
            Double_t TTWJetsToLNu_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::TTWJetsToLNu_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_TTWJetsToLNu = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_TTWJetsToLNu");
            h_after_Zpt_scan_xsWeighted_TTWJetsToLNu->Scale(TTWJetsToLNu_weight);
        }

        if (inputFile.find("TTWJetsToQQ") != string::npos)
        {
            Double_t TTWJetsToQQ_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::TTWJetsToQQ_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_TTWJetsToQQ = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_TTWJetsToQQ");
            h_after_Zpt_scan_xsWeighted_TTWJetsToQQ->Scale(TTWJetsToQQ_weight);
        }

        if (inputFile.find("TTZToLLNuNu") != string::npos)
        {
            Double_t TTZToLLNuNu_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::TTZToLLNuNu_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_TTZToLLNuNu = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_TTZToLLNuNu");
            h_after_Zpt_scan_xsWeighted_TTZToLLNuNu->Scale(TTZToLLNuNu_weight);
        }

        if (inputFile.find("TTZToQQ") != string::npos)
        {
            Double_t TTZToQQ_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::TTZToQQ_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_TTZToQQ = (TH1D*)h_after_Zpt_scan_bkg->Clone("h_after_Zpt_scan_xsWeighted_TTZToQQ");
            h_after_Zpt_scan_xsWeighted_TTZToQQ->Scale(TTZToQQ_weight);
        }

        //Diboson
        if (inputFile.find("GluGluToContinToZZTo2e2mu") != string::npos)
        {
            Double_t gg_ZZ_2e2mu_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::gg_ZZ_2e2mu_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2mu = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_gg_ZZ_2e2mu");
            h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2mu->Scale(gg_ZZ_2e2mu_weight);
        }

        if (inputFile.find("GluGluToContinToZZTo2e2nu") != string::npos)
        {
            Double_t gg_ZZ_2e2nu_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::gg_ZZ_2e2nu_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2nu = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_gg_ZZ_2e2nu");
            h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2nu->Scale(gg_ZZ_2e2nu_weight);
        }

        if (inputFile.find("GluGluToContinToZZTo2e2tau") != string::npos)
        {
            Double_t gg_ZZ_2e2tau_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::gg_ZZ_2e2tau_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2tau = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_gg_ZZ_2e2tau");
            h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2tau->Scale(gg_ZZ_2e2tau_weight);
        }
        
        if (inputFile.find("GluGluToContinToZZTo2mu2nu") != string::npos)
        {
            Double_t gg_ZZ_2mu2nu_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::gg_ZZ_2mu2nu_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2nu = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2nu");
            h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2nu->Scale(gg_ZZ_2mu2nu_weight);
        }

        if (inputFile.find("GluGluToContinToZZTo2mu2tau") != string::npos)
        {
            Double_t gg_ZZ_2mu2tau_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::gg_ZZ_2mu2tau_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2tau = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2tau");
            h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2tau->Scale(gg_ZZ_2mu2tau_weight);
        }

        if (inputFile.find("GluGluToContinToZZTo4e") != string::npos)
        {
            Double_t gg_ZZ_4e_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::gg_ZZ_4e_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_gg_ZZ_4e = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_gg_ZZ_4e");
            h_after_Zpt_scan_xsWeighted_gg_ZZ_4e->Scale(gg_ZZ_4e_weight);
        }

        if (inputFile.find("GluGluToContinToZZTo4mu") != string::npos)
        {
            Double_t gg_ZZ_4mu_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::gg_ZZ_4mu_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_gg_ZZ_4mu = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_gg_ZZ_4mu");
            h_after_Zpt_scan_xsWeighted_gg_ZZ_4mu->Scale(gg_ZZ_4mu_weight);
        }

        if (inputFile.find("GluGluToContinToZZTo4tau") != string::npos)
        {
            Double_t gg_ZZ_4tau_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::gg_ZZ_4tau_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_gg_ZZ_4tau = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_gg_ZZ_4tau");
            h_after_Zpt_scan_xsWeighted_gg_ZZ_4tau->Scale(gg_ZZ_4tau_weight);
        }

        if (inputFile.find("WWTo2L2Nu") != string::npos)
        {
            Double_t qq_WW_2L2Nu_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::qq_WW_2L2Nu_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_qq_WW_2L2Nu = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_qq_WW_2L2Nu");
            h_after_Zpt_scan_xsWeighted_qq_WW_2L2Nu->Scale(qq_WW_2L2Nu_weight);
        }

        if (inputFile.find("WZTo3LNu") != string::npos)
        {
            Double_t WZ_3LNu_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::WZ_3LNu_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_WZ_3LNu = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_WZ_3LNu");
            h_after_Zpt_scan_xsWeighted_WZ_3LNu->Scale(WZ_3LNu_weight);
        }

        if (inputFile.find("ZZTo2L2Nu") != string::npos)
        {
            Double_t ZZ_2L2Nu_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::ZZ_2L2Nu_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_ZZ_2L2Nu = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_ZZ_2L2Nu");
            h_after_Zpt_scan_xsWeighted_ZZ_2L2Nu->Scale(ZZ_2L2Nu_weight);
        }

        if (inputFile.find("ZZTo4L") != string::npos)
        {
            Double_t ZZ_4L_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::ZZ_4L_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_ZZ_4L = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_ZZ_4L");
            h_after_Zpt_scan_xsWeighted_ZZ_4L->Scale(ZZ_4L_weight);
        }

        //Triboson
        if (inputFile.find("WWZ") != string::npos)
        {
            Double_t WWZ_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::WWZ_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_WWZ = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_WWZ");
            h_after_Zpt_scan_xsWeighted_WWZ->Scale(WWZ_weight);
        }

        if (inputFile.find("WZZ") != string::npos)
        {
            Double_t WZZ_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::WZZ_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_WZZ = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_WZZ");
            h_after_Zpt_scan_xsWeighted_WZZ->Scale(WZZ_weight);
        }

        if (inputFile.find("ZZZ") != string::npos)
        {
            Double_t ZZZ_weight = (GlobalConstants::Lumi2017) * 1000 * (GlobalConstants::ZZZ_CS) / nTotalBeforePreselection;
            h_after_Zpt_scan_xsWeighted_ZZZ = (TH1D*)h_after_Zpt_scan_bkg->Clone("hh_after_Zpt_scan_xsWeighted_ZZZ");
            h_after_Zpt_scan_xsWeighted_ZZZ->Scale(ZZZ_weight);
        }

        

    }// end of flist
    
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DYincl_HT0to70);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DYincl_HT70to100);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DYincl_HT100to200);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DYincl_HT200to400);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DYincl_HT400to600);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DYincl_HT600to800);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DYincl_HT800to1200);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DYincl_HT1200to2500);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DYincl_HT2500toInf);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DY_HT70to100);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DY_HT100to200);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DY_HT200to400);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DY_HT400to600);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DY_HT600to800);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DY_HT800to1200);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DY_HT1200to2500);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_DY_HT2500toInf);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_ST_tW_antitop);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_ST_tW_top);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_TTTo2L2Nu);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_TTWJetsToLNu);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_TTWJetsToQQ);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_TTZToLLNuNu);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_TTZToQQ);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2mu);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2nu);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2tau);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2nu);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2tau);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_gg_ZZ_4e);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_gg_ZZ_4mu);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_gg_ZZ_4tau);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_qq_WW_2L2Nu);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_WZ_3LNu);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_ZZ_2L2Nu);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_ZZ_4L);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_WWZ);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_WZZ);
    h_sum_after_Zpt_scan_xsWeighted->Add(h_after_Zpt_scan_xsWeighted_ZZZ);

    //SAVE h_sum_after_Zpt_scan_xsWeighted to output root file. Add this histo to the xPlot_signalefficiency to calculate the punzi significance

    TString outputfile(outputtxtFilename);

    TFile *outFile = TFile::Open(outputfile, "RECREATE");
    outFile->cd();
    h_HT_eventCount->Write();
    h_sum_after_Zpt_scan_xsWeighted->Write();
    h_after_Zpt_scan_xsWeighted_DYincl_HT0to70->Write();
    h_after_Zpt_scan_xsWeighted_DYincl_HT70to100->Write();
    h_after_Zpt_scan_xsWeighted_DYincl_HT100to200->Write();
    h_after_Zpt_scan_xsWeighted_DYincl_HT200to400->Write();
    h_after_Zpt_scan_xsWeighted_DYincl_HT400to600->Write();
    h_after_Zpt_scan_xsWeighted_DYincl_HT600to800->Write();
    h_after_Zpt_scan_xsWeighted_DYincl_HT800to1200->Write();
    h_after_Zpt_scan_xsWeighted_DYincl_HT1200to2500->Write();
    h_after_Zpt_scan_xsWeighted_DYincl_HT2500toInf->Write();
    h_after_Zpt_scan_xsWeighted_DY_HT70to100->Write();
    h_after_Zpt_scan_xsWeighted_DY_HT100to200->Write();
    h_after_Zpt_scan_xsWeighted_DY_HT200to400->Write();
    h_after_Zpt_scan_xsWeighted_DY_HT400to600->Write();
    h_after_Zpt_scan_xsWeighted_DY_HT600to800->Write();
    h_after_Zpt_scan_xsWeighted_DY_HT800to1200->Write();
    h_after_Zpt_scan_xsWeighted_DY_HT1200to2500->Write();
    h_after_Zpt_scan_xsWeighted_DY_HT2500toInf->Write();
    h_after_Zpt_scan_xsWeighted_ST_tW_antitop->Write();
    h_after_Zpt_scan_xsWeighted_ST_tW_top->Write();
    h_after_Zpt_scan_xsWeighted_TTTo2L2Nu->Write();
    h_after_Zpt_scan_xsWeighted_TTWJetsToLNu->Write();
    h_after_Zpt_scan_xsWeighted_TTWJetsToQQ->Write();
    h_after_Zpt_scan_xsWeighted_TTZToLLNuNu->Write();
    h_after_Zpt_scan_xsWeighted_TTZToQQ->Write();
    h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2mu->Write();
    h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2nu->Write();
    h_after_Zpt_scan_xsWeighted_gg_ZZ_2e2tau->Write();
    h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2nu->Write();
    h_after_Zpt_scan_xsWeighted_gg_ZZ_2mu2tau->Write();
    h_after_Zpt_scan_xsWeighted_gg_ZZ_4e->Write();
    h_after_Zpt_scan_xsWeighted_gg_ZZ_4mu->Write();
    h_after_Zpt_scan_xsWeighted_gg_ZZ_4tau->Write();
    h_after_Zpt_scan_xsWeighted_qq_WW_2L2Nu->Write();
    h_after_Zpt_scan_xsWeighted_WZ_3LNu->Write();
    h_after_Zpt_scan_xsWeighted_ZZ_2L2Nu->Write();
    h_after_Zpt_scan_xsWeighted_ZZ_4L->Write();
    h_after_Zpt_scan_xsWeighted_WWZ->Write();
    h_after_Zpt_scan_xsWeighted_WZZ->Write();
    h_after_Zpt_scan_xsWeighted_ZZZ->Write();
    outFile->Close();


    //canvas
    gStyle->SetOptStat(0);
    TCanvas* c1 = new TCanvas("c1","c1",1200,700); //width-height

    h_sum_after_Zpt_scan_xsWeighted->SetTitle("Sum of weighted number of events after Z pT cut");
    //h_sum_after_Zpt_scan_xsWeighted->SetLineColor(800); //green
    h_sum_after_Zpt_scan_xsWeighted->SetLineWidth(2);
    h_sum_after_Zpt_scan_xsWeighted->GetYaxis()->SetTitle("Number of Events");
    h_sum_after_Zpt_scan_xsWeighted->GetXaxis()->SetTitle("Z pT (GeV)");
    h_sum_after_Zpt_scan_xsWeighted->Draw("hist");
    c1->Print("Histo_AllBkg_ZptOnly_Scan_wDYincl.pdf");
    c1->Print("Histo_AllBkg_ZptOnly_Scan_wDYincl.png");

} // end of main loop
