#include <TH1D.h>
#include <TF1.h>
#include <string>
#include <iostream>

class HistogramFitter {
    public:
        HistogramFitter();
        ~HistogramFitter();

        void setHistogram(TH1D* histogram);
        bool fitHistogram(const char* formula, const char* options = "", double xMin = 0, double xMax = 0);
        void printFitResults() const;

    private:
        TH1D* histogram;
        TF1* fitFunction;
};
