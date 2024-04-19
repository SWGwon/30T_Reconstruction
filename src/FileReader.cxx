#include "FileReader.hxx"
#include <iostream>

FileReader::FileReader(const std::string& rootFileName) : rootFileName(rootFileName), rootFile(nullptr) {}

FileReader::~FileReader() {}

bool FileReader::open() {
    rootFile = std::make_unique<TFile>(rootFileName.c_str(), "READ");
    if (!rootFile || rootFile->IsZombie()) {
        throw std::runtime_error("File '" + rootFileName + "' not opened or is corrupted");
    }
    return true;
}

TTree* FileReader::getTree(const std::string& treeName) {
    if (!rootFile || rootFile->IsZombie()) {
        throw std::runtime_error("File '" + rootFileName + "' not opened or is corrupted");
    }

    TTree* tree = (TTree*)rootFile->Get(treeName.c_str());
    if (!tree) {
        throw std::runtime_error("TTree '" + treeName + "' not found in file '" + rootFileName + "'");
    }

    return tree;
}
