# FFT-Medical-Images
    Medical images dataset from: https://graphics.stanford.edu/data/voldata/ - CT study of a cadaver head from the North Carolina Memorial Hospital
    One file per slice (total of 100 slices)
    Most intense noise in the 75-th, 76-th, 77-th images.

    Done:
        - Gaussian filtering 
        - Anisotropic diffusion 
        - Kuwahara
    To do:
        - Rebuild 3d volume 
        - Doc
        - Report pdf


# Instructions
    Pull the repo inside the shared folder. Start the hpc-courses docker and then:
        - module load dealii vtk
        - mkdir build
        - cd build
        - cmake ..
        - make && ./FFt
