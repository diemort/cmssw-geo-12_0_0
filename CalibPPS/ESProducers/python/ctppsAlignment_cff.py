import FWCore.ParameterSet.Config as cms

# by default, alignment is now loaded from CondDB using a GT

from Configuration.Eras.Modifier_run2_miniAOD_devel_cff import run2_miniAOD_devel

def useLocalAlignment(process):
  process.load("CalibPPS.ESProducers.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi")
  process.ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = cms.vstring(
    # internal pixel alignment
    #"CalibPPS/ESProducers/data/alignment/common/RPixGeometryCorrections_old.xml",
    "CalibPPS/ESProducers/data/alignment/common/RPixGeometryCorrections-2017-2018.xml",

    # 2016 pre-TS2
    #"CalibPPS/ESProducers/data/alignment/2016/tracking_RP_preTS2_version_10.xml",
    "CalibPPS/ESProducers/data/alignment/2016/tracking_RP_preTS2_version_11.xml", # fix for fill 5264

    # 2016 post-TS2
    "CalibPPS/ESProducers/data/alignment/2016/tracking_RP_postTS2_version_10.xml",

    # 2017 pre-TS2
    "CalibPPS/ESProducers/data/alignment/2017/tracking_RP_preTS2_version_10.xml",

    "CalibPPS/ESProducers/data/alignment/2017/timing_RP_preTS2_v2.xml",

    # 2017 post-TS2
    "CalibPPS/ESProducers/data/alignment/2017/tracking_RP_postTS2_version_10.xml",

    "CalibPPS/ESProducers/data/alignment/2017/timing_RP_postTS2_v4.xml",

    # 2018
    "CalibPPS/ESProducers/data/alignment/2018/tracking_RP_version_10.xml",
    #"CalibPPS/ESProducers/data/alignment/2018/tracking_RP_version_11_test1.xml",

    "CalibPPS/ESProducers/data/alignment/2018/timing_RP_v4.xml"
  )

  process.esPreferLocalAlignment = cms.ESPrefer("CTPPSRPAlignmentCorrectionsDataESSourceXML", "ctppsRPAlignmentCorrectionsDataESSourceXML")

ctppsAlignmentUseLocalAlignment = run2_miniAOD_devel.makeProcessModifier(useLocalAlignment)
