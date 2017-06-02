package main

import (
	"fmt"
	"os"
	"os/signal"
	"syscall"
	"test/net/common"
	"time"
)

type Hello struct {
	close bool
}

func (h *Hello) OnTransportMade(t *common.Transport) {
	fmt.Println("connect mad")
	for i := 0; i < 1000; i++ {
		time.Sleep(100 * time.Millisecond)
		t.WriteData([]byte(fmt.Sprintf("client%d", i)))
	}
}
func (h *Hello) OnTransportData(t *common.Transport, data []byte) {
	fmt.Println("connect data", string(data))
	if h.close {
		fmt.Println("OnTransportData close")
		return
	}
	time.Sleep(5 * time.Second)
	t.WriteData([]byte(string(data) + "client"))
}
func (h *Hello) OnTransportLost(t *common.Transport) {
	fmt.Println("connect lost")
}

func (h *Hello) Close() {
	h.close = true
	fmt.Println("hello close")
}

func main() {
	fmt.Println("vim-go")
	closeChan := make(chan os.Signal, 1)
	signal.Notify(closeChan, syscall.SIGTERM)

	hello := &Hello{}
	t := common.NewTClient("127.0.0.01:9099", hello)

	<-closeChan
	hello.Close()
	t.Close()
	time.Sleep(3 * time.Second)
}
