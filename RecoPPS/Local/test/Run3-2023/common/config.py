import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')

from Configuration.StandardSequences.Eras import eras
process = cms.Process('TEST', eras.Run3)

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    cerr = cms.untracked.PSet(
        enable = cms.untracked.bool(False)
    ),
    cout = cms.untracked.PSet(
        enable = cms.untracked.bool(True),
        threshold = cms.untracked.string('WARNING')
    )
)

# good lumis
import FWCore.PythonUtilities.LumiList as LumiList

# raw data source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( open('input-local.txt').readlines() ),
    inputCommands = cms.untracked.vstring(
        'drop *',
        'keep FEDRawDataCollection_*_*_*'
    )
)

process.source.lumisToProcess = LumiList.LumiList(filename = 'xjson').getVLuminosityBlockRange()

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True),
        allowUnscheduled = cms.untracked.bool(True),
        )
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

# local RP reconstruction chain with standard settings
process.load("RecoPPS.Configuration.recoCTPPS_cff")

# define GT
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, "xgt")

# override alignment settings
process.load("CalibPPS.ESProducers.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi")
process.ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = cms.vstring("Validation/CTPPS/alignment/xalign")
process.esPreferLocalAlignment = cms.ESPrefer(
    "CTPPSRPAlignmentCorrectionsDataESSourceXML",
    "ctppsRPAlignmentCorrectionsDataESSourceXML"
    )

# track plotter
process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
    tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
    outputFile = cms.string("xfileout")
)
process.ctppsTrackDistributionPlotter.rpId_45_F = cms.uint32(23)
process.ctppsTrackDistributionPlotter.rpId_45_N = cms.uint32(3)
process.ctppsTrackDistributionPlotter.rpId_56_N = cms.uint32(103)
process.ctppsTrackDistributionPlotter.rpId_56_F = cms.uint32(123)

# processing sequences
process.path = cms.Path(
    process.ctppsRawToDigi
    * process.recoCTPPS
    * process.ctppsTrackDistributionPlotter
)

# output configuration
from RecoPPS.Configuration.RecoCTPPS_EventContent_cff import RecoCTPPSAOD
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("xaodout"),
     outputCommands = RecoCTPPSAOD.outputCommands
)

process.outpath = cms.EndPath(process.output)
