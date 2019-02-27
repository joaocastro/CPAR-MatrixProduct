#!/bin/bash

iterations=1
algorithm=1

for n in {600..3000..400}
do
    echo "Iterations: $iterations, Algorithm: $algorithm, Dimension: $n"
    ./matrix $iterations $algorithm $n > $algorithm-$n.csv
done