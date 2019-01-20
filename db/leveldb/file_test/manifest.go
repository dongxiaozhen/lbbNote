package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io/ioutil"
)

const (
	recComparer    = 1
	recJournalNum  = 2
	recNextFileNum = 3
	recSeqNum      = 4
	recCompPtr     = 5
	recDelTable    = 6
	recAddTable    = 7
	// 8 was used for large value refs
	recPrevJournalNum = 9
)

func manifest() {
	data, err := ioutil.ReadFile("MANIFEST-000000")
	if err != nil {
		fmt.Println(err)
		return
	}
	all := len(data)
	tmp := 0

	defer func() {
		for i := range srds {
			fmt.Println(*srds[i])
		}
	}()
	for {
		length := readManifestblk(data)
		tmp = tmp + 7 + int(length)
		if tmp >= all {
			return
		}
		data = data[7+length:]
	}
}

var srds = []*sessionRecord{}

type sessionRecord struct {
	hasRec         int
	comparer       string
	journalNum     int64
	prevJournalNum int64
	nextFileNum    int64
	seqNum         uint64
	compPtrs       []cpRecord
	addedTables    []atRecord
	deletedTables  []dtRecord
}

func (s sessionRecord) String() string {
	ss := fmt.Sprintf("comparer:%s,journalNum:%d,prevJournalNum:%d,nextFileNum:%d,seqNum:%d", s.comparer, s.journalNum, s.prevJournalNum, s.nextFileNum, s.seqNum)
	s1 := fmt.Sprintf("comptrs:%v", s.compPtrs)
	s2 := fmt.Sprintf("addedTables:%v", s.addedTables)
	s3 := fmt.Sprintf("deletedTables:%v", s.deletedTables)
	return ss + s1 + s2 + s3
}

type cpRecord struct {
	level int
	ikey  []byte
}

func (c cpRecord) String() string {
	return fmt.Sprintf("level:%d,key:%s", c.level, c.ikey)
}

type atRecord struct {
	level int
	num   int64
	size  int64
	imin  []byte
	imax  []byte
}

func (c atRecord) String() string {
	return fmt.Sprintf("level:%d,num:%d,size:%d,imin:%s,imax:%s", c.level, c.num, c.size, c.imin, c.imax)
}

type dtRecord struct {
	level int
	num   int64
}

func (c dtRecord) String() string {
	return fmt.Sprintf("level:%d,num:%d", c.level, c.num)
}

func readManifestblk(data []byte) (length uint16) {
	var err error
	checksum := binary.LittleEndian.Uint32(data)
	length = binary.LittleEndian.Uint16(data[4:6])
	chunkType := data[6]
	fmt.Println("checksum,length,chunkType", checksum, length, chunkType)

	r := bytes.NewBuffer(data[7 : 7+length])

	srd := &sessionRecord{}
	defer func() {
		srds = append(srds, srd)
	}()
	for err == nil {
		rec, err := binary.ReadUvarint(r)
		if err != nil {
			return
		}

		var x uint64
		switch rec {
		case recComparer:
			x := readBytes(r)
			if err == nil {
				srd.comparer = string(x)
			}
		case recJournalNum:
			x, err = binary.ReadUvarint(r)
			if err == nil {
				srd.journalNum = int64(x)
			}
		case recPrevJournalNum:
			x, err = binary.ReadUvarint(r)
			if err == nil {
				srd.prevJournalNum = int64(x)
			}
		case recNextFileNum:
			x, err = binary.ReadUvarint(r)
			if err == nil {
				srd.nextFileNum = int64(x)
			}
		case recSeqNum:
			x, err = binary.ReadUvarint(r)
			if err == nil {
				srd.seqNum = x
			}

		case recCompPtr:
			level, err := binary.ReadUvarint(r)
			ikey := readBytes(r)
			if err == nil {
				srd.compPtrs = append(srd.compPtrs, cpRecord{int(level), ikey})
			}
		case recAddTable:
			level, err := binary.ReadUvarint(r)
			num, err := binary.ReadUvarint(r)
			size, err := binary.ReadUvarint(r)
			imin := readBytes(r)
			imax := readBytes(r)
			if err == nil {
				srd.addedTables = append(srd.addedTables, atRecord{int(level), int64(num), int64(size), imin, imax})
			}
		case recDelTable:
			level, err := binary.ReadUvarint(r)
			num, err := binary.ReadUvarint(r)
			if err == nil {
				srd.deletedTables = append(srd.deletedTables, dtRecord{int(level), int64(num)})
			}
		}
	}
	return
}
