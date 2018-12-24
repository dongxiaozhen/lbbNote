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
	buf := [4096]byte{}

	// 先读取两个meta数据
	for i := int64(0); i < 2; i++ {
		offset := psize * i
		n, err := file.ReadAt(buf[:], offset)
		if err != nil {
			continue
		}
		if n != 4096 {
			fmt.Println("size not equip 16")
			continue
		}
		ptr := unsafe.Pointer(&buf[0])
		aa := *(*page)(ptr)
		if aa.flags == 4 {
			fmt.Println("----------------------------------------------------------meta------------------------------------------------------------")
			fmt.Printf("meta %v\n", aa)
			dealMeta(ptr, offset)
		}
	}

	for i := int64(2); i < num; i++ {
		fmt.Println("begin enter", i)
		buf1 := make([]byte, 4096)
		offset := psize * i
		n, err := file.ReadAt(buf1, offset)
		if err != nil || n != 4096 {
			fmt.Println("size not equal 4096", err)
			continue
		}
		vptr := unsafe.Pointer(&buf1[0])
		vaa := *(*page)(vptr)
		if vaa.overflow > 0 {
			fmt.Println("xxete", vaa, i)
			length := (vaa.overflow + 1) * 4096
			buf1 = make([]byte, length)
			n, err := file.ReadAt(buf1, offset)
			if err != nil || uint32(n) != length {
				continue
			}
			i += int64(vaa.overflow)
		}

		ptr := unsafe.Pointer(&buf1[0])
		aa := *(*page)(ptr)
		deal(ptr, aa)
	}
}

func deal(ptr unsafe.Pointer, aa page) {
	fmt.Println("deal------------page", aa)
	switch aa.flags {
	// leaf data ,contain normal-data and bucket-data
	case 2:
		fmt.Println("----------------------------------------------------------leaf---------------------------------------------------------------------------")
		fmt.Printf("leaf %v\n", aa)
		dealLeaf(ptr, aa.count)

	case 1:
		fmt.Println("----------------------------------------------------------b+tree---------------------------------------------------------------------------")
		fmt.Printf("b+tree %v\n", aa)
		dealBranch(ptr, aa.count)

	case 0x10:
		fmt.Println("----------------------------------------------------------freelist---------------------------------------------------------------------------")
		fmt.Printf("free %v\n", aa)
		dealFreeList(ptr, aa.count)
	}
}

type pgid uint64
type txid uint64
type bucket struct {
	root     pgid   // page id of the bucket's root-level page
	sequence uint64 // monotonically incrementing, used by NextSequence()
}
