import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Modifier_ctpps_2016_cff import ctpps_2016
process = cms.Process('CTPPSTestBeamSmearing', ctpps_2016)

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

# beam-smearing validation
process.ctppsBeamSmearingValidator = cms.EDAnalyzer("CTPPSBeamSmearingValidator",
  tagBeforeSmearing = cms.InputTag("generator", "unsmeared"),
  tagAfterSmearing = cms.InputTag("beamDivergenceVtxGenerator"),
  outputFile = cms.string("test_beam_smearing.root")
)

# processing path
process.p = cms.Path(
    process.generator
    * process.beamDivergenceVtxGenerator
    * process.ctppsBeamSmearingValidator
)
