import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
import FWCore.ParameterSet.Config as cms

# setup process
process = cms.Process("FWLitePlots")
process.inputs = cms.PSet (
    lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
)

# get JSON file correctly parced
JSONfile = '/storage/af/user/christiw/login-1/christiw/LLP/Run3/CMSSW_14_1_0_pre4/src/RazorCommon/Tools/data/Run3/Cert_Collisions2024_378981_386951_Golden.json'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')

process.inputs.lumisToProcess.extend(myList)
