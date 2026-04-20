// reco_calo.cpp
//
// Standalone executable that runs the RecoCalo reconstruction.
//
// Usage:
//   reco_calo <input_digi.root> <output_reco.root>
//
// Compile (example, adjust ROOT flags as needed):
//   g++ -o reco_calo reco_calo.cpp RecoCalo.cc \
//       $(root-config --cflags --libs) \
//       -lROOTNTuple -std=c++17

#include "RecoCalo.hh"
#include "ConfigReader.h"
#include "TFile.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: reco_calo <input_digi.root> <output_reco.root> [calo.cfg]\n";
        return 1;
    }

    const std::string input_path  = argv[1];
    const std::string output_path = argv[2];
    const std::string cfg_path    = (argc == 4) ? argv[3] : "";

    TFile* f_digi = TFile::Open(input_path.c_str(), "READ");
    if (!f_digi || f_digi->IsZombie()) {
        std::cerr << "[reco_calo] ERROR: cannot open input file: " << input_path << "\n";
        return 1;
    }
    std::cout << "[reco_calo] Input:  " << input_path  << "\n";
    std::cout << "[reco_calo] Output: " << output_path << "\n";

    RecoCalo reco(f_digi);
    reco.SetOutputFile(output_path);

    if (!cfg_path.empty()) {
        std::cout << "[reco_calo] Config: " << cfg_path << "\n";
        CalorimeterConfig cfg = readConfigFile(cfg_path);
        reco.BuildGeometryFromConfig(cfg);
    } else {
        std::cout << "[reco_calo] WARNING: no config file provided — gap veto disabled\n";
    }

    reco.SetDebug(true);
    reco.SetDebugFile("debug_clusters.root");
    reco.Loop();

    f_digi->Close();
    delete f_digi;

    std::cout << "[reco_calo] Finished.\n";
    return 0;
}
