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
	task  *common.WorkTask
}

func (h *Hello) Init() {
	h.task = common.NewWorkTask(10, 100)
	h.task.Run()
}

func (h *Hello) OnTransportMade(t *common.Transport) {
	fmt.Println("t made")
}
func (h *Hello) OnTransportLost(t *common.Transport) {
	fmt.Println("t lost")
}
func (h *Hello) OnTransportData(t *common.Transport, data []byte) {
	if h.close {
		fmt.Println("process close", string(data))
		return
	}
	h.task.SendTask(func() {
		fmt.Println("process data", string(data))
		time.Sleep(1 * time.Second)
	})
}

func (h *Hello) Close() {
	h.close = true
	h.task.Stop()
}

func main() {
	fmt.Println("vim-go")
	closeChan := make(chan os.Signal, 1)
	signal.Notify(closeChan, syscall.SIGTERM)

	hello := &Hello{}
	hello.Init()
	s := common.NewTServer(":9099", hello)
	<-closeChan

	hello.Close()
	s.Close()
	time.Sleep(3 * time.Second)

}
