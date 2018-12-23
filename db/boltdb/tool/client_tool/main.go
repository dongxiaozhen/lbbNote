package main

import (
	"fmt"
	"os"
	"unsafe"
)

type page struct {
	id       uint64
	flags    uint16
	count    uint16
	overflow uint32
}

func (p page) String() string {
	return fmt.Sprintf("id:%d\t flags:%x\t count:%d\t overflow:%d\t", p.id, p.flags, p.count, p.overflow)
}

func main() {
	file, err := os.Open("my.db") // For read access.
	if err != nil {
		return
	}

	info, err := file.Stat()
	if err != nil {
		return
	}

	size := info.Size()

	psize := int64(os.Getpagesize())
	num := size / psize
	fmt.Printf("-------------------------page num: %d---------------------\n", num)

	// page size 16
	buf := [16]byte{}

	for i := int64(0); i < num; i++ {
		offset := psize * i
		n, err := file.ReadAt(buf[:], offset)
		if err != nil {
			continue
		}
		if n != 16 {
			fmt.Println("size not equip 16")
			continue
		}
		aa := *(*page)(unsafe.Pointer(&buf))

		if aa.flags == 4 {
			fmt.Println("----------------------------------------------------------meta------------------------------------------------------------")
			fmt.Printf("%v\n", aa)
			dealMeta(file, offset)
		}
		if i >= 2 {
			if i == int64(m2.root.root) {
				fmt.Println("----------------------------------------------------------root---------------------------------------------------------------------------")
			}
		}
		if aa.flags == 2 {
			fmt.Println("----------------------------------------------------------leaf---------------------------------------------------------------------------")
			fmt.Printf("bucket %v\n", aa)
			// dealLeaf(file, offset, aa.count)
		}
		if aa.flags == 1 {
			fmt.Println("----------------------------------------------------------branch---------------------------------------------------------------------------")
			fmt.Printf("bucket %v\n", aa)
			dealBranch(file, offset, aa.count)
		}
	}
}

type pgid uint64
type txid uint64
type bucket struct {
	root     pgid   // page id of the bucket's root-level page
	sequence uint64 // monotonically incrementing, used by NextSequence()
}
