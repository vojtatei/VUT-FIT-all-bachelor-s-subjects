using System;
using System.Collections.Generic;
using System.Net; // For access to IPAddress
using System.Text;
using IPK_Proj1.Factory;

namespace IPK_Proj1.Mess
{
    // Represents an error message, used to communicate errors in both UDP and TCP communications.
    public class ErrorMess(string displayName, string content, ushort? refMessId = null) : IMessage
    {
        // Sender's display name.
        public string DisplayName { get; set; } = displayName;
        // Content of the error message.
        public string Content { get; set; } = content;
        // Optional reference message ID the error is related to.
        public ushort? RefMessId { get; set; } = refMessId;
        // Indicates no reply is expected for an error message.
        public bool AwaitReply { get; set; } = false;

        // Converts the error message to a byte array for UDP communication, including type, message ID, display name, content, and optionally reference message ID.
        public byte[] ToUDPString(ushort messageId)
        {
            List<byte> bytesList =
            [
                0xFE, // Message type for ERROR.
            ];

            // Convert messageId to network byte order.
            ushort networkOrderMessageId = (ushort)IPAddress.HostToNetworkOrder((short)messageId);
            bytesList.AddRange(BitConverter.GetBytes(networkOrderMessageId));

            // Add display name and content, each terminated with a null byte.
            bytesList.AddRange(Encoding.UTF8.GetBytes(DisplayName + "\0"));
            bytesList.AddRange(Encoding.UTF8.GetBytes(Content + "\0"));

            // If RefMessId is provided, add it as well.
            if (RefMessId.HasValue)
            {
                ushort networkOrderRefMessId = (ushort)IPAddress.HostToNetworkOrder((short)RefMessId.Value);
                bytesList.AddRange(BitConverter.GetBytes(networkOrderRefMessId));
            }

            return [.. bytesList];
        }

        // Converts the error message to a string for TCP communication, formatting as specified.
        public string ToTCPString()
        {
            return $"ERROR FROM {DisplayName} IS {Content}\r\n";
        }
    }
}
