import FWCore.ParameterSet.Config as cms

from Validation.CTPPS.simu_config.year_2017_cff import *

# alignment
from CalibPPS.ESProducers.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi import *
alignmentFile = "Validation/CTPPS/alignment/2017_postTS2.xml"
ctppsRPAlignmentCorrectionsDataESSourceXML.MisalignedFiles = [alignmentFile]
ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = [alignmentFile]

# aperture cuts
ctppsDirectProtonSimulation.useEmpiricalApertures = True
ctppsDirectProtonSimulation.empiricalAperture45_xi0_int = 0.073
ctppsDirectProtonSimulation.empiricalAperture45_xi0_slp = 4.1E-4
ctppsDirectProtonSimulation.empiricalAperture45_a_int = +40
ctppsDirectProtonSimulation.empiricalAperture45_a_slp = 0.76
ctppsDirectProtonSimulation.empiricalAperture56_xi0_int = 0.067
ctppsDirectProtonSimulation.empiricalAperture56_xi0_slp = 6.82E-4
ctppsDirectProtonSimulation.empiricalAperture56_a_int = -49
ctppsDirectProtonSimulation.empiricalAperture56_a_slp = 1.73
