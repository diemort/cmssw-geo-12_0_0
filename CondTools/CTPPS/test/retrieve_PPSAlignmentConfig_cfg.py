##### configuration #####
input = 'sqlite_file:alignment_config.db'  # input database
db_tag = 'PPSAlignmentConfig_test'  # database tag
#########################

import FWCore.ParameterSet.Config as cms

process = cms.Process("retrievePPSAlignmentConfig")

# message logger
process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('retrieve_PPSAlignmentConfig'
                                        ),
    retrieve_PPSAlignmentConfig = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    )
)

# Load CondDB service
process.load("CondCore.CondDB.CondDB_cfi")

# input database (in this case the local sqlite file)
process.CondDB.connect = input

# A data source must always be defined. We don't need it, so here's a dummy one.
process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)

# input service
process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    process.CondDB,
    DumbStat = cms.untracked.bool(True),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('PPSAlignmentConfigRcd'),
        tag = cms.string(db_tag)
    ))
)

# DB object retrieve module
process.retrieve_config = cms.EDAnalyzer("RetrievePPSAlignmentConfig",
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('PPSAlignmentConfigRcd'),
        data = cms.vstring('PPSAlignmentConfig')
    )),
    verbose = cms.untracked.bool(True)
)

process.path = cms.Path(process.retrieve_config)