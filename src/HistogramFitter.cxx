#include <HistogramFitter.hxx>

HistogramFitter::HistogramFitter() : histogram(nullptr), fitFunction(nullptr) {}

HistogramFitter::~HistogramFitter() {
    // Clean up any created fit functions
    delete fitFunction;
}

void HistogramFitter::setHistogram(TH1D* histogram) {
    this->histogram = histogram;
}

bool HistogramFitter::fitHistogram(const char* formula, const char* options, double xMin, double xMax) {
    if (!histogram) {
        std::cerr << "Error: No histogram set for fitting." << std::endl;
        return false;
    }

    // Create the fit function based on the formula
    if (fitFunction) delete fitFunction;
    fitFunction = new TF1("fitFunction", formula, xMin, xMax);

    // Perform the fit
    int status = histogram->Fit(fitFunction, options);

    // Check if fit was successful
    if (status != 0) {
        std::cerr << "Fitting failed with status " << status << std::endl;
        return false;
    }
    return true;
}

void HistogramFitter::printFitResults() const {
    if (fitFunction && histogram) {
        std::cout << "Fit Results: " << std::endl;
        fitFunction->Print();
    } else {
        std::cout << "No fit results available." << std::endl;
    }
}

