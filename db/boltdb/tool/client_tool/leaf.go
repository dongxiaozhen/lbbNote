package main

import (
	"fmt"
	"os"
	"unsafe"
)

type leafPageElement struct {
	flags uint32
	pos   uint32
	ksize uint32
	vsize uint32
}

func (p leafPageElement) String() string {
	return fmt.Sprintf("flags:%x\t pos:%d\t ksize:%d\t vsize:%d\t ", p.flags, p.pos, p.ksize, p.vsize)
}

func dealLeaf(file *os.File, offset int64, count uint16) {
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

	aa := leafPageElements(unsafe.Pointer(&buf))

	// begin := uint32(16 * count)
	for i := uint16(0); i < count; i++ {
		l := aa[i]
		fmt.Println("--->leaf", l, "<---")
		first := uint32(i*16) + l.pos
		second := first + l.ksize
		fmt.Printf("--->valu-key:%s\t value:%s\n----", buf[first:second], buf[second:second+l.vsize])
	}
}
func leafPageElements(ptr unsafe.Pointer) []leafPageElement {
	return ((*[0x7FFFFFF]leafPageElement)(ptr))[:]
}
