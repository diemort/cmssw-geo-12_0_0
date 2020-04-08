import FWCore.ParameterSet.Config as cms

from CalibPPS.ESProducers.ctppsLHCInfo_cff import *

# by default, (raw) optical functions are now loaded from CondDB using a GT

# TODO: remove

###   from CalibPPS.ESProducers.ctppsOpticalFunctionsESSource_cfi import *
###
###   config_2018 = cms.PSet(
###     validityRange = cms.EventRange("0:min - 999999:max"),
###
###     opticalFunctions = cms.VPSet(
###         cms.PSet( xangle = cms.double(120), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version5-test/120urad.root") ),
###         cms.PSet( xangle = cms.double(130), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version5-test/130urad.root") ),
###         cms.PSet( xangle = cms.double(140), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version5-test/140urad.root") )
###     ),
###
###     scoringPlanes = cms.VPSet(
###         # z in cm
###         cms.PSet( rpId = cms.uint32(2014838784), dirName = cms.string("XRPH_D6L5_B2"), z = cms.double(-21255.1) ),  # RP 003, pixel
###         cms.PSet( rpId = cms.uint32(2023227392), dirName = cms.string("XRPH_B6L5_B2"), z = cms.double(-21955.0) ),  # RP 023, pixel
###         cms.PSet( rpId = cms.uint32(2031616000), dirName = cms.string("XRPH_D6R5_B1"), z = cms.double(+21255.1) ),  # RP 103, pixel
###         cms.PSet( rpId = cms.uint32(2040004608), dirName = cms.string("XRPH_B6R5_B1"), z = cms.double(+21955.0) ),  # RP 123, pixel
###     )
###   )
###
###   ctppsOpticalFunctionsESSource.configuration.append(config_2018)
###
###   esPreferLocalOptics = cms.ESPrefer("CTPPSOpticalFunctionsESSource", "ctppsOpticalFunctionsESSource")

# optics interpolation between crossing angles
from CalibPPS.ESProducers.ctppsInterpolatedOpticalFunctionsESSource_cfi import *
ctppsInterpolatedOpticalFunctionsESSource.lhcInfoLabel = ctppsLHCInfoLabel
