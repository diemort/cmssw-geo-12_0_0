import FWCore.ParameterSet.Config as cms

def Initialize(process, f_in, f_out):
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

  # exporter module
  process.ctppsExportAlignment = cms.EDAnalyzer("CTPPSExportAlignment",
    inputFiles = cms.vstring(f_in),
    outputFile = cms.string(f_out)
  )

  process.p = cms.Path(
      process.ctppsExportAlignment
  )
