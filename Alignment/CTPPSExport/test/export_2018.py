import FWCore.ParameterSet.Config as cms
process = cms.Process("AlignmentExport")

# geometry
process.load("Geometry.VeryForwardGeometry.geometryRPFromDD_2018_cfi")

# configure
from base_cff import *

#Initialize(process, "Alignment/CTPPSExport/data/2018/fit_alignments_2019_07_11.out", "tracking_RP_2018_version_10.xml")
Initialize(process, "Alignment/CTPPSExport/data/2018/fit_alignments_2020_04_10.out", "tracking_RP_2018_version_11_test1.xml")
