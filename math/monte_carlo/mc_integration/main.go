package main

import (
	"fmt"
	"math"
	"math/rand"
	"time"
)

func main() {
	rand.Seed(time.Now().UTC().UnixNano())

	const numPoints = 1_000_000
	fmt.Printf("Estimating the integral of f with %d point(s).\n\n", numPoints)

	integral := monteCarloIntegrator(gaussian, -20.0, 20.0, numPoints)
	fmt.Printf("Approx. integral: %9f \n", integral)
}

// monteCarloIntegrator receives a function to be integrated.
func monteCarloIntegrator(f func(float64) float64, a float64, b float64, n int) float64 {
	var s float64
	for i := 0; i < n; i++ {
		ui := rand.Float64()
		xi := a + (b-a)*ui
		s += f(xi)
	}

	s = ((b - a) / float64(n)) * s
	return s
}

// function to be integrated
func gaussian(x float64) float64 {
	return math.Exp(-x * x)
}
