import FWCore.ParameterSet.Config as cms
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer

ppsAlignmentWorker = DQMEDAnalyzer("PPSAlignmentWorker",
	label = cms.string(""),
	tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
	folder = cms.string("CalibPPS/Common"),
	debug = cms.bool(False)
)