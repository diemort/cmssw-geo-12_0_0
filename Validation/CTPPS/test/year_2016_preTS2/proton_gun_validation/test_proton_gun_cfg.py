import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Modifier_ctpps_2016_cff import ctpps_2016
process = cms.Process('CTPPSFastSimulation', ctpps_2016)

# load common code
process.load("Validation.CTPPS.year_2016_preTS2.direct_simu_reco_cff")

# load template
from Validation.CTPPS.templates.proton_gun_validation.test_proton_gun_cff import Test
Test(process)
