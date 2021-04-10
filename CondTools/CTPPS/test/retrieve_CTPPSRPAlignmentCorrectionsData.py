import FWCore.ParameterSet.Config as cms

process = cms.Process("retrieveCTPPSRPAlignmentCorrectionsData")

# message logger
process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('retrieve_CTPPSRPAlignmentCorrectionsData'
                                        ),
    retrieve_CTPPSRPAlignmentCorrectionsData = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    )
)

# Load CondDB service
process.load("CondCore.CondDB.CondDB_cfi")

# input database (in this case the local sqlite file)
process.CondDB.connect = 'sqlite_file:alignment_results.db'

# A data source must always be defined. We don't need it, so here's a dummy one.
run_number = 0  # You can change the run number
process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(run_number),
    lastValue = cms.uint64(run_number),
    interval = cms.uint64(1)
)

# We define the input service.
process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    process.CondDB,
    DumbStat = cms.untracked.bool(True),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('CTPPSRPAlignmentCorrectionsDataRcd'),
        tag = cms.string('CTPPSRPAlignmentCorrectionsData_test')
    ))
)

process.receive_config = cms.EDAnalyzer("RetrieveCTPPSRPAlignmentCorrectionsData",
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('CTPPSRPAlignmentCorrectionsDataRcd'),
        data = cms.vstring('CTPPSRPAlignmentCorrectionsData')
    )),
    verbose = cms.untracked.bool(True)
)

process.path = cms.Path(process.receive_config)