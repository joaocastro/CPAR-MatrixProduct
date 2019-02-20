package main

import (
	"fmt"
	"log"
	"time"
)

//MatrixMult makes the normal multiplication
func MatrixMult(line, col int) {

	var temp int
	// var pha, phb, phc []int

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

func main() {
	var line, col, op int

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

		}
		if op == 3 {

		}
	}
}
