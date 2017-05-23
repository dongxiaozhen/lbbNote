package common

import (
	"fmt"
	"io"
	"net"
)

type Transport struct {
	con           *net.TCPConn
	closeRead     bool
	handCloseRead bool // false:关闭writeChan,true:手动关闭write
	closeWrite    bool
	readChan      chan []byte
	writeChan     chan []byte
}

func NewTransport(con *net.TCPConn) *Transport {
	return &Transport{con: con, readChan: make(chan []byte, 10), writeChan: make(chan []byte, 10)}
}

func (t *Transport) Close() bool {
	// write close represent  transport close
	return t.closeWrite
}

func (t *Transport) CloseRead() {
	fmt.Println("-->transport CloseRead()")
	if t.closeRead {
		fmt.Println("-->transport CloseRead() has close read")
		return
	}
	t.handCloseRead = true
	t.con.CloseRead()
	fmt.Println("-->transport CloseRead() close read")
}

func (t *Transport) CloseWrite() {
	fmt.Println("--->Transport CloseWrite()")
	if t.closeWrite {
		fmt.Println("--->Transport CloseWrite() has close write")
		return
	}
	t.con.CloseWrite()
	close(t.writeChan)
	fmt.Println("--->Transport CloseWrite() close write")
}

func (t *Transport) ReadData() []byte {
	s, ok := <-t.readChan
	if !ok {
		return nil
	}
	return s
}

func (t *Transport) WriteData(data []byte) {
	if t.closeWrite {
		return
	}
	t.writeChan <- data
}

func (t *Transport) beginWork() {
	go t.beginToRead()
	go t.beginToWrite()
}
func (t *Transport) beginToRead() {
	fmt.Println("-->transport read begin")
	defer func() {
		t.closeRead = true
		close(t.readChan)
		fmt.Println("-->transport read end")
	}()
	for {
		buf := make([]byte, 30)
		_, err := t.con.Read(buf)
		if err == io.EOF {
			if t.handCloseRead {
				fmt.Println("-->transport read send close write")
				t.CloseWrite()
			}
			fmt.Println("-->transport read err io.EOF")
			return
		}
		t.readChan <- buf
	}
}

func (t *Transport) beginToWrite() {
	fmt.Println("--->transport write begin")
	defer func() {
		t.closeWrite = true
		fmt.Println("--->transport write over")
	}()
	for buf := range t.writeChan {
		n, err := t.con.Write(buf)
		if err != nil || n != len(buf) {
			fmt.Println("--->transport write err %#v", err)
			return
		}
	}
}
