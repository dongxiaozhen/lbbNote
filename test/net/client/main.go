package main

import (
	"errors"
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
	fmt.Println("-->transport CloseRead()")
	if t.closeRead {
		fmt.Println("-->transport CloseRead() has close read")
		return
	}
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

type TClient struct {
	close     bool
	addr      string
	wg        sync.WaitGroup
	pf        Protocol
	transport *Transport
}

type Protocol interface {
	OnTransportMade(t *Transport)
	OnTransportData(t *Transport, data []byte)
	OnTransportLost(t *Transport)
}

func NewTClient(addr string, pf Protocol) *TClient {
	t := &TClient{addr: addr, pf: pf}
	t.connect()
	return t
}

func (p *TClient) Close() {
	p.close = true
	p.wg.Wait()
}

func (p *TClient) connect() error {
	conn, err := net.Dial("tcp", p.addr)
	if err != nil {
		fmt.Println("dial err")
		return err
	}
	con, ok := conn.(*net.TCPConn)
	if !ok {
		fmt.Println("conver err")
		return errors.New("conv err")
	}
	go p.handlerConnect(con)
	return nil
}
func (p *TClient) handlerConnect(con *net.TCPConn) {
	defer func() {
		fmt.Println("reconnect")
		p.recon()
	}()

	p.wg.Add(1)
	defer p.wg.Done()

	p.transport = NewTransport(con)
	p.transport.beginWork()
	p.pf.OnTransportMade(p.transport)

	defer func() {
		fmt.Println("send closeWrite()")
		p.transport.CloseWrite()
	}()

	defer p.pf.OnTransportLost(p.transport)
	// close read
	defer func() {
		fmt.Println("send closeRead()")
		p.transport.CloseRead()
	}()

	p.handlerData()
}

func (p *TClient) recon() {
	if p.close {
		return
	}
	for !p.close {
		err := p.connect()
		if err != nil {
			time.Sleep(5 * time.Second)
		}
	}
}

func (p *TClient) handlerData() {
	defer func() {
		fmt.Println("tclient handlerData over")
	}()

	for {
		if s := p.transport.ReadData(); s == nil {
			fmt.Println("client handlerData return")
			return
		} else {
			p.pf.OnTransportData(p.transport, s)
		}
	}
}

type Hello struct {
}

func (h *Hello) OnTransportMade(t *Transport) {
	fmt.Println("connect mad")
}
func (h *Hello) OnTransportData(t *Transport, data []byte) {
	fmt.Println("connect data", string(data))
}
func (h *Hello) OnTransportLost(t *Transport) {
	fmt.Println("connect lost")
}

func main() {
	fmt.Println("vim-go")
	closeChan := make(chan os.Signal, 1)
	signal.Notify(closeChan, syscall.SIGTERM)

	hello := &Hello{}
	t := NewTClient("127.0.0.01:9099", hello)

	<-closeChan
	t.Close()
	time.Sleep(3 * time.Second)
}
