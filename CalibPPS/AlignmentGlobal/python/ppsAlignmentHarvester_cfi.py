import FWCore.ParameterSet.Config as cms
from DQMServices.Core.DQMEDHarvester import DQMEDHarvester

ppsAlignmentHarvester = DQMEDHarvester("PPSAlignmentHarvester",
	folder = cms.string("CalibPPS/Common"),
	sequence = cms.vstring(
		"x_alignment",
		"x_alignment_relative",
		"y_alignment"
	),
	text_results_path = cms.string("./alignment_results.txt"),
	debug = cms.bool(True)
)