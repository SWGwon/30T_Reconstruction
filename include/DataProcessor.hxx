#ifndef DATAPROCESSOR_HXX
#define DATAPROCESSOR_HXX

#include <TTree.h>
#include <TH1.h>
#include <TH2.h>

class DataProcessor {
    public:
        DataProcessor(TTree* tree);
        void Process();
        void SaveResults(const std::string& filename);

        void SetEntryDenominator(int entryDenominator) {this->entryDenominator = entryDenominator;};
        int GetEntryDenominator() {return this->entryDenominator;};

        void SetTimeBinCount(int timeBinCount) {this->timeBinCount = timeBinCount;};
        int GetTimeBinCount() {return this->timeBinCount;};

        void SetPmtBinCount(int pmtBinCount) {this->pmtBinCount = pmtBinCount;};
        int GetPmtBinCount() {return this->pmtBinCount;};

        void SetEnergyBinCount(int energyBinCount) {this->energyBinCount = energyBinCount;};
        int GetEnergyBinCount() {return this->energyBinCount;};

        void SetMaxEnergy(int maxEnergy) {this->maxEnergy = maxEnergy;};
        int GetMaxEnergy() {return this->maxEnergy;};

        void InitializeHistograms();

        void PrintProgressBar(int width, double percentage);

    private:
        TTree* tree;
        int entryDenominator = 1;
        int timeBinCount = 36;
        int pmtBinCount = 36;
        int energyBinCount = 100;
        int maxEnergy = 4000;

        std::vector<std::vector<TH1D*>> histE;
        std::vector<TH2D*> histTimePMTID;
        TH2D* histTimePMTIDAll;
};

#endif // DATAPROCESSOR_HXX

