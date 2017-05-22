package main

import (
	"fmt"
	"net"
	"time"
)

func main() {
	fmt.Println("vim-go")
	ld, err := net.ResolveTCPAddr("tcp", ":8081")
	l, err := net.ListenTCP("tcp", ld)
	if err != nil {
		fmt.Println("%#v", err)
		return
	}
	for {
		con, err := l.AcceptTCP()
		if err != nil {
			fmt.Println("accept err %#v", err)
			return
		}
		con.SetReadBuffer(44)
		con.SetNoDelay(true)
		go handler(con)
	}
}

func handler(conn *net.TCPConn) {
	go beginToRead(conn)
	go beginToWrite(conn)
	// go closeRead(conn)
}

func closeRead(conn *net.TCPConn) {
	time.Sleep(10 * time.Second)
	conn.CloseRead()
}
func beginToRead(conn *net.TCPConn) {
	fmt.Println("-->read begin")
	buf := make([]byte, 30)
	for {
		n, err := conn.Read(buf)
		if err != nil {
			fmt.Println("read err %#v", err)
			return
		}
		fmt.Println(string(buf), n)
	}
	fmt.Println("-->read end")
}
func beginToWrite(conn *net.TCPConn) {
	fmt.Println("--->write begin")
	for {
		n, err := conn.Write([]byte("aaaa"))
		if err != nil {
			fmt.Println("write %#v", err)
			return
		}
		fmt.Println(n)
		time.Sleep(10 * time.Second)
	}
	fmt.Println("--->write end")
}
