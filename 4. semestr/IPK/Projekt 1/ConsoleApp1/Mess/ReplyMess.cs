using System;
using System.Collections.Generic;
using System.Linq;
using System.Net; // Necessary for accessing IPAddress
using System.Text;
using System.Threading.Tasks;
using IPK_Proj1.Factory;

namespace IPK_Proj1.Mess
{
    // Defines the structure and functionality of a reply message.
    public class ReplyMess(string ok, string content, ushort? messId = null, ushort? refMessId = null) : IMessage
    {
        // Indicates if the message is an acknowledgment of success (OK) or failure (NOK).
        public string Ok { get; set; } = ok;
        // The content or details of the reply.
        public string Content { get; set; } = content;
        // Optional: The unique identifier of the message.
        public ushort? MessId { get; set; } = messId;
        // Optional: The unique identifier of the message this reply is referring to.
        public ushort? RefMessId { get; set; } = refMessId;
        // Indicates whether this message expects a reply. False for ReplyMess.
        public bool AwaitReply { get; set; } = false;

        // Converts the message to a byte array for UDP transmission.
        public byte[] ToUDPString(ushort messageId)
        {
            if (!MessId.HasValue || !RefMessId.HasValue)
            {
                throw new InvalidOperationException("MessId and RefMessId must be provided.");
            }

            List<byte> udpMessage =
            [
                // Adds message type, assuming a value for REPLY.
                0x01,
            ];

            // Converts MessId and RefMessId to network byte order.
            ushort networkOrderMessageId = (ushort)IPAddress.HostToNetworkOrder((short)MessId.Value);
            ushort networkOrderRefMessId = (ushort)IPAddress.HostToNetworkOrder((short)RefMessId.Value);

            udpMessage.AddRange(BitConverter.GetBytes(networkOrderMessageId));
            udpMessage.Add(Ok == "OK" ? (byte)1 : (byte)0); // Adds OK status as byte.
            udpMessage.AddRange(BitConverter.GetBytes(networkOrderRefMessId));
            udpMessage.AddRange(Encoding.UTF8.GetBytes(Content + "\0")); // Adds content, terminated with null.

            return [.. udpMessage];
        }

        // Converts the message to a string for TCP transmission.
        public string ToTCPString()
        {
            // Formats the reply message for TCP with status and content.
            return $"REPLY {Ok} IS {Content}\r\n";
        }
    }
}
