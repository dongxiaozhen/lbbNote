package common

import (
	"net/http"
	"sync"

	"extra/github.com/hashicorp/consul/api"
)

type (
	ConsulClient struct {
		*api.Client
		FreeTkLoger
		ServiceID   string
		MonitorAddr string
		ConsulAddr  string
		ServiceName string
		Tags        []string
		Ip          string
		Port        int
		Clients     map[string][]*ServiceInfo
		sync.RWMutex
	}
	ServiceInfo struct {
		ServiceID string
		IP        string
		Port      int
	}
)

var GConsulClient = &ConsulClient{}

func (c *ConsulClient) Open(serviceName, serviceID, consulAddr, monitorAddr, ip string, tags []string, port int) (err error) {
	consulConf := api.DefaultConfig()
	consulConf.Address = consulAddr

	c.Client, err = api.NewClient(consulConf)
	if err != nil {
		c.Error(err)
		return
	}
	http.HandleFunc("/status", c.StatusHandler)
	c.Debug("start listen...")
	if err = http.ListenAndServe(monitorAddr, nil); err != nil {
		c.Error(err)
		return err
	}

	c.MonitorAddr = monitorAddr
	c.ServiceID = serviceID
	c.ServiceName = serviceName
	c.ConsulAddr = consulAddr
	c.Ip = ip
	c.Tags = tags
	c.Port = port
	c.Clients = make(map[string][]*ServiceInfo)
	err = c.RegistService(consulAddr, serviceName, tags, ip, port)
	return err
}

func (c *ConsulClient) Close() error {
	return c.Deregister()
}

// check handler ,could overwrites
func (c *ConsulClient) StatusHandler(w http.ResponseWriter, r *http.Request) {
	c.Debug("check status.")
}

func (c *ConsulClient) Deregister() error {
	return c.Agent().ServiceDeregister(c.ServiceID)
}

func (c *ConsulClient) RegistService(consulAddr string, serviceName string, tags []string, ip string, port int) error {
	service := &api.AgentServiceRegistration{
		ID:      c.ServiceID,
		Name:    serviceName,
		Port:    port,
		Address: ip,
		Tags:    tags,
		Check: &api.AgentServiceCheck{
			HTTP:     "http://" + c.MonitorAddr + "/status",
			Interval: "5s",
			Timeout:  "1s",
		},
	}

	err := c.Agent().ServiceRegister(service)
	return err
}

// 服务发现
// serviceID 要监测的服务名称
func (c *ConsulClient) DiscoverService(healthyOnly bool, foundService string) error {
	services, _, err := c.Client.Catalog().Services(&api.QueryOptions{})
	if err != nil {
		c.Error(err)
		return err
	}
	var sers []*ServiceInfo
	for name := range services {
		servicesData, _, err := c.Client.Health().Service(name, "", healthyOnly, &api.QueryOptions{})
		if err != nil {
			c.Error(err)
			return err
		}
		for _, entry := range servicesData {
			if foundService != entry.Service.Service {
				continue
			}
			for _, health := range entry.Checks {
				if foundService != health.ServiceName {
					continue
				}

				node := &ServiceInfo{}
				node.IP = entry.Service.Address
				node.Port = entry.Service.Port
				node.ServiceID = health.ServiceID
				sers = append(sers, node)
			}
		}
	}

	c.Lock()
	c.Clients[foundService] = sers
	c.Unlock()
	return nil
}

func (c *ConsulClient) GetAllService(serviceName string) ([]*ServiceInfo, bool) {
	services, ok := c.Clients[serviceName]
	return services, ok
}

func (c *ConsulClient) PutKV(key string, value []byte) error {
	kv := &api.KVPair{Key: key, Value: value, Flags: 0}
	_, err := c.KV().Put(kv, nil)
	return err
}

func (c *ConsulClient) GetKV(key string) (value []byte, err error) {
	kv, _, err := c.KV().Get(key, nil)
	if err != nil {
		c.Error(err)
		return nil, err
	}
	return kv.Value, nil
}
