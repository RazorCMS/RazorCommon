import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
import FWCore.ParameterSet.Config as cms

# setup process
process = cms.Process("FWLitePlots")
process.inputs = cms.PSet (
    lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
)

# get JSON file correctly parced
JSONfile = '/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_5_3_26/src/RazorAnalyzer/lists/razorNtuplerV1p5-Run1/lumiSummary/lumiSummary_DoubleMuParked_Run2012_GoodRuns.json'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')

process.inputs.lumisToProcess.extend(myList)
