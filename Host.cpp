#include "Host.h"

Host::Host(IPv4Address Ip_address){
    ip_address = Ip_address;
}

Host::Host(bool Active, IPv4Address Ip_address, std::string Os_type, ARP::hwaddress_type Mac_address, std::vector<int> Open_ports){
    active = Active;
    ip_address = Ip_address;
    os_type = Os_type;
    mac_address = Mac_address;
    open_ports = Open_ports;
}

void Host::setActive(bool Active){
    active = Active;
}

void Host::setIp_address(IPv4Address Ip_address){
    ip_address = Ip_address;
}

void Host::setOs_type(std::string Os_type){
    os_type = Os_type;
}

void Host::setMac_address(ARP::hwaddress_type Mac_address){
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

std::string Host::getOs() const{
    return os_type;
}

ARP::hwaddress_type Host::getMac() const{
    return mac_address;
}

std::vector<int> Host::getPorts() const{
    return open_ports;
}

void Host::check_availability(){
    NetworkInterface iface = NetworkInterface::default_interface();
    NetworkInterface::Info iface_addresses = iface.addresses();
    PacketSender sender;
    EthernetII icmp_request = EthernetII("ff:ff:ff:ff:ff:ff",iface_addresses.hw_addr) / IP(ip_address,iface_addresses.ip_addr) / ICMP();
    std::unique_ptr<PDU> icmp_response(sender.send_recv(icmp_request, iface));
    if (icmp_response){
		active = true;
        const IP &ip = icmp_response->rfind_pdu<IP>();
        uint8_t ttl = ip.ttl();
        if (ttl == 64){
            os_type = "Unix";
        }
        else if (ttl == 128){
            os_type = "Windows";
        }
    }
}

void Host::check_mac(){
    NetworkInterface iface = NetworkInterface::default_interface();
    NetworkInterface::Info iface_addresses = iface.addresses();
    PacketSender sender;
    EthernetII arp_request = ARP::make_arp_request(ip_address, iface_addresses.ip_addr, iface_addresses.hw_addr);
    std::unique_ptr<PDU> arp_response(sender.send_recv(arp_request, iface));
    if (arp_response) {
        const ARP &arp = arp_response->rfind_pdu<ARP>();
        mac_address = arp.sender_hw_addr();
    }
}

void Host::scan_ports(std::vector<int> ports){
    NetworkInterface iface = NetworkInterface::default_interface();
    NetworkInterface::Info iface_addresses = iface.addresses();
    PacketSender sender;
    if (ports.size() > 0){
        for (const int &port : ports){
            IP tcp_request = IP(ip_address) / TCP(port,64738);
            tcp_request.rfind_pdu<TCP>().set_flag(TCP::SYN,1);
            std::unique_ptr<PDU> tcp_response(sender.send_recv(tcp_request));
            if (tcp_response){
                TCP &tcp = tcp_response->rfind_pdu<TCP>();
                if (!tcp.get_flag(TCP::RST)){
                    open_ports.push_back(port);
                }
            }
        }
    }
    else{
        for (int port=1; port<=65535; port++){
            IP tcp_request = IP(ip_address) / TCP(port,64738);
            tcp_request.rfind_pdu<TCP>().set_flag(TCP::SYN,1);
            std::unique_ptr<PDU> tcp_response(sender.send_recv(tcp_request));
            if (tcp_response){
                TCP &tcp = tcp_response->rfind_pdu<TCP>();
                if (!tcp.get_flag(TCP::RST)){
                    open_ports.push_back(port);
                }
            }
        }
    }
}
