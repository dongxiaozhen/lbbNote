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
	for {
		con, err := p.l.AcceptTCP()
		if err != nil {
			fmt.Println("accept err %#v", err)
			return
		}
		con.SetReadBuffer(100)
		con.SetNoDelay(true)
		t := NewTransport(con)
		t.BeginWork()
		p.wg.Add(1)
		go p.handler(t)
	}
}

func (p *TServer) handler(t *Transport) {
	defer p.wg.Done()

	defer t.Close()

	p.pf.OnNetMade(t)
	defer p.pf.OnNetLost(t)

	for {
		s := t.ReadData()
		if s == nil {
			return
		}
		p.pf.OnNetData(t, s)
	}
}
