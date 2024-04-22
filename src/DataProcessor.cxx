#include "DataProcessor.hxx"
#include <iostream>
#include <TFile.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TF1.h>
#include <TH2.h>
#include <TFile.h>
#include <TH1D.h>
#include <TVector3.h>
#include <TROOT.h>
#include <TFitResult.h>
#include <TMath.h>
#include <TMinuit.h>
#include <Math/MinimizerOptions.h>

DataProcessor::DataProcessor(TTree* tree) : tree(tree) {}

void DataProcessor::Process() {
    std::cout << "start processing..." << std::endl;
    this->InitializeHistograms();
    int mcpdg; tree->SetBranchAddress("mcpdg", &mcpdg);
    double mcx; tree->SetBranchAddress("mcx", &mcx);
    double mcy; tree->SetBranchAddress("mcy", &mcy);
    double mcz; tree->SetBranchAddress("mcz", &mcz);
    double mcu; tree->SetBranchAddress("mcu", &mcu);
    double mcv; tree->SetBranchAddress("mcv", &mcv);
    double mcw; tree->SetBranchAddress("mcw", &mcw);
    double mcke; tree->SetBranchAddress("mcke", &mcke);
    double mct; tree->SetBranchAddress("mct", &mct);
    int evid; tree->SetBranchAddress("evid", &evid);
    int subev; tree->SetBranchAddress("subev", &subev);
    int nhits; tree->SetBranchAddress("nhits", &nhits);
    double triggerTime; tree->SetBranchAddress("triggerTime", &triggerTime);
    int mcparticlecount; tree->SetBranchAddress("mcparticlecount", &mcparticlecount);
    int mcpecount; tree->SetBranchAddress("mcpecount", &mcpecount);
    int mcnhits; tree->SetBranchAddress("mcnhits", &mcnhits);
    double scintEdep; tree->SetBranchAddress("scintEdep", &scintEdep);
    double scintEdepQuenched; tree->SetBranchAddress("scintEdepQuenched", &scintEdepQuenched);
    double scintPhotons; tree->SetBranchAddress("scintPhotons", &scintPhotons);
    double remPhotons; tree->SetBranchAddress("remPhotons", &remPhotons);
    double cherPhotons; tree->SetBranchAddress("cherPhotons", &cherPhotons);
    std::vector<int>* mcpdgs = nullptr; tree->SetBranchAddress("mcpdgs", &mcpdgs);
    std::vector<double>* mcxs = nullptr; tree->SetBranchAddress("mcxs", &mcxs);
    std::vector<double>* mcys = nullptr; tree->SetBranchAddress("mcys", &mcys);
    std::vector<double>* mczs = nullptr; tree->SetBranchAddress("mczs", &mczs);
    std::vector<double>* mcus = nullptr; tree->SetBranchAddress("mcus", &mcus);
    std::vector<double>* mcvs = nullptr; tree->SetBranchAddress("mcvs", &mcvs);
    std::vector<double>* mcws = nullptr; tree->SetBranchAddress("mcws", &mcws);
    std::vector<double>* mckes = nullptr; tree->SetBranchAddress("mckes", &mckes);
    std::vector<double>* mcts = nullptr; tree->SetBranchAddress("mcts", &mcts);
    std::vector<int>* hitPMTID = nullptr; tree->SetBranchAddress("hitPMTID", &hitPMTID);
    std::vector<double>* hitPMTTime = nullptr; tree->SetBranchAddress("hitPMTTime", &hitPMTTime);
    std::vector<double>* hitPMTCharge = nullptr; tree->SetBranchAddress("hitPMTCharge", &hitPMTCharge);
    std::vector<double>* hitPMTDigitizedTime = nullptr; tree->SetBranchAddress("hitPMTDigitizedTime", &hitPMTDigitizedTime);
    std::vector<double>* hitPMTDigitizedCharge = nullptr; tree->SetBranchAddress("hitPMTDigitizedCharge", &hitPMTDigitizedCharge);
    std::vector<int>* hitPMTNCrossings = nullptr; tree->SetBranchAddress("hitPMTNCrossings", &hitPMTNCrossings);
    std::vector<int>* mcPMTID = nullptr; tree->SetBranchAddress("mcPMTID", &mcPMTID);
    std::vector<int>* mcPMTNPE = nullptr; tree->SetBranchAddress("mcPMTNPE", &mcPMTNPE);
    std::vector<double>* mcPETime = nullptr; tree->SetBranchAddress("mcPETime", &mcPETime);
    std::vector<int>* mcPEProcess = nullptr; tree->SetBranchAddress("mcPEProcess", &mcPEProcess);
    std::vector<double>* mcPEWavelength = nullptr; tree->SetBranchAddress("mcPEWavelength", &mcPEWavelength);
    std::vector<double>* mcPEx = nullptr; tree->SetBranchAddress("mcPEx", &mcPEx);
    std::vector<double>* mcPEy = nullptr; tree->SetBranchAddress("mcPEy", &mcPEy);
    std::vector<double>* mcPEz = nullptr; tree->SetBranchAddress("mcPEz", &mcPEz);

    int nEntries = tree->GetEntries()/this->entryDenominator;
    for (int i = 0; i < nEntries; i++) {
        tree->GetEntry(i);
        PrintProgressBar(50, i / static_cast<double>(nEntries));
        int tempEnergyBin = mcke/(this->maxEnergy/this->energyBinCount);
        for (int j = 0; j < mcPMTID->size(); ++j) {
            histTimePMTIDAll->Fill(mcPMTID->at(j), mcPETime->at(j));
            histTimePMTID[tempEnergyBin-1]->Fill(mcPMTID->at(j), mcPETime->at(j));
        }
    }
    std::cout << std::endl;

    //normalize 2D histograms
    for (int i = 0; i < this->energyBinCount; ++i) {
        if (histTimePMTID[i]->Integral() != 0)
            histTimePMTID[i]->Scale(1./histTimePMTID[i]->Integral(), "nosw2");
    }
    

    for (int i = 0; i < this->pmtBinCount; ++i) {
        for (int j = 0; j < this->timeBinCount; ++j) {
            for (int k = 0; k < this->energyBinCount; ++k) {
                this->histE[i][j]->SetBinContent(k+1, (this->histTimePMTID[k]->GetBinContent(i+1,j+1)));
            }
        }
    }
}

void DataProcessor::InitializeHistograms() {
    std::cout << "Initializing histograms..." << std::endl;
    this->histE.resize(this->pmtBinCount);
    for (auto& row : this->histE) {
        row.resize(this->timeBinCount, nullptr);
    }
    for (int i = 0; i < this->pmtBinCount; ++i) {
        for (int j = 0; j < this->timeBinCount; ++j) {
            this->histE[i][j] = new TH1D(Form("energy_dist_of_%d_%d",i,j), Form("energy_dist_of_%d_%d;incident mu- KE; #PE pmtid:%d, time:%dns",i,j,i+1,j), this->energyBinCount,0,this->maxEnergy);
        }
    }

    this->histTimePMTID.resize(this->energyBinCount);
    for (int i = 0; i < this->energyBinCount; ++i) {
        this->histTimePMTID[i] = new TH2D(Form("histTimePMTID_%fGeV", i*4./this->energyBinCount + 0.1), Form("histTimePMTID_%fGeV;PMT ID;PE time", i*4./this->energyBinCount + 0.1), this->pmtBinCount,0,this->pmtBinCount, this->timeBinCount,0,this->timeBinCount);
    }

    this->histTimePMTIDAll = new TH2D("histTimePMTID", "histTimePMTID_all_energy;PMT ID;PE time", this->pmtBinCount,0,this->pmtBinCount, this->timeBinCount,0,this->timeBinCount);
}

void DataProcessor::SaveResults(const std::string& filename) {
    TFile* outputFile = new TFile(filename.c_str(), "RECREATE");

    std::cout << "writing histograms..." << std::endl;
    this->histTimePMTIDAll->Write();
    for (int i = 0; i < this->energyBinCount; ++i)  {
        this->histTimePMTID[i]->Write();
    }
    for (int i = 0; i < this->pmtBinCount; ++i) {
        for (int j = 0; j < this->timeBinCount; ++j) {
            this->histE[i][j]->Write();
        }
    }
}

void DataProcessor::PrintProgressBar(int width, double percentage) {
    int pos = width * percentage;
    std::cout << "[";
    for (int i = 0; i < width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(percentage * 100.0) << " %\r";
    std::cout.flush();
}
