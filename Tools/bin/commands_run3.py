import os

version = 'v10'
directory="/storage/af/group/phys_exotica/delayedjets/displacedJetMuonAnalyzer/Run3/V1p19/Data2022/{0}/normalized/".format(version)
golden_json_path = '/storage/af/user/christiw/login-1/christiw/LLP/Run3/CMSSW_10_6_30/src/run3_llp_analyzer/data/Certification/20221215/'
samples = [
#'DisplacedJet-EXOCSCCluster_Run2022E-PromptReco-v1',
'DisplacedJet-EXOCSCCluster_Run2022F-PromptReco-v1',
#'DisplacedJet-EXOCSCCluster_Run2022G-PromptReco-v1',
]
for sample in samples:
        print(sample)
        if '2022E' in sample:cert = golden_json_path + 'Cert_Collisions2022_eraE_359022_360331_Golden.json'
        elif '2022F' in sample: cert = golden_json_path + 'Cert_Collisions2022_eraF_360390_362167_Golden.json'
        elif '2022G' in sample: cert = golden_json_path + 'Cert_Collisions2022_eraG_362433_362760_Golden.json'
        else: assert(False)

	input_file = sample + '.root'
	output_file = sample + '_goodLumi.root'
	os.system("sed -i \"/JSONfile =/c\JSONfile = \'{}\'\" ../python/loadJson.py".format(cert))
	#cat ../python/loadJson.py
	os.system("FWLiteGoodLumi ../python/loadJson.py {} {}".format(directory + input_file, output_file))
	os.system("cp {} {}/{}".format(output_file, directory, output_file))
	if os.path.isfile(directory + output_file):
		print("SUCCESS") 
		os.system("rm {}".format(output_file))
	else: print("SOMETHING WENT WRONG")
