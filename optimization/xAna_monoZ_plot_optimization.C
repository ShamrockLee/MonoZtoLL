#include <iostream>
#include <memory>
#include <vector>

#include <ROOT/RDataFrame.hxx>
#include <ROOT/RResultPtr.hxx>
#include <TH1.h>

#define OPTPARSE_IMPLEMENTATION
#include "skeeto_optparse.h"

void xAna_monoZ_plot_optimization(std::string fileOut, std::vector<std::string> vFileIn, size_t nThread, int debug) {
    ROOT::RDataFrame dfIn("outTree", vFileIn);
    ROOT::RDF::RNode dfOut = dfIn.Define("unsigned_one", "1u");
    ROOT::RDF::RResultPtr<TH1D> histview_mcweight = dfOut.Histo1D({"h_pfMetCorrPt", "h_pfMetCorrPt", 1u, 0., 0.}, "unsigned_one", "mcweight");
    ROOT::RDF::RResultPtr<TH1D> histview_pfMetCorrPt = dfOut.Histo1D({"h_pfMetCorrPt", "h_pfMetCorrPt", 500u, 0., 500.}, "st_pfMetCorrPt", "mcweight");
    // See https://root.cern/manual/root_files/
    std::unique_ptr<TFile> tfOut(TFile::Open(fileOut.c_str(), "recreate"));
    tfOut->cd();
    histview_mcweight->Write();
    histview_pfMetCorrPt->Write();
    tfOut->Close();
}

int main(int argc, char **argv) {
  size_t nThread = 1;
  int debug = 0;
  struct optparse_long longopts[] = {
    {"help", 'h', OPTPARSE_NONE},
    {"debug", 'v', OPTPARSE_NONE},
    {"threads", 'j', OPTPARSE_REQUIRED},
    {NULL} // NULL termination
  };
  int option;
  struct optparse options;
  optparse_init(&options, argv);
  // argv is NULL terminated, so no need to check argc here
  while ((option = optparse_long(&options, longopts, NULL)) != -1) {
    switch (option) {
      case 'h':
        std::cout << "Description:\n"
        "xAna_monoZ_preselect_2016 [-v] [-j n] pathFileOut pathFileInGlob1 [pathFileInGlob2 ...]\n"
        "  -h --help\tDisplay this help message.\n"
        "  -v --debug\tRun with debug message and examinations.\n"
        "\tThis can be specified multiple times.\n"
        "  -j --threads\tSpecify the number of threads to use by EnableImplicitMT.\n"
        "\t0 means the number of all the logical cores.\n"
        "\tDefault to 1.\n"
        << std::endl;
        return 0;
        break;
      case 'v':
        ++debug;
        break;
      case 'j':
        nThread = std::atoi(options.optarg);
        break;
      case '?':
        std::fprintf(stderr, "%s: %s\n", argv[0], options.errmsg);
        std::cerr << std::flush;
        return 1;
        break;
    }
  }
  if (debug) std::cerr << "Debug level: " << debug << std::endl;
  if (debug) std::cerr << "Number of remaining command-line arguments: " << argc - options.optind << std::endl;
  if (argc - options.optind < 2) {
    std::cerr << "error: Expect pathFileOut pathFileInGlob1 pathFileInGlob2 ..." << std::endl;
    return 1;
  }
  const std::string pathFileOut(optparse_arg(&options));
  if (debug) std::cerr << "pathFileOut: " << pathFileOut << std::endl;
  const Int_t nFileIn = argc - options.optind;
  if (debug) std::cerr << "nFileInGolbs: " << nFileIn;
  std::vector<std::string> vPathFileIn(nFileIn, "");
  for (std::string &pathFileIn: vPathFileIn) {
    pathFileIn = optparse_arg(&options);
    if (debug) std::cerr << "pathFileInGlob: " << pathFileIn << std::endl;
  }
  xAna_monoZ_plot_optimization(pathFileOut, vPathFileIn, nThread, debug);
  return 0;
  ;
}
