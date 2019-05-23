import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Modifier_ctpps_2016_cff import ctpps_2016
process = cms.Process('CTPPSTestAcceptance', ctpps_2016)

# minimal logger settings
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

# load common code
process.load("Validation.CTPPS.year_2016_preTS2.direct_simu_reco_cff")

# number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

# acceptance plotter
process.ctppsAcceptancePlotter = cms.EDAnalyzer("CTPPSAcceptancePlotter",
  tagHepMC = cms.InputTag("generator", "unsmeared"),
  tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),

  rpId_45_F = cms.uint32(rpId_45_F),
  rpId_45_N = cms.uint32(rpId_45_N),
  rpId_56_N = cms.uint32(rpId_56_N),
  rpId_56_F = cms.uint32(rpId_56_F),

  outputFile = cms.string("test_acceptance.root")
)

# processing path
process.p = cms.Path(
  process.generator
  * process.beamDivergenceVtxGenerator
  * process.ctppsDirectProtonSimulation

  * process.reco_local

  * process.ctppsAcceptancePlotter
)
