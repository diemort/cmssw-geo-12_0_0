import FWCore.ParameterSet.Config as cms

process = cms.Process("writePPSAlignmentConfig")

# message logger
process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('write_PPSAlignmentConfig'
                                        ),
    write_PPSAlignmentConfig = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    )
)

# Load CondDB service
process.load("CondCore.CondDB.CondDB_cfi")

# output database (in this case local sqlite file)
process.CondDB.connect = 'sqlite_file:PPSAlignmentConfig.db'

# A data source must always be defined. We don't need it, so here's a dummy one.
process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)

# We define the output service.
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('PPSAlignmentConfigRcd'),
        tag = cms.string('PPSAlignmentConfig_v1')
    ))
)

# ESSource
data_label = 'db_test'  # example
process.ppsAlignmentConfigESSource = cms.ESSource("PPSAlignmentConfigESSource",
    # PPSAlignmentConfigESSource parameters, defaults will be taken from fillDescriptions
    label = cms.string(data_label),
    sequence = cms.vstring(
        'db_test',
        'db_test'
    ),
    sector_45 = cms.PSet(
        rp_N = cms.PSet(
            name = cms.string('db_test_RP'),
            id = cms.int32(44),
            y_max_fit_mode = cms.double(66.6)
        )
    ),
    y_alignment = cms.PSet(
        rp_L_F = cms.PSet(
            x_min = cms.double(102),
            x_max = cms.double(210.0)
        )
    )
)

# DB object maker
process.config_writer = cms.EDAnalyzer("WritePPSAlignmentConfig",
    record = cms.string('PPSAlignmentConfigRcd'),
    loggingOn = cms.untracked.bool(True),
    SinceAppendMode = cms.bool(True),
    Source = cms.PSet(
        IOVRun = cms.untracked.uint32(1)
    ),
    label = cms.string(data_label)  # product label (ESSource parameter)
)

process.path = cms.Path(process.config_writer)
