import FWCore.ParameterSet.Config as cms

from Validation.CTPPS.simu_config.year_2017_cff import *

# alignment
from CalibPPS.ESProducers.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi import *
alignmentFile = "Validation/CTPPS/alignment/2017_preTS2.xml"
ctppsRPAlignmentCorrectionsDataESSourceXML.MisalignedFiles = [alignmentFile]
ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = [alignmentFile]

# aperture cuts
ctppsDirectProtonSimulation.useEmpiricalApertures = True
ctppsDirectProtonSimulation.empiricalAperture45_xi0_int = 0.066
ctppsDirectProtonSimulation.empiricalAperture45_xi0_slp = 3.54E-4
ctppsDirectProtonSimulation.empiricalAperture45_a_int = +56
ctppsDirectProtonSimulation.empiricalAperture45_a_slp = 0.38
ctppsDirectProtonSimulation.empiricalAperture56_xi0_int = 0.062
ctppsDirectProtonSimulation.empiricalAperture56_xi0_slp = 5.96E-4
ctppsDirectProtonSimulation.empiricalAperture56_a_int = -39
ctppsDirectProtonSimulation.empiricalAperture56_a_slp = 1.36
