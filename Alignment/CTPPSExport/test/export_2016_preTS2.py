import FWCore.ParameterSet.Config as cms
process = cms.Process("AlignmentExport")

# geometry
process.load("Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi") # 2017 is OK here

# configure
from base_cff import *

Initialize(process, "Alignment/CTPPSExport/data/2016_preTS2/collect_alignments_2018_07_30.5.out", "tracking_RP_2016_preTS2_version_10.xml")
