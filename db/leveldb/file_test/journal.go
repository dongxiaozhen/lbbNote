package main

import (
	"encoding/binary"
	"fmt"
	"io/ioutil"
)

func log() {
	data, err := ioutil.ReadFile("020385.log")
	if err != nil {
		fmt.Println(err)
		return
	}
	for {
		checksum := binary.LittleEndian.Uint32(data[:4])
		length := binary.LittleEndian.Uint16(data[4:])
		chunkType := data[6]
		fmt.Println("log", checksum, length, chunkType)
		return
	}

}
