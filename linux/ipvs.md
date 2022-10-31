ipvs lvs
LVS是Linux Virtual Server的简写，意即Linux虚拟服务器
	lvs/nat 网络地址转换模式，进出流量都进过分发器, 分发修改目标ip ,后端的默认路由是分发
	lvs/dr 直接路由模式，只有进站数量流量经过分发器，后端和分发有统一的vip, 分发只修改mac 
	lvs/tun 隧道模式，只有进站数量流量经过分发器 ，增加新的包头，公网
	lvs/full-nat 双向转换

