package main

import (
	"fmt"
	"math/rand"
)

const LevelMax = 4

type Element struct {
	nexts []*Element
	key   int
	value int
}

type skipList struct {
	front  Element
	length int
}

func newSkipList() *skipList {
	re := &skipList{}
	re.front.nexts = make([]*Element, LevelMax)
	return re
}

func (list *skipList) list() {

	for level := LevelMax - 1; level >= 0; level-- {
		fmt.Println("----------level", level)

		next := list.front.nexts[level]
		for next != nil {
			fmt.Println("value info", next.key, next.value, len(next.nexts))
			next = next.nexts[level]
		}
	}
}

func (list *skipList) set(key, value int) {

	ppv := list.getPrev(key)
	if element := ppv[0].nexts[0]; element != nil {
		element.value = value
	}

	level := rand.Intn(LevelMax + 1)
	fmt.Println("random level", level)
	if level == 0 {
		level = 1
	}
	newElement := &Element{
		nexts: make([]*Element, level),
		key:   key,
		value: value,
	}

	for i := range newElement.nexts {
		newElement.nexts[i] = ppv[i].nexts[i]
		ppv[i].nexts[i] = newElement
	}
}

func (list *skipList) get(key int) int {

	prev := &list.front
	var next *Element
	for level := LevelMax - 1; level >= 0; level-- {

		next = prev.nexts[level]
		if next != nil {
			fmt.Println("---------- get level", level, next.key, next.value, len(next.nexts))
		}
		for next != nil && key >= next.key {
			prev = next
			next = next.nexts[level]
			if next != nil {
				fmt.Println("22----------22 get level", level, next.key, next.value, len(next.nexts))
			} else {
				fmt.Println("22--nil:")
			}
		}
		fmt.Println("---------- get level over", level, prev)
	}

	if prev != nil && prev.key == key {
		return prev.value
	}
	return 0
}

func (list *skipList) del(key int) {
}

func (list *skipList) getPrev(key int) []*Element {
	ret := make([]*Element, LevelMax)

	prev := &list.front
	for level := LevelMax - 1; level >= 0; level-- {
		next := prev.nexts[level]
		for next != nil && (key >= next.key) {
			prev = next
			next = next.nexts[level]
		}

		ret[level] = prev
	}
	fmt.Println("getPrev", ret)
	return ret
}

func main() {
	fmt.Println("vim-go")

	list := newSkipList()
	list.set(1, 1)
	list.set(2, 2)
	list.set(3, 3)
	list.set(4, 4)

	list.set(14, 14)
	list.set(10, 10)
	list.set(44, 44)
	list.set(24, 24)

	list.list()

	fmt.Println("get ret", list.get(10))
	fmt.Println("get ret", list.get(9))
}
