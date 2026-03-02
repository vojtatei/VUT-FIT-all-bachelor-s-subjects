using System;
using System.Collections.Generic;
using System.Net; // For access to IPAddress
using System.Text;
using System.Threading.Tasks;

using IPK_Proj1.Factory;

namespace IPK_proj1.Mess
{
    // Represents a message for signaling the end of a session.
    public class ByeMess : IMessage
    {
        // Indicates whether a reply is expected for this message.
        public bool AwaitReply { get; set; }

        // Constructor sets AwaitReply to false as no reply is expected for a BYE message.
        public ByeMess()
        {
            AwaitReply = false;
        }

        // Converts the BYE message to a byte array for UDP communication.
        public byte[] ToUDPString(ushort messageId)
        {
            List<byte> bytes =
            [
                0xFF, // Message type for BYE.
            ];

            // Convert messageId to network byte order for consistency across different platforms.
            ushort networkOrderMessageId = (ushort)IPAddress.HostToNetworkOrder((short)messageId);
            bytes.AddRange(BitConverter.GetBytes(networkOrderMessageId));

            return [.. bytes];
        }

        // Converts the BYE message to a string for TCP communication.
        public string ToTCPString()
        {
            return "BYE\r\n"; // The BYE command followed by a carriage return and newline.
        }
    }
}
