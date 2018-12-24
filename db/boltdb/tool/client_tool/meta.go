package main

import (
	"fmt"
	"unsafe"
)

type meta struct {
	magic    uint32
	version  uint32
	pageSize uint32
	flags    uint32
	root     bucket
	freelist pgid
	pgid     pgid
	txid     txid
	checksum uint64
}

func (p meta) String() string {
	return fmt.Sprintf("mgc:%x\t version:%d\t pageSize:%d\t flags:%x\t root:%v freelist:%d pgid:%d txid:%d checksum:%d", p.magic, p.version, p.pageSize, p.flags, p.root, p.freelist, p.pgid, p.txid, p.checksum)
}

var m0 meta
var m1 meta
var m2 meta

func dealMeta(ptr unsafe.Pointer, offset int64) {
	data := *(*[4096]byte)(ptr)
	buf := data[16:]

	aa := *(*meta)(unsafe.Pointer(&buf[0]))
	fmt.Printf("%v\n\n", aa)
	if offset == 0 {
		m0 = aa
	} else {
		m1 = aa

		if m0.txid > m1.txid {
			m2 = m0
		} else {
			m2 = m1
		}
	}
}
