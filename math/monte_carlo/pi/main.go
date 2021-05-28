package main

import (
	"fmt"
	"math"
	"math/rand"
	"time"
)

func main() {
	rand.Seed(time.Now().UTC().UnixNano())

	const points = 1_000_000_000
	fmt.Printf("Estimating pi with %d point(s).\n\n", points)

	var sucess int
	for i := 0; i < points; i++ {
		x, y := genRandomPoint()

		// Check if point lies within the circular region:
		if x*x+y*y < 1 {
			sucess++
		}
	}

	piApprox := 4.0 * (float64(sucess) / float64(points))
	errorPct := 100.0 * math.Abs(piApprox-math.Pi) / math.Pi

	fmt.Printf("Estimated pi: %9f \n", piApprox)
	fmt.Printf("pi: %9f \n", math.Pi)
	fmt.Printf("Error: %9f%%\n", errorPct)
}

// generates a random point p = (x, y)
func genRandomPoint() (x, y float64) {
	x = 2.0*rand.Float64() - 1.0
	y = 2.0*rand.Float64() - 1.0
	return x, y
}
