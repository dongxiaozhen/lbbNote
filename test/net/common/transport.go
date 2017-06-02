package common

import (
	"fmt"
	"net"
)

type Transport struct {
	con       *net.TCPConn
	readChan  chan []byte
	writeChan chan []byte
	close     bool
}

func NewTransport(con *net.TCPConn) *Transport {
	return &Transport{con: con, readChan: make(chan []byte, 10), writeChan: make(chan []byte, 10)}
}

func (t *Transport) ReadData() []byte {
	s, ok := <-t.readChan
	if !ok {
		return nil
	}
	return s
}

func (t *Transport) Close() {
	if t.close {
		return
	}
	t.close = true
	t.con.Close()
	close(t.writeChan)
}

func (t *Transport) WriteData(data []byte) {
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
	fmt.Println("-->transport read begin")
	defer func() {
		close(t.readChan)
		fmt.Println("-->transport read end")
	}()
	for {
		buf := make([]byte, 30)
		_, err := t.con.Read(buf)
		if err != nil {
			fmt.Println("-->transport read err", err)
			return
		}
		t.readChan <- buf
	}
}

func (t *Transport) beginToWrite() {
	fmt.Println("--->transport write begin")
	defer func() {
		fmt.Println("--->transport write over")
		if !t.close {
			t.close = true
			t.con.Close()
			close(t.writeChan)
		}
	}()
	for buf := range t.writeChan {
		n, err := t.con.Write(buf)
		if err != nil || n != len(buf) {
			fmt.Println("--->transport write err %#v", err)
			return
		}
	}
}
