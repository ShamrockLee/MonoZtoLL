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


void xPlot_punzisignificance(string inputsignalpath = "../signal_efficiency/plots/signalEff.root", string inputbkgpath = "../total_background/plots/allBkg_wDYincl.root", string outputtxtFilename = "punziSig_wDYincl.root")
{

    Double_t lumi = 41500.0; // integrated luminosity; unit: pb^{-1}
    Double_t XS_DYincl = 6077.22; // unit: pb

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

    //histograms for sum of all background after Z pT scan and XS weight
    TH1D *h_sum_after_Zpt_scan_xsWeighted = new TH1D("h_sum_after_Zpt_scan_xsWeighted", "", 100, 0, 1000);
    h_sum_after_Zpt_scan_xsWeighted->Sumw2();

    //histograms for punzi significance
    TH1D *h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan = new TH1D("h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan", "", 100, 0, 1000);
    h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan->Sumw2();

    TH1D *h_punzi_mchi2_150_ctau1p0_ZptOnly_Scan = new TH1D("h_punzi_mchi2_150_ctau1p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_punzi_mchi2_150_ctau1p0_ZptOnly_Scan->Sumw2();

    TH1D *h_punzi_mchi2_150_ctau10p0_ZptOnly_Scan = new TH1D("h_punzi_mchi2_150_ctau10p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_punzi_mchi2_150_ctau10p0_ZptOnly_Scan->Sumw2();

    TH1D *h_punzi_mchi2_150_ctau100p0_ZptOnly_Scan = new TH1D("h_punzi_mchi2_150_ctau100p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_punzi_mchi2_150_ctau100p0_ZptOnly_Scan->Sumw2();

    TH1D *h_punzi_mchi2_150_ctau1000p0_ZptOnly_Scan = new TH1D("h_punzi_mchi2_150_ctau1000p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_punzi_mchi2_150_ctau1000p0_ZptOnly_Scan->Sumw2();


    TH1D *h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan = new TH1D("h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan", "", 100, 0, 1000);
    h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan->Sumw2();

    TH1D *h_punzi_mchi2_1_ctau1p0_ZptOnly_Scan = new TH1D("h_punzi_mchi2_1_ctau1p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_punzi_mchi2_1_ctau1p0_ZptOnly_Scan->Sumw2();

    TH1D *h_punzi_mchi2_1_ctau10p0_ZptOnly_Scan = new TH1D("h_punzi_mchi2_1_ctau10p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_punzi_mchi2_1_ctau10p0_ZptOnly_Scan->Sumw2();

    TH1D *h_punzi_mchi2_1_ctau100p0_ZptOnly_Scan = new TH1D("h_punzi_mchi2_1_ctau100p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_punzi_mchi2_1_ctau100p0_ZptOnly_Scan->Sumw2();

    TH1D *h_punzi_mchi2_1_ctau1000p0_ZptOnly_Scan = new TH1D("h_punzi_mchi2_1_ctau1000p0_ZptOnly_Scan", "", 100, 0, 1000);
    h_punzi_mchi2_1_ctau1000p0_ZptOnly_Scan->Sumw2();



    //OPEN INPUT FILE
    //signal
    TString signalEffFile(inputsignalpath.data());
    cout << "\nsignalEffFile: " << signalEffFile << endl;

    TFile *openSignalEff = TFile::Open(signalEffFile);

    h_eff_mchi2_150_ctau0p1_ZptOnly_Scan = static_cast<TH1D*>(openSignalEff->Get("h_eff_mchi2_150_ctau0p1_ZptOnly_Scan"));
    h_eff_mchi2_150_ctau1p0_ZptOnly_Scan = static_cast<TH1D*>(openSignalEff->Get("h_eff_mchi2_150_ctau1p0_ZptOnly_Scan"));
    h_eff_mchi2_150_ctau10p0_ZptOnly_Scan = static_cast<TH1D*>(openSignalEff->Get("h_eff_mchi2_150_ctau10p0_ZptOnly_Scan"));
    h_eff_mchi2_150_ctau100p0_ZptOnly_Scan = static_cast<TH1D*>(openSignalEff->Get("h_eff_mchi2_150_ctau100p0_ZptOnly_Scan"));
    h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan = static_cast<TH1D*>(openSignalEff->Get("h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan"));
    h_eff_mchi2_1_ctau0p1_ZptOnly_Scan = static_cast<TH1D*>(openSignalEff->Get("h_eff_mchi2_1_ctau0p1_ZptOnly_Scan"));
    h_eff_mchi2_1_ctau1p0_ZptOnly_Scan = static_cast<TH1D*>(openSignalEff->Get("h_eff_mchi2_1_ctau1p0_ZptOnly_Scan"));
    h_eff_mchi2_1_ctau10p0_ZptOnly_Scan = static_cast<TH1D*>(openSignalEff->Get("h_eff_mchi2_1_ctau10p0_ZptOnly_Scan"));
    h_eff_mchi2_1_ctau100p0_ZptOnly_Scan = static_cast<TH1D*>(openSignalEff->Get("h_eff_mchi2_1_ctau100p0_ZptOnly_Scan"));
    h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan = static_cast<TH1D*>(openSignalEff->Get("h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan"));


    //all background
    TString allBkgFile(inputbkgpath.data());

    cout << "\nAll Bkg File: " << allBkgFile << endl;

    TFile *openAllBkg= TFile::Open(allBkgFile);

    h_sum_after_Zpt_scan_xsWeighted = static_cast<TH1D*>(openAllBkg->Get("h_sum_after_Zpt_scan_xsWeighted"));



    //punzi significance
    for (int i = 0; i <= 100; i++)
    {
        double nSumAllBkgEachZpt = 0;
        double denominator = 0;

        nSumAllBkgEachZpt = h_sum_after_Zpt_scan_xsWeighted->GetBinContent(i+1);
        denominator = 1 + sqrt(nSumAllBkgEachZpt);

        //mchi2_150_ctau0p1
        double nominator_mchi2_150_ctau0p1 = h_eff_mchi2_150_ctau0p1_ZptOnly_Scan->GetBinContent(i+1);
        double punziSig_mchi2_150_ctau0p1 = nominator_mchi2_150_ctau0p1/denominator;
        h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan->SetBinContent(i+1, punziSig_mchi2_150_ctau0p1);

        //mchi2_150_ctau1p0
        double nominator_mchi2_150_ctau1p0 = h_eff_mchi2_150_ctau1p0_ZptOnly_Scan->GetBinContent(i+1);
        double punziSig_mchi2_150_ctau1p0 = nominator_mchi2_150_ctau1p0/denominator;
        h_punzi_mchi2_150_ctau1p0_ZptOnly_Scan->SetBinContent(i+1, punziSig_mchi2_150_ctau1p0);

        //mchi2_150_ctau10p0
        double nominator_mchi2_150_ctau10p0 = h_eff_mchi2_150_ctau10p0_ZptOnly_Scan->GetBinContent(i+1);
        double punziSig_mchi2_150_ctau10p0 = nominator_mchi2_150_ctau10p0/denominator;
        h_punzi_mchi2_150_ctau10p0_ZptOnly_Scan->SetBinContent(i+1, punziSig_mchi2_150_ctau10p0);

        //mchi2_150_ctau100p0
        double nominator_mchi2_150_ctau100p0 = h_eff_mchi2_150_ctau100p0_ZptOnly_Scan->GetBinContent(i+1);
        double punziSig_mchi2_150_ctau100p0 = nominator_mchi2_150_ctau100p0/denominator;
        h_punzi_mchi2_150_ctau100p0_ZptOnly_Scan->SetBinContent(i+1, punziSig_mchi2_150_ctau100p0);

        //mchi2_150_ctau1000p0
        double nominator_mchi2_150_ctau1000p0 = h_eff_mchi2_150_ctau1000p0_ZptOnly_Scan->GetBinContent(i+1);
        double punziSig_mchi2_150_ctau1000p0 = nominator_mchi2_150_ctau1000p0/denominator;
        h_punzi_mchi2_150_ctau1000p0_ZptOnly_Scan->SetBinContent(i+1, punziSig_mchi2_150_ctau1000p0);


        //mchi2_1_ctau0p1
        double nominator_mchi2_1_ctau0p1 = h_eff_mchi2_1_ctau0p1_ZptOnly_Scan->GetBinContent(i+1);
        double punziSig_mchi2_1_ctau0p1 = nominator_mchi2_1_ctau0p1/denominator;
        h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan->SetBinContent(i+1, punziSig_mchi2_1_ctau0p1);

        //mchi2_1_ctau1p0
        double nominator_mchi2_1_ctau1p0 = h_eff_mchi2_1_ctau1p0_ZptOnly_Scan->GetBinContent(i+1);
        double punziSig_mchi2_1_ctau1p0 = nominator_mchi2_1_ctau1p0/denominator;
        h_punzi_mchi2_1_ctau1p0_ZptOnly_Scan->SetBinContent(i+1, punziSig_mchi2_1_ctau1p0);

        //mchi2_1_ctau10p0
        double nominator_mchi2_1_ctau10p0 = h_eff_mchi2_1_ctau10p0_ZptOnly_Scan->GetBinContent(i+1);
        double punziSig_mchi2_1_ctau10p0 = nominator_mchi2_1_ctau10p0/denominator;
        h_punzi_mchi2_1_ctau10p0_ZptOnly_Scan->SetBinContent(i+1, punziSig_mchi2_1_ctau10p0);

        //mchi2_1_ctau100p0
        double nominator_mchi2_1_ctau100p0 = h_eff_mchi2_1_ctau100p0_ZptOnly_Scan->GetBinContent(i+1);
        double punziSig_mchi2_1_ctau100p0 = nominator_mchi2_1_ctau100p0/denominator;
        h_punzi_mchi2_1_ctau100p0_ZptOnly_Scan->SetBinContent(i+1, punziSig_mchi2_1_ctau100p0);

        //mchi2_1_ctau1000p0
        double nominator_mchi2_1_ctau1000p0 = h_eff_mchi2_1_ctau1000p0_ZptOnly_Scan->GetBinContent(i+1);
        double punziSig_mchi2_1_ctau1000p0 = nominator_mchi2_1_ctau1000p0/denominator;
        h_punzi_mchi2_1_ctau1000p0_ZptOnly_Scan->SetBinContent(i+1, punziSig_mchi2_1_ctau1000p0);
    }
    
    TString outputfile(outputtxtFilename);

    TFile *outFile = TFile::Open(outputfile, "RECREATE");
    outFile->cd();
    h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan->Write();
    h_punzi_mchi2_150_ctau1p0_ZptOnly_Scan->Write();
    h_punzi_mchi2_150_ctau10p0_ZptOnly_Scan->Write();
    h_punzi_mchi2_150_ctau100p0_ZptOnly_Scan->Write();
    h_punzi_mchi2_150_ctau1000p0_ZptOnly_Scan->Write();
    h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan->Write();
    h_punzi_mchi2_1_ctau1p0_ZptOnly_Scan->Write();
    h_punzi_mchi2_1_ctau10p0_ZptOnly_Scan->Write();
    h_punzi_mchi2_1_ctau100p0_ZptOnly_Scan->Write();
    h_punzi_mchi2_1_ctau1000p0_ZptOnly_Scan->Write();
    outFile->Close();

    

    //canvas
    gStyle->SetOptStat(0);
    TCanvas* c1 = new TCanvas("c1","c1",1000,700); //width-height
    c1->SetLeftMargin(0.15);

    //legend
    auto leg = new TLegend(0.56,0.74,0.86,0.89); //x1,y1,x2,y2
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);


    //overlay and draw signal efficiency in histograms
    cout << "\nCreating Overlaid Histo 1..." << endl;
    leg->Clear();
    h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan->SetTitle("Punzi Significance (Mphi-500_Mchi2-150_Mchi1-1)");
    h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan->SetLineColor(800); //orange or brown
    h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan->SetLineWidth(2);
    h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan->SetMaximum(0.0025);
    h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan->GetYaxis()->SetTitle("Punzi Significance");
    h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan->GetXaxis()->SetTitle("Z pT (GeV)");
    leg->AddEntry(h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan, "Mphi-500_Mchi2-150_Mchi1-1_ctau0p1", "l");
    
    h_punzi_mchi2_150_ctau1p0_ZptOnly_Scan->SetLineColor(801);
    h_punzi_mchi2_150_ctau1p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_punzi_mchi2_150_ctau1p0_ZptOnly_Scan, "Mphi-500_Mchi2-150_Mchi1-1_ctau1p0", "l");

    h_punzi_mchi2_150_ctau10p0_ZptOnly_Scan->SetLineColor(805);
    h_punzi_mchi2_150_ctau10p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_punzi_mchi2_150_ctau10p0_ZptOnly_Scan, "Mphi-500_Mchi2-150_Mchi1-1_ctau10p0", "l");
    
    h_punzi_mchi2_150_ctau100p0_ZptOnly_Scan->SetLineColor(804);
    h_punzi_mchi2_150_ctau100p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_punzi_mchi2_150_ctau100p0_ZptOnly_Scan, "Mphi-500_Mchi2-150_Mchi1-1_ctau100p0", "l");

    h_punzi_mchi2_150_ctau1000p0_ZptOnly_Scan->SetLineColor(803);
    h_punzi_mchi2_150_ctau1000p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_punzi_mchi2_150_ctau1000p0_ZptOnly_Scan, "Mphi-500_Mchi2-150_Mchi1-1_ctau1000p0", "l");

    h_punzi_mchi2_150_ctau0p1_ZptOnly_Scan->Draw("hist");
    h_punzi_mchi2_150_ctau1p0_ZptOnly_Scan->Draw("histsame");
    h_punzi_mchi2_150_ctau10p0_ZptOnly_Scan->Draw("histsame");
    h_punzi_mchi2_150_ctau100p0_ZptOnly_Scan->Draw("histsame");
    h_punzi_mchi2_150_ctau1000p0_ZptOnly_Scan->Draw("histsame");
    leg->Draw();
    c1->Print("Histo_PunziSignificance_Mphi-500_Mchi2-150_ZptOnly_Scan_wDYincl.pdf");
    //c1->Print("Histo_PunziSignificance_Mphi-500_Mchi2-150_ZptOnly_Scan_wDYincl.png");


    //overlay and draw signal efficiency in histograms
    cout << "\nCreating Overlaid Histo 2..." << endl;
    leg->Clear();
    h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan->SetTitle("Punzi Significance (Mphi-500_Mchi2-1_Mchi1-0p1)");
    h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan->SetLineColor(829); //green
    h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan->SetLineWidth(2);
    h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan->SetMaximum(0.0025);
    h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan->GetYaxis()->SetTitle("Punzi Significance");
    h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan->GetXaxis()->SetTitle("Z pT (GeV)");
    leg->AddEntry(h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan, "Mphi-500_Mchi2-1_Mchi1-0p1_ctau0p1", "l");
    
    h_punzi_mchi2_1_ctau1p0_ZptOnly_Scan->SetLineColor(824);
    h_punzi_mchi2_1_ctau1p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_punzi_mchi2_1_ctau1p0_ZptOnly_Scan, "Mphi-500_Mchi2-1_Mchi1-0p1_ctau1p0", "l");

    h_punzi_mchi2_1_ctau10p0_ZptOnly_Scan->SetLineColor(418);
    h_punzi_mchi2_1_ctau10p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_punzi_mchi2_1_ctau10p0_ZptOnly_Scan, "Mphi-500_Mchi2-1_Mchi1-0p1_ctau10p0", "l");
    
    h_punzi_mchi2_1_ctau100p0_ZptOnly_Scan->SetLineColor(419);
    h_punzi_mchi2_1_ctau100p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_punzi_mchi2_1_ctau100p0_ZptOnly_Scan, "Mphi-500_Mchi2-1_Mchi1-0p1_ctau100p0", "l");

    h_punzi_mchi2_1_ctau1000p0_ZptOnly_Scan->SetLineColor(420);
    h_punzi_mchi2_1_ctau1000p0_ZptOnly_Scan->SetLineWidth(2);
    leg->AddEntry(h_punzi_mchi2_1_ctau1000p0_ZptOnly_Scan, "Mphi-500_Mchi2-1_Mchi1-0p1_ctau1000p0", "l");

    h_punzi_mchi2_1_ctau0p1_ZptOnly_Scan->Draw("hist");
    h_punzi_mchi2_1_ctau1p0_ZptOnly_Scan->Draw("histsame");
    h_punzi_mchi2_1_ctau10p0_ZptOnly_Scan->Draw("histsame");
    h_punzi_mchi2_1_ctau100p0_ZptOnly_Scan->Draw("histsame");
    h_punzi_mchi2_1_ctau1000p0_ZptOnly_Scan->Draw("histsame");
    leg->Draw();
    c1->Print("Histo_PunziSignificance_Mphi-500_Mchi2-1_ZptOnly_Scan_wDYincl.pdf");
    //c1->Print("Histo_PunziSignificance_Mphi-500_Mchi2-1_ZptOnly_Scan_wDYincl.png");

} // end of main loop
