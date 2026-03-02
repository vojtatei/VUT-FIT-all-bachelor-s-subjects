using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IPK_proj2
{
    // Holds the configuration options parsed from the command line arguments.
    // This class is used to configure the packet capturing based on user specifications.
    public class CommandLineOptions
    {
        // Specifies the network interface on which to capture packets.
        public string? Interface { get; set; }

        // Specifies a port number to filter packets. Applies to both source and destination.
        public int? Port { get; set; }

        // Enable capturing TCP packets if set to true.
        public bool Tcp { get; set; }

        // Enable capturing UDP packets if set to true.
        public bool Udp { get; set; }

        // Enable capturing ICMPv4 packets if set to true.
        public bool Icmp4 { get; set; }

        // Enable capturing IGMP packets if set to true.
        public bool Igmp { get; set; }

        // Enable capturing NDP packets if set to true.
        public bool Ndp { get; set; }

        // Enable capturing MLD packets if set to true.
        public bool Mld { get; set; }

        // Enable capturing ICMPv6 packets if set to true.
        public bool Icmp6 { get; set; }

        // Specifies the number of packets to capture before automatically stopping.
        public int PacketCount { get; set; } = 1;

        // Specifies a source port number to filter packets specifically for the source port.
        public int? SourcePort { get; set; }

        // Specifies a destination port number to filter packets specifically for the destination port.
        public int? DestinationPort { get; set; }

        // Enable capturing ARP packets if set to true.
        public bool Arp { get; set; }
    }
}
