import FWCore.ParameterSet.Config as cms

# by default, alignment is now loaded from CondDB using a GT

# TODO: comment out

from CalibPPS.ESProducers.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi import *
ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = cms.vstring(
  # internal pixel alignment
  #"CalibPPS/ESProducers/data/alignment/common/RPixGeometryCorrections_old.xml",
  "CalibPPS/ESProducers/data/alignment/common/RPixGeometryCorrections-2017-2018.xml",

  # 2016 pre-TS2
  "CalibPPS/ESProducers/data/alignment/2016/tracking_RP_preTS2_version_10.xml",

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

  "CalibPPS/ESProducers/data/alignment/2018/timing_RP_v4.xml"
)

esPreferLocalAlignment = cms.ESPrefer("CTPPSRPAlignmentCorrectionsDataESSourceXML", "ctppsRPAlignmentCorrectionsDataESSourceXML")
