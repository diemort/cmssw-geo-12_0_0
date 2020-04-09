import FWCore.ParameterSet.Config as cms

# by default, alignment is now loaded from CondDB using a GT

# TODO: comment out

from CalibPPS.ESProducers.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi import *
ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = cms.vstring(
  # internal pixel alignment
  #"CalibPPS/ESProducers/data/alignment/common/RPixGeometryCorrections_old.xml",
  "CalibPPS/ESProducers/data/alignment/common/RPixGeometryCorrections-2017-2018.xml",

  # global alignment of tracking RPs (strips and pixels)
  "CalibPPS/ESProducers/data/alignment/common/alignment_export_2019_12_04.xml", # alignment used for UL re-reco of Run2
  #"CalibPPS/ESProducers/data/alignment/common/alignment_export_2020_03_23.xml", # test version of 2018 vertical alignment

  # alignment of timing RPs (diamonds and UFSDs)
  "CalibPPS/ESProducers/data/alignment/2017/timing_RP_preTS2_v2.xml",

  "CalibPPS/ESProducers/data/alignment/2017/timing_RP_postTS2_v4.xml",

  "CalibPPS/ESProducers/data/alignment/2018/timing_RP_v4.xml"
)

esPreferLocalAlignment = cms.ESPrefer("CTPPSRPAlignmentCorrectionsDataESSourceXML", "ctppsRPAlignmentCorrectionsDataESSourceXML")
