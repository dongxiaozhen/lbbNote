package common

type Protocol interface {
	OnTransportMade(t *Transport)
	OnTransportData(t *Transport, data []byte)
	OnTransportLost(t *Transport)
}
