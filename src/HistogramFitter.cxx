#include <HistogramFitter.hxx>

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
}

bool HistogramFitter::fitHistogram(const char* formula, const char* options, double xMin, double xMax) {
}

void HistogramFitter::printFitResults() const {
}

