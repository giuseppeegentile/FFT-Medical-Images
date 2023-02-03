# FFT-Medical-Images
    Medical images dataset from: https://graphics.stanford.edu/data/voldata/ - CT study of a cadaver head from the North Carolina Memorial Hospital
    One file per slice (total of 100 slices)
    Most intense noise in the 75-th, 76-th, 77-th images.

    Done:
        - Gaussian filtering 
        - Anisotropic diffusion 
        - Kuwahara
        - Rebuild 3d volume


# Instructions
    Pull the repo inside the shared folder.
    Remember to give permission to the folder project (on linux): sudo chmod -R 777 FFT-Medical-Images
    Start the hpc-courses docker and do the following:
        - module load dealii vtk
        - mkdir build
        - cd build
        - cmake ..
        - make && ./FFT

    The 2D processed images are in the build/processed_medical_images folder. Each subfolder for different filters applied.
    The .vti 3d images volume are named as filter_applied.vti. Visualize them in Paraview glance, suggested the MR-Default or MR-T2-Brain colors.
