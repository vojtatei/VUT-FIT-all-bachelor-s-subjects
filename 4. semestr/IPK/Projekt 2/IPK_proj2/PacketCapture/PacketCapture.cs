using System;
using System.Text; // For StringBuilder and text manipulation
using SharpPcap; // For packet capturing
using PacketDotNet; // For packet processing (Ethernet, IP, TCP, UDP...)
using System.Net.NetworkInformation;


namespace IPK_proj2
{
    // Class responsible for packet capturing logic
    public class PacketCapture : IPacketCapture
    {
        private CaptureDeviceList devices; // Holds the list of network devices

        public PacketCapture()
        {
            // Load the device list on initialization
            devices = CaptureDeviceList.Instance;
        }

        // Starts the packet capturing process on a specified interface with given filters
        public void StartCapture(string filter, string? interfaceName, int packetCount, CommandLineOptions options)
        {
            // If no interface is specified, list available interfaces and return
            if (string.IsNullOrWhiteSpace(interfaceName))
            {
                ListAvailableInterfaces();
                return;
            }


            // Attempt to find the specified interface
            var device = devices.FirstOrDefault(dev => dev.Name == interfaceName);
            if (device == null)
            {
                Console.WriteLine("Specified interface is not found. Available interfaces:");
                ListAvailableInterfaces();
                return;
            }

            // Define what happens when a packet arrives
            device.OnPacketArrival += (sender, e) => {
                // If the specified packet count reaches zero, stop capturing
                if (packetCount <= 0)
                {
                    device.StopCapture();
                    device.Close();
                    Environment.Exit(0);
                }


                // Parse the packet according to the LinkLayerType and its data
                var packet = Packet.ParsePacket(e.Packet.LinkLayerType, e.Packet.Data);
                var arpPacket = packet.Extract<ArpPacket>();
                // If ARP packets are specified, process them accordingly
                if (options.Arp)
                {
                    if (arpPacket != null)
                    {
                        Device_OnPacketArrival(sender, e, options);  // Process ARP packet
                    }
                }
                else
                {
                    Device_OnPacketArrival(sender, e, options);  // Process other packet types
                }

                // Decrement the remaining packet count
                packetCount--;


                if (packetCount <= 0)
                {
                    device.StopCapture(); 
                    device.Close();         
                    Environment.Exit(0);    
                }
            };


            // Open the device in promiscuous mode
            device.Open(DeviceMode.Promiscuous);

            // Set the filter if provided
            if (!string.IsNullOrEmpty(filter))
            {
                device.Filter = filter; 
            }
            // Start packet capturing
            device.StartCapture(); 
        }


        // Method to list all available network interfaces
        public void ListAvailableInterfaces()
        {
            Console.WriteLine("Available network interfaces:");
            foreach (var dev in CaptureDeviceList.Instance)
            {
                Console.WriteLine($"{dev.Name} - {dev.Description}");
            }
        }

        // Convert a MAC address to a readable string format
        public string MacAddressToString(PhysicalAddress address)
        {
            return string.Join(":", address.GetAddressBytes().Select(b => b.ToString("x2")));
        }


        // Handles the arrival of packets and prints the relevant information
        private void Device_OnPacketArrival(object sender, CaptureEventArgs e, CommandLineOptions options)
        {
            // Retrieve the timestamp of the packet and convert to local time for Czech Republic
            var utcTimestamp = e.Packet.Timeval.Date.ToUniversalTime();
            TimeZoneInfo cestZone = TimeZoneInfo.FindSystemTimeZoneById("Central European Standard Time");
            var localTimestamp = TimeZoneInfo.ConvertTimeFromUtc(utcTimestamp, cestZone);

            // Extract packet data length
            var len = e.Packet.Data.Length;
            // Parse the packet
            var packet = Packet.ParsePacket(e.Packet.LinkLayerType, e.Packet.Data);
            // Extract the Ethernet packet
            var ethernetPacket = packet.Extract<EthernetPacket>();
            if (ethernetPacket != null)
            {
                // Print basic information of the Ethernet frame
                Console.WriteLine($"timestamp: {localTimestamp:yyyy-MM-ddTHH:mm:ss.fffzzz}");
                Console.WriteLine($"src MAC: {MacAddressToString(ethernetPacket.SourceHardwareAddress)}");
                Console.WriteLine($"dst MAC: {MacAddressToString(ethernetPacket.DestinationHardwareAddress)}");
                Console.WriteLine($"frame length: {len} bytes");


                // Attempt to extract an IP packet
                var ipPacket = packet.Extract<IPPacket>();
                if (ipPacket != null)
                {
                    // Check if the packet is IPv6 and format accordingly
                    if (ipPacket.Version == IPVersion.IPv6)
                    {
                        var ipv6Packet = (IPv6Packet)ipPacket;
                        Console.WriteLine($"src IP: {ipv6Packet.SourceAddress.ToString().ToLowerInvariant()}");
                        Console.WriteLine($"dst IP: {ipv6Packet.DestinationAddress.ToString().ToLowerInvariant()}");
                    }
                    else
                    {
                        Console.WriteLine($"src IP: {ipPacket.SourceAddress}");
                        Console.WriteLine($"dst IP: {ipPacket.DestinationAddress}");
                    }
                }

                // Attempt to extract a TCP packet
                var tcpPacket = packet.Extract<TcpPacket>();
                if (tcpPacket != null)
                {
                    // Processing the TCP packet if the specified port matches
                    if (!options.Port.HasValue || tcpPacket.SourcePort == options.Port || tcpPacket.DestinationPort == options.Port)
                    {
                        Console.WriteLine($"src port: {tcpPacket.SourcePort}");
                        Console.WriteLine($"dst port: {tcpPacket.DestinationPort}");
                    }
                }

                // Attempt to extract a UDP packet
                var udpPacket = packet.Extract<UdpPacket>();
                if (udpPacket != null)
                {
                    // Similar processing for UDP packets
                    if (!options.Port.HasValue || udpPacket.SourcePort == options.Port || udpPacket.DestinationPort == options.Port)
                    {
                        Console.WriteLine($"src port: {udpPacket.SourcePort}");
                        Console.WriteLine($"dst port: {udpPacket.DestinationPort}");
                    }
                }

                // Handle ARP packets if specified
                if (options.Arp)
                {
                    var arpPacket = packet.Extract<ArpPacket>();
                    if (arpPacket != null)
                    {
                        Console.WriteLine("ARP Packet:");
                        Console.WriteLine($"Operation: {arpPacket.Operation}");
                        Console.WriteLine($"Sender IP: {arpPacket.SenderProtocolAddress}");
                        Console.WriteLine($"Target IP: {arpPacket.TargetProtocolAddress}");
                    }
                }


                // Handle ICMPv4 packets
                var icmpv4Packet = packet.Extract<IcmpV4Packet>();
                if (icmpv4Packet != null)
                {
                    Console.WriteLine("ICMPv4 Packet:");
                    Console.WriteLine($"Type: {icmpv4Packet.TypeCode}");
                    Console.WriteLine($"Checksum: {icmpv4Packet.Checksum}");
                    Console.WriteLine($"ID: {icmpv4Packet.Id}");
                    Console.WriteLine($"Sequence: {icmpv4Packet.Sequence}");
                }


                // Handle ICMPv6 packets including specific types like NDP and MLD
                var icmpv6Packet = packet.Extract<IcmpV6Packet>();
                if (icmpv6Packet != null)
                {
                    // Special handling for ICMPv6 packets based on the NDP and MLD options
                    if (options.Ndp &&
                        (icmpv6Packet.Type == IcmpV6Type.NeighborSolicitation ||
                         icmpv6Packet.Type == IcmpV6Type.NeighborAdvertisement ||
                         icmpv6Packet.Type == IcmpV6Type.RouterSolicitation ||
                         icmpv6Packet.Type == IcmpV6Type.RouterAdvertisement ||
                         icmpv6Packet.Type == IcmpV6Type.RedirectMessage))
                    {
                        // Print information specific to NDP packets
                        Console.WriteLine("NDP Packet:");
                        Console.WriteLine($"Type: {icmpv6Packet.Type}");
                        Console.WriteLine($"Code: {icmpv6Packet.Code}");
                        Console.WriteLine($"Checksum: {icmpv6Packet.Checksum}");
                    }
                    else if (options.Mld &&
                         (icmpv6Packet.Type == IcmpV6Type.MulticastListenerQuery ||
                          icmpv6Packet.Type == IcmpV6Type.MulticastListenerReport ||
                          icmpv6Packet.Type == IcmpV6Type.MulticastListenerDone))
                    {
                        // Print information specific to MLD packets
                        Console.WriteLine("MLD Packet:");
                        Console.WriteLine($"Type: {icmpv6Packet.Type}");
                        Console.WriteLine($"Code: {icmpv6Packet.Code}");
                        Console.WriteLine($"Checksum: {icmpv6Packet.Checksum}");
                    }
                    else
                    {
                        // Generic handling for other ICMPv6 packets
                        Console.WriteLine("ICMPv6 Packet:");
                        Console.WriteLine($"Type: {icmpv6Packet.Type}");
                        Console.WriteLine($"Code: {icmpv6Packet.Code}");
                        Console.WriteLine($"Checksum: {icmpv6Packet.Checksum}");
                    }
                }

                // Attempt to extract an IGMP packet
                var igmpPacket = packet.Extract<IgmpV2Packet>();
                if (igmpPacket != null)
                {
                    // Print information specific to IGMP packets
                    Console.WriteLine($"IGMP Packet:");
                    Console.WriteLine($"Type: {igmpPacket.Type}");
                    Console.WriteLine($"Group Address: {igmpPacket.GroupAddress}");
                }



                // Print out the packet contents in both hexadecimal and ASCII formats
                Console.WriteLine();
                int byteIndex = 0;
                int lineIndex = 0;
                StringBuilder byteOffsetHex = new StringBuilder();
                StringBuilder byteOffsetAscii = new StringBuilder(" ");
                foreach (var b in ethernetPacket.Bytes)
                {
                    // Format the output line with byte offset in hex
                    if (byteIndex % 16 == 0)
                    {
                        byteOffsetHex.AppendFormat("0x{0:x4}: ", lineIndex * 16);
                        lineIndex++;
                    }

                    // Convert byte value to hex and ASCII representations
                    byteOffsetHex.AppendFormat("{0:x2} ", b);
                    byteOffsetAscii.Append(b > 31 && b < 127 ? (char)b : '.');
                    byteIndex++;

                    // Print the line after 16 bytes and prepare for the next line
                    if (byteIndex % 16 == 0)
                    {
                        Console.WriteLine($"{byteOffsetHex}{byteOffsetAscii}");
                        byteOffsetHex.Clear();
                        byteOffsetAscii.Clear().Append(" ");
                    }
                }

                // Handle the last line if it does not contain 16 bytes
                if (byteIndex % 16 != 0)
                {
                    Console.WriteLine($"{byteOffsetHex}{new string(' ', (16 - (byteIndex % 16)) * 3)}{byteOffsetAscii}");
                }
                Console.WriteLine();
            }
        }
    }
}