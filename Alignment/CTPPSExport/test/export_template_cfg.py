import FWCore.ParameterSet.Config as cms
process = cms.Process("AlignmentExport")

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
  statistics = cms.untracked.vstring(),
  destinations = cms.untracked.vstring('cout'),
  cout = cms.untracked.PSet(
      threshold = cms.untracked.string('INFO')
  )
)

# no events to process
process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(1)
)

# geometry
process.load("Geometry.VeryForwardGeometry.geometryRPFromDD_$geometry_cfi")

# exporter module
process.ctppsExportAlignment = cms.EDAnalyzer("CTPPSExportAlignment",
  inputFiles = cms.vstring("$input"),
  outputFile = cms.string("$output")
)

process.p = cms.Path(
    process.ctppsExportAlignment
)
