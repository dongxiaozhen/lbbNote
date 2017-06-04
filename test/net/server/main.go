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
	common.NetProcess
}

func (h *Hello) init() {
	h.Init()
	h.RegisterFunc(1, fa)
	h.RegisterFunc(2, fb)
}

func fa(data *common.NetPacket) {
	time.Sleep(1 * time.Second)
	suf := fmt.Sprintf("1 %d,%d,%d,%d,seqid %d", data.UserId, data.ServerId, data.SessionId, data.PacketType, data.SeqId)
	tmp := suf + string(data.Data)
	fmt.Println(tmp)
	buf := data.Encoder([]byte(tmp))
	data.Rw.WriteData(buf)
}
func fb(data *common.NetPacket) {
	time.Sleep(1 * time.Second)
	suf := fmt.Sprintf("2 %d,%d,%d,%d,seqid %d", data.UserId, data.ServerId, data.SessionId, data.PacketType, data.SeqId)
	tmp := suf + string(data.Data)
	fmt.Println(tmp)
	buf := data.Encoder([]byte(tmp))
	data.Rw.WriteData(buf)
}

func main() {
	closeChan := make(chan os.Signal, 1)
	signal.Notify(closeChan, syscall.SIGTERM)

	hello := &Hello{}
	hello.init()
	s := common.NewTServer(":9099", hello)
	<-closeChan

	// close listern
	s.Close()

	// not hander data, wait all done
	hello.Close()
}
