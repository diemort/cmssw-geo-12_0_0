import FWCore.ParameterSet.Config as cms

# beam parameters as declared by LHC
ctppsLHCInfoESSource = cms.ESSource("CTPPSLHCInfoESSource",
  label = cms.string(""),
  validityRange = cms.EventRange("1:min - 999999:max"),
  beamEnergy = cms.double(6500),  # GeV
  xangle = cms.double(185)  # murad
)

# beam parameters as determined by PPS
ctppsBeamParametersESSource = cms.ESSource("CTPPSBeamParametersESSource",
  setBeamPars = cms.bool(True),

  #  beam momentum  (GeV)
  beamMom45 = cms.double(6500.),
  beamMom56 = cms.double(6500.),

  #  beta*  (cm)
  betaStarX45 = cms.double(0.),
  betaStarX56 = cms.double(0.),
  betaStarY45 = cms.double(0.),
  betaStarY56 = cms.double(0.),

  #  beam divergence  (rad)
  beamDivX45 = cms.double(20E-6),
  beamDivX56 = cms.double(20E-6),
  beamDivY45 = cms.double(20E-6),
  beamDivY56 = cms.double(20E-6),

  #  half crossing angle  (rad)
  halfXangleX45 = cms.double(179.394E-6),
  halfXangleX56 = cms.double(191.541E-6),
  halfXangleY45 = cms.double(0.),
  halfXangleY56 = cms.double(0.),

  #  vertex offset  (cm)
  vtxOffsetX45 = cms.double(0.),
  vtxOffsetX56 = cms.double(0.),
  vtxOffsetY45 = cms.double(0.),
  vtxOffsetY56 = cms.double(0.),
  vtxOffsetZ45 = cms.double(0.),
  vtxOffsetZ56 = cms.double(0.),

  #  vertex sigma  (cm)
  vtxStddevX = cms.double(10E-4),
  vtxStddevY = cms.double(10E-4),
  vtxStddevZ = cms.double(5)
)

# beam optics
from CalibPPS.ESProducers.ctppsOpticalFunctionsESSource_cfi import *

config_2016_preTS2 = cms.PSet(
  validityRange = cms.EventRange("273725:min - 280385:max"),

  opticalFunctions = cms.VPSet(
    cms.PSet( xangle = cms.double(185), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2016_preTS2/version1/optical_functions.root") )
  ),

  scoringPlanes = cms.VPSet(
      # z in cm
      cms.PSet( rpId = cms.uint32(0x76100000), dirName = cms.string("XRPH_C6L5_B2"), z = cms.double(-20382.6) ),  # RP 002, strip
      cms.PSet( rpId = cms.uint32(0x76180000), dirName = cms.string("XRPH_D6L5_B2"), z = cms.double(-21255.1) ),  # RP 003, strip
      cms.PSet( rpId = cms.uint32(0x77100000), dirName = cms.string("XRPH_C6R5_B1"), z = cms.double(+20382.6) ),  # RP 102, strip
      cms.PSet( rpId = cms.uint32(0x77180000), dirName = cms.string("XRPH_D6R5_B1"), z = cms.double(+21255.1) ),  # RP 103, strip
  )
)

ctppsOpticalFunctionsESSource.configuration.append(config_2016_preTS2)

# geometry
from Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi import * # NB: 2017 is OK here
del(XMLIdealGeometryESSource_CTPPS.geomXMLFiles[-1])
XMLIdealGeometryESSource_CTPPS.geomXMLFiles.append("Validation/CTPPS/test/year_2016/RP_Dist_Beam_Cent.xml")

# particle-data table
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *

# random seeds
RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    sourceSeed = cms.PSet(initialSeed =cms.untracked.uint32(98765)),
    generator = cms.PSet(initialSeed = cms.untracked.uint32(98766)),
    beamDivergenceVtxGenerator = cms.PSet(initialSeed =cms.untracked.uint32(3849))
)

# default source
source = cms.Source("EmptySource",
    firstRun = cms.untracked.uint32(1)
)

# particle generator
from Validation.CTPPS.randomXiThetaGunProducer_cfi import *

# beam smearing
from IOMC.EventVertexGenerators.beamDivergenceVtxGenerator_cfi import *

# direct simulation
from Validation.CTPPS.ctppsDirectProtonSimulation_cfi import *
ctppsDirectProtonSimulation.verbosity = 0
ctppsDirectProtonSimulation.hepMCTag = cms.InputTag('beamDivergenceVtxGenerator')
ctppsDirectProtonSimulation.useEmpiricalApertures = False
ctppsDirectProtonSimulation.roundToPitch = True
ctppsDirectProtonSimulation.pitchStrips = 66E-3 * 12 / 19 # effective value to reproduce real RP resolution
ctppsDirectProtonSimulation.produceHitsRelativeToBeam = True
ctppsDirectProtonSimulation.produceScoringPlaneHits = False
ctppsDirectProtonSimulation.produceRecHits = True

# local reconstruction
# TODO: load common config
from RecoCTPPS.TotemRPLocal.totemRPUVPatternFinder_cfi import *
totemRPUVPatternFinder.tagRecHit = cms.InputTag('ctppsDirectProtonSimulation')

from RecoCTPPS.TotemRPLocal.totemRPLocalTrackFitter_cfi import *

from RecoCTPPS.TotemRPLocal.ctppsLocalTrackLiteProducer_cff import *
ctppsLocalTrackLiteProducer.includeDiamonds = False
ctppsLocalTrackLiteProducer.includePixels = False

reco_local = cms.Sequence(
  totemRPUVPatternFinder
  * totemRPLocalTrackFitter
  * ctppsLocalTrackLiteProducer
)

# proton reconstruction
from RecoCTPPS.ProtonReconstruction.ctppsProtons_cff import *
ctppsProtons.tagLocalTrackLite = cms.InputTag('ctppsLocalTrackLiteProducer')

# RP ids
rpId_45_F = 3
rpId_45_N = 2
rpId_56_N = 102
rpId_56_F = 103

#----------------------------------------------------------------------------------------------------

def SetLevel1():
  ctppsBeamParametersESSource.vtxStddevX = 0E-4
  ctppsBeamParametersESSource.vtxStddevZ = 0

  ctppsBeamParametersESSource.beamDivX45 = 0E-6
  ctppsBeamParametersESSource.beamDivX56 = 0E-6
  ctppsBeamParametersESSource.beamDivY45 = 0E-6
  ctppsBeamParametersESSource.beamDivY56 = 0E-6

  ctppsDirectProtonSimulation.roundToPitch = False


def SetLevel2():
  ctppsBeamParametersESSource.beamDivX45 = 0E-6
  ctppsBeamParametersESSource.beamDivX56 = 0E-6
  ctppsBeamParametersESSource.beamDivY45 = 0E-6
  ctppsBeamParametersESSource.beamDivY56 = 0E-6

  ctppsDirectProtonSimulation.roundToPitch = False


def SetLevel3():
  ctppsDirectProtonSimulation.roundToPitch = False


def SetLevel4():
  pass


def SetLowTheta():
  generator.theta_x_sigma = 0E-6
  generator.theta_y_sigma = 0E-6


def SetLargeTheta():
  pass
