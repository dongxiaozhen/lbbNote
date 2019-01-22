package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
)

func main() {
	sstable()
	// log()
	// manifest()
}
func readBytes(r *bytes.Buffer) []byte {
	n, err := binary.ReadUvarint(r)
	if err != nil {
		return nil
	}
	fmt.Println("readbytes:", n)
	x := make([]byte, n)
	_, err = io.ReadFull(r, x)
	if err != nil {
		return nil
	}

	return x
}

const (
	fullChunkType   = 1
	firstChunkType  = 2
	middleChunkType = 3
	lastChunkType   = 4
)
