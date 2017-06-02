package common

import (
	"fmt"
	"net"
	"sync"
)

type TServer struct {
	addr string
	pf   Protocol
	l    *net.TCPListener
	wg   sync.WaitGroup
}

func NewTServer(addr string, pf Protocol) *TServer {
	t := &TServer{addr: addr, pf: pf}
	t.listern()
	return t
}

func (p *TServer) Close() {
	p.l.Close()
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
		t := NewTransport(con)
		t.BeginWork()
		p.wg.Add(1)
		go p.handler(t)
	}
}

func (p *TServer) handler(t *Transport) {
	fmt.Println("TServer handler begin")
	defer fmt.Println("TServer handler over")

	defer p.wg.Done()

	defer t.Close()

	p.pf.OnTransportMade(t)
	defer func() {
		p.pf.OnTransportLost(t)
		fmt.Println("auto close: call OnTransportLost and delete transport from mp")
	}()

	for {
		s := t.ReadData()
		if s == nil {
			fmt.Println("tserver readdata err")
			return
		}
		p.pf.OnTransportData(t, s)
	}
}
