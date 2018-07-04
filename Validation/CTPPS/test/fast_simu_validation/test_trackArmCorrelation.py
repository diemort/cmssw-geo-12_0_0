import FWCore.ParameterSet.Config as cms

import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

# minimal logger settings
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cout'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

# number of events
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:SimulatedTracksReference10Mevt.root'
    )
)



process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# distribution plotter
process.ctppsTrackDistributionPlotter_reco = cms.EDAnalyzer("CTPPSTrackArmPlotter",
  tracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),
  outputFile = cms.string("output_trackArmCorrelation_Reference10Mevt.root")
)

# # processing path
process.p = cms.Path(
    process.ctppsTrackDistributionPlotter_reco
)
