import os

version = 'v39'
golden_json_path = os.getenv("CMSSW_BASE") + "/src/RazorCommon/Tools/data/Run3/"
samples = {
        '2022':"DisplacedJet-EXOCSCCluster_Run2022-PromptReco",
        '2023':"Muon-EXOCSCCluster_Run2023-PromptReco",
        '2024':"Muon-EXOCSCCluster_Run2024-PromptReco",

        }
json = {
        '2022':'Cert_Collisions2022_355100_362760_Golden.json',
        '2023':'Cert_Collisions2023_366442_370790_Golden.json',
        '2024': 'Cert_Collisions2024_378981_386951_Golden.json',
        }
for year, sample in samples.items():
        directory="/storage/af/group/phys_exotica/delayedjets/displacedJetMuonAnalyzer/Run3/V1p19/Data{0}/{1}/normalized/".format(year, version)
        print(sample)
        cert = golden_json_path + json[year]

        input_file = sample + '.root'
        output_file = sample + '_goodLumi.root'
        os.system("sed -i \"/JSONfile =/c\JSONfile = \'{}\'\" ../python/loadJson.py".format(cert))
        os.system("FWLiteGoodLumi ../python/loadJson.py {} {}".format(directory + input_file, output_file))
        os.system("cp {} {}/{}".format(output_file, directory, output_file))
        if os.path.isfile(directory + output_file):
        	print("SUCCESS") 
        	os.system("rm {}".format(output_file))
        else: print("SOMETHING WENT WRONG")
