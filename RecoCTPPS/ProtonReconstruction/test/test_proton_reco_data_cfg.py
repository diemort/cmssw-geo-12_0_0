import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("CTPPSTestProtonReconstruction", eras.ctpps_2016)

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
  statistics = cms.untracked.vstring(),
  destinations = cms.untracked.vstring('cout'),
  cout = cms.untracked.PSet(
    threshold = cms.untracked.string('WARNING')
  )
)

# raw data source
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    "/store/data/Run2016B/DoubleEG/MINIAOD/18Apr2017_ver2-v1/00000/00220DCF-073E-E711-AB1A-0025905C2CBA.root"
    #'/store/data/Run2016B/DoubleEG/MINIAOD/07Aug17_ver2-v2/00000/3274EFC6-46AA-E711-B9DD-C45444922BFE.root'
  ),
  lumisToProcess = cms.untracked.VLuminosityBlockRange("275371:1-275371:999999")
)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(10000)
)

process.load("RecoCTPPS.ProtonReconstruction.ctppsProtonReconstruction_cfi")
process.ctppsProtonReconstruction.applyExperimentalAlignment = True

process.ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
    tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
    tagRecoProtons = cms.InputTag("ctppsProtonReconstruction"),
    outputFile = cms.string("test_data_plots.root")
)

process.p = cms.Path(
    process.ctppsProtonReconstruction
    * process.ctppsProtonReconstructionPlotter
)
