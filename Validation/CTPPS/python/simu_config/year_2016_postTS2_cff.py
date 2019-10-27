import FWCore.ParameterSet.Config as cms

from Validation.CTPPS.simu_config.year_2016_cff import *

# alignment
from CalibPPS.ESProducers.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi import *
alignmentFile = "Validation/CTPPS/alignment/2016_postTS2.xml"
ctppsRPAlignmentCorrectionsDataESSourceXML.MisalignedFiles = [alignmentFile]
ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = [alignmentFile]

# beam optics
from CalibPPS.ESProducers.ctppsOpticalFunctionsESSource_cfi import *

config_2016_postTS2 = cms.PSet(
  validityRange = cms.EventRange("0:min - 999999:max"),

  opticalFunctions = cms.VPSet(
    cms.PSet( xangle = cms.double(140), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2016_postTS2/version2/140urad.root") )
  ),

  scoringPlanes = cms.VPSet(
      # z in cm
      cms.PSet( rpId = cms.uint32(0x76100000), dirName = cms.string("XRPH_C6L5_B2"), z = cms.double(-20382.6) ),  # RP 002, strip
      cms.PSet( rpId = cms.uint32(0x76180000), dirName = cms.string("XRPH_D6L5_B2"), z = cms.double(-21255.1) ),  # RP 003, strip
      cms.PSet( rpId = cms.uint32(0x77100000), dirName = cms.string("XRPH_C6R5_B1"), z = cms.double(+20382.6) ),  # RP 102, strip
      cms.PSet( rpId = cms.uint32(0x77180000), dirName = cms.string("XRPH_D6R5_B1"), z = cms.double(+21255.1) ),  # RP 103, strip
  )
)

ctppsOpticalFunctionsESSource.configuration.append(config_2016_postTS2)

from CalibPPS.ESProducers.ctppsInterpolatedOpticalFunctionsESSource_cfi import *
ctppsInterpolatedOpticalFunctionsESSource.lhcInfoLabel = ""

# aperture cuts
# TODO: needs updating
ctppsDirectProtonSimulation.useEmpiricalApertures = True
ctppsDirectProtonSimulation.empiricalAperture45_xi0_int = 0.073
ctppsDirectProtonSimulation.empiricalAperture45_xi0_slp = 4.1E-4
ctppsDirectProtonSimulation.empiricalAperture45_a_int = +40
ctppsDirectProtonSimulation.empiricalAperture45_a_slp = 0.76
ctppsDirectProtonSimulation.empiricalAperture56_xi0_int = 0.067
ctppsDirectProtonSimulation.empiricalAperture56_xi0_slp = 6.82E-4
ctppsDirectProtonSimulation.empiricalAperture56_a_int = -49
ctppsDirectProtonSimulation.empiricalAperture56_a_slp = 1.73

# defaults
def SetDefaults(process):
  UseCrossingAngle(140, process)
