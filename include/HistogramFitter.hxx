#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TFile.h>
#include <string>
#include <iostream>

class HistogramFitter {
    public:
        HistogramFitter();
        ~HistogramFitter();

        void SetFile(TFile* inputFile) {this->inputFile = inputFile;};
        bool FitHistograms(TF1* func, const char* options = "", double xMin = 0, double xMax = 0);
        void SaveResults(const std::string& filename);
        void SetHistogramAxis(int pmtBinCount, int timeBinCount);
        void InitializeHistograms();
        void SetHistograms();

        TH1D* GetHistogram(int pmtBin, int timeBin) {return this->histE[pmtBin][timeBin];};

    private:
        TF1* fitFunction;
        TFile* inputFile;

        std::vector<std::vector<TH1D*>> histE;
        TH2D* histTimePMTIDChi2;

        int timeBinCount = 36;
        int pmtBinCount = 36;
};
