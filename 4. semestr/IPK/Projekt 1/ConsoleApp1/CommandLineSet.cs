using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IPK_Proj1
{
    // Represents the settings configuration derived from the command-line arguments.
    public class CommandLineSet
    {
        // Indicates whether help information is requested.
        public bool Help { get; set; } = false;

        // The communication protocol specified by the user, e.g., "tcp" or "udp".
        public string Protocol { get; set; } = "";

        // The server IP address or hostname to connect to.
        public string ServerIp { get; set; } = "";

        // The port number on the server to connect to. Default is 4567.
        public ushort ServerPort { get; set; } = 4567;

        // The number of retries for communication attempts. Default is 3.
        public byte Retries { get; set; } = 3;

        // The timeout duration in milliseconds for communication attempts. Default is 250ms.
        public ushort Timeout { get; set; } = 250;
    }
}
