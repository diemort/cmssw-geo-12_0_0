import FWCore.ParameterSet.Config as cms
process = cms.Process("AlignmentExport")

# geometry
process.load("Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi") # 2017 is OK here

# configure
from base_cff import *

Initialize(process, "Alignment/CTPPSExport/data/2016_postTS2/fit_alignments_2019_09_16.out", "tracking_RP_2016_postTS2_version_10.xml")
