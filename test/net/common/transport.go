package common

import (
	"encoding/binary"
	"fmt"
	"net"
	"sync"
)

type Transport struct {
	con       *net.TCPConn
	readChan  chan []byte
	writeChan chan []byte
	close     bool
	sync.RWMutex
}

func NewTransport(con *net.TCPConn) *Transport {
	return &Transport{con: con, readChan: make(chan []byte, 10), writeChan: make(chan []byte, 10)}
}

func (t *Transport) ReadData() []byte {
	s, ok := <-t.readChan
	if !ok {
		return nil
	}

	// 这里可以处理协议相关的解析工作
	// Here you can deal with protocol related parsing work
	return s
}

func (t *Transport) Close() {
	t.Lock()
	defer t.Unlock()

	if t.close {
		return
	}
	t.close = true
	t.con.Close()
	close(t.writeChan)
}

func (t *Transport) WriteData(data []byte) {
	t.RLock()
	defer t.RUnlock()

	if t.close {
		fmt.Println("transport end", string(data))
		return
	}

	t.writeChan <- data
}

func (t *Transport) BeginWork() {
	go t.beginToRead()
	go t.beginToWrite()
}
func (t *Transport) beginToRead() {
	defer func() {
		t.Close()
		close(t.readChan)
	}()
	var (
		headLen uint64
		err     error
	)
	for {
		err = binary.Read(t.con, binary.LittleEndian, &headLen)
		if err != nil {
			return
		}
		buf := make([]byte, headLen)
		_, err := t.con.Read(buf)
		if err != nil {
			fmt.Println("-->transport read err", err)
			return
		}
		t.readChan <- buf
	}
}

func (t *Transport) beginToWrite() {
	defer func() {
		t.Close()
	}()
	var (
		headLen uint64
		err     error
	)
	for buf := range t.writeChan {
		headLen = uint64(len(buf))
		err = binary.Write(t.con, binary.LittleEndian, headLen)
		if err != nil {
			return
		}
		n, err := t.con.Write(buf)
		if err != nil || n != len(buf) {
			fmt.Println("--->transport write err %#v", err)
			return
		}
	}
}
