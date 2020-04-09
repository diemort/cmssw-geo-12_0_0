import FWCore.ParameterSet.Config as cms
process = cms.Process("AlignmentExport")

# geometry
process.load("Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi")

# configure
from base_cff import *

Initialize(process, "Alignment/CTPPSExport/data/2017_postTS2/collect_alignments_2019_12_04.out", "tracking_RP_2017_postTS2_version_10.xml")
