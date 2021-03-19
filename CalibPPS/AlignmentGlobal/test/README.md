# AlignmentGlobal/test

## Files
### Reference dataset
 - `config_reference_cff.py` - configuration (Event Setup) for the reference dataset
 - `input_files_reference_cff.py` - file with vstring of ROOT input files for the reference dataset 
 - `run_distributions_reference_cfg.py` - process configuration for PPSAlignmentWorker. Produces standard ROOT file with reference histograms for x alignment of the test dataset.
### Test dataset
 - `config_cff.py` - configuration (Event Setup) for the test dataset
 - `input_files_cff.py` - file with vstring of ROOT input files for the test dataset
 - `run_distributions_cfg.py` - process configuration for PPSAlignmentWorker. Produces DQMIO ROOT file with histograms for the harvester.
 - `run_analysis_manual_cfg.py` - process configuration for PPSAlignmentHarvester. Produces alignment results.

## Running instructions
```
cmsRun run_distributions_reference_cfg.py
cmsRun run_distributions_cfg.py
cmsRun run_analysis_manual_cfg.py
```

## Expected results
### x_alignment
 - RP 3: x = -3680.0 +- 17.4 um
 - RP 23: x = -41680.0 +- 17.7 um
 - RP 103: x = -2720.0 +- 16.1 um
 - RP 123: x = -41860.0 +- 16.9 um

### x_alignment_relative:
 - RP 3: x = 18984.7 +- 1.2 um
 - RP 23: x = -18984.7 +- 1.2 um
 - RP 103: x = 19485.1 +- 1.5 um
 - RP 123: x = -19485.1 +- 1.5 um

### x_alignment_relative_sl_fix:
 - RP 3: x = 18983.8 +- 0.3 um
 - RP 23: x = -18983.8 +- 0.3 um
 - RP 103: x = 19486.7 +- 0.3 um
 - RP 123: x = -19486.7 +- 0.3 um

### y_alignment:
 - RP 3: y = 3443.8 +- 103.5 um
 - RP 23: y = 4184.3 +- 89.0 um
 - RP 103: y = 2885.4 +- 70.1 um
 - RP 123: y = 3488.3 +- 133.4 um

### y_alignment_sl_fix:
 - RP 3: y = 3492.3 +- 16.3 um
 - RP 23: y = 4204.1 +- 16.1 um
 - RP 103: y = 2754.9 +- 17.1 um
 - RP 123: y = 3424.8 +- 24.6 um