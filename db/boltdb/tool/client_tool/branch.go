package main

import (
	"fmt"
	"unsafe"
)

// branchPageElement represents a node on a branch page.
type branchPageElement struct {
	pos   uint32
	ksize uint32
	pgid  pgid
}

func (p branchPageElement) String() string {
	return fmt.Sprintf("pos:%d\t ksize:%d\t pgid:%d\t ", p.pos, p.ksize, p.pgid)
}

func dealBranch(ptr unsafe.Pointer, count uint16) {
	// sizeLeaf := 4 * 4
	data := *(*[4096]byte)(ptr)
	buf := data[16:]

	aa := branchPageElements(unsafe.Pointer(&buf[0]))

	// begin := uint32(16 * count)
	for i := uint16(0); i < count; i++ {
		l := aa[i]
		first := uint32(i*16) + l.pos
		second := first + l.ksize
		fmt.Printf("--->branch---%v <------->%s\n", l, buf[first:second])
	}
}

func branchPageElements(ptr unsafe.Pointer) []branchPageElement {
	return ((*[0x7FFFFFF]branchPageElement)(ptr))[:]
}
