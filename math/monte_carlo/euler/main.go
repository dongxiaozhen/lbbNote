package main

import (
	"fmt"
	"math"
	"math/rand"
	"time"
)

func main() {
	rand.Seed(time.Now().UTC().UnixNano())

	const (
		e           = math.E
		experiments = 1_000_000
	)

	fmt.Printf("Estimating e with %d experiment(s).\n\n", experiments)

	var acc int
	for i := 0; i < experiments; i++ {
		var (
			sum         float64
			num2Success int
		)

		for sum <= 1 {
			n := rand.Float64()
			sum += n
			num2Success++
		}
		acc += num2Success
	}

	expected := float64(acc) / float64(experiments)
	errorPct := 100.0 * math.Abs(expected-e) / e

	fmt.Printf("Expected vale: %9f \n", expected)
	fmt.Printf("e: %9f \n", e)
	fmt.Printf("Error: %9f%%\n", errorPct)
}
