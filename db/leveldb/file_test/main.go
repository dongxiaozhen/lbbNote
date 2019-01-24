package main

import (
	"flag"
)

var index = flag.Int64("index", 1, "1:table,2:log,3:manifest")

func main() {
	flag.Parse()

	switch *index {
	case 1:
		sstable()
	case 2:
		log()
	case 3:
		manifest()
	default:
	}
}

const (
	fullChunkType   = 1
	firstChunkType  = 2
	middleChunkType = 3
	lastChunkType   = 4
)
