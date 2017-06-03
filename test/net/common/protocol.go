package common

type Protocol interface {
	OnNetMade(t *Transport)
	OnNetData(t *Transport, data []byte)
	OnNetLost(t *Transport)
}
