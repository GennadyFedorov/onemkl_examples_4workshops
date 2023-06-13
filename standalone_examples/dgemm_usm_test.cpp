/****************************************************************************************
Content: 
This is dgemm dpcpp benchmark (oneapi::mkl::blas::gemm) to execute on CPU/GPU devices
and measure the performance of the square matrixes.
    DPCPP USM API
    Usage: ./a.out <inpt_size> <mode>
	./a.out 2000 
	./a.out 2000 (0 - CPU|1 - GPU)  
	Gennady Fedorov - Gennady.Fedorov@intel.com
*****************************************************************************************/
#include <limits>
#include <vector>
#include <algorithm>
#include <iterator>
#include <float.h>
#include <CL/sycl.hpp>
#include "oneapi/mkl/blas.hpp"
#include "mkl.h"

// local includes
#include "common_for_examples.hpp"

#define AL  0.0
#define BU  1.0

using namespace cl::sycl;
using namespace oneapi::mkl::blas;

void init_arr(MKL_INT M, MKL_INT N, double* arr);
void preamble(sycl::device & dev);

void dgemm_usm(const cl::sycl::device &dev, int size  ) 
{
    oneapi::mkl::transpose transA = oneapi::mkl::transpose::trans;
    oneapi::mkl::transpose transB = oneapi::mkl::transpose::nontrans;
    std::vector<cl::sycl::event> gemm_dependencies;
   
    // matrix data sizes
    MKL_INT m = size;
    MKL_INT n = size;
    MKL_INT k = size;
    
    MKL_INT ldA = size;
    MKL_INT ldB = size;
    MKL_INT ldC = size;

    // set scalar fp values
    double alpha = 2.0; 
    double beta  = 3.0; 

	double t1, t2, dtmin = FLT_MAX;
    // Catch asynchronous exceptions
    auto exception_handler = [] (cl::sycl::exception_list exceptions) {
        for (std::exception_ptr const& e : exceptions) {
            try {
                std::rethrow_exception(e);
            } catch(cl::sycl::exception const& e) {
                std::cout << "Caught asynchronous SYCL exception during GEMM:\n"
                << e.what() << std::endl;
            }
        }
    };


    cl::sycl::context cxt(dev);
    cl::sycl::queue   queue(cxt, dev, exception_handler);
  
//  USM allocation and Initialization    
    double* A = (double *) malloc_shared(size*size * sizeof(double), queue);
    double* B = (double *) malloc_shared(size*size * sizeof(double), queue);
    double* C = (double *) malloc_shared(size*size * sizeof(double), queue);
    if (!A || !B || !C )
        throw std::runtime_error("Failed to allocate USM memory.");

	init_arr(size, size, A);
	init_arr(size, size, B);
	init_arr(size, size, C);  
  
//  create execution queue and buffers of matrix data
	cl::sycl::queue main_queue(dev);	
    cl::sycl::event gemm_done;
  
       
/* axilary variables and arrays for performance measuring */
	int i, NITERATIONS = 1;
	if(size >= 4000) {
		NITERATIONS = 3;
	} else if( size >= 500 && size < 4000 ) {
		NITERATIONS = 11;
	} else if ( size < 500 ) {
		NITERATIONS = 99;
	} else {
		std::cout <<"...Warning, the wrong working size. exit(-2) ..." <<std::endl; exit(-2);
	}	

/***********************************************************************************************
The warming call could be skipped as we will run dgemm NITERATIONS times and we will measure 
the shortest exectime, therfore the very first call will not be taken into account
***********************************************************************************************/
	for( i = 0; i < NITERATIONS; i++) 
	{
		t1 = dsecnd();
			try {
				gemm_done = gemm( main_queue, transA, transB, m, n, k, alpha, A, ldA, B, ldB, beta, C, ldC, gemm_dependencies );
			}
			catch(cl::sycl::exception const& e) {
				std::cout << "\t\tCaught synchronous SYCL exception during GEMM:\n"
                  		<< e.what() << std::endl << "OpenCL status: " << e.get_cl_code() << std::endl;
			}
			gemm_done.wait();
		t2 = dsecnd();
		if( t2-t1 < dtmin ) {
			dtmin = t2-t1;
		}		
	} // for
	
	double op_count = (2.0*m*n*k + 2*m*n); 
	double GFlops    = op_count/dtmin;
	GFlops = GFlops/(1024*1024*1024);
	printf("\t size == %d, GFlops  == %.3f  \n", size, GFlops );
    
	free(A, queue);
	free(B, queue);
	free(C, queue);	
}//_run_dgemm_bench

/******************************************************************************************************
//  
******************************************************************************************************/
int main (int argc, char ** argv) 
{
	int size 	= 0; // input size 
	int mode 	= 0; // Optional: 0 - cpu, 1 - gpu 
       
	if(argc == 2) {
		 size 	= atoi( argv[1] );
	} else if (argc == 3) {
		size 	= atoi( argv[1] );
		mode 	= atoi( argv[2] );
	} 	
      
#ifdef VERBOSE	  
	std::cout <<"input parameters: size, mode : " << size <<"," <<mode<<std::endl;
#endif
	
	device my_dev; 
	if( 0 == mode) {
		my_dev = device(cpu_selector());	
	} else if( 1 == mode){
		my_dev = device(gpu_selector());
	} else {
		std::cout<<"..false mode...." <<std::endl;
	}
#ifdef VERBOSE
	if (my_dev.is_cpu()) printf("Running on CPU device\n");
	if (my_dev.is_gpu()) printf("Running on GPU device\n");	
	preamble( my_dev);
#endif

	dgemm_usm( my_dev, size );
        
    return 0;
}

void preamble(sycl::device & dev) 
{
    std::string dev_name       = dev.template get_info<sycl::info::device::name>();
    std::string driver_version = dev.template get_info<sycl::info::device::version>();

    std::cerr << std::endl
              << "running on:         " << std::endl
              << "       device name: " << dev_name << std::endl
              << "    driver version: " << driver_version << std::endl;
}

void init_arr(MKL_INT M, MKL_INT N, double* arr) 
{
	MKL_INT i,j;
	for( i = 0; i < M; i++ ) {
	    for ( j = 0; j < N; j++) {
			arr[j+i*N] = (double)rand()/RAND_MAX*(BU-AL) + AL;
		}
	}
}
