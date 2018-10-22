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

// 左左型，右旋操作
func (n *Node) R_Rotate() *Node {
	fmt.Println("r_rotate", *n)
	c := n.left

	n.left = c.right
	c.right = n

	n.depth = int(math.Max(childrenDepth(n.left), childrenDepth(n.right))) + 1
	c.depth = int(math.Max(childrenDepth(c.left), childrenDepth(c.right))) + 1
	return c
}

// 右右型，左旋操作
func (n *Node) L_Rotate() *Node {
	fmt.Println("l_rotate", *n)
	c := n.right

	n.right = c.left
	c.left = n

	n.depth = int(math.Max(childrenDepth(n.left), childrenDepth(n.right))) + 1
	c.depth = int(math.Max(childrenDepth(c.left), childrenDepth(c.right))) + 1
	return c
}

//右-左型，进行右旋，再左旋
func (n *Node) R_L_Rotate() *Node {
	//先中间节点旋转
	// fmt.Println("r_l_rotate", *n)
	n.right = n.right.R_Rotate()
	return n.L_Rotate()
}

//左-右型，进行左旋，再右旋
func (n *Node) L_R_Rotate() *Node {
	// fmt.Println("l_r_rotate", *n)
	n.left = n.left.L_Rotate()
	return n.R_Rotate()
}

// 1 在倒数第二排插入一个新节点
// 2 检查节点的平衡状态
// 3 更新当前节点的depth
// 4 更新父节点孩子指针
func (n *Node) Insert(data int) *Node {
	if n.data == data {
		return n
	}

	t := n
	if n.data > data {
		if n.left == nil {
			n.left = &Node{data: data}
		} else {
			n.left = n.left.Insert(data)
		}

		if childrenDepth(n.left)-childrenDepth(n.right) == 2 {
			if data < n.left.data {
				// left-left
				t = n.R_Rotate()
			} else {
				// left-right
				t = n.L_R_Rotate()
			}

		}
	} else {
		if n.right == nil {
			n.right = &Node{data: data}
		} else {
			n.right = n.right.Insert(data)
		}

		if childrenDepth(n.right)-childrenDepth(n.left) == 2 {
			if data > n.right.data {
				// right-right
				t = n.L_Rotate()

			} else {
				// right-left
				t = n.R_L_Rotate()
			}
		}
	}

	n.depth = int(math.Max(childrenDepth(n.left), childrenDepth(n.right))) + 1
	return t
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

	r.root = r.root.Insert(data)
}

func main() {
	r := &tree{}

	r.Print()
	fmt.Println("-----------------------------------")

	r.Insert(3)
	r.Print()
	fmt.Println("-----------------------------------")
	r.Insert(2)
	r.Print()
	fmt.Println("-----------------------------------")
	r.Insert(1)
	r.Print()
	fmt.Println("-----------------------------------")
	r.Insert(4)
	r.Print()
	fmt.Println("-----------------------------------")
	r.Insert(5)
	r.Print()
	fmt.Println("-----------------------------------")
	r.Insert(6)
	r.Print()
	fmt.Println("-----------------------------------")
	r.Insert(7)
	r.Print()
	fmt.Println("-----------------------------------")
	r.Insert(10)
	r.Print()
	fmt.Println("-----------------------------------")
	r.Insert(9)
	r.Print()
	fmt.Println("-----------------------------------")
	// r.Insert(8)
	// r.Print()
	// fmt.Println("-----------------------------------")
	// r.Print()
}
