package main

import (
	"fmt"
	"sort"
)

// 插入： 一只查找到叶子节点然后添加，同时在父节点里判断孩子节点是否需要分裂
// 删除： 找到就删除，同时在父节点里判断孩子是否需要合并

func main() {

	b := NewBTree(5)
	for i := int32(1); i <= 19; i++ {
		b.ReplaceOrInsert(i)
	}

	fmt.Println("begin loop")
	b.Loop()

	fmt.Println("begin delete")
	b.Delete(5)
	b.Delete(1)
	b.Delete(10)
	b.Loop()
}

func NewBTree(degree int) *BTree {
	return &BTree{degree: degree}
}

type BTree struct {
	degree int
	length int
	root   *node
}

func (b *BTree) maxItem() int {
	return b.degree*2 - 1
}

func (b *BTree) minItem() int {
	return b.degree - 1
}

func (b *BTree) Loop() {
	b.root.Loop(1)
}

type node struct {
	items []int32
	child []*node
}

func (n *node) Loop(level int) {
	fmt.Println("level:", level, "items:", n.items)
	for _, child := range n.child {
		child.Loop(level + 1)
	}
}

func (n *node) String() string {
	return fmt.Sprintf("%v", n.items)
}

func (n *node) split(i int) (int32, *node) {
	item := n.items[i]

	nn := &node{}
	nn.items = append(nn.items, n.items[i+1:]...)
	n.items = n.items[:i]

	if len(n.child) > 0 {
		nn.child = append(nn.child, n.child[i+1:]...)
		n.child = n.child[:i]
	}
	return item, nn

}

func (b *BTree) ReplaceOrInsert(value int32) {
	if b.root == nil {
		b.root = &node{items: []int32{value}}
		b.length++
		return
	} else {
		if len(b.root.items) > b.maxItem() {
			item2, second := b.root.split(b.maxItem() / 2)
			oldRoot := b.root
			b.root = &node{items: []int32{item2}, child: []*node{oldRoot, second}}
		}
	}

	insertValue := b.root.insert(value, b.maxItem())
	if insertValue != 0 {
		b.length++
	}
}

func (n *node) insert(value int32, maxItem int) int32 {
	index, finded := n.find(value)
	if finded {
		n.items[index] = value
		return 0
	}

	if len(n.child) == 0 {
		n.insertItemAt(index, value)
		return value
	}

	if n.maybeSplitChild(index, maxItem) {
		oldValue := n.items[index]
		if value < oldValue {
		} else if value > oldValue {
			index++
		} else {
			return 0
		}
	}
	return n.child[index].insert(value, maxItem)
}
func (n *node) insertItemAt(index int, value int32) {
	n.items = append(n.items, 0)
	if index < len(n.items) {
		copy(n.items[index+1:], n.items[index:])
	}
	n.items[index] = value
}

func (n *node) insertChildAt(index int, second *node) {
	n.child = append(n.child, nil)
	if index < len(n.child) {
		copy(n.child[index+1:], n.child[index:])
	}
	n.child[index] = second
}

func (n *node) maybeSplitChild(index int, maxItem int) bool {

	if len(n.child[index].items) < maxItem {
		return false
	}

	first := n.child[index]
	value, second := first.split(maxItem / 2)
	n.insertItemAt(index, value)
	n.insertChildAt(index+1, second)
	return true
}

func (n *node) find(value int32) (int, bool) {
	index := sort.Search(len(n.items), func(i int) bool {
		if n.items[i] > value {
			return true
		}
		return false
	})

	if index > 0 && n.items[index-1] == value {
		return index - 1, true
	}
	return index, false
}

func (b *BTree) Delete(value int32) {
	b.deleteItem(value, RemoveItem)
}

func (b *BTree) deleteItem(value int32, rtype int32) {
	if b.root == nil || len(b.root.items) < 1 {
		return
	}
	removed := b.root.remove(value, rtype, b.minItem())
	if len(b.root.items) == 0 && len(b.root.child) > 0 {
		b.root = b.root.child[0]
	}
	if removed {
		b.length--
	}
	return
}

func (n *node) remove(value int32, rtype int32, minNum int) bool {
	var i int
	var found bool
	switch rtype {
	case RemoveMin:
		if len(n.child) < 1 {
			n.items = n.items[1:]
			return true
		}
		i = 0
	case RemoveMax:
		if len(n.child) < 1 {
			n.items = n.items[:len(n.items)-1]
			return true
		}
		i = len(n.items)
	case RemoveItem:
		i, found = n.find(value)
		if len(n.child) < 1 {
			if found {
				copy(n.items[i:], n.items[i+1:])
				n.items = n.items[:len(n.items)-1]
				return true
			}
			return false
		}
	}
	if len(n.child[i].items) < minNum {
		return n.maybeMergeChild(i, value, rtype, minNum)
	}

	child := n.child[i]
	if found {
		n.items[i] = child.items[len(child.items)-1]
		child.remove(0, RemoveMax, minNum)
		return true
	}
	return n.child[i].remove(value, rtype, minNum)
}

func (n *node) maybeMergeChild(i int, value int32, rtype int32, minNum int) bool {
	if i > 0 && len(n.child[i-1].items) > minNum {
		// 向左借一个
		// 父节点里的拿进来放到第一位
		// 左节点去掉一个最大值放到父节点
		child := n.child[i]
		steal := n.child[i-1]
		sitem := steal.items[len(steal.items)-1]
		steal.items = steal.items[:len(steal.items)-2]
		if len(steal.child) > 0 {
			child.child = append(child.child, nil)
			child.child[0] = steal.child[len(steal.child)-1]
			steal.child = steal.child[:len(steal.child)-2]
		}
		child.items = append(child.items, 0)
		copy(child.items[1:], child.items[0:])
		child.items[0] = n.items[i]

		n.items[i] = sitem
	} else if i < len(n.items) && len(n.child[i+1].items) > minNum {
		// 向右借一个
		// 父节点的拿进来放到末尾
		// 右节点选择最小的一个放到父节点
		child := n.child[i]
		steal := n.child[i+1]
		sitem := steal.items[0]
		steal.items = steal.items[1:]
		if len(steal.child) > 0 {
			child.child = append(child.child, steal.child[0])
			steal.child = steal.child[1:]
		}
		child.items = append(child.items, n.items[i])
		n.items[i] = sitem
	} else {
		// 合并右面的
		// 右节点的所有信息和父节点的一个 都合并到当前节点
		// 删除父节点里的右节点
		if i >= len(n.items) {
			i--
		}
		child := n.child[i]
		child.items = append(child.items, n.items[i])
		child.items = append(child.items, n.child[i+1].items...)
		child.child = append(child.child, n.child[i+1].child...)

		copy(n.child[i+1:], n.child[i+2:])
		n.child = n.child[:len(n.child)-1]
		copy(n.items[i:], n.items[i+1:])
		n.items = n.items[:len(n.items)-1]
	}

	return n.remove(value, rtype, minNum)
}

const (
	RemoveItem int32 = 1 + iota
	RemoveMax
	RemoveMin
)
