ver=V1p17
anaVer=v5/v199/

dir=(
/storage/af/group/phys_exotica/delayedjets/displacedJetMuonAnalyzer/csc/V1p17/Data2016/${anaVer}/normalized/
/storage/af/group/phys_exotica/delayedjets/displacedJetMuonAnalyzer/csc/V1p17/Data2017/${anaVer}/normalized/
/storage/af/group/phys_exotica/delayedjets/displacedJetMuonAnalyzer/csc/V1p17/Data2018/${anaVer}/normalized/
)

sample=(
Run2_displacedJetMuonNtupler_${ver}_Data2016_Run2016-HighMET-07Aug17
Run2_displacedJetMuonNtupler_${ver}_Data2017_Run2017-HighMET-17Nov2017
Run2_displacedJetMuonNtupler_${ver}_Data2018_17Sept2018_Run2018-HighMET-17Sep2018
)
json=(
/storage/af/user/christiw/login-1/christiw/LLP/CMSSW_9_4_4/src/llp_analyzer/lists/officialJson/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt
/storage/af/user/christiw/login-1/christiw/LLP/CMSSW_9_4_4/src/llp_analyzer/lists/officialJson/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt
/storage/af/user/christiw/login-1/christiw/LLP/CMSSW_9_4_4/src/llp_analyzer/lists/officialJson/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt
)
for i in ${!sample[@]}
do
	echo ${i}

	input=${dir[$i]}/${sample[$i]}.root
	echo "input file" ${input}
	output=${sample[$i]}_goodLumi.root
	
	
	ulimit -c 0
	eval `scram runtime -sh`
	echo `which root`
	sed -i "/JSONfile =/c\JSONfile = '${json[$i]}'" ../python/loadJson.py
	#cat ../python/loadJson.py
	echo FWLiteGoodLumi ../python/loadJson.py ${input} ${output}
	FWLiteGoodLumi ../python/loadJson.py ${input} ${output}
	eval `scram unsetenv -sh`
	#gfal-mkdir -p gsiftp://transfer-lb.ultralight.org//${dir[$i]}
	#gfal-copy -f --checksum-mode=both ${output}  gsiftp://transfer-lb.ultralight.org//${dir[$i]}/${output}
	cp ${output} ${dir[$i]}/${output}
	if [ -f ${dir[$i]}/${output} ]
	then
		echo "SUCCESS"
		rm ${output}
	else
		echo "SOMETHING WENT WRONG"
	fi
done
