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
}

func (h *Hello) OnTransportMade(t *common.Transport) {
	fmt.Println("connect mad")
}
func (h *Hello) OnTransportData(t *common.Transport, data []byte) {
	fmt.Println("connect data", string(data))
	time.Sleep(5 * time.Second)
	t.WriteData([]byte(string(data) + "client"))
}
func (h *Hello) OnTransportLost(t *common.Transport) {
	fmt.Println("connect lost")
}

func main() {
	fmt.Println("vim-go")
	closeChan := make(chan os.Signal, 1)
	signal.Notify(closeChan, syscall.SIGTERM)

	hello := &Hello{}
	t := common.NewTClient("127.0.0.01:9099", hello)

	<-closeChan
	t.Close()
	time.Sleep(3 * time.Second)
}
