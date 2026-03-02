using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IPK_proj2
{
    // The FilterBuilder class constructs a network packet filter string based on specified command line options.
    // It implements the IFilterBuilder interface.
    public class FilterBuilder : IFilterBuilder
    {
        // Builds a filter string using the command line options provided.
        // The string is compatible with packet capture libraries that use pcap filter syntax.
        public string BuildFilter(CommandLineOptions options)
        {
            List<string> parts = new List<string>();
            // Add TCP protocol filter if specified
            if (options.Tcp)
                parts.Add("tcp");

            // Add UDP protocol filter if specified
            if (options.Udp)
                parts.Add("udp");

            // Add port filter if a specific port is provided for either TCP or UDP
            if (options.Port.HasValue) 
                parts.Add($"(tcp port {options.Port.Value} or udp port {options.Port.Value})");
            // Add source port filter if provided
            if (options.SourcePort.HasValue)
            {
                parts.Add($"src port {options.SourcePort.Value}");
            }
            // Add destination port filter if provided
            if (options.DestinationPort.HasValue)
            {
                parts.Add($"dst port {options.DestinationPort.Value}");
            }

            // Add ARP protocol filter if specified
            if (options.Arp)
            {
                parts.Add("arp");
            }

            // Add ICMPv4 protocol filter if specified
            if (options.Icmp4)
            {
                parts.Add("icmp");
            }

            // Add ICMPv6 protocol filter if specified
            if (options.Icmp6)
            {
                parts.Add("icmp6");
            }

            // Add Neighbor Discovery Protocol filter if specified
            if (options.Ndp)
            {
                parts.Add("icmp6 && (ip6[40] == 135 || ip6[40] == 136 || ip6[40] == 133 || ip6[40] == 134 || ip6[40] == 137)");
            }

            // Add IGMP protocol filter if specified
            if (options.Igmp)
            {
                parts.Add("igmp");
            }

            // Add Multicast Listener Discovery filter if specified
            if (options.Mld)
            {
                parts.Add("icmp6 && (ip6[40] == 130 || ip6[40] == 131 || ip6[40] == 132 || ip6[40] == 143)");
            }


            // Combine all parts with 'and' to form the complete filter string
            // If no parts were added, return an empty string
            return parts.Count > 0 ? string.Join(" and ", parts) : "";
        }
    }

}
