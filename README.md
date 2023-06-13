# onemkl_examples_4workshops
onemkl examples for workshops

This project contains some of oneMKL (https://www.intel.com/content/www/us/en/developer/tools/oneapi/onemkl.html#gs.qw2c2p) examples which could be used for different workshops or customer's presentations. These examples are part of oneMKL version 2023 u1.
Content: 
examples_core_c.tgz - examples for the oneMKL C API
examples_core_f.tgz - examples for the oneMKL Fortran API (F77 & F90 Interfaces)
examples_dpcpp.tgz  - examples for the oneMKL DPC++ API ( Unified Shared Memory and Buffer API )
examples_offload_c.tgz - examples for the oneMKL C API with OpenMP Offload
examples_offload_f.tgz - examples for the oneMKL Fortran API with OpenMP Offload ( F90 )

------------
How to build and run (Linux OS, for example):
**source /opt/intel/oneapi/mkl/latest/env/vars.sh intel64**
  check:  $ echo $MKLROOT
          /opt/intel/oneapi/mkl/2023.1.0
------------
1. extract tgz to the local directory: tar -xzvf examples_core_c.tgz  /* the similar with another *.tgz */
	  
2. $> cd ./c    

3. Create the build folder and change directory to it.
    $> mkdir build && cd build

4. Call CMake to configure the build and customize build options if needed 
  $> cmake .. -G "Ninja" -DCMAKE_C_COMPILER=**gcc** -DTARGET_DOMAINS=blas
  
  the default compiler value is **icc** for Linux OS. In the case if you like to use icc or icx, you need to source the intel compiler....

5. Call CMake to build the configured project:
    $> cmake --build . -j <num of threads> --verbose
Finally, by the default, You will have the whole list executables of oneMKL C examples ( 214 in total). You might edit blas.lst file to choose only few specific functions. check **../blas/blas.lst** file. 
	
6. **./blas-cblas_dgemmx ../blas/data/cblas_dgemmx.d**
or with verbose mode enabled:
    MKL_VERBOSE=2 ./blas-cblas_dgemmx ../blas/data/cblas_dgemmx.d
---------	
The similar You could do in the case of another examples 
