package main

import (
	"fmt"
	"io"
	"net"
	"os"
	"os/signal"
	"sync"
	"syscall"
	"time"
)

type Transport struct {
	con        *net.TCPConn
	closeRead  bool
	closeWrite bool
	readChan   chan []byte
	writeChan  chan []byte
}

func NewTransport(con *net.TCPConn) *Transport {
	return &Transport{con: con, readChan: make(chan []byte, 10), writeChan: make(chan []byte, 10)}
}
func (t *Transport) CloseRead() {
	fmt.Println("-->close transport read")
	if t.closeRead {
		return
	}
	t.con.CloseRead()
}

func (t *Transport) CloseWrite() {
	fmt.Println("--->close transport write")
	if t.closeWrite {
		return
	}
	t.con.CloseWrite()
	close(t.writeChan)
}

func (t *Transport) ReadData() []byte {
	if t.closeRead {
		return nil
	}
	return <-t.readChan
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
	fmt.Println("-->read begin")
	defer func() {
		t.closeRead = true
		fmt.Println("-->read end")
	}()
	for {
		buf := make([]byte, 30)
		_, err := t.con.Read(buf)
		if err == io.EOF {
			fmt.Println("-->read err io.EOF")
			if !t.closeRead { //  非手动关闭
				fmt.Println("--->close write chan")
				t.CloseWrite()
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
		t.closeWrite = true
		fmt.Println("--->write end")
	}()
	for buf := range t.writeChan {
		n, err := t.con.Write(buf)
		if err != nil || n != len(buf) {
			fmt.Println("--->write err %#v", err)
			t.CloseRead()
			return
		}
		time.Sleep(10 * time.Second)
	}
}

type TClient struct {
	close bool
	addr  string
	wg    sync.WaitGroup
}

func NewTClient(addr string) *TClient {
	t := &TClient{addr: addr}
	go t.connect()
}

func (p *TClient) Close() {
	p.close = true
	p.wg.Wait()
}

func (p *TClient) connect() {
	defer func() {
		p.recon()
	}()

	p.wg.Add(1)
	defer p.wg.Done()

	conn, err := net.Dial("tcp", p.addr)
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

	defer func() {
		t.CloseWrite()
	}()

	// close read
	defer t.CloseRead()

	p.handlerData(t)
}

func (p *TClient) recon() {
	if p.close {
		return
	}
	for !p.close {
		p.connect()
	}
}

func (p *TClient) handlerData(t *Transport) {
	for {
		if s := t.ReadData(); s == nil {
			fmt.Println("client handlerData return")
			return
		} else {
			fmt.Println("read print data", string(s))
		}
	}
}

func main() {
	fmt.Println("vim-go")
	closeChan := make(chan os.Signal, 1)
	signal.Notify(closeChan, syscall.SIGTERM)

	t := NewTClient("127.0.0.01:9099")

	<-closeChan
	time.Sleep(3 * time.Second)
}
