#include <HistogramFitter.hxx>
#include <Math/MinimizerOptions.h>


HistogramFitter::HistogramFitter() : fitFunction(nullptr) {}

HistogramFitter::~HistogramFitter() {
    // Clean up any created fit functions
    delete fitFunction;
}

void HistogramFitter::InitializeHistograms() {
    this->histE.resize(this->pmtBinCount);
    for (auto& row : this->histE) {
        row.resize(this->timeBinCount, nullptr);
    }

}

void HistogramFitter::SetHistogramAxis(int pmtBinCount, int timeBinCount) {
    this->pmtBinCount = pmtBinCount;
    this->timeBinCount = timeBinCount;
}

void HistogramFitter::SetHistograms() {
    for (int i = 0; i < this->pmtBinCount; ++i) {
        for (int j = 0; j < this->timeBinCount; ++j) {
            this->histE[i][j] = (TH1D*)this->inputFile->Get(Form("energy_dist_of_%d_%d",i,j));
        }
    }
    this->histTimePMTIDChi2 = new TH2D("histTimePMTIDChi2", "histTimePMTIDChi2;PMT ID;PE time", this->pmtBinCount,0,this->pmtBinCount, this->timeBinCount,0,this->timeBinCount);
}

bool HistogramFitter::FitHistograms(TF1* func, const char* options, double xMin, double xMax) {
    //ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2", "Minimize");
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2", "Simplex");
    ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);

    for (int i = 0; i < this->pmtBinCount; ++i) {
        for (int j = 0; j < this->timeBinCount; ++j) {
            func->SetParameters(this->histE[i][j]->GetMaximum(), 500, 150, 0, 0, 0, 0); // 초기 파라미터 추정
            func->SetParLimits(0, 0, this->histE[i][j]->GetMaximum());
            //func->SetParLimits(2, 100, 200);
            func->SetParNames("Amplitude", "Mean", "Sigma", "Const", "Linear", "Quadratic", "Cubic");
            this->histE[i][j]->Fit(func, "RSVQ");
            double chi2 = func->GetChisquare();
            double ndf = func->GetNDF();
            this->histTimePMTIDChi2->SetBinContent(i+1, j+1, chi2/ndf);
            std::cout << "i: " << i << ", j: " << j << std::endl;
            std::cout << "Chi2: " << chi2 << std::endl;
            std::cout << "NDF (Number of Degrees of Freedom): " << ndf << std::endl;
            std::cout << "Chi2/NDF: " << chi2 / ndf << std::endl;
        }
    }
}

void HistogramFitter::SaveResults(const std::string& filename) {
    TFile* outputFile = new TFile(filename.c_str(), "RECREATE");

    std::cout << "writing histograms...";
    for (int i = 0; i < this->pmtBinCount; ++i) {
        for (int j = 0; j < this->timeBinCount; ++j) {
            this->histE[i][j]->Write();
        }
    }
    this->histTimePMTIDChi2->Write();
    std::cout << "done" << std::endl;
}

