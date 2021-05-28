package main

import (
	"fmt"
	"math/rand"
	"time"
)

func main() {
	rand.Seed(time.Now().UTC().UnixNano())

	const trials = 1_000_000
	var (
		numPeople = 23
		success   = 0
	)
	for i := 0; i < trials; i++ {
		bdays := genBdayList(numPeople)
		uniques := uniqueSlice(bdays)

		if len(uniques) != numPeople {
			success++
		}
	}
	probability := float64(success) / float64(trials)
	fmt.Printf("The probability of at least 2 persons in a group of %d people sharing a birthday is %.2f%%\n", numPeople, 100.0*probability)
}

// returns a slice with the uniqueSlice elements of a given slice
func uniqueSlice(s []int) []int {
	keys := make(map[int]struct{}) // Could also be a map[int]bool
	list := []int{}
	for _, entry := range s {
		// We check if the key to the empty value exists.         // By not storing a boolean value, we save memory.
		if _, ok := keys[entry]; !ok {
			keys[entry] = struct{}{}
			list = append(list, entry)
		}
	}
	return list
}

// generates the list of birth days
func genBdayList(n int) []int {
	var bdays = make([]int, n)
	for i := 0; i < n; i++ {
		bdays[i] = rand.Intn(365) // Let's not bother about Leap years
	}
	return bdays
}
