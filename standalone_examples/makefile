CFLAGS = -w -fsycl -DMKL_ILP64 
CFLAGSVERBOSE = -w -fsycl -DMKL_ILP64 -DVERBOSE
test = dgemm_usm_test.cpp 
LIBS =-L${MKLROOT}/lib/intel64 -lmkl_sycl -lmkl_intel_ilp64 -lmkl_tbb_thread -lmkl_core -lsycl -lOpenCL -lpthread -lm -ldl

all:
	make test
test:
	dpcpp ${CFLAGS} -I"${MKLROOT}/include"  ${test} -o usm.x $(LIBS)	
verbose:
	dpcpp ${CFLAGSVERBOSE} -I"${MKLROOT}/include"  ${test} -o usmverbose.x $(LIBS)	
help:
	dpcpp --version
	icx --version
	echo $MKLROOT
clean:
	rm -f *.o
	rm -f *.x
