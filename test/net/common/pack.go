package common

import (
	"encoding/binary"
	"errors"
)

type NetPacket struct {
	UserId     uint64
	ServerId   uint32
	PacketType uint32
	SessionId  uint32
	Data       []byte
	Rw         *Transport
}

var ErrDataLenLimit = errors.New("data len not enough")

func (t *NetPacket) Decoder(data []byte) error {
	if len(data) < 20 {
		return ErrDataLenLimit
	}
	t.UserId = binary.LittleEndian.Uint64(data[:8])
	t.ServerId = binary.LittleEndian.Uint32(data[8:12])
	t.PacketType = binary.LittleEndian.Uint32(data[12:16])
	t.SessionId = binary.LittleEndian.Uint32(data[16:20])
	t.Data = data[20:]
	return nil
}

func (t *NetPacket) Encoder(data []byte) []byte {
	buf := make([]byte, 20+len(data))
	binary.LittleEndian.PutUint64(buf[:8], t.UserId)
	binary.LittleEndian.PutUint32(buf[8:12], t.ServerId)
	binary.LittleEndian.PutUint32(buf[12:16], t.PacketType)
	binary.LittleEndian.PutUint32(buf[16:20], t.SessionId)
	copy(buf[20:], data)
	return buf
}
