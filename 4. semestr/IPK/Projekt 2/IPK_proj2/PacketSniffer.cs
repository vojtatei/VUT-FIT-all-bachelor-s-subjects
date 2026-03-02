using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IPK_proj2
{

    // Defines a class responsible for orchestrating the packet capturing process.
    // This class integrates components for capturing packets, building packet filters, and parsing command line options.
    public class PacketSniffer
    {
        // Holds a reference to an object that implements the IPacketCapture interface for capturing packets.
        private IPacketCapture packetCapture;

        // Holds a reference to an object that implements the IFilterBuilder interface for constructing packet filters.
        private IFilterBuilder filterBuilder;

        // Holds a reference to an object that implements the ICommandLineParser interface for parsing command line arguments.
        private ICommandLineParser commandLineParser;


        // Constructor that initializes the PacketSniffer with specific implementations for packet capturing, filter building, and command line parsing.
        public PacketSniffer(IPacketCapture packetCapture, IFilterBuilder filterBuilder, ICommandLineParser commandLineParser)
        {
            this.packetCapture = packetCapture;
            this.filterBuilder = filterBuilder;
            this.commandLineParser = commandLineParser;
        }


        // Starts the packet capturing process using the options provided through the command line.
        // This method is responsible for setting up and initiating the packet capturing based on the user-defined options.
        public void Start(CommandLineOptions options)
        {
            var packetCapture = new PacketCapture();
            var filter = new FilterBuilder().BuildFilter(options); // Build the packet filter based on command line options
            packetCapture.StartCapture(filter, options.Interface, options.PacketCount, options); // Start capturing packets using the built filter
        }
    }

}
