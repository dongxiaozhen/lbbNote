package main

import (
	"fmt"
	"math"
)

type Node struct {
	data  int
	left  *Node
	right *Node
	depth int
}

func (n *Node) Print() {
	if n == nil {
		fmt.Println("null")
		return
	}

	fmt.Println(n.data, n.depth)
	n.left.Print()
	n.right.Print()
}

func (n *Node) Insert(data int) {
	if n.data == data {
		return
	}

	if n.data > data {
		if n.left == nil {
			n.left = &Node{data: data}
		} else {
			n.left.Insert(data)
		}

		if childrenDepth(n.left)-childrenDepth(n.right) == 2 {
			if data < n.left.data {
				// left-left
				fmt.Println("left-left", data, n.left.data)
			} else {
				// left-right
				fmt.Println("left-right", data, n.left.data)
			}

		}
	} else {
		if n.right == nil {
			n.right = &Node{data: data}
		} else {
			n.right.Insert(data)
		}

		if childrenDepth(n.right)-childrenDepth(n.left) == 2 {
			if data > n.right.data {
				// right-right
				fmt.Println("right-right", data, n.right.data)

			} else {
				// right-left
				fmt.Println("right-left", data, n.right.data)
			}
		}
	}

	n.depth = int(math.Max(childrenDepth(n.left), childrenDepth(n.right))) + 1
}

func childrenDepth(n *Node) float64 {
	if n == nil {
		return float64(-1)
	}
	return float64(n.depth)
}

type tree struct {
	root *Node
}

func (r *tree) Print() {
	if r == nil {
		fmt.Println("nil")
		return
	}
	r.root.Print()
}

func (r *tree) Insert(data int) {
	if r.root == nil {
		r.root = &Node{data: data}
		return
	}

	r.root.Insert(data)
}

func main() {
	r := &tree{}

	r.Print()

	r.Insert(1)
	r.Insert(2)
	r.Insert(3)
	// r.Insert(0)
	// r.Insert(4)
	r.Print()
}
