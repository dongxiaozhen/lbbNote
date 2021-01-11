package main

import (
	"fmt"
	"unsafe"
)

// structure A 4字节
type structa_tag struct {
	c byte
	s uint16
}

// structure B 8字节
type structb_tag struct {
	s uint16
	c byte
	i int32
}

// structure C 24字节
type structc_tag struct {
	c byte
	d uint64
	s int32
}

// structure D 16字节
type structd_tag struct {
	d uint64
	s int32
	c byte
}

func main() {
	structa_t := structa_tag{}
	structb_t := structb_tag{}
	structc_t := structc_tag{}
	structd_t := structd_tag{}

	fmt.Printf("sizeof(structa_t) = %d\n", unsafe.Sizeof(structa_t))
	fmt.Printf("sizeof(structb_t) = %d\n", unsafe.Sizeof(structb_t))
	fmt.Printf("sizeof(structc_t) = %d\n", unsafe.Sizeof(structc_t))
	fmt.Printf("sizeof(structd_t) = %d\n", unsafe.Sizeof(structd_t))
}
