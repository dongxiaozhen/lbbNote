package main

import (
	"encoding/binary"
	"fmt"
	"io/ioutil"

	"github.com/golang/snappy"
)

func sstable() {
	data, err := ioutil.ReadFile("000033.ldb")
	if err != nil {
		fmt.Println(err)
		return
	}
	dataLen := len(data)
	foot := data[dataLen-48:]
	_, tmp := decodeBlockHandle(foot)
	b, _ := decodeBlockHandle(foot[tmp:])
	// fmt.Println(a)
	fmt.Println(b.offset, b.length)
	fmt.Printf("%d,%x\n", dataLen, foot[40:])

	indexData := data[b.offset:]
	bh := b
	compareBool := indexData[b.length]
	fmt.Println("comarebool", compareBool)
	bdata := indexData[:b.length]
	if compareBool == 1 {
		decLen, err := snappy.DecodedLen(indexData[:bh.length])
		if err != nil {
			fmt.Println("snappy decodelen", err)
			return
		}
		fmt.Println("snapp", bh.length, decLen)
		decData := make([]byte, decLen)
		decData, err = snappy.Decode(decData, indexData[:bh.length])
		if err != nil {
			fmt.Println("snappy decode err", err)
			return
		}
		bdata = decData
	}
	// fmt.Println("bdata", len(bdata))
	blk, err := readBlock(bdata)
	if err != nil {
		fmt.Println("read block err", err)
		return
	}
	fmt.Println("blk", blk.restartsLen, blk.restartsOffset)
	for i := 0; i < blk.restartsLen; i++ {
		blk.read(i)
	}
}

type block struct {
	data           []byte
	restartsLen    int
	restartsOffset int
}

func (b *block) read(rstart int) {
	i := 0
	// restart里读取offset
	offset := int(binary.LittleEndian.Uint32(b.data[b.restartsOffset+4*(rstart+i):]))
	offset++                                    // shared always zero, since this is a restart point
	v1, n1 := binary.Uvarint(b.data[offset:])   // key length
	_, n2 := binary.Uvarint(b.data[offset+n1:]) // value length
	m := offset + n1 + n2
	key := b.data[m : m+int(v1)]
	value := b.data[m+int(v1):]
	bhd, n := decodeBlockHandle(value)
	fmt.Println("key", rstart, offset, string(key), bhd, n)
}

func readBlock(data []byte) (*block, error) {
	restartsLen := int(binary.LittleEndian.Uint32(data[len(data)-4:]))
	b := &block{
		data:           data,
		restartsLen:    restartsLen,
		restartsOffset: len(data) - (restartsLen+1)*4,
	}
	ss := data[b.restartsOffset:]
	offsets := make([]uint32, b.restartsLen)
	for i := 0; i < restartsLen; i++ {
		offset := binary.LittleEndian.Uint32(ss)
		ss = ss[4:]
		offsets[i] = offset
	}
	fmt.Println(offsets)
	return b, nil
}

func decodeBlockHandle(src []byte) (blockHandle, int) {
	offset, n := binary.Uvarint(src)
	length, m := binary.Uvarint(src[n:])
	if n == 0 || m == 0 {
		return blockHandle{}, 0
	}
	return blockHandle{offset, length}, n + m
}

type blockHandle struct {
	offset, length uint64
}
