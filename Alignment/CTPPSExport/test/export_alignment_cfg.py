import FWCore.ParameterSet.Config as cms
process = cms.Process("GeometryInfo")

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cout'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    )
)

# no events to process
process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(1)
)

# geometry
process.load("Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi")

#process.load("Geometry.VeryForwardGeometryBuilder.ctppsIncludeAlignmentsFromXML_cfi")

# exporter module
process.ctppsExportAlignment = cms.EDAnalyzer("CTPPSExportAlignment",
  inputFiles = cms.vstring(
    "Alignment/CTPPSExport/data/2016_preTS2/collect_alignments_2018_07_30.5.out",

    "Alignment/CTPPSExport/data/2016_postTS2/fit_alignments_2019_09_16.out",

    "Alignment/CTPPSExport/data/2017_preTS2/collect_alignments_2019_06_12.out",

    "Alignment/CTPPSExport/data/2017_postTS2/collect_alignments_2019_12_04.out",

    "Alignment/CTPPSExport/data/2018/fit_alignments_2020_03_23.out",
  ),

  outputFile = cms.string("alignment_export_TEST_2020_04_09.xml")
)

process.p = cms.Path(
    process.ctppsExportAlignment
)
