package main

import (
	"fmt"
	"io"
	"net"
	"os"
	"os/signal"
	"syscall"
	"time"
)

type Transport struct {
	con       *net.TCPConn
	close     bool
	readChan  chan []byte
	writeChan chan []byte
}

func NewTransport(con *net.TCPConn) *Transport {
	return &Transport{con: con, readChan: make(chan []byte, 10), writeChan: make(chan []byte, 10), close: false}
}
func (t *Transport) closeRead() {
	t.con.CloseRead()
}
func (t *Transport) ReadData() []byte {
	if t.close {
		return nil
	}
	return <-t.readChan
}
func (t *Transport) WriteData(data []byte) {
	if t.writeClose {
		return
	}
	t.writeChan <- data
}

func (t *Transport) beginWork() {
	go t.beginToRead()
	go t.beginToWrite()
}
func (t *Transport) beginToRead() {
	fmt.Println("-->read begin")
	defer func() {
		t.close = true
		fmt.Println("-->read end")
	}()
	for {
		buf := make([]byte, 30)
		_, err := t.con.Read(buf)
		if err == io.EOF {
			fmt.Println("-->read err io.EOF")
			if t.close {
				fmt.Println("--->close write chan")
				close(t.writeChan)
				return
			}
			return
		}
		t.readChan <- buf
	}
}

func (t *Transport) beginToWrite() {
	fmt.Println("--->write begin")
	defer func() {
		t.close = true
		fmt.Println("--->write end")
	}()
	for buf := range t.writeChan {
		n, err := t.con.Write(buf)
		if err != nil {
			fmt.Println("--->write err %#v", err)
			t.con.CloseRead()
			return
		}
		fmt.Println(n)
		time.Sleep(10 * time.Second)
	}
}

type TClient struct {
	close bool
	addr  string
}

func NewTClient(addr string) *TClient {
	return &TClient{addr: addr}
}
func (t *TClient) Begin() {
	conn, err := net.Dial("tcp", t.addr)
	if err != nil {
		fmt.Println("dial err")
		return
	}
	con, ok := conn.(*net.TCPConn)
	if !ok {
		fmt.Println("conver err")
		return
	}
	t := NewTransport(con)
	t.beginWork()

	defer t.close()
}

func (t *TClient) beginWork(tspt *Transport) {

}

func main() {
	fmt.Println("vim-go")
	closeChan := make(chan os.Signal, 1)
	signal.Notify(closeChan, syscall.SIGTERM)
	<-closeChan
	t.closeRead()
	time.Sleep(3 * time.Second)
}

func closeRead(con *net.TCPConn) {
	time.Sleep(10 * time.Second)
	con.CloseRead()
}
