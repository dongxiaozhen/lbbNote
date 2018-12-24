package main

import (
	"fmt"
	"unsafe"
)

func dealFreeList(ptr unsafe.Pointer, count uint16) {
	// sizeLeaf := 4 * 4
	data := *(*[4096]byte)(ptr)
	buf := data[16:]

	aa := freeListPageElements(unsafe.Pointer(&buf[0]))
	aa = aa[:count]

	fmt.Printf("free-list %v\n", aa)
}

func freeListPageElements(ptr unsafe.Pointer) []uint64 {
	return ((*[0x7FFFFFF]uint64)(ptr))[:]
}
