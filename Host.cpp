#include "Host.h"

Host::Host(bool Active, IPv4Address Ip_address, std::string Mac_address, std::vector<int> Open_ports){
    active = Active;
    ip_address = Ip_address;
    mac_address = Mac_address;
    open_ports = Open_ports;
}

void Host::setActive(bool Active){
    active = Active;
}

void Host::setIp_address(IPv4Address Ip_address){
    ip_address = Ip_address;
}

void Host::setMac_address(std::string Mac_address){
    mac_address = Mac_address;
}

void Host::setOpen_ports(std::vector<int> Open_ports){
    open_ports = Open_ports;
}

bool Host::getActive() const{
    return active;
}

IPv4Address Host::getIp() const{
    return ip_address;
}

std::string Host::getMac() const{
    return mac_address;
}

std::vector<int> Host::getPorts() const{
    return open_ports;
}