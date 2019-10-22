#
# Installing and configure DHCP server and write a program (C++\Python\Java) to
# install the software on remote machine.
#
# Author: Bhushan Pagare
#

# yum install dhcp

''' /etc/dhcp/dhcpd.conf

subnet 192.168.8.0 netmask 255.255.255.0 {
  # interface p1p1;  
  range 192.168.8.100 192.168.8.200;
  option domain-name-servers ns1.internal.example.org;
  option domain-name "internal.example.org";
  option routers 192.168.8.0;
  option broadcast-address 192.168.8.200;
  default-lease-time 600;
  max-lease-time 7200;
}
'''

# systemctl enable dhcpd
# systemctl start dhcpd
# systemctl status dhcpd -l
# cat /var/lib/dhcp/dhcpd.leases

import os
import ipaddress

# IP address input validation
def input_ipaddress():
    while True:
        try:
            ip = input('Enter the IP address: ')
            return ipaddress.ip_address(ip)
        except ValueError:
            print('Not a valid IP address !')

ip_address = input_ipaddress()
os.system('ssh -Y root@' + str(ip_address) + ' yum install -y -q nasm')