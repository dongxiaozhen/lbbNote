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

func (h *Hello) OnNetMade(t *common.Transport) {
	fmt.Println("t made")
}
func (h *Hello) OnNetLost(t *common.Transport) {
	fmt.Println("t lost")
}
func (h *Hello) OnNetData(data *common.NetPacket) {
	if h.close {
		fmt.Println("process close", *data)
		return
	}

	// 这里可以处理协议相关的解析工作
	// Here you can deal with protocol related parsing work

	h.task.SendTask(func() {
		time.Sleep(1 * time.Second)
		data.Rw.WriteData([]byte("process data" + string(data.Data)))
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

	// close listern
	s.Close()

	// not hander data, wait all done
	hello.Close()
}
