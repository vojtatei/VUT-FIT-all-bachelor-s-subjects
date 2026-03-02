using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IPK_proj2
{

    // Interface defining the packet capture functionality.
    public interface IPacketCapture
    {
        // Method to initiate packet capture.
        // 'filter' defines what packets will be captured based on given criteria.
        // 'interfaceName' specifies which network interface to use for capturing packets.
        // 'packetCount' determines how many packets to capture before stopping.
        // 'options' provides additional settings that may influence the capture, such as specific protocol capture.
        void StartCapture(string filter, string? interfaceName, int packetCount, CommandLineOptions options);
    }
}
