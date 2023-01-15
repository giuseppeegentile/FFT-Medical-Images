# FFT-Medical-Images
    Medical images dataset from: https://graphics.stanford.edu/data/voldata/ - CT study of a cadaver head from the North Carolina Memorial Hospital
    One file per slice (total of 100 slices)
    Most intense noise in the 75-th, 76-th, 77-th images.

    Done:
        - Gaussian filtering 
        - Anisotropic diffusion 
        - Kuwahara
    To do:
        - use unique_ptr instead of raw pointer
        - rewrite clear tests and main
        - use FFT 1D instead of 2D for convolution
        - Rebuild 3d volume 