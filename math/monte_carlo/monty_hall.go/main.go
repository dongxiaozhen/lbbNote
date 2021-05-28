package main

import (
	"fmt"
	"math"
	"math/rand"
	"time"
)

// 三个门
// 先猜一个数字，
// 主持人去掉一个错误答案
// 在剩下的两个里选择一个

func main() {
	rand.Seed(time.Now().UTC().UnixNano())

	const games = 10_000_000
	fmt.Printf("Estimating the probability of winning by switching doors with %d game(s).\n\n", games)

	var sucess int
	for i := 0; i < games; i++ {
		newDoor, prizeDoor := setMontyHallGame()
		if newDoor == prizeDoor {
			sucess++
		}
	}
	probability := float64(sucess) / float64(games)
	const theoreticalValue = 2. / 3.

	errorPct := 100. * math.Abs(probability-theoreticalValue) / theoreticalValue

	fmt.Printf("Estimated probability: %9f\n", probability)
	fmt.Printf("Theoretical value: %9f\n", theoreticalValue)
	fmt.Printf("Error: %9f%%\n", errorPct)
}

// randomly sets the game
func setMontyHallGame() (int, int) {
	var (
		montysChoice int
		prizeDoor    int
		goat1Door    int
		goat2Door    int
		newDoor      int
	)

	guestDoor := rand.Intn(3)

	areDoorsSelected := false
	for !areDoorsSelected {
		prizeDoor = rand.Intn(3)
		goat1Door = rand.Intn(3)
		goat2Door = rand.Intn(3)
		if prizeDoor != goat1Door && prizeDoor != goat2Door && goat1Door != goat2Door {
			areDoorsSelected = true
		}
	}

	showGoat := false
	for !showGoat {
		montysChoice = rand.Intn(3)
		if montysChoice != prizeDoor && montysChoice != guestDoor {
			showGoat = true
		}
	}

	madeSwitch := false
	for !madeSwitch {
		newDoor = rand.Intn(3)
		if newDoor != guestDoor && newDoor != montysChoice {
			madeSwitch = true
		}
	}
	return newDoor, prizeDoor
}
