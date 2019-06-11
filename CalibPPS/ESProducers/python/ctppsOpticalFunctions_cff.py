import FWCore.ParameterSet.Config as cms

from CalibPPS.ESProducers.ctppsLHCInfo_cff import *

from CalibPPS.ESProducers.ctppsOpticalFunctionsESSource_cfi import *

# add 2016 pre-TS2 configuration
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

# add 2016 post-TS2 configuration
config_2016_postTS2 = cms.PSet(
  validityRange = cms.EventRange("282730:min - 284044:max"),

  opticalFunctions = cms.VPSet(
    # fake placeholder at the moment
    cms.PSet( xangle = cms.double(140), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2016_postTS2/placeholder_v1/140urad.root") )
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

# add 2017 configuration
config_base_2017 = cms.PSet(
  validityRange = cms.EventRange("0:0 - 1:1"),

  opticalFunctions = cms.VPSet(
      cms.PSet( xangle = cms.double(120), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2017/version4/120urad.root") ),
      cms.PSet( xangle = cms.double(130), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2017/version4/130urad.root") ),
      cms.PSet( xangle = cms.double(140), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2017/version4/140urad.root") )
  ),

  scoringPlanes = cms.VPSet(
      # z in cm
      cms.PSet( rpId = cms.uint32(0x76180000), dirName = cms.string("XRPH_D6L5_B2"), z = cms.double(-21255.1) ),  # RP 003, strip
      cms.PSet( rpId = cms.uint32(2023227392), dirName = cms.string("XRPH_B6L5_B2"), z = cms.double(-21955.0) ),  # RP 023, pixel
      cms.PSet( rpId = cms.uint32(0x77180000), dirName = cms.string("XRPH_D6R5_B1"), z = cms.double(+21255.1) ),  # RP 103, strip
      cms.PSet( rpId = cms.uint32(2040004608), dirName = cms.string("XRPH_B6R5_B1"), z = cms.double(+21955.0) ),  # RP 123, pixel
  )
)
ctppsOpticalFunctionsESSource.configuration.append(config_base_2017.clone(validityRange = cms.EventRange("297046:min - 297723:max"))) # pre-TS2 begin to Jul alig. run
ctppsOpticalFunctionsESSource.configuration.append(config_base_2017.clone(validityRange = cms.EventRange("298653:min - 302663:max"))) # Jul alig. run to pre-TS2 end
ctppsOpticalFunctionsESSource.configuration.append(config_base_2017.clone(validityRange = cms.EventRange("303719:min - 306462:max"))) # post-TS2: beginning to end

# add config for 2018 data
config_base_2018 = cms.PSet(
  validityRange = cms.EventRange("0:0 - 1:1"),

  opticalFunctions = cms.VPSet(
      cms.PSet( xangle = cms.double(120), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version3/120urad.root") ),
      cms.PSet( xangle = cms.double(130), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version3/130urad.root") ),
      cms.PSet( xangle = cms.double(140), fileName = cms.FileInPath("CalibPPS/ESProducers/data/optical_functions/2018/version3/140urad.root") )
  ),

  scoringPlanes = cms.VPSet(
      # z in cm
      cms.PSet( rpId = cms.uint32(2014838784), dirName = cms.string("XRPH_D6L5_B2"), z = cms.double(-21255.1) ),  # RP 003, pixel
      cms.PSet( rpId = cms.uint32(2023227392), dirName = cms.string("XRPH_B6L5_B2"), z = cms.double(-21955.0) ),  # RP 023, pixel
      cms.PSet( rpId = cms.uint32(2031616000), dirName = cms.string("XRPH_D6R5_B1"), z = cms.double(+21255.1) ),  # RP 103, pixel
      cms.PSet( rpId = cms.uint32(2040004608), dirName = cms.string("XRPH_B6R5_B1"), z = cms.double(+21955.0) ),  # RP 123, pixel
  )
)

ctppsOpticalFunctionsESSource.configuration.append(config_base_2018.clone(validityRange = cms.EventRange("314747:min - 319077:max"))) # beginning to 90m runs
ctppsOpticalFunctionsESSource.configuration.append(config_base_2018.clone(validityRange = cms.EventRange("319337:min - 322633:max"))) # 90m runs to Sep align. run (TS2)
ctppsOpticalFunctionsESSource.configuration.append(config_base_2018.clone(validityRange = cms.EventRange("323363:min - 324420:max"))) # Sep align. run (TS2) to 900 GeV runs
ctppsOpticalFunctionsESSource.configuration.append(config_base_2018.clone(validityRange = cms.EventRange("324612:min - 325175:max"))) # 900 GeV runs to end

esPreferLocalOptics = cms.ESPrefer("CTPPSOpticalFunctionsESSource", "ctppsOpticalFunctionsESSource")

# optics interpolation between crossing angles
from CalibPPS.ESProducers.ctppsInterpolatedOpticalFunctionsESSource_cfi import *
ctppsInterpolatedOpticalFunctionsESSource.lhcInfoLabel = ctppsLHCInfoLabel
