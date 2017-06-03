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

func (h *Hello) OnNetMade(t *common.Transport) {
	fmt.Println("connect mad")
	go func() {
		for i := 0; i < 1000; i++ {
			time.Sleep(100 * time.Millisecond)
			t.WriteData([]byte(fmt.Sprintf("client%d", i)))
		}
	}()
}

func (h *Hello) OnNetData(t *common.Transport, data []byte) {
	if h.close {
		fmt.Println("OnNetData close")
		return
	}
	time.Sleep(1 * time.Second)
	fmt.Println("recv", string(data))
}
func (h *Hello) OnNetLost(t *common.Transport) {
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
