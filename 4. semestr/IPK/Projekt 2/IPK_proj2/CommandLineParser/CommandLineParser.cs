using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IPK_proj2
{
    // Parses command line arguments into an options object.
    public class CommandLineParser : ICommandLineParser
    {

        // Parses an array of command line arguments and returns an options object.
        public CommandLineOptions Parse(string[] args)
        {
            var options = new CommandLineOptions();
            for (int i = 0; i < args.Length; i++)
            {
                // Check each argument and match with possible options.
                switch (args[i])
                {
                    // Network interface option with shorthand '-i'.
                    case "--interface":
                    case "-i":
                        if (i + 1 < args.Length)
                        {
                            options.Interface = args[++i];
                        }
                        else
                        {
                            // Trigger a display of available interfaces if the interface name is not provided.
                            options.Interface = null;
                        }
                        break;
                    // Destination port option.
                    case "--port-destination":
                        // Ensure that the next argument is an integer and greater than zero.
                        if (i + 1 < args.Length && int.TryParse(args[i + 1], out int destPort) && destPort > 0)
                        {
                            options.DestinationPort = destPort;
                            i++; // Increment index to skip the processed argument.
                        }
                        else
                        {
                            throw new ArgumentException("Invalid destination port value");
                        }
                        break;
                    // Source port option.
                    case "--port-source":
                        // Same checks as for destination port.
                        if (i + 1 < args.Length && int.TryParse(args[i + 1], out int sourcePort) && sourcePort > 0)
                        {
                            options.SourcePort = sourcePort;
                            i++; 
                        }
                        else
                        {
                            throw new ArgumentException("Invalid source port value");
                        }
                        break;
                    // TCP option.
                    case "--tcp":
                    case "-t":
                        options.Tcp = true;
                        break;
                    // UDP option.
                    case "--udp":
                    case "-u":
                        options.Udp = true;
                        break;
                    // General port option.
                    case "-p":
                        // Ensure the next argument is a non-negative integer.
                        if (i + 1 < args.Length && int.TryParse(args[i + 1], out int port) && port >= 0)
                        {
                            options.Port = port;
                            i++; 
                        }
                        else
                        {
                            throw new ArgumentException("Invalid port value");
                        }
                        break;
                    // Packet count option.
                    case "-n":
                        // Ensure the next argument is an integer greater than zero.
                        if (i + 1 < args.Length && int.TryParse(args[i + 1], out int packetCount) && packetCount > 0)
                        {
                            options.PacketCount = packetCount;
                            i++; 
                        }
                        else
                        {
                            throw new ArgumentException("Invalid packet count value");
                        }
                        break;
                    // ARP option.
                    case "--arp":
                        options.Arp = true;
                        break;
                    // ICMPv4 option.
                    case "--icmp4":
                        options.Icmp4 = true;
                        break;
                    // ICMPv6 option.
                    case "--icmp6":
                        options.Icmp6 = true;
                        break;
                    // NDP option.
                    case "--ndp":
                        options.Ndp = true;
                        break;
                    // MLD option.
                    case "--mld":
                        options.Mld = true;
                        break;
                    // IGMP option.
                    case "--igmp":
                        options.Igmp = true;
                        break;

                    // Handle any unknown arguments.
                    default: 
                        throw new ArgumentException("Invalid argument");

                }
            }

            if (options.Port.HasValue && !options.Tcp && !options.Udp)
            {
                throw new ArgumentException("-p parameter is only valid with -t/--tcp or -u/--udp");
            }

            if (options.Port.HasValue && (options.SourcePort.HasValue || options.DestinationPort.HasValue))
            {
                throw new ArgumentException("Cannot use -p with --port-source or --port-destination at the same time.");
            }

            // Return the populated options object.
            return options;
        }
    }

}
