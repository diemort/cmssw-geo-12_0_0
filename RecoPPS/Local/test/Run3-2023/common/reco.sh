#!/bin/bash

source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc11
export X509_USER_PROXY=/afs/cern.ch/user/s/silveira/private/x509up_u37419
voms-proxy-info -all
voms-proxy-info -all -file /afs/cern.ch/user/s/silveira/private/x509up_u37419
id=$1
cmssw=$2
perfile=$3
eosarea=$4
jobname=$5
json=$6
align=$7
gt=$8
scram project $cmssw
cd $cmssw/src/
eval `scramv1 runtime -sh`
mv ../../tarball.tar.gz .
tar xvf tarball.tar.gz
mv ../../${align} Validation/CTPPS/alignment
mv ../../${json} .
mv ../../input.txt .
mv ../../config.py .
scram b -j8
#filein=$(sed "${id}q;d" input_${id}.txt)
first=$(( $id*$perfile + 1))
last=$(( $id*$perfile + $perfile))
for i in `seq $first $last`
do
    sed "${i}q;d" input.txt >> input-local.txt
done
fileout=recopps_${jobname}_${id}.root
aodout=recoppsAOD_${jobname}_${id}.root
sed -i "s@xfileout@$fileout@g" config.py
sed -i "s@xaodout@$aodout@g" config.py
sed -i "s@xalign@$align@g" config.py
sed -i "s@xjson@$json@g" config.py
sed -i "s@xgt@$gt@g" config.py
cmsRun config.py
mkdir -p ${eosarea}/${jobname}/split/
xrdcp -fs $fileout ${eosarea}/${jobname}/split/plots/${fileout}
xrdcp -fs $aodout ${eosarea}/${jobname}/split/aod/${aodout}
cd ../..
rm -rf CMSSW*
rm -rf $(whoami).cc

