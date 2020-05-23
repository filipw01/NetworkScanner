#pragma once
#include <tins/tins.h>
#include <string>
#include <vector>
using namespace Tins;

class Host{
    bool active = false; /*!< Informacja o dostępności hosta w sieci */
    IPv4Address ip_address; /*!< Adres ip hosta */
    ARP::hwaddress_type mac_address; /*!< Adres mac hosta */
    std::vector<int> open_ports; /*!< Otwarte porty sieciowe */

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

    //! Metoda sprawdzająca dostępność hosta w sieci
    void check_availability();

    //! Metoda sprawdzająca adres mac hosta
    void check_mac();

    //! Metoda sprawdzająca adres mac hosta
    /*!
    \param ports argument typu wektorowego, przekazujący numery portów do sprawdzenia
    */
    void scan_ports(std::vector<int> ports);
};