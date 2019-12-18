#!/bin/bash
ifconfig eth0 down
ifconfig eth1 down
ifconfig eth0 up
ifconfig eth0 10.10.0.254/24
ifconfig eth1 up
ifconfig eth1 172.16.21.253/24
route add default gw 172.16.21.254
route -n
echo 1 > /proc/sys/net/ipv4/ip_forward
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts

iptables -t nat -A POSTROUTING -o eth1 -s 10.10.0.0/24 -j SNAT --to-source 172.16.21.253
iptables -t nat -A PREROUTING -i eth1 -d 172.16.21.253 -j DNAT --to-destination 10.10.0.1