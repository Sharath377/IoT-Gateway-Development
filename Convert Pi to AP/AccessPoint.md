# Setup a wireless access point
Open up the terminal in Pi and type the following commands:
```
sudo apt-get update
sudo apt-get upgrade
```
If you get an upgrade,reboot with sudo reboot.
# Set Static IP Address
Configure static ip using Dhcpcd method.
```
sudo nano /etc/dhcpcd.conf
```
At the bottom, paste this:
```
interface wlan0
static ip_address=192.168.4.1/24
nohook wpa_supplicant
timeout 5
```
CTRL+X to save and click Y to confirm.Press Enter to return to terminal. 
# Install Hostapd
```
sudo apt-get install hostapd
```
Stop the hostapd service.
```
sudo service hostapd stop
```
Open Hostapd configuration file.
```
sudo nano /etc/hostapd/hostapd.conf
```
Paste this configuration:
```
interface=wlan0
ssid=YOUR SSID NAME
wpa_passphrase=PASSWORD
hw_mode=g
ieee80211n=1
channel=6
wmm_enabled=1
ignore_broadcast_ssid=0
auth_algs=1
wpa=2
wpa_key_mgmt=WPA-PSK
rsn_pairwise=CCMP
```
Save and exit.
To apply the configuration we made:
```
sudo nano /etc/default/hostapd
```
Find #DAEMON_CONF,remove the line and paste the following command:
```
DAEMON_CONF="/etc/hostapd/hostapd.conf"
```
# Enable the wireless access point
```
sudo systemctl unmask hostapd
sudo systemctl enable hostapd
```
Activate IPv4 forwarding.
```
sudo nano /etc/sysctl.conf
```
Un-comment #net.ipv4.ip_foward=1
```
net.ipv4.ip_forward=1
```
Save and exit.

Run this code:
```
sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
```
# Install Dnsmasq
We will install DHCP server
```
sudo apt install dnsmasq
```
Stop Dnsmasq service
```
systemctl stop dnsmasq
```
Backup the default configuration
```
sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
```
Create your own configuration:
```
sudo nano /etc/dnsmasq.conf
```
Paste this:
```
interface=wlan0
dhcp-range=192.168.4.2,192.168.4.50,255.255.255.0,24h
domain=wlan
address=/gw.wlan/192.168.4.1
```
Exit and save.
# Setting up the Firewall
Change the firewall rules:
```
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT
sudo iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT
```
Check the iptables to see the changes made.
```
sudo iptables -L -n -v
```
Save the rules.
```
sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"
```
Making this rule to load automatically every system reboot.
```
sudo nano /etc/rc.local
```
Paste this code before “exit 0”
```
iptables-restore < /etc/iptables.ipv4.nat
```
Enable the service and start
```
systemctl enable dnsmasq
systemctl start dnsmasq
systemctl status dnsmasq
```
# Reboot
```
sudo reboot
```

### Now your Pi will be working as a AccessPoint/wireless router to which devices can connect to
