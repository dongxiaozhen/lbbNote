package main

import (
	"fmt"
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

func dealLeaf(ptr unsafe.Pointer, count uint16) {
	// sizeLeaf := 4 * 4
	data := (*[0xFFFFFFF]byte)(ptr)
	buf := data[16:]
	aa := leafPageElements(unsafe.Pointer(&data[16]))

	// begin := uint32(16 * count)
	for i := uint16(0); i < count; i++ {
		l := aa[i]
		// 暂时先过滤非bucket数据
		if l.flags != 1 {
			continue
		}
		fmt.Println("--->leaf", l, "<---")
		first := uint32(i*16) + l.pos
		second := first + l.ksize

		// normal data
		if l.flags == 0 {
			fmt.Printf("--->valu-key:%s\t value:%s\n----", buf[first:second], buf[second:second+l.vsize])
		} else {
			// bucket data
			av := bucketPageElement(unsafe.Pointer(&buf[second]))
			fmt.Printf("---***---->bucket name:%s, bucket %v", buf[first:second], av)
			// inline page data |--key--|--bucket--|--page-|---data---|
			//                               16       16
			if av.root == 0 {
				ptr2 := unsafe.Pointer(&buf[second+16])
				ap := *(*page)(ptr2)
				fmt.Printf("--->valu-key:%s\t value:%v inlinePage %v\n----", buf[first:second], av, ap)
				deal(ptr2, ap)

			} else {
				fmt.Printf("--->valu-key:%s\t value:%v\n----", buf[first:second], av)
			}
		}
	}
}

func bucketPageElement(ptr unsafe.Pointer) bucket {
	return *((*bucket)(ptr))
}

func leafPageElements(ptr unsafe.Pointer) []leafPageElement {
	return ((*[0x7FFFFFF]leafPageElement)(ptr))[:]
}
