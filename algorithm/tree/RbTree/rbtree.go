package rbtree

import "fmt"

// color of node
const (
	RED   = 0
	BLACK = 1
)

type keytype interface {
	LessThan(interface{}) bool
}

type valuetype interface{}

type node struct {
	left, right, parent *node
	color               int
	Key                 keytype
	Value               valuetype
}

// Tree is a struct of red-black tree
type Tree struct {
	root *node
	size int
}

// NewTree returns a new rbtree
func NewTree() *Tree {
	return &Tree{}
}

// Find finds the node and return its value
func (t *Tree) Find(key keytype) interface{} {
	n := t.findnode(key)
	if n != nil {
		return n.Value
	}
	return nil
}

// FindIt finds the node and return it as a iterator
func (t *Tree) FindIt(key keytype) *node {
	return t.findnode(key)
}

// Empty checks whether the rbtree is empty
func (t *Tree) Empty() bool {
	if t.root == nil {
		return true
	}
	return false
}

// Iterator creates the rbtree's iterator that points to the minmum node
func (t *Tree) Iterator() *node {
	return minimum(t.root)
}

// Size returns the size of the rbtree
func (t *Tree) Size() int {
	return t.size
}

// Clear destroys the rbtree
func (t *Tree) Clear() {
	t.root = nil
	t.size = 0
}

// Insert inserts the key-value pair into the rbtree
func (t *Tree) Insert(key keytype, value valuetype) {
	x := t.root
	var y *node

	for x != nil {
		y = x
		if key.LessThan(x.Key) {
			x = x.left
		} else {
			x = x.right
		}
	}

	z := &node{parent: y, color: RED, Key: key, Value: value}
	t.size++

	if y == nil {
		z.color = BLACK
		t.root = z
		return
	} else if z.Key.LessThan(y.Key) {
		y.left = z
	} else {
		y.right = z
	}
	t.rbInsertFixup(z)

}

// Delete deletes the node by key
func (t *Tree) Delete(key keytype) {
	z := t.findnode(key)
	if z == nil {
		return
	}

	var x, y, parent *node
	y = z
	yOriginalColor := y.color
	parent = z.parent
	if z.left == nil {
		x = z.right
		t.transplant(z, z.right)
	} else if z.right == nil {
		x = z.left
		t.transplant(z, z.left)
	} else {
		y = minimum(z.right)
		yOriginalColor = y.color
		x = y.right

		if y.parent == z {
			if x == nil {
				parent = y
			} else {
				x.parent = y
			}
		} else {
			t.transplant(y, y.right)
			y.right = z.right
			y.right.parent = y
		}
		t.transplant(z, y)
		y.left = z.left
		y.left.parent = y
		y.color = z.color
	}

	if yOriginalColor == BLACK {
		t.rbDeleteFixup(x, parent)
	}
	t.size--
}

func (t *Tree) rbInsertFixup(z *node) {
	var y *node
	// 不可能存在两个连续红色的节点
	for z.parent != nil && z.parent.color == RED {
		if z.parent == z.parent.parent.left {
			y = z.parent.parent.right
			if y != nil && y.color == RED {
				// 兄弟节点时红色，不旋转
				z.parent.color = BLACK
				y.color = BLACK
				z.parent.parent.color = RED
				z = z.parent.parent
			} else {
				// 右节点是空或者右节点时黑色(黑色说明其深度肯定要比左节点的分支浅),所以最后要parent.parent在右旋转
				if z == z.parent.right {
					z = z.parent
					// 不一致点，左旋
					t.leftRotate(z)
				}
				z.parent.color = BLACK
				z.parent.parent.color = RED
				// 一致，右旋
				t.rightRotate(z.parent.parent)
			}
		} else {
			y = z.parent.parent.left
			if y != nil && y.color == RED {
				// 兄弟节点时红色，不旋转
				z.parent.color = BLACK
				y.color = BLACK
				z.parent.parent.color = RED
				z = z.parent.parent
			} else {
				// 左节点是空或者左节点时黑色(黑色说明其深度肯定要比右节点的分支浅),所以最后要parent.parent在左旋转
				if z == z.parent.left {
					z = z.parent
					// 不一致，右旋
					t.rightRotate(z)
				}
				z.parent.color = BLACK
				z.parent.parent.color = RED
				// 一致，左旋
				t.leftRotate(z.parent.parent)
			}
		}
	}
	t.root.color = BLACK
}

func (t *Tree) rbDeleteFixup(x, parent *node) {
	var w *node

	// x是红色,直接删除
	for x != t.root && getColor(x) == BLACK {
		if x != nil {
			parent = x.parent
		}
		if x == parent.left {
			// w是红色，p肯定是黑色,
			w = parent.right
			if w.color == RED {
				w.color = BLACK
				parent.color = RED
				t.leftRotate(parent)
				w = parent.right
			}
			// sl==sr==nil
			if getColor(w.left) == BLACK && getColor(w.right) == BLACK {
				w.color = RED
				x = parent
			} else {
				// sl是nil(以w左旋) == sr是nil的特殊情况,(以w右旋，然后以sl左旋)
				if getColor(w.right) == BLACK {
					if w.left != nil {
						w.left.color = BLACK
					}
					w.color = RED
					t.rightRotate(w)
					w = parent.right
				}
				w.color = parent.color
				parent.color = BLACK
				if w.right != nil {
					w.right.color = BLACK
				}
				t.leftRotate(parent)
				x = t.root
			}
		} else {
			w = parent.left
			if w.color == RED {
				w.color = BLACK
				parent.color = RED
				t.rightRotate(parent)
				w = parent.left
			}
			if getColor(w.left) == BLACK && getColor(w.right) == BLACK {
				w.color = RED
				x = parent
			} else {
				if getColor(w.left) == BLACK {
					if w.right != nil {
						w.right.color = BLACK
					}
					w.color = RED
					t.leftRotate(w)
					w = parent.left
				}
				w.color = parent.color
				parent.color = BLACK
				if w.left != nil {
					w.left.color = BLACK
				}
				t.rightRotate(parent)
				x = t.root
			}
		}
	}
	if x != nil {
		x.color = BLACK
	}
}

func (t *Tree) leftRotate(x *node) {
	y := x.right
	x.right = y.left
	if y.left != nil {
		y.left.parent = x
	}
	y.parent = x.parent
	if x.parent == nil {
		t.root = y
	} else if x == x.parent.left {
		x.parent.left = y
	} else {
		x.parent.right = y
	}
	y.left = x
	x.parent = y
}

func (t *Tree) rightRotate(x *node) {
	y := x.left
	x.left = y.right
	if y.right != nil {
		y.right.parent = x
	}
	y.parent = x.parent
	if x.parent == nil {
		t.root = y
	} else if x == x.parent.left {
		x.parent.left = y
	} else {
		x.parent.right = y
	}
	y.right = x
	x.parent = y
}

// Preorder prints the tree in pre order
func (t *Tree) Preorder() {
	fmt.Println("preorder begin!")
	if t.root != nil {
		t.root.preorder()
	}
	fmt.Println("preorder end!")
}

// findnode finds the node by key and return it,if not exists return nil
func (t *Tree) findnode(key keytype) *node {
	x := t.root
	for x != nil {
		if key.LessThan(x.Key) {
			x = x.left
		} else {
			if key == x.Key {
				return x
			}
			x = x.right
		}
	}
	return nil
}

// transplant transplants the subtree u and v
func (t *Tree) transplant(u, v *node) {
	if u.parent == nil {
		t.root = v
	} else if u == u.parent.left {
		u.parent.left = v
	} else {
		u.parent.right = v
	}
	if v == nil {
		return
	}
	v.parent = u.parent
}

// Next returns the node's successor as an iterator
func (n *node) Next() *node {
	return successor(n)
}

func (n *node) preorder() {
	fmt.Printf("(%v %v)", n.Key, n.Value)
	if n.parent == nil {
		fmt.Printf("nil")
	} else {
		fmt.Printf("whose parent is %v", n.parent.Key)
	}
	if n.color == RED {
		fmt.Println(" and color RED")
	} else {
		fmt.Println(" and color BLACK")
	}
	if n.left != nil {
		fmt.Printf("%v's left child is ", n.Key)
		n.left.preorder()
	}
	if n.right != nil {
		fmt.Printf("%v's right child is ", n.Key)
		n.right.preorder()
	}
}

// successor returns the successor of the node
func successor(x *node) *node {
	if x.right != nil {
		return minimum(x.right)
	}
	y := x.parent
	for y != nil && x == y.right {
		x = y
		y = x.parent
	}
	return y
}

// getColor gets color of the node
func getColor(n *node) int {
	if n == nil {
		return BLACK
	}
	return n.color
}

// minimum finds the minimum node of subtree n.
func minimum(n *node) *node {
	for n.left != nil {
		n = n.left
	}
	return n
}

// maximum finds the maximum node of subtree n.
func maximum(n *node) *node {
	for n.right != nil {
		n = n.right
	}
	return n
}
