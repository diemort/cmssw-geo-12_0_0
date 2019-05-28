import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Modifier_ctpps_2016_cff import ctpps_2016
process = cms.Process('CTPPSProtonReconstructionTest', ctpps_2016)

# load common code
process.load("Validation.CTPPS.year_2016_preTS2.direct_simu_reco_cff")
from Validation.CTPPS.year_2016_preTS2.customisation_cff import *

# load template
from Validation.CTPPS.templates.proton_reconstruction_MC.misalignment_cff import Test
Test(process)

#----------

SetLargeTheta(process)
SetLevel4(process)

