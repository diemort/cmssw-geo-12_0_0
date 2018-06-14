import FWCore.ParameterSet.Config as cms

# geometry
from Geometry.VeryForwardGeometry.geometryRPFromDD_2018_cfi import *
del(XMLIdealGeometryESSource_CTPPS.geomXMLFiles[-1])
XMLIdealGeometryESSource_CTPPS.geomXMLFiles.append("Validation/CTPPS/test/rp_positions/simulation_default/RP_Dist_Beam_Cent.xml")

# reco (rec hits --> tracks)
from RecoCTPPS.TotemRPLocal.totemRPUVPatternFinder_cfi import *
totemRPUVPatternFinder.tagRecHit = cms.InputTag('ctppsFastProtonSimulation')

from RecoCTPPS.TotemRPLocal.totemRPLocalTrackFitter_cfi import *

from RecoCTPPS.TotemRPLocal.ctppsDiamondLocalTracks_cfi import *
ctppsDiamondLocalTracks.recHitsTag = cms.InputTag('ctppsFastProtonSimulation')

from RecoCTPPS.PixelLocal.ctppsPixelLocalTracks_cfi import *
ctppsPixelLocalTracks.label = cms.string('ctppsFastProtonSimulation')

from RecoCTPPS.TotemRPLocal.ctppsLocalTrackLiteProducer_cff import *

recoHitsToTracks = cms.Sequence(
    totemRPUVPatternFinder
    * totemRPLocalTrackFitter
    * ctppsDiamondLocalTracks
    * ctppsPixelLocalTracks
    * ctppsLocalTrackLiteProducer
)
