package main

import (
	"fmt"
	"test/net/common"
	"time"
)

func main() {
	t := common.Rpc{}
	err := t.Open("127.0.0.01:9099")
	if err != nil {
		fmt.Println("bb", err)
		return
	}

	for i := 0; i < 5; i++ {
		go func() {
			ret, err := t.Call(1, 90001, []byte("l1"))
			if err != nil {
				fmt.Println("addd", err)
			} else {
				fmt.Println("aa", ret.UserId)
			}
		}()
	}
	time.Sleep(20 * time.Second)
}
