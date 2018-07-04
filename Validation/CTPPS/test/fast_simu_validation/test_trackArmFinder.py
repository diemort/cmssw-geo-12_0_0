import FWCore.ParameterSet.Config as cms
import math

# from Configuration.StandardSequences.Eras import eras
# process = cms.Process('CTPPSFastSimulation', eras.ctpps_2016)

import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

import FWCore.ParameterSet.VarParsing as VarParsing
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    FailPath = cms.untracked.vstring('ProductNotFound','Type Mismatch')
    )
options = VarParsing.VarParsing ()
options.register('numberOfParticles',
                '',
                VarParsing.VarParsing.multiplicity.singleton,
                VarParsing.VarParsing.varType.int,
                "select bunch to be read")
options.parseArguments()


# minimal logger settings
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
        FwkReport = cms.untracked.PSet(
          optionalPSet = cms.untracked.bool(True),
          reportEvery = cms.untracked.int32(10000),
      ),
    )
)


# number of events
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:Simulated_' + str(options.numberOfParticles) + 'protons.root'
    )
)



process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# particle-data table
# process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# # particle generator
# process.generator = cms.EDProducer("RandomXiThetaGunProducer",
#   particleId = cms.uint32(2212),

#   energy = cms.double(6500),  # nominal beam energy, GeV
#   xi_min = cms.double(0.),
#   xi_max = cms.double(0.20),
#   theta_x_mean = cms.double(0),
#   theta_x_sigma = cms.double(100E-6), # in rad
#   theta_y_mean = cms.double(0),
#   theta_y_sigma = cms.double(100E-6),

#   nParticlesSector45 = cms.uint32(1),
#   nParticlesSector56 = cms.uint32(1),
# )

# # random seeds
# process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
#     sourceSeed = cms.PSet(initialSeed =cms.untracked.uint32(98765)),
#     generator = cms.PSet(initialSeed = cms.untracked.uint32(98766)),
#     SmearingGenerator = cms.PSet(initialSeed =cms.untracked.uint32(3849))
# )

# fast simulation
# process.load('SimCTPPS.OpticsParameterisation.ctppsFastProtonSimulation_cfi')
# process.ctppsFastProtonSimulation.produceScoringPlaneHits = False
# process.ctppsFastProtonSimulation.produceRecHits = True
# process.ctppsFastProtonSimulation.checkApertures = False
# process.ctppsFastProtonSimulation.produceHitsRelativeToBeam = False
# process.ctppsFastProtonSimulation.roundToPitch = False
# process.ctppsFastProtonSimulation.checkIsHit = False


# geometry and reco (rec hits --> tracks)
# process.load('common_cff')

# distribution plotter
process.ctppsTrackDistributionPlotter_reco = cms.EDAnalyzer("CTPPSTrackArmFinder",
  tracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),
  outputFile = cms.string("output_trackArmFinder_" + str(options.numberOfParticles) + "protons.root"),
  armMapFileName = cms.string("output_trackArmCorrelation_Reference10Mevt.root"),
  xSigmaMultiplierMin = cms.double(0.5),
  xSigmaMultiplierStepSize = cms.double(0.5),
  xSigmaMultiplierStepNumber = cms.uint32(20),
  ySigmaMultiplierMin = cms.double(0.5),
  ySigmaMultiplierStepSize = cms.double(0.5),
  ySigmaMultiplierStepNumber = cms.uint32(20),
  xSigmaTrackSinglePot = cms.double(0.15),
  ySigmaTrackSinglePot = cms.double(0.1),
)
# # proton reconstruction
# process.load("RecoCTPPS.ProtonReconstruction.ctppsProtonReconstruction_cfi")
# process.ctppsProtonReconstruction.tagLocalTrackLite = cms.InputTag('ctppsLocalTrackLiteProducer')

# # distribution plotter
# process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
#   tracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),
#   outputFile = cms.string("output_test_base.root")
# )

# # processing path
process.p = cms.Path(
    process.ctppsTrackDistributionPlotter_reco
    # * process.ctppsProtonReconstruction
    # * process.ctppsTrackDistributionPlotter
)

# process.o1 = cms.OutputModule("PoolOutputModule",
#   outputCommands = cms.untracked.vstring('drop *',
# #                                         'keep CTPPS*_*_*_*',
# #                                         'keep CTPPSPixelRecHitedmDetSetVector_ctppsPixelRecHits_*_*',
# #                                         'keep CTPPSPixelLocalTrackedmDetSetVector_ctppsPixelLocalTracks_*_*',
#                                          'keep CTPPSLocalTrackLites_ctppsLocalTrackLiteProducer_*_*'
#   ),
#   fileName = cms.untracked.string("SimulatedTracksReference.root")
# )

# process.outpath = cms.EndPath(process.o1)
