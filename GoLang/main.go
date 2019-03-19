package main

import (
	"fmt"
	"log"
	"time"
)

//Min calculates the minimum value
func Min(num1, num2 int) int {
	if num1 < num2 {
		return num1
	}
	return num2
}

//MatrixMult makes the normal multiplication
func MatrixMult(line, col int) {

	var temp int

	pha := make([]int, line*col)
	phb := make([]int, line*col)
	phc := make([]int, line*col)

	for i := 0; i < line; i++ {
		for j := 0; j < col; j++ {
			pha[i*line+j] = 1.0
		}
	}
	for i := 0; i < line; i++ {
		for j := 0; j < col; j++ {
			phb[i*line+j] = i + 1.0
		}
	}
	for i := 0; i < line; i++ {
		for j := 0; j < col; j++ {
			phc[i*line+j] = 0.0
		}
	}

	start := time.Now()

	for i := 0; i < line; i++ {
		for j := 0; j < col; j++ {
			temp = 0
			for k := 0; k < line; k++ {
				temp += pha[i*line+k] * phb[k*col+j]
			}
			phc[i*line+j] = temp
		}
	}

	elapsed := time.Since(start)
	log.Printf("Time: %s seconds\n", elapsed)

	// fmt.Printf("%v", phc)
	for i := 0; i < 10; i++ {
		fmt.Println(phc[i])
	}
}

//MatrixMultLine makes the line multiplication
func MatrixMultLine(line, col int) {

	pha := make([]int, line*col)
	phb := make([]int, line*col)
	phc := make([]int, line*col)

	for i := 0; i < line; i++ {
		for j := 0; j < col; j++ {
			pha[i*line+j] = 1.0
		}
	}
	for i := 0; i < line; i++ {
		for j := 0; j < col; j++ {
			phb[i*line+j] = i + 1.0
		}
	}
	for i := 0; i < line; i++ {
		for j := 0; j < col; j++ {
			phc[i*line+j] = 0.0
		}
	}

	start := time.Now()

	for i := 0; i < line; i++ {
		for k := 0; k < col; k++ {
			for j := 0; j < line; j++ {
				phc[i*line+j] += pha[i*line+k] * phb[k*col+j]
			}
		}
	}

	elapsed := time.Since(start)
	log.Printf("Time: %s seconds\n", elapsed)

	// fmt.Printf("%v", phc)
	for i := 0; i < 10; i++ {
		fmt.Println(phc[i])
	}
}

//MatrixMultBlock makes the block multiplication
func MatrixMultBlock(line, col, block int) {

	pha := make([]int, line*col)
	phb := make([]int, line*col)
	phc := make([]int, line*col)

	for i := 0; i < line; i++ {
		for j := 0; j < col; j++ {
			pha[i*line+j] = 1.0
		}
	}
	for i := 0; i < line; i++ {
		for j := 0; j < col; j++ {
			phb[i*line+j] = i + 1.0
		}
	}
	for i := 0; i < line; i++ {
		for j := 0; j < col; j++ {
			phc[i*line+j] = 0.0
		}
	}

	start := time.Now()

	for i0 := 0; i0 < line; i0 += block {
		for j0 := 0; j0 < col; j0 += block {
			for k0 := 0; k0 < line; k0 += block {

				for i := i0; i < Min(line, i0+block); i++ {
					for k := k0; k < Min(col, k0+block); k++ {
						for j := j0; j < Min(line, j0+block); j++ {
							phc[i*line+j] += pha[i*line+k] * phb[k*col+j]
						}
					}
				}
			}
		}
	}

	elapsed := time.Since(start)
	log.Printf("Time: %s seconds\n", elapsed)

	// fmt.Printf("%v", phc)
	for i := 0; i < 10; i++ {
		fmt.Println(phc[i])
	}
}

func main() {
	var line, block, col, op int

	for true {
		fmt.Print("1. Multiplication\n")
		fmt.Print("2. Line Multiplication\n")
		fmt.Print("3. Block Multiplication\n")
		fmt.Print("Selection?\n")
		fmt.Scanln(&op)

		if op == 1 {
			fmt.Print("Dimensions? Line&Col: \n")
			fmt.Scanln(&line)
			col = line

			MatrixMult(line, col)
		}
		if op == 2 {
			fmt.Print("Dimensions? Line&Col: \n")
			fmt.Scanln(&line)
			col = line

			MatrixMultLine(line, col)
		}
		if op == 3 {
			fmt.Print("Dimensions? Line&Col: \n")
			fmt.Scanln(&line)
			col = line
			fmt.Print("Blocks: \n")
			fmt.Scanln(&block)

			MatrixMultBlock(line, col, block)
		}
	}
}
