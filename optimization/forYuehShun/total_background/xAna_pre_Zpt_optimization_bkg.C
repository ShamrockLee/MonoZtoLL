#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <TH1D.h>
#include <TH1F.h>
#include <TFile.h>
#include "untuplizer.h"
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TAxis.h>
#include <math.h>
#include <string>
#include <TString.h>
#include <TTreeReader.h>
using namespace std;

void efferr(float nsig, float ntotal, float factor = 1)
{
    float eff = nsig / ntotal;
    float err = sqrt((1 - eff) * eff / ntotal);
    cout << "efficiency = " << eff * factor << " +- " << err * factor << endl;
}

bool pt_greater(const TLorentzVector a, const TLorentzVector b)
{
    double A = a.Pt();
    double B = b.Pt();
    return (A > B);
}

float median_value(vector<float> tmpvector)
{
    float med_value = 0.0;
    sort(tmpvector.begin(), tmpvector.end());
    
    if (tmpvector.size() % 2 == 0) //even
    {
        med_value = (tmpvector[tmpvector.size() / 2 - 1] + tmpvector[tmpvector.size() / 2]) / 2;
    }

    else //odd
    {
        med_value = tmpvector[tmpvector.size() / 2];
    }

    return (med_value);
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

float cal_dphi(float phi1, float phi2)
{
    float dphi = phi1 - phi2;
    while (dphi >= TMath::Pi()) dphi -= 2 * TMath::Pi();
    while (dphi < -TMath::Pi()) dphi += 2 * TMath::Pi();
    return TMath::Abs(dphi);
}

void xAna_pre_Zpt_optimization_bkg(string inputtxtFilename, string outputtxtFilename)
//void xAna_Zpt_optimization(string inputtxtFilename = "inputtexttest.txt", string outputfile)
//void xAna_bkg_ztoee_forCheckSkimmedTree(string inputFilename = "../DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8.root", string outputfile = "outputEffCheck_DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8.root")
{
    //start = time.clock()

    //TFile *file = TFile::Open(inputFilename);
    //string inputFile(inputtxtFilename.data());
    cout << "inputtxtFilename = " << inputtxtFilename << endl;


    fstream fin(inputtxtFilename, ios::in);
    if (!fin)
    {
        // cout << "bug" << endl;
    }
    string tmps;
    int line = 0;
    while (getline(fin, tmps))
    {
        line++;
    }
    fin.close();
    cout << "line: " << line << endl;

    //------------------
    // Create histrogram
    //------------------
    //TH1D *h_genee_event = new TH1D("h_genee_event", "gen events", 5, 0, 5);
    //h_genee_event->Sumw2();

    //TH1D *h_recoee_event = new TH1D("h_recoee_event", "reco events", 5, 0, 5);
    //h_recoee_event->Sumw2();

    //TH1D *h_totevent = new TH1D("h_totevent", "total events", 5, 0, 5);
    //h_totevent->Sumw2();

    TH1F* h_total_mcweight = new TH1F("h_total_mcweight", "total MC events", 5, 0, 5);
    h_total_mcweight->Sumw2();

    TH1F* h_total_mcweight_new = new TH1F("h_total_mcweight_new", "total MC events of all files of certain process", 5, 0, 5);
    h_total_mcweight_new->Sumw2();

    TH1D* h_Zpt_after_preselection_bkg = new TH1D("h_Zpt_after_preselection_bkg", "Z pT After Preselection", 100, 0, 1000);
    h_Zpt_after_preselection_bkg->Sumw2();

    TH1D* h_before_Zpt_bkg = new TH1D("h_before_Zpt_bkg", "nEvents before any Zpt cut", 5, 0, 5);
    h_before_Zpt_bkg->Sumw2();

    TH1D* h_after_Zpt50_bkg = new TH1D("h_after_Zpt50_bkg", "After Z pT cut >50 GeV", 5, 0, 5);
    h_after_Zpt50_bkg->Sumw2();

    TH1D* h_after_Zpt60_bkg = new TH1D("h_after_Zpt60_bkg", "After Z pT cut >60 GeV", 5, 0, 5);
    h_after_Zpt60_bkg->Sumw2();

    TH1D* h_after_Zpt100_bkg = new TH1D("h_after_Zpt100_bkg", "After Z pT cut >=100 GeV", 5, 0, 5);
    h_after_Zpt100_bkg->Sumw2();

    TH1D* h_after_Zpt_scan_bkg = new TH1D("h_after_Zpt_scan_bkg", "After Z pt cut scanning", 100, 0, 1000);
    h_after_Zpt_scan_bkg->Sumw2();

    //TH1F *h_HT_eventCout = new TH1F("h_HT_eventCout", "", 10, 0, 10);
    //h_HT_eventCout->SetYTitle("N event");
    //h_HT_eventCout->Sumw2();

    TH1F *h_ee_npass = new TH1F("h_ee_npass", "", 10, 0, 10);
    h_ee_npass->SetXTitle("npass");
    h_ee_npass->Sumw2();

    TH1D *h_ee_npass_noweight = new TH1D("h_ee_npass_noweight", "", 10, 0, 10);
    h_ee_npass_noweight->SetXTitle("npass");
    h_ee_npass_noweight->Sumw2();

    /*TH1D *gen_chi2numb = new TH1D("gen_chi2numb", "Chi2", 5, 0, 5);
    TH1D *gen_dquarknumb = new TH1D("gen_dquarknumb", "d quark", 10, 0, 10);
    TH1D *gen_eenumber = new TH1D("gen_eenumber", "e", 10, 0, 10);
    TH1D *match_dquarknumb = new TH1D("match_dquarknumb", "d quark", 10, 0, 10);

    TH1D *h_ele_n = new TH1D("h_ele_n", "Nr of electron", 10, 0, 10);
    h_ele_n->GetXaxis()->SetTitle("Number of electron");
    h_ele_n->GetYaxis()->SetTitle("Number of Events");
    h_ele_n->Sumw2();

    TH1D *h_tau_n = new TH1D("h_tau_n", "Nr of tau", 10, 0, 10);
    h_tau_n->GetXaxis()->SetTitle("Number of tau");
    h_tau_n->GetYaxis()->SetTitle("Number of Events");
    h_tau_n->Sumw2();

    TH1D *h_mu_n = new TH1D("h_mu_n", "Nr of Muon", 10, 0, 10);
    h_mu_n->GetXaxis()->SetTitle("Number of muon");
    h_mu_n->GetYaxis()->SetTitle("Number of Events");
    h_mu_n->Sumw2();

    TH1D *h_Zboson_n = new TH1D("h_Zboson_n", "Nr of Z boson", 10, 0, 10);
    h_Zboson_n->GetXaxis()->SetTitle("Number of Z boson");
    h_Zboson_n->GetYaxis()->SetTitle("Number of Events");
    h_Zboson_n->Sumw2();

    TH1D *Z_eemass = new TH1D("Z_eemass", "Z->ee", 150, 0, 150);
    Z_eemass->GetXaxis()->SetTitle("Z mass");
    Z_eemass->GetYaxis()->SetTitle("Number of Events");
    Z_eemass->Sumw2();

    TH1D *h_jet_n = new TH1D("h_jet_n", "Nr of Matched jets", 15, 0, 15);
    h_jet_n->GetXaxis()->SetTitle("Number of Jets");
    h_jet_n->GetYaxis()->SetTitle("Number of Events");
    h_jet_n->Sumw2();

    TH1D *h_jet_rank = new TH1D("h_jet_rank", "Nr of rank Matched jets", 15, 0, 15);
    h_jet_rank->GetXaxis()->SetTitle("Jet_rank");
    h_jet_rank->GetYaxis()->SetTitle("Number of Events");
    h_jet_rank->Sumw2();*/


    //for goodleptons kinematics before reco-level
    /*TH1D *h_goodElePt = new TH1D("h_goodElePt", "Electrons pT after ele selection", 100, 0, 1000);
    h_goodElePt->GetXaxis()->SetTitle("Ele pT (GeV)");
    h_goodElePt->GetYaxis()->SetTitle("Number of Events");
    h_goodElePt->Sumw2();

    TH1D *h_goodEleMass = new TH1D("h_goodEleMass", "Electrons Mass after ele selection", 100, -0.5, 0.5);
    h_goodEleMass->GetXaxis()->SetTitle("Ele Mass (GeV)");
    h_goodEleMass->GetYaxis()->SetTitle("Number of Events");
    h_goodEleMass->Sumw2();

    TH1D *h_goodEleEta = new TH1D("h_goodEleEta", "Electrons Eta after ele selection", 100, -5.0, 5.0);
    h_goodEleEta->GetXaxis()->SetTitle("Ele Eta");
    h_goodEleEta->GetYaxis()->SetTitle("Number of Events");
    h_goodEleEta->Sumw2();

    TH1D *h_goodElePhi = new TH1D("h_goodElePhi", "Electrons Phi after ele selection", 100, -5.0, 5.0);
    h_goodElePhi->GetXaxis()->SetTitle("Ele Phi");
    h_goodElePhi->GetYaxis()->SetTitle("Number of Events");
    h_goodElePhi->Sumw2();

    TH1D *h_goodMuPt = new TH1D("h_goodMuPt", "Muons pT bfr after Mu selection", 100, 0, 1000);
    h_goodMuPt->GetXaxis()->SetTitle("Mu pT (GeV)");
    h_goodMuPt->GetYaxis()->SetTitle("Number of Events");
    h_goodMuPt->Sumw2();

    TH1D *h_goodMuMass = new TH1D("h_goodMuMass", "Muons Mass after Mu selection", 100, -0.5, 0.5);
    h_goodMuMass->GetXaxis()->SetTitle("Mu Mass (GeV)");
    h_goodMuMass->GetYaxis()->SetTitle("Number of Events");
    h_goodMuMass->Sumw2();

    TH1D *h_goodMuEta = new TH1D("h_goodMuEta", "Muons Eta after Mu selection", 100, -5.0, 5.0);
    h_goodMuEta->GetXaxis()->SetTitle("Mu Eta");
    h_goodMuEta->GetYaxis()->SetTitle("Number of Events");
    h_goodMuEta->Sumw2();

    TH1D *h_goodMuPhi = new TH1D("h_goodMuPhi", "Muons Phi after Mu selection", 100, -5.0, 5.0);
    h_goodMuPhi->GetXaxis()->SetTitle("Mu Phi");
    h_goodMuPhi->GetYaxis()->SetTitle("Number of Events");
    h_goodMuPhi->Sumw2();

    TH1D *h_goodTauPt = new TH1D("h_goodTauPt", "Good Taus pT after Tau selection", 100, 0, 1000);
    h_goodTauPt->GetXaxis()->SetTitle("Tau pT (GeV)");
    h_goodTauPt->GetYaxis()->SetTitle("Number of Events");
    h_goodTauPt->Sumw2();

    TH1D *h_goodTauMass = new TH1D("h_goodTauMass", "Good Taus Mass after Tau selection", 100, -0.5, 0.5);
    h_goodTauMass->GetXaxis()->SetTitle("Tau Mass (GeV)");
    h_goodTauMass->GetYaxis()->SetTitle("Number of Events");
    h_goodTauMass->Sumw2();

    TH1D *h_goodTauEta = new TH1D("h_goodTauEta", "Good Taus Eta after Tau selection", 100, -5.0, 5.0);
    h_goodTauEta->GetXaxis()->SetTitle("Tau Eta");
    h_goodTauEta->GetYaxis()->SetTitle("Number of Events");
    h_goodTauEta->Sumw2();

    TH1D *h_goodTauPhi = new TH1D("h_goodTauPhi", "Good Taus Phi after Tau selection", 100, -5.0, 5.0);
    h_goodTauPhi->GetXaxis()->SetTitle("Tau Phi");
    h_goodTauPhi->GetYaxis()->SetTitle("Number of Events");
    h_goodTauPhi->Sumw2();*/

    //for calculating efficiency
    /*TH1D *h_recoee_Vtxpass = new TH1D("h_recoee_Vtxpass", "Nr of Reco ee Events after nVtx selection", 10, 0, 10);
    h_recoee_Vtxpass->GetXaxis()->SetTitle("");
    h_recoee_Vtxpass->GetYaxis()->SetTitle("Number of Events");
    h_recoee_Vtxpass->Sumw2();

    TH1D *h_recoee_vetoTau = new TH1D("h_recoee_vetoTau", "Nr of Reco ee Events after vetoing Tau", 10, 0, 10);
    h_recoee_vetoTau->GetXaxis()->SetTitle("");
    h_recoee_vetoTau->GetYaxis()->SetTitle("Number of Events");
    h_recoee_vetoTau->Sumw2();

    TH1D *h_recoee_METpass = new TH1D("h_recoee_METpass", "Nr of Reco ee Events after MET cut", 10, 0, 10);
    h_recoee_METpass->GetXaxis()->SetTitle("");
    h_recoee_METpass->GetYaxis()->SetTitle("Number of Events");
    h_recoee_METpass->Sumw2();
    
    TH1D *h_recoee_eePtpass = new TH1D("h_recoee_eePtpass", "Nr of Reco ee Events after ee pT cut", 10, 0, 10);
    h_recoee_eePtpass->GetXaxis()->SetTitle("");
    h_recoee_eePtpass->GetYaxis()->SetTitle("Number of Events");
    h_recoee_eePtpass->Sumw2();

    TH1D *h_recoee_deltaMasspass = new TH1D("h_recoee_deltaMasspass", "Nr of Reco ee Events after deltaMass cut", 10, 0, 10);
    h_recoee_deltaMasspass->GetXaxis()->SetTitle("");
    h_recoee_deltaMasspass->GetYaxis()->SetTitle("Number of Events");
    h_recoee_deltaMasspass->Sumw2();

    //TH1D *h_recoee_ZbosonPtpass = new TH1D("h_recoee_ZbosonPtpass", "Nr of Reco ee Events after Z boson pT cut", 10, 0, 10);
    //h_recoee_ZbosonPtpass->GetXaxis()->SetTitle("");
    //h_recoee_ZbosonPtpass->GetYaxis()->SetTitle("Number of Events");
    //h_recoee_ZbosonPtpass->Sumw2();

    TH1D *h_recoee_nAK4pass = new TH1D("h_recoee_nAK4pass", "Nr of Reco ee Events with nAK4 gte 1", 10, 0, 10);
    h_recoee_nAK4pass->GetXaxis()->SetTitle("");
    h_recoee_nAK4pass->GetYaxis()->SetTitle("Number of Events");
    h_recoee_nAK4pass->Sumw2();*/

    //----------------------
    // Void Tree variable
    //----------------------
    Int_t I_event;
    Int_t I_weight;
    ULong64_t I_eventID;
    Float_t f_Met;
    //Float_t f_HT;

    /*Float_t f_goodElePt;
    Float_t f_goodEleMass;
    Float_t f_goodEleEta;
    Float_t f_goodElePhi;
    Float_t f_goodMuPt;
    Float_t f_goodMuMass;
    Float_t f_goodMuEta;
    Float_t f_goodMuPhi;
    Float_t f_goodTauPt;
    Float_t f_goodTauMass;
    Float_t f_goodTauEta;
    Float_t f_goodTauPhi;*/

    Float_t f_dileptonPT;
    Float_t f_dileptonMass;
    Float_t f_ZbosonPt;
    Float_t f_ZbosonMass;
    Float_t f_ZbosonEta;
    Float_t f_ZbosonPhi;
    vector<float> v_met;
    vector<float> v_met_lepdeltaPhi;
    Int_t I_nThinJets;
    //vector<float> v_passJetindex;
    vector<float> v_passJetEta;
    vector<float> v_passJetPt;
    vector<float> v_passJetCSV;
    vector<int> v_passjethadronflavor;
    vector<int> v_passJetIndex;
    //vector<int> v_passjetpartonflavor;
    vector<int> v_nTrack;
    vector<float> v_TrackPT;
    vector<float> v_TrackEta;
    vector<float> v_Trackdr;
    //vector<float> v_Trackindex;
    vector<float> v_Trackdz;
    vector<float> v_Trackdzerror;
    vector<float> v_Trackdxy;
    vector<float> v_Trackdxyerror;
    vector<int> v_TrackStatus;
    vector<int> v_TrackHighPurity;

    vector<float> v_emergingTrackPT;
    vector<float> v_emergingTrackEta;
    vector<float> v_emergingTrackPhi;
    vector<float> v_emergingTrackAK4jetdR;
    
    /*vector<float> v_goodElePx;
    vector<float> v_goodElePy;
    vector<float> v_goodElePz;
    vector<float> v_goodEleE;
    vector<float> v_goodMuPx;
    vector<float> v_goodMuPy;
    vector<float> v_goodMuPz;
    vector<float> v_goodMuE;
    vector<float> v_goodTauPx;
    vector<float> v_goodTauPy;
    vector<float> v_goodTauPz;
    vector<float> v_goodTauE;*/


    TString outputfile(outputtxtFilename);

    //TFile *outFile = TFile::Open(outputfile, "RECREATE");
    TTree *tree = new TTree("tree", "Tree");
    tree->Branch("I_event", &I_event);
    tree->Branch("I_weight", &I_weight);
    tree->Branch("I_eventID", &I_eventID);
    tree->Branch("f_Met", &f_Met);
    //tree->Branch("f_HT", &f_HT);
    tree->Branch("f_dileptonPT", &f_dileptonPT);
    tree->Branch("f_dileptonMass", &f_dileptonMass);
    tree->Branch("f_ZbosonPt", &f_ZbosonPt);
    tree->Branch("f_ZbosonMass", &f_ZbosonMass);
    tree->Branch("f_ZbosonEta", &f_ZbosonEta);
    tree->Branch("f_ZbosonPhi", &f_ZbosonPhi);
    
    /*tree->Branch("f_goodElePt", &f_goodElePt);
    tree->Branch("f_goodEleMass", &f_goodEleMass);
    tree->Branch("f_goodEleEta", &f_goodEleEta);
    tree->Branch("f_goodElePhi", &f_goodElePhi);
    tree->Branch("f_goodMuPt", &f_goodMuPt);
    tree->Branch("f_goodMuMass", &f_goodMuMass);
    tree->Branch("f_goodMuEta", &f_goodMuEta);
    tree->Branch("f_goodMuPhi", &f_goodMuPhi);
    tree->Branch("f_goodTauPt", &f_goodTauPt);
    tree->Branch("f_goodTauMass", &f_goodTauMass);
    tree->Branch("f_goodTauEta", &f_goodTauEta);
    tree->Branch("f_goodTauPhi", &f_goodTauPhi);*/
    
    tree->Branch("v_met", &v_met);
    tree->Branch("v_met_lepdeltaPhi", &v_met_lepdeltaPhi);
    tree->Branch("I_nThinJets", &I_nThinJets);
    //tree->Branch("v_passJetindex", &v_passJetindex);
    tree->Branch("v_passJetEta", &v_passJetEta);
    tree->Branch("v_passJetPt", &v_passJetPt);
    tree->Branch("v_passJetCSV", &v_passJetCSV);
    tree->Branch("v_passjethadronflavor", &v_passjethadronflavor);
    tree->Branch("v_passJetIndex", &v_passJetIndex);
    //tree->Branch("v_passjetpartonflavor", &v_passjetpartonflavor);
    tree->Branch("v_nTrack", &v_nTrack);
    tree->Branch("v_TrackPT", &v_TrackPT);
    tree->Branch("v_TrackEta", &v_TrackEta);
    tree->Branch("v_Trackdr", &v_Trackdr);
    //tree->Branch("v_Trackindex", &v_Trackindex);
    tree->Branch("v_Trackdz", &v_Trackdz);
    tree->Branch("v_Trackdzerror", &v_Trackdzerror);
    tree->Branch("v_Trackdxy", &v_Trackdxy);
    tree->Branch("v_Trackdxyerror", &v_Trackdxyerror);
    tree->Branch("v_TrackStatus", &v_TrackStatus);
    tree->Branch("v_TrackHighPurity", &v_TrackHighPurity);

    tree->Branch("v_emergingTrackPT", &v_emergingTrackPT);
    tree->Branch("v_emergingTrackEta", &v_emergingTrackEta);
    tree->Branch("v_emergingTrackPhi", &v_emergingTrackPhi);
    tree->Branch("v_emergingTrackAK4jetdR", &v_emergingTrackAK4jetdR);
    
    /*tree->Branch("v_goodElePx", &v_goodElePx);
    tree->Branch("v_goodElePy", &v_goodElePy);
    tree->Branch("v_goodElePz", &v_goodElePz);
    tree->Branch("v_goodEleE", &v_goodEleE);
    tree->Branch("v_goodMuPx", &v_goodMuPx);
    tree->Branch("v_goodMuPy", &v_goodMuPy);
    tree->Branch("v_goodMuPz", &v_goodMuPz);
    tree->Branch("v_goodMuE", &v_goodMuE);
    tree->Branch("v_goodTauPx", &v_goodTauPx);
    tree->Branch("v_goodTauPy", &v_goodTauPy);
    tree->Branch("v_goodTauPz", &v_goodTauPz);
    tree->Branch("v_goodTauE", &v_goodTauE);*/

    //Declare other variables
    vector<double> vec_bkgEffLow;
    vector<double> vec_bkgEffMed;
    vector<double> vec_bkgEffHigh;

    vector<double> vec_PunziSigLow;
    vector<double> vec_PunziSigMed;
    vector<double> vec_PunziSigHigh;

    h_total_mcweight_new->Reset();
    h_Zpt_after_preselection_bkg->Reset();
    h_before_Zpt_bkg->Reset();
    h_after_Zpt50_bkg->Reset();
    h_after_Zpt60_bkg->Reset();
    h_after_Zpt100_bkg->Reset();
    h_after_Zpt_scan_bkg->Reset();
    h_ee_npass->Reset();
    h_ee_npass_noweight->Reset();

    double nEventsBkgLow = 0.;
    double nEventsBkgMed = 0.;
    double nEventsBkgHigh = 0.;

    ifstream flist(inputtxtFilename.data());
    string inputFile;
    int filenumber = 0;
    int max_filenumber = line;
    while (getline(flist, inputFile))
    {

        cout << "inputFile: " << inputFile << endl;

        if (filenumber < max_filenumber - 1)
        {
            cout << "file number = " << filenumber << endl;
            cout << "process = "
                << "[" << filenumber * 100.0 / (max_filenumber - 1) << "%"
                << "]"
                << "\n"
                << endl;
        }
        else
        {
            cout << "file number = " << filenumber << endl;
            cout << "finish = "
                << "[" << filenumber * 100.0 / (max_filenumber - 1) << "%"
                << "]" << endl;
        }
        filenumber++;

        //h_genee_event->Reset();
        //h_totevent->Reset();
        //h_recoee_nAK4pass->Reset();
        h_total_mcweight->Reset();
        
        
        

        TString file(inputFile); 
        //TFile* file = new TFile(inputFile.c_str(), "READ");
        TFile *openfile = TFile::Open(file);
        h_total_mcweight = static_cast<TH1F*>(openfile->Get("h_total_mcweight"));

        h_total_mcweight_new->Add(h_total_mcweight);

        //get TTree from file ...
        //const char *opentree = inputtxtFilename;
        //TreeReader data(inputFile.c_str());
        TreeReader data(inputFile.data(), "outTree");
        //TreeReader data(inputFilename.data());

        Long64_t nTotal = 0;
        Long64_t nZboson = 0;

        //Event Loop
        for (Long64_t jEntry = 0; jEntry < data.GetEntriesFast(); jEntry++)
        {
            if (jEntry % 1000 == 0)
            {
                fprintf(stderr, "Processing event %lli of %lli\n", jEntry+1, data.GetEntriesFast());
            }

            //-------------------
            // void some variable
            // clear Tree vector for each event
            //-------------------
            v_met.clear();
            v_met_lepdeltaPhi.clear();
            //v_passJetindex.clear();
            v_passJetEta.clear();
            v_passJetPt.clear();
            v_passJetCSV.clear();
            v_passjethadronflavor.clear();
            v_passJetIndex.clear();
            //v_passjetpartonflavor.clear();
            v_nTrack.clear();
            v_TrackPT.clear();
            v_TrackEta.clear();
            v_Trackdr.clear();
            //v_Trackindex.clear();
            v_Trackdz.clear();
            v_Trackdzerror.clear();
            v_Trackdxy.clear();
            v_Trackdxyerror.clear();
            v_TrackStatus.clear();
            v_TrackHighPurity.clear();
            
            v_emergingTrackPT.clear();
            v_emergingTrackEta.clear();
            v_emergingTrackPhi.clear();
            v_emergingTrackAK4jetdR.clear();

            /*v_goodElePx.clear();
            v_goodElePy.clear();
            v_goodElePz.clear();
            v_goodEleE.clear();
            v_goodMuPx.clear();
            v_goodMuPy.clear();
            v_goodMuPz.clear();
            v_goodMuE.clear();
            v_goodTauPx.clear();
            v_goodTauPy.clear();
            v_goodTauPz.clear();
            v_goodTauE.clear();*/
            
            data.GetEntry(jEntry);
            nTotal ++;

            Float_t mcWeight = data.GetFloat("mcweight");
            Double_t eventWeight = mcWeight;
            if (eventWeight > 0)
                {
                    eventWeight = 1;
                }
            else if (eventWeight < 0)
                {
                    eventWeight = -1;
                }
            else
                {
                    eventWeight = 1;
                }

            //---------------------------
            // Get Total event number
            //---------------------------
            //h_totevent = static_cast<TH1F*>(file->Get("h_totevent"));
            //h_totevent->Fill(1.0, eventWeight);

            // Generator-Level
            bool matchee = false;
            vector<TLorentzVector> myEles;
            vector<TLorentzVector> dquark;
            vector<TLorentzVector> chi2s;
            dquark.clear();
            myEles.clear();
            chi2s.clear();
            // 0. check the generator-level information and make sure there is a Z->e+e-
            Long64_t nGenPar = data.GetLong64("st_nGenPar");
            Float_t* genParPx = data.GetPtrFloat("st_genParPx");
            Float_t* genParPy = data.GetPtrFloat("st_genParPy");
            Float_t* genParPz = data.GetPtrFloat("st_genParPz");
            Float_t* genParE = data.GetPtrFloat("st_genParEnergy");
            Int_t *genParId = data.GetPtrInt("st_genParId");
            Int_t *genParSt = data.GetPtrInt("st_genParSt");
            Int_t *genMomParId = data.GetPtrInt("st_genMomParId");
            for (int ig = 0; ig < nGenPar; ig++)
            {
                TLorentzVector *thisGen = new TLorentzVector(genParPx[ig], genParPy[ig], genParPz[ig], genParE[ig]);
                int pid = genParId[ig];
                int mompid = genMomParId[ig];
                int status = genParSt[ig];
                if (abs(pid) == 11 && mompid == 23) //ele = 11 and Z is 23
                {
                    matchee = true;
                    myEles.push_back(*thisGen);
                }
                // chi2Id 18
                if (abs(pid) == 1 && abs(mompid) == 18) //dquark is 1 and chi2 is 18
                {
                    dquark.push_back(*thisGen);
                }
                if (abs(pid) == 18)
                {
                    chi2s.push_back(*thisGen);
                }
            }
            /*gen_dquarknumb->Fill(dquark.size(), eventWeight);
            gen_chi2numb->Fill(chi2s.size(), eventWeight);
            gen_eenumber->Fill(myEles.size(), eventWeight);*/

            if (matchee)
            {
                //h_genee_event->Fill(1.0, eventWeight);

                // Starting reco-level
                // 1. electron
                Long64_t nEle = data.GetLong64("st_nEle");
                Float_t* elePx = data.GetPtrFloat("st_elePx");
                Float_t* elePy = data.GetPtrFloat("st_elePy");
                Float_t* elePz = data.GetPtrFloat("st_elePz");
                Float_t* eleEnergy = data.GetPtrFloat("st_eleEnergy");
                //vector<bool>& eleIsPassLoose = *((vector<bool>*)data.GetPtr("eleIsPassLoose"));
                vector<bool>& eleIsPassMedium = *((vector<bool>*)data.GetPtr("st_eleIsPassMedium"));
                //vector<bool>& eleIsPassVeto = *((vector<bool>*)data.GetPtr("eleIsPassVeto"));

                vector<TLorentzVector> goodElectrons;
                goodElectrons.clear();
                
                for (int ie = 0; ie < nEle; ie++)
                {
                    //nEleBefore ++;
                    TLorentzVector* myEle = new TLorentzVector(elePx[ie], elePy[ie], elePz[ie], eleEnergy[ie]);
                    if (!eleIsPassMedium[ie])
                    {
                        continue;
                    }
                    goodElectrons.push_back(*myEle);
                    
                    /*h_goodElePt->Fill(myEle->Pt(), eventWeight);
                    h_goodEleMass->Fill(myEle->M(), eventWeight);
                    h_goodEleEta->Fill(myEle->Eta(), eventWeight);
                    h_goodElePhi->Fill(myEle->Phi(), eventWeight);

                    f_goodElePt = myEle->Pt();
                    f_goodEleMass = myEle->M();
                    f_goodEleEta = myEle->Eta();
                    f_goodElePhi = myEle->Phi();
                    v_goodElePx.push_back(elePx[ie]);
                    v_goodElePy.push_back(elePy[ie]);
                    v_goodElePz.push_back(elePz[ie]);
                    v_goodEleE.push_back(eleEnergy[ie]);
                    */
                } //End of Ele Loop

                //h_ele_n->Fill(goodElectrons.size(), eventWeight);

                sort(goodElectrons.begin(), goodElectrons.end(), pt_greater);
                
                // 2. Muon
                Long64_t nMu = data.GetLong64("st_nMu");
                Float_t* muPx = data.GetPtrFloat("st_muPx");
                Float_t* muPy = data.GetPtrFloat("st_muPy");
                Float_t* muPz = data.GetPtrFloat("st_muPz");
                Float_t* muEnergy = data.GetPtrFloat("st_muEnergy");
                //vector<bool>& isTightMuon = *((vector<bool>*)data.GetPtr("isTightMuon"));
                //vector<bool>& isSoftLooseIsoMuon = *((vector<bool>*)data.GetPtr("st_isSoftLooseMuon"));
                vector<bool>& isSoftLooseIsoMuon = *((vector<bool>*)data.GetPtr("st_isSoftLooseIsoMuon"));
                //Int_t *muTrkLayers = data.GetPtrInt("muTrkLayers"); //can't find this information in the Tuples

                vector<TLorentzVector> goodmuons;
                goodmuons.clear();

                for (int imu = 0; imu < nMu; imu++)
                {
                    TLorentzVector* myMu = new TLorentzVector(muPx[imu], muPy[imu], muPz[imu], muEnergy[imu]);
                    //if (muTrkLayers[imu] < 5) continue;
                    if (!isSoftLooseIsoMuon[imu])
                    {
                        continue;
                    }

                    goodmuons.push_back(*myMu);
                    
                    /*h_goodMuPt->Fill(myMu->Pt(), eventWeight);
                    h_goodMuMass->Fill(myMu->M(), eventWeight);
                    h_goodMuEta->Fill(myMu->Eta(), eventWeight);
                    h_goodMuPhi->Fill(myMu->Phi(), eventWeight);

                    f_goodMuPt = myMu->Pt();
                    f_goodMuMass = myMu->M();
                    f_goodMuEta = myMu->Eta();
                    f_goodMuPhi = myMu->Phi();
                    v_goodMuPx.push_back(muPx[imu]);
                    v_goodMuPy.push_back(muPy[imu]);
                    v_goodMuPz.push_back(muPz[imu]);
                    v_goodMuE.push_back(muEnergy[imu]);*/
                } //End of Muon Loop

                //h_mu_n->Fill(goodmuons.size(), eventWeight);

                bool recoeeEvent = false;
                bool recouuEvent = false;
                if (goodmuons.size() == goodElectrons.size())
                {
                    continue;
                }
                if (goodElectrons.size() >= 2 && goodmuons.size() < 2)
                {
                    recoeeEvent = true;
                }
                if (goodmuons.size() >= 2 && goodElectrons.size() < 2)
                {
                    recouuEvent = true;
                }

                if (recoeeEvent)
                {
                    //h_recoee_event->Fill(1, eventWeight);
                    h_ee_npass->Fill(1, eventWeight);
                    h_ee_npass_noweight->Fill(1);

                    // 3. Good Vertex
                    Long64_t nVtx = data.GetLong64("st_nVtx");
                    if (nVtx < 1)
                        continue;
                    //h_recoee_Vtxpass->Fill(1, eventWeight);
                    h_ee_npass->Fill(2, eventWeight);
                    h_ee_npass_noweight->Fill(2);

                    // 4. Tau Veto
                    //Long64_t nTau_DRBased_EleVeto = data.GetLong64("st_nTau_DRBased_EleMuVeto");
                    Long64_t nTau_DRBased_EleVeto = data.GetLong64("st_nTau_DRBased_EleVeto");
                    if (nTau_DRBased_EleVeto > 0)
                        continue;
                    //h_recoee_vetoTau->Fill(1, eventWeight);
                    h_ee_npass->Fill(3, eventWeight);
                    h_ee_npass_noweight->Fill(3);

                    // 6. Z boson
                    if (goodElectrons[0].Pt() < 25 && goodElectrons[1].Pt() < 20)
                    {
                        continue;
                    }
                    h_ee_npass->Fill(4, eventWeight);
                    h_ee_npass_noweight->Fill(4);
                    //h_recoee_eePtpass->Fill(1, eventWeight);

                    float PDGZmass = 91.1876;
                    TLorentzVector dilep = goodElectrons[0] + goodElectrons[1];
                    float dilepMass = dilep.M(); 
                    float deltaMass = abs(PDGZmass - dilepMass);
                    if (deltaMass > 15)
                    {
                        continue;
                    }
                    h_ee_npass->Fill(5, eventWeight);
                    h_ee_npass_noweight->Fill(5);
                    //h_recoee_deltaMasspass->Fill(1, eventWeight);
            
                    //----------------------
                    // To reduce diboson case (veto extra leptons)
                    //----------------------
                    if (goodElectrons.size() > 2 || goodmuons.size() != 0)
                    {
                        continue;
                    }
                    //h_Zboson_n->Fill(1, eventWeight);
                    h_ee_npass->Fill(6, eventWeight);
                    h_ee_npass_noweight->Fill(6);
                    nZboson ++;

                    TLorentzVector Zboson = goodElectrons[0] + goodElectrons[1];
                    float Zboson_pt = Zboson.Pt();
                    float Zboson_mass = Zboson.M();
                    float Zboson_eta = Zboson.Eta();
                    float Zboson_phi = Zboson.Phi();

                    // 6. Thin Jet
                    Long64_t nTHINjets = data.GetLong64("st_THINnJet");
                    Float_t* THINjetPx = data.GetPtrFloat("st_THINjetPx");
                    Float_t* THINjetPy = data.GetPtrFloat("st_THINjetPy");
                    Float_t* THINjetPz = data.GetPtrFloat("st_THINjetPz");
                    Float_t* THINjetEnergy = data.GetPtrFloat("st_THINjetEnergy");
                    Int_t* THINjetHadronFlavor = data.GetPtrInt("st_THINjetHadronFlavor");
                    Float_t* THINjetCSV = data.GetPtrFloat("st_THINjetCISVV2");
                    /*vector<int> indexForPassAK4;
                    indexForPassAK4.clear();
                    vector<TLorentzVector> goodTHINjets;
                    goodTHINjets.clear();

                    if (nTHINjets < 2)
                        continue;

                    for (int itj = 0; itj < nTHINjets; itj++)
                    {
                        TLorentzVector* thisTHINjet = new TLorentzVector(THINjetPx[itj], THINjetPy[itj], THINjetPz[itj], THINjetEnergy[itj]);
                        goodTHINjets.push_back(*thisTHINjet);
                    } // End of nTHINjets loop

                    // Sort goodTHINjets by pT
                    sort(goodTHINjets.begin(), goodTHINjets.end(), pt_greater);
                    
                    //h_recoee_nAK4pass->Fill(1, eventWeight); //add this -> write in tree
                    h_ee_npass->Fill(7, eventWeight);
                    h_ee_npass_noweight->Fill(7);*/
                    h_Zpt_after_preselection_bkg->Fill(Zboson.Pt(), eventWeight);
                    //nEventAfterAllPreselection ++;


                    //APPLYING SEVERAL Z PT CUTS//
                    //declare the h_low_Zpt, h_med_Zpt, and h_high_Zpt first
                    h_before_Zpt_bkg->Fill(1, eventWeight);

                    int cutvalue = 0;
                    int discrepancy = 10;
                    //int tempvalue = 0;

                    while (cutvalue<1000)
                    {
                        cutvalue+=discrepancy;
                        //tempvalue+=1;
                        if (Zboson.Pt() > cutvalue)
                            //h_after_Zpt_scan_bkg->Fill(tempvalue, eventWeight);
                            h_after_Zpt_scan_bkg->Fill(cutvalue, eventWeight);
                    }

                    if (Zboson.Pt() > 50)
                        h_after_Zpt50_bkg->Fill(1, eventWeight);

                    if (Zboson.Pt() > 60)
                        h_after_Zpt60_bkg->Fill(1, eventWeight);

                    if (Zboson.Pt() >= 100)
                        h_after_Zpt100_bkg->Fill(1, eventWeight);


                    //---------------------------
                    // Study match Jet's tracks of Thin Jets
                    //---------------------------
                    Int_t* THINjetNTracks = data.GetPtrInt("st_THINjetNTracks");
                    //vector<float>* THINjetTrackImpdz = data.GetPtrVectorFloat("THINjetTrackImpdz", nTHINjets);
                    //vector<float>* THINjetTrackImpdzError = data.GetPtrVectorFloat("THINjetTrackImpdzError", nTHINjets);
                    //vector<float>* THINjetTrackImpdxy = data.GetPtrVectorFloat("THINjetTrackImpdxy", nTHINjets);
                    //vector<float>* THINjetTrackImpdxyError = data.GetPtrVectorFloat("THINjetTrackImpdxyError", nTHINjets);
                    vector<float>* THINjetTrackPt = data.GetPtrVectorFloat("st_THINjetTrackPt");//, nTHINjets);
                    vector<float>* THINjetTrackEta = data.GetPtrVectorFloat("st_THINjetTrackEta");//, nTHINjets);
                    vector<float>* THINjetTrackPhi = data.GetPtrVectorFloat("st_THINjetTrackPhi");//, nTHINjets);
                    vector<int>* THINjetTrackStatus = data.GetPtrVectorInt("st_THINjetTrackStatus");//, nTHINjets);
                    //vector<int>* THINjetTrackHighPurity = data.GetPtrVectorInt("THINjetTrackHighPurity", nTHINjets);
                    vector<float>* THINjet3Dsignificance = data.GetPtrVectorFloat("THINjet3Dsignificance");//, nTHINjets);
                    vector<float>* THINjetLog10_of_3Dsignificance = data.GetPtrVectorFloat("THINjetLog10_of_3Dsignificance");//, nTHINjets);
                    
                    Float_t* THINjetAlpha3D = data.GetPtrFloat("THINjetAlpha3D");
                    Float_t* THINjetSumTrackPt = data.GetPtrFloat("THINjetSumTrackPt");
                    Float_t* THINjetSumTrackPt_cutLog10_3Dsig = data.GetPtrFloat("THINjetSumTrackPt_cutLog10_3Dsig");

                    int emergTrackMultiplicity;
                    vector<float> temp_emergTrackPt;
                    vector<float> temp_emergTrackEta;
                    vector<float> temp_emergTrackPhi;
                    vector<float> temp_emergTrackdR;
                    

                    for (int iak4j = 0; iak4j < nTHINjets; iak4j++)
                    {
                        temp_emergTrackPt.clear();
                        temp_emergTrackEta.clear();
                        temp_emergTrackPhi.clear();
                        temp_emergTrackdR.clear();

                        //if (THINjetAlpha3D[iak4j] < 0.1)
                        //{
                            emergTrackMultiplicity = THINjetNTracks[iak4j];
                            for (int it = 0; it < THINjetNTracks[iak4j]; it++)
                            {
                                float emergTrackPt = THINjetTrackPt[iak4j][it];
                                float emergTrackEta = THINjetTrackEta[iak4j][it];
                                float emergTrackPhi = THINjetTrackPhi[iak4j][it];
                                TLorentzVector* thisJet = new TLorentzVector(THINjetPx[iak4j], THINjetPy[iak4j], THINjetPz[iak4j], THINjetEnergy[iak4j]);
                                float ak4JetEta = thisJet->Eta();
                                float ak4JetPhi = thisJet->Phi();
                                float deta = ak4JetEta - emergTrackEta;
                                float dphi = ak4JetPhi - emergTrackPhi;
                                while (dphi > TMath::Pi())
                                    dphi -= 2 * TMath::Pi();
                                while (dphi <= -TMath::Pi())
                                    dphi += 2 * TMath::Pi(); 
                                float dR = sqrt(deta * deta + dphi * dphi);
                                //save dR in vector
                                temp_emergTrackPt.push_back(emergTrackPt);
                                temp_emergTrackEta.push_back(emergTrackEta);
                                temp_emergTrackPhi.push_back(emergTrackPhi);
                                temp_emergTrackdR.push_back(dR);
                            }
                            v_emergingTrackPT.insert(v_emergingTrackPT.end(), temp_emergTrackPt.begin(), temp_emergTrackPt.end());
                            v_emergingTrackEta.insert(v_emergingTrackEta.end(), temp_emergTrackEta.begin(), temp_emergTrackEta.end());
                            v_emergingTrackPhi.insert(v_emergingTrackPhi.end(), temp_emergTrackPhi.begin(), temp_emergTrackPhi.end());
                            v_emergingTrackAK4jetdR.insert(v_emergingTrackAK4jetdR.end(), temp_emergTrackdR.begin(), temp_emergTrackdR.end());
                        //}
                        //else
                        //    continue;
                    } //end of ak4jet loop for emerg track (emerg jet)
                    

                    

                    //Store the result in outputfile (root)
                    //---------------------------
                    //  Fill Tree event variable
                    //---------------------------
                    I_event = jEntry;
                    I_weight = eventWeight;
                    ULong64_t eventId = data.GetLong64("st_eventId");
                    I_eventID = eventId;
                    //f_dileptonPT = dilepPt;
                    //f_dileptonMass = dilepMass;
                    f_ZbosonPt = Zboson_pt;
                    f_ZbosonMass = Zboson_mass;
                    f_ZbosonEta = Zboson_eta;
                    f_ZbosonPhi = Zboson_phi;
                    //f_HT = HT;
                    //f_Met = met;
                    //I_nThinJets = indexForPassAK4.size();
                    tree->Fill();
                } // End of recoeeEvent Loop
            }     // End of matchee (gen ee)
        }         // End of Event Entries loop
    } //end of flist

    // out Tree branches
    //TFile *outFile = new TFile(outputfile.c_str(), "RECREATE");
    TFile *outFile = TFile::Open(outputfile, "RECREATE");
    outFile->cd();
    tree->Write();
    h_total_mcweight_new->Write();
    h_Zpt_after_preselection_bkg->Write();
    h_before_Zpt_bkg->Write();
    h_after_Zpt50_bkg->Write();
    h_after_Zpt60_bkg->Write();
    h_after_Zpt100_bkg->Write();
    h_after_Zpt_scan_bkg->Write();
    h_ee_npass->Write();
    h_ee_npass_noweight->Write();
    //outFile->mkdir("Event_Variable", "Event_Variable")->cd();
    //h_totevent->Write();
    //h_genee_event->Write();
    //h_recoee_event->Write();
    //h_ele_n->Write();
    //h_mu_n->Write();
    //h_tau_n->Write();
    //h_Zboson_n->Write();
    //h_recoee_METpass->Write();
    //h_recoee_ZbosonPtpass->Write();
    //h_recoee_nAK4pass->Write();
    //Z_eemass->Write();
    //h_ee_npass->Write();
    //gen_chi2numb->Write();
    //gen_dquarknumb->Write();
    //gen_eenumber->Write();
    //match_dquarknumb->Write();
    //h_HT_eventCout->Write();
    //outFile->cd("/");
    //outFile->mkdir("Jet_Variable", "Jet_Variable")->cd();
    //h_jet_rank->Write();
    //h_jet_n->Write();
    //outFile->cd("/");
    //outFile->mkdir("Track_Variable", "Track_Variable")->cd();
    //ratioTrackInferror->Write();
    //outFile->cd("/");
    /*outFile->mkdir("Kinematics_Variable_afterEachLeptonSelection", "Kinematics_Variable_afterEachLeptonSelection")->cd();
    h_goodElePt->Write();
    h_goodEleMass->Write();
    h_goodEleEta->Write();
    h_goodElePhi->Write();
    h_goodMuPt->Write();
    h_goodMuMass->Write();
    h_goodMuEta->Write();
    h_goodMuPhi->Write();
    //h_goodTauPt->Write();
    //h_goodTauMass->Write();
    //h_goodTauEta->Write();
    //h_goodTauPhi->Write();
    outFile->cd("/");*/
    outFile->Close();
    //outFile->Write();
    cout << "output written to " << outputfile << endl;
    //end = time.clock()
    //print "%.4gs" % (end-start)



    /*cout << "nEleBefore = " << nEleBefore << "\n";
    cout << "nEleAfter = " << nEleAfter << "\n";
    cout << "nEleAfterEventWeighted = " << nEleAfterEventWeighted << "\n";
    cout << "nZboson = " << nZboson << "\n";
    cout << "nEventAfterAllPreselection = " << nEventAfterAllPreselection << "\n";

    //--------------------//
    //Calculate efficiency//
    //-------------------//
    float ntotevent = h_totevent->Integral();
    cout << "\nnumber of weighted totEvents = " << ntotevent << "\n";
    float ngeneeEvent = h_genee_event->Integral();
    cout << "number of weighted gen-level ee Events = " << ngeneeEvent << "\n";
    cout << "efficiency of weighted gen-lev ee compared with totEvents:" << "\n";
    efferr(ngeneeEvent, ntotevent);
    
    float nrecoee_event = h_recoee_event->Integral();
    cout << "\nnumber of weighted reco-level ee Events = " << nrecoee_event << "\n";
    //cout << "efficiency of weighted reco-lev ee compared to totEvents:" << "\n";
    //efferr(nrecoee_event, ntotevent);
    cout << "efficiency of weighted reco-lev ee compared to gen-lev ee:" << "\n";
    efferr(nrecoee_event, ngeneeEvent);
    
    float nrecoee_event_Vtxpass = h_recoee_Vtxpass->Integral();
    cout << "\nnumber of weighted Events with nVtx > 1 = " << nrecoee_event_Vtxpass << "\n";
    cout << "efficiency of weighted Events passing nVtx compared to gen-lev ee:" << "\n";
    efferr(nrecoee_event_Vtxpass, ngeneeEvent);
    //cout << "efficiency of weighted Events passing nVtx compared to reco-lev ee:" << "\n";
    //efferr(nrecoee_event_Vtxpass, nrecoee_event);

    float nrecoee_event_tauVeto = h_recoee_vetoTau->Integral();
    cout << "\nnumber of weighted Events after vetoing tau = " << nrecoee_event_tauVeto << "\n";
    cout << "efficiency of weighted Events after vetoing tau compared to gen-lev ee:" << "\n";
    efferr(nrecoee_event_tauVeto, ngeneeEvent);
    //cout << "efficiency of weighted Events after vetoing tau compared to reco-lev ee:" << "\n";
    //efferr(nrecoee_event_tauVeto, nrecoee_event);

    //float nrecoee_event_METcut = h_recoee_METpass->Integral();
    //cout << "number of weighted Events after MET cut = " << nrecoee_event_METcut << "\n";
    //cout << "efficiency of weighted Events after MET cut compared to gen-lev ee:" << "\n";
    //efferr(nrecoee_event_METcut, ngeneeEvent);

    float nrecoee_event_eePtpass = h_recoee_eePtpass->Integral();
    cout << "\nnumber of weighted Events after Pt selection = " << nrecoee_event_eePtpass << "\n";
    cout << "efficiency of weighted Events passing Pt selection compared to gen-lev ee:" << "\n";
    efferr(nrecoee_event_eePtpass, ngeneeEvent);
    //cout << "efficiency of weighted Events passing Pt selection compared to reco-lev ee:" << "\n";
    //efferr(nrecoee_event_eePtpass, nrecoee_event);

    float nrecoee_event_deltaMasspass = h_recoee_deltaMasspass->Integral();
    cout << "\nnumber of weighted Events after deltaMassZ selection = " << nrecoee_event_deltaMasspass << "\n";
    cout << "efficiency of weighted Events passing deltaMassZ selection compared to gen-lev ee:" << "\n";
    efferr(nrecoee_event_deltaMasspass, ngeneeEvent);
    //cout << "efficiency of weighted Events passing deltaMassZ selection compared to reco-lev ee:" << "\n";
    //efferr(nrecoee_event_deltaMasspass, nrecoee_event);

    float nZbosonEvent = h_Zboson_n->Integral();
    cout << "\nnumber of weighted Events after vetoing extra leptons (nZboson Events) = " << nZbosonEvent << "\n";
    //efficiency of Z boson
    cout << "efficiency of weighted Events after vetoing extra leptons (nZboson Events) compared to gen-lev ee:" << "\n";
    efferr(nZbosonEvent, ngeneeEvent);
    //cout << "efficiency of weighted Events after vetoing extra leptons (nZboson Events) compared to reco-lev ee:" << "\n";
    //efferr(nZbosonEvent, nrecoee_event);

    //float nZbosonEventAfterZPtCut = h_recoee_ZbosonPtpass->Integral();
    //cout << "number of weighted Events after Z Pt Cut (nZboson Events) = " << nZbosonEventAfterZPtCut << "\n";
    //efficiency of Z boson
    //cout << "efficiency of weighted Events after Z Pt Cut (nZboson Events) compared to gen-lev ee:" << "\n";
    //efferr(nZbosonEventAfterZPtCut, ngeneeEvent);

    float nrecoee_events_nAK4pass = h_recoee_nAK4pass->Integral();
    cout << "\nnumber of weighted Events with AK4 >= 2 = " << nrecoee_events_nAK4pass << "\n";
    cout << "efficiency of weighted Events with AK4 >= 2 compared to gen-lev ee:" << "\n";
    efferr(nrecoee_events_nAK4pass, ngeneeEvent);
    //cout << "efficiency of weighted Events with AK4 >= 2 compared to reco-lev ee:" << "\n";
    //efferr(nrecoee_events_nAK4pass, nrecoee_event);*/


    
    

    
} // big end
//Store the result in outputfile (root)