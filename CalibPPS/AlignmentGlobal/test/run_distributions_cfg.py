########## Configuration ##########
# if set to True, a file with logs will be produced.
produce_logs = False

# if set to True, the process will load the conditions input from an SQLite file. 
# Otherwise, it will use ESSource.
conditions_input_from_db = False 

# Input database. Used only if conditions_input_from_db is set to True.
input_conditions = 'sqlite_file:alignment_config.db'

# Database tag. Used only if conditions_input_from_db is set to True.
db_tag = 'PPSAlignmentConfig_test'

# Path for a ROOT file with the histograms
output_distributions = 'dqm_run_distributions_test.root'
###################################

import FWCore.ParameterSet.Config as cms

from input_files_cff import input_files

process = cms.Process('testDistributions')

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("CalibPPS.AlignmentGlobal.ppsAlignmentWorker_cfi")
process.load("DQMServices.Core.DQMStore_cfi")

# Message Logger
if produce_logs:
    process.MessageLogger = cms.Service("MessageLogger",
        destinations = cms.untracked.vstring('run_distributions', 
                                            'cout'
                                            ),
        run_distributions = cms.untracked.PSet(
        	threshold = cms.untracked.string("INFO")
        ),
        cout = cms.untracked.PSet(
            threshold = cms.untracked.string('WARNING')
        )
    )
else:
    process.MessageLogger = cms.Service("MessageLogger",
        destinations = cms.untracked.vstring('cout'),
        cout = cms.untracked.PSet(
            threshold = cms.untracked.string('WARNING')
        )
    )

# Source
process.source = cms.Source("PoolSource",
	fileNames = input_files
)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

# Event Setup
if not conditions_input_from_db:
    from config_cff import ppsAlignmentConfigESSource
    process.ppsAlignmentConfigESSource = ppsAlignmentConfigESSource
else:
    process.load("CondCore.CondDB.CondDB_cfi")
    process.CondDB.connect = input_conditions
    process.PoolDBESSource = cms.ESSource("PoolDBESSource",
		process.CondDB,
		DumbStat = cms.untracked.bool(True),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string('PPSAlignmentConfigRcd'),
			tag = cms.string(db_tag)
		))
	)

# Output for the histograms
process.dqmOutput = cms.OutputModule("DQMRootOutputModule",
	fileName = cms.untracked.string(output_distributions)
)

process.path = cms.Path(
  	process.ppsAlignmentWorker
)

process.end_path = cms.EndPath(
	process.dqmOutput
)

process.schedule = cms.Schedule(
	process.path,
	process.end_path
)