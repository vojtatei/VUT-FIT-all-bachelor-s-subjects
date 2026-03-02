from scapy.all import IPv6, ICMPv6MLQuery, send

# Nastavit cílovou multicastovou adresu a rozhraní
multicast_address = "ff02::1" # Příklad IPv6 všech zařízení na lokálním linku
interface = "eth0"

# Vytvořit IPv6 hlavičku
ip6 = IPv6(dst=multicast_address)

# Vytvořit MLD zprávu
mld = ICMPv6MLQuery()

# Odeslat paket
send(ip6/mld, iface=interface)
