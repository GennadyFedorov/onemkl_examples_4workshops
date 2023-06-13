# onemkl_examples_4workshops
onemkl examples for workshops

This project contains some of oneMKL (https://www.intel.com/content/www/us/en/developer/tools/oneapi/onemkl.html#gs.qw2c2p) examples which could be used for different workshops or customer's presentations. These examples are part of oneMKL version 2023 u1.
Content: 
examples_core_c.tgz - examples for the oneMKL C API
examples_core_f.tgz - examples for the oneMKL Fortran API (F77 & F90 Interfaces)
examples_dpcpp.tgz  - examples for the oneMKL DPC++ API ( Unified Shared Memory and Buffer API )
examples_offload_c.tgz - examples for the oneMKL C API with OpenMP Offload
examples_offload_f.tgz - examples for the oneMKL Fortran API with OpenMP Offload ( F90 )

How Use:
> extract: tar -xzvf examples_core_c.tgz

How to build and run:
1. source MKL to make MKLROOT availalbe. 

2. Choose the oneMKL API type and change directory to the selected oneMKL API type.
    $> cd $MKLROOT/examples/c

3. Create the build folder and change directory to it.
    $> mkdir build && cd build

4. Call CMake to configure the build and customize build options if needed 
  $> cmake .. -G "Ninja" -DCMAKE_C_COMPILER=icx -DTARGET_DOMAINS=blas

5. Call CMake to build the configured project:
    $> cmake --build . -j 24 --verbose

6. Run examples using the CTest tool.
   ./blas-dgemm
or 
	MKL_VERBOSE=2 ./blas-dgemm
