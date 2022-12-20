

%: %.cu
	nvcc -O0 -Iinclude -o bin/$@ $< -std=c++11
	bin/$@