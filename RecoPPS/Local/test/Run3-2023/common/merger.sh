#!/bin/bash

#export SCRAM_ARCH=el8_amd64_gcc11
cmssw=$1
eosarea=$2
jobname=$3
scram project $cmssw
cd $cmssw/src/
eval `scramv1 runtime -sh`
scram b -j8
rsync -avPz ${eosarea}/${jobname}/split/plots/*.root .
fileout=recopps_${jobname}.root
hadd $fileout recopps_*.root 
xrdcp -fs $fileout ${eosarea}/${jobname}/${fileout}
cd ../..
rm -rf CMSSW*
rm -rf $(whoami).cc
