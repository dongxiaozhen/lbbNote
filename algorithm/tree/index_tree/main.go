package main

import (
	"fmt"
	"math"
)

type Info struct {
	info  []int
	tree  []int
	index []int
	sum   []int
	num   int
}

func (i *Info) calBitValue(index int) {
	bigIndex := index
	maxPower := getBit(index)
	i.index[index] = maxPower

	loop := 0
	sum := 0
	for {
		if loop == 0 {
			sum += i.info[index]
		} else {
			sum += i.tree[index]
		}

		index = bigIndex - int(math.Pow(2, float64(loop)))
		loop++
		if loop >= maxPower {
			break
		}
	}
	i.tree[bigIndex] = sum
}

func (i *Info) build() {
	printSlice("build before", i.info)
	for index := 0; index < len(i.info); index++ {
		i.calBitValue(index)
	}
}

func (i *Info) print() {
	printSlice("build index1", i.index)
	printSlice("build tree1 ", i.tree)
	printSlice("build sum1  ", i.sum)

}

func (i *Info) getSum(endIndex int) int {
	sum := 0
	if endIndex == 0 {
		sum += i.tree[0]
	}
	for endIndex > 0 {
		sum += i.tree[endIndex]
		endIndex -= int(math.Pow(2, float64(getBit(endIndex)-1)))
	}
	return sum
}
func (i *Info) getParents(index int) []int {
	ret := make([]int, 0, 1)
	ret = append(ret, index)
	for {
		curStep := math.Pow(2, float64(getBit(index)-1))
		index += int(curStep)
		if index >= i.num {
			break
		}
		ret = append(ret, index)
	}
	return ret
}

func (i *Info) updatePoint(index int, value int) {
	parrents := i.getParents(index)
	fmt.Println("parents", index, parrents)
	i.info[index] = value
	for _, parrent := range parrents {
		i.calBitValue(parrent)
	}
	fmt.Println(i.info[index], i.tree[index])
}
func NewInfo(info []int) *Info {
	a := Info{info: info}
	a.num = len(info)
	a.tree = make([]int, a.num)
	a.index = make([]int, a.num)
	a.sum = make([]int, a.num)
	for i := 1; i < a.num; i++ {
		a.sum[i] = a.info[i] + a.sum[i-1]
	}
	return &a
}

func getBit(index int) int {
	if index <= 0 {
		return 0
	}
	index = index & (-index)
	return int(math.Log2(float64(index))) + 1
}

func main() {
	var info []int
	for i := 0; i < 18; i++ {
		info = append(info, i)
	}

	a := NewInfo(info)
	a.build()
	a.print()

	// fmt.Printf("cal result:  ")
	// for i := 0; i < a.num; i++ {
	// fmt.Printf("%d\t", a.getSum(i))
	// }
	// fmt.Printf("\n")

	a.updatePoint(8, 18)
	a.print()

	// for i := 0; i < 36; i++ {
	// fmt.Printf("%d, %b, %d, %b \n", i, i, getBit(i), getBit(i))
	// }
}

func printSlice(msg string, info []int) {
	fmt.Printf("%s", msg)
	for _, v := range info {
		fmt.Printf("%d\t", v)
	}
	fmt.Println("\n")
}
