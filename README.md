# RazorCommon

Code to apply golden json file to Ntuples.


Setup
-------------

```bash
cmsrel CMSSW_14_1_0_pre4
cd CMSSW_14_1_0_pre4/src
cmsenv
git clone git@github.com:RazorCMS/RazorCommon.git
cd RazorCommon
scramv1 b clean; scramv1 b  

```
    


Apply Golden json
-------------

make sure the correct input/output path and file name is set and run:
`python3 commands_run3.py`