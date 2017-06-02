package common

import (
	"errors"
	"fmt"
	"net"
	"sync"
	"time"
)

type TClient struct {
	close     bool
	addr      string
	wg        sync.WaitGroup
	pf        Protocol
	transport *Transport
}

func NewTClient(addr string, pf Protocol) *TClient {
	t := &TClient{addr: addr, pf: pf}
	t.connect()
	return t
}

func (p *TClient) Close() {
	p.close = true
	p.transport.Close()
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
	p.close = false
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
	p.transport.BeginWork()
	p.pf.OnTransportMade(p.transport)

	defer p.pf.OnTransportLost(p.transport)
	defer func() {
		fmt.Println("transport close")
		p.transport.Close()
	}()

	p.handlerData()
}

func (p *TClient) recon() {
	if p.close {
		return
	}
	p.close = true
	for p.close {
		err := p.connect()
		if err != nil {
			time.Sleep(5 * time.Second)
		}
	}
}

func (p *TClient) handlerData() {
	defer func() {
		fmt.Println("tclient handlerData over")
		time.Sleep(2 * time.Second)
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
