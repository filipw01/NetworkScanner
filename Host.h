#pragma once
#include <tins/tins.h>
#include <string>
#include <vector>
using namespace Tins;

class Host{
    bool active = false; // host availability in network
    IPv4Address ip_address;
    ARP::hwaddress_type mac_address;
    std::vector<int> open_ports;

public:
    Host() = default;
    Host(bool Active, IPv4Address Ip_address, ARP::hwaddress_type Mac_address, std::vector<int> Open_ports);

    void setActive(bool Active);
    void setIp_address(IPv4Address Ip_address);
    void setMac_address(ARP::hwaddress_type Mac_address);
    void setOpen_ports(std::vector<int> Open_ports);
    
    bool getActive() const;
    IPv4Address getIp() const;
    ARP::hwaddress_type getMac() const;
    std::vector<int> getPorts() const;
};