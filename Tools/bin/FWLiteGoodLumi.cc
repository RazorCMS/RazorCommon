#include <iostream>
#include <boost/bind.hpp>
#include <algorithm>
#include <TFile.h>
#include <TSystem.h>
#include <TTree.h>
#include <TKey.h>

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "DataFormats/FWLite/interface/Run.h"
#include "DataFormats/FWLite/interface/LuminosityBlock.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "DataFormats/Provenance/interface/LuminosityBlockRange.h"

std::string RUN_BRANCH = "runNum";
std::string LUMI_BRANCH = "lumiSec";

bool jsonContainsEvent (const std::vector< edm::LuminosityBlockRange > &jsonVec, int run, int lumi)
{
   // if the jsonVec is empty, then no JSON file was provided so all
   // events should pass
   if (jsonVec.empty())
   {
      return true;
   }
   bool (* funcPtr) (edm::LuminosityBlockRange const &,
                     edm::LuminosityBlockID const &) = &edm::contains;
   edm::LuminosityBlockID lumiID (run,lumi);
   std::vector< edm::LuminosityBlockRange >::const_iterator iter = 
      std::find_if (jsonVec.begin(), jsonVec.end(),
                    boost::bind(funcPtr, _1, lumiID) );
   return jsonVec.end() != iter;

}

int main(int argc, char ** argv){
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  if (argc != 4) {
    std::cout << "Use as follows: FWLiteGoodLumi <pythonFileWithGoodLumiJson> <inputDataFilename> <outputDataFilename> \n";
    return 0;
  }


  PythonProcessDesc builder (argv[1], argc, argv);
  edm::ParameterSet const& inputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("inputs");

  std::vector<edm::LuminosityBlockRange> jsonVector;
  if ( inputs.exists("lumisToProcess") ) 
    {
      std::vector<edm::LuminosityBlockRange> const & lumisTemp =
	inputs.getUntrackedParameter<std::vector<edm::LuminosityBlockRange> > ("lumisToProcess");
      jsonVector.resize( lumisTemp.size() );
      copy( lumisTemp.begin(), lumisTemp.end(), jsonVector.begin() );
    }

  std::string inputFilename = argv[2];
  std::string outputFilename = argv[3];

  //create output file
  TFile *outputFile = TFile::Open( outputFilename.c_str(), "RECREATE");
  
  //loop over all TTrees in the file and add the weight branch to each of them
  TFile *inputFile = TFile::Open(inputFilename.c_str(), "READ");
  assert(inputFile);
  inputFile->cd();
  TIter nextkey(inputFile->GetListOfKeys());
  TKey *key;
  TKey *previous = NULL;
  while((key = (TKey*)nextkey())){
    std::string className = key->GetClassName();
    std::cout << "Getting key from file.  Class type: " << className << std::endl;
    if(className.compare("TTree") != 0){
      std::cout << "Skipping key (not a TTree)" << std::endl;
      outputFile->cd();
      TObject *outObj = key->ReadObj();
      outObj->Write();
      inputFile->cd();
      continue;
    }

    //if this key has the same name as the previous one, it's an unwanted cycle and we skip it
    if(previous != NULL && strcmp(key->GetName(), previous->GetName()) == 0)
    {
        continue;
    }
    previous = key;
    
    TTree *inputTree = (TTree*)key->ReadObj();
    std::cout << "Processing tree " << inputTree->GetName() << std::endl;
    
    //create new normalized tree
    outputFile->cd();
    TTree *outputTree = inputTree->CloneTree(0);  
    std::cout << "Events in the ntuple: " << inputTree->GetEntries() << std::endl;
    
    UInt_t run = 0;
    UInt_t lumi = 0;
    inputTree->SetBranchAddress(RUN_BRANCH.c_str(), &run);
    inputTree->SetBranchAddress(LUMI_BRANCH.c_str(), &lumi);


    //store the weights
    for (int n=0;n<inputTree->GetEntries();n++) { 
      if (n%1000000==0) std::cout << "Processed Event " << n << "\n";
      inputTree->GetEntry(n);

      if(jsonContainsEvent( jsonVector, run, lumi) ) {	
	outputTree->Fill(); 
      }
    }
    //save
    outputTree->Write();
    inputFile->cd();
    std::cout << "Output Number of Events: " << outputTree->GetEntries() << "\n";
  }
  inputFile->Close();
  std::cout << "Closing output file." << std::endl;    
  outputFile->Close();
  delete outputFile;   
  
  return 0;
}
