package main

import (
	"fmt"
	"net"
	"os"
	"os/signal"
	"sync"
	"syscall"
	"test/net/common"
	"time"
)

type TServer struct {
	addr string
	pf   common.Protocol
	l    *net.TCPListener
	wg   sync.WaitGroup
	mp   map[*common.Transport]struct{}
}

func NewTServer(addr string, pf common.Protocol) *TServer {
	t := &TServer{addr: addr, pf: pf, mp: make(map[*common.Transport]struct{})}
	t.listern()
	return t
}

func (p *TServer) Close() {
	// close listern
	p.l.Close()

	p.closeRead()
	p.wg.Wait() // wait read close

	// wait work group

	p.closeWrite()

	p.calllost()
}

func (p *TServer) calllost() {
	fmt.Println("TServer call OnTransportLost")
	for t := range p.mp {
		p.pf.OnTransportLost(t)
	}
}

func (p *TServer) closeRead() {
	fmt.Println("TServer close read")
	for t := range p.mp {
		t.CloseRead()
	}
}

func (p *TServer) closeWrite() {
	fmt.Println("TServer close write")
	for t := range p.mp {
		t.CloseWrite()
	}
}

func (p *TServer) listern() {
	ld, err := net.ResolveTCPAddr("tcp", p.addr)
	if err != nil {
		fmt.Println("%#v", err)
		return
	}
	p.l, err = net.ListenTCP("tcp", ld)
	if err != nil {
		fmt.Println("%#v", err)
		return
	}
	go p.accept()
}

func (p *TServer) accept() {
	fmt.Println("TServer accept begin")
	defer fmt.Println("TServer accept over")

	for {
		con, err := p.l.AcceptTCP()
		if err != nil {
			fmt.Println("accept err %#v", err)
			return
		}
		con.SetReadBuffer(44)
		con.SetNoDelay(true)
		t := common.NewTransport(con)
		go p.handler(t)
	}
}

func (p *TServer) handler(t *common.Transport) {
	fmt.Println("TServer handler begin")
	defer fmt.Println("TServer handler over")

	p.wg.Add(1)
	defer p.wg.Done()

	p.pf.OnTransportMade(t)
	defer func() {
		if t.Close() {
			p.pf.OnTransportLost(t)
			delete(p.mp, t)
			fmt.Println("auto close: call OnTransportLost and delete transport from mp")
		}
	}()

	for {
		s := t.ReadData()
		if s == nil {
			return
		}
		p.pf.OnTransportData(t, s)
	}
}

type Hello struct {
}

func (h *Hello) OnTransportMade(t *common.Transport) {
	fmt.Println("t made")
	time.Sleep(5 * time.Second)
	t.WriteData([]byte("libinbin"))
}
func (h *Hello) OnTransportLost(t *common.Transport) {
	fmt.Println("t lost")
}
func (h *Hello) OnTransportData(t *common.Transport, data []byte) {
	fmt.Println("process data", string(data))
	time.Sleep(5 * time.Second)
	t.WriteData(data)
}
func main() {
	fmt.Println("vim-go")
	closeChan := make(chan os.Signal, 1)
	signal.Notify(closeChan, syscall.SIGTERM)

	hello := &Hello{}
	s := NewTServer(":9099", hello)
	<-closeChan
	s.Close()
	time.Sleep(3 * time.Second)

}
