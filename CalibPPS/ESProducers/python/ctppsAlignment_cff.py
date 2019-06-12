import FWCore.ParameterSet.Config as cms

from CalibPPS.ESProducers.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi import *
ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = cms.vstring(
  # internal pixel alignment
  #"CalibPPS/ESProducers/data/alignment/RPixGeometryCorrections_old.xml",
  "CalibPPS/ESProducers/data/alignment/RPixGeometryCorrections-2017-2018.xml",

  # global alignment of tracking RPs (strips and pixels)
  #"CalibPPS/ESProducers/data/alignment/alignment_export_2019_03_18.5.xml",
  #"CalibPPS/ESProducers/data/alignment/alignment_export_2019_04_18.1.xml",
  #"CalibPPS/ESProducers/data/alignment/alignment_export_2019_05_09.1.xml",
  "CalibPPS/ESProducers/data/alignment/alignment_export_2019_06_12.xml",

  # alignment of timing RPs (diamonds and UFSDs)
  #"CalibPPS/ESProducers/data/alignment/timing_RP_2017_preTS2_v1.xml",
  "CalibPPS/ESProducers/data/alignment/timing_RP_2017_preTS2_v2.xml",
  #"CalibPPS/ESProducers/data/alignment/timing_RP_2017_postTS2_v1.xml",
  "CalibPPS/ESProducers/data/alignment/timing_RP_2017_postTS2_v2.xml",
  #"CalibPPS/ESProducers/data/alignment/timing_RP_2018_v1.xml"
  "CalibPPS/ESProducers/data/alignment/timing_RP_2018_v2.xml"
)

esPreferLocalAlignment = cms.ESPrefer("CTPPSRPAlignmentCorrectionsDataESSourceXML", "ctppsRPAlignmentCorrectionsDataESSourceXML")
