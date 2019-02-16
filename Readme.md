# Matrix Product

Project for Parallel Computing with the goal of getting the most efficient result from a matrix product.

## Possible Ways for testing

1. See results with calloc
2. See results with malloc
3. See results with a double cast

### Prerequisites

To run our code properly it will be needed the PAPI library.

Download PAPI: https://icl.utk.edu/papi/software/index.html

```
Uncompress to a directory and change to "src" directory
./configure 
make 
sudo make install 
sudo ldconfig 
```

## Running

```
g++ -O2 matrixProd.cpp -o matrix -lpapi
./matrix
```