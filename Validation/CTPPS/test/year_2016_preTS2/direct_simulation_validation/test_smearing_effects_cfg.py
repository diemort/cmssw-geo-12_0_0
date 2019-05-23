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

# direct simulation with and without smearing
process.ctppsDirectProtonSimulationSm = process.ctppsDirectProtonSimulation.clone()

process.ctppsDirectProtonSimulationNoSm = process.ctppsDirectProtonSimulation.clone(
  hepMCTag = cms.InputTag("generator", "unsmeared"),
  produceScoringPlaneHits = True,
  produceRecHits = False,
)

# plotters
process.ctppsDirectProtonSimulationValidatorBeamSm = cms.EDAnalyzer("CTPPSDirectProtonSimulationValidator",
  simuTracksTag = cms.InputTag("ctppsDirectProtonSimulationNoSm"),
  recoTracksTag = cms.InputTag("ctppsDirectProtonSimulationSm"),
  outputFile = cms.string("test_smearing_effects_beam.root")
)

process.ctppsDirectProtonSimulationValidatorSensorSm = cms.EDAnalyzer("CTPPSDirectProtonSimulationValidator",
  simuTracksTag = cms.InputTag("ctppsDirectProtonSimulationSm"),
  recoTracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),
  outputFile = cms.string("test_smearing_effects_sensor.root")
)

# processing path
process.p = cms.Path(
  process.generator
  * process.beamDivergenceVtxGenerator
  * process.ctppsDirectProtonSimulationNoSm
  * process.ctppsDirectProtonSimulationSm

  * process.reco_local

  * process.ctppsDirectProtonSimulationValidatorBeamSm
  * process.ctppsDirectProtonSimulationValidatorSensorSm
)
