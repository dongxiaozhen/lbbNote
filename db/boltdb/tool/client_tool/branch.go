package main

import (
	"fmt"
	"os"
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

func dealBranch(file *os.File, offset int64, count uint16) {
	// sizeLeaf := 4 * 4
	buf := [4080]byte{}
	n, err := file.ReadAt(buf[:], offset+16)
	if err != nil {
		return
	}
	if n != 4080 {
		fmt.Println("size not equip 100")
		return
	}
	fmt.Println("xxxx", offset, count)

	aa := branchPageElements(unsafe.Pointer(&buf))

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
