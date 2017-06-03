package common

type Protocol interface {
	OnNetMade(t *Transport)
	OnNetData(data *NetPacket)
	OnNetLost(t *Transport)
}
