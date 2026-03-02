using System;
using System.Collections.Generic;
using System.Net; // For access to IPAddress
using System.Text;
using IPK_Proj1.Factory;

namespace IPK_Proj1.Mess
{
    // Represents a chat message, used for sending chat content between users in both UDP and TCP communications.
    public class ChatMess(string displayName, string content, ushort? refMessId = null) : IMessage
    {
        // Sender's display name.
        public string DisplayName { get; set; } = displayName;
        // Content of the chat message.
        public string Content { get; set; } = content;
        // Optional reference message ID this chat message might be responding to.
        public ushort? RefMessId { get; set; } = refMessId;
        // Indicates no reply is expected for a chat message.
        public bool AwaitReply { get; set; } = false;

        // Converts the chat message to a byte array for UDP communication, including type, message ID, display name, content, and optionally reference message ID.
        public byte[] ToUDPString(ushort messageID)
        {
            List<byte> bytes =
            [
                0x04, // Message type for CHAT.
            ];

            // Convert messageID to network byte order.
            ushort networkOrderMessageID = (ushort)IPAddress.HostToNetworkOrder((short)messageID);
            bytes.AddRange(BitConverter.GetBytes(networkOrderMessageID));

            // Add display name and content, each terminated with a null byte.
            bytes.AddRange(Encoding.UTF8.GetBytes(DisplayName + "\0"));
            bytes.AddRange(Encoding.UTF8.GetBytes(Content + "\0"));

            // If RefMessId is provided, add it as well.
            if (RefMessId.HasValue)
            {
                ushort networkOrderRefMessId = (ushort)IPAddress.HostToNetworkOrder((short)RefMessId.Value);
                bytes.AddRange(BitConverter.GetBytes(networkOrderRefMessId));
            }

            return [.. bytes];
        }

        // Converts the chat message to a string for TCP communication, formatting as specified.
        public string ToTCPString()
        {
            return $"MSG FROM {DisplayName} IS {Content}\r\n";
        }
    }
}
