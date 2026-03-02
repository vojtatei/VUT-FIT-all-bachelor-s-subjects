using System;
using System.Collections.Generic;
using System.Net; // Added for access to IPAddress
using System.Text;
using System.Threading.Tasks;
using IPK_Proj1.Factory;

namespace IPK_Proj1.Mess
{
    // AuthMess represents an authentication message.
    public class AuthMess(string username, string secret, string displayName) : IMessage
    {
        // Username for authentication.
        private string Username { get; set; } = username;

        // Display name for the user.
        private string DisplayName { get; set; } = displayName;

        // Secret or password for authentication.
        private string Secret { get; set; } = secret;

        // Indicates the message expects a reply.
        public bool AwaitReply { get; set; } = true;

        // Converts the message to a byte array formatted for UDP communication.
        public byte[] ToUDPString(ushort messageId)
        {
            List<byte> bytesList =
            [
                0x02, // Message type for authentication.
            ];

            // Convert messageId to network byte order.
            ushort networkOrderMessageId = (ushort)IPAddress.HostToNetworkOrder((short)messageId);
            bytesList.AddRange(BitConverter.GetBytes(networkOrderMessageId));

            // Add username, display name, and secret key to the message.
            bytesList.AddRange(Encoding.UTF8.GetBytes(Username + "\0"));
            bytesList.AddRange(Encoding.UTF8.GetBytes(DisplayName + "\0"));
            bytesList.AddRange(Encoding.UTF8.GetBytes(Secret + "\0"));

            return [.. bytesList];
        }

        // Converts the message to a string formatted for TCP communication.
        public string ToTCPString()
        {
            return $"AUTH {Username} AS {DisplayName} USING {Secret}\r\n";
        }
    }
}
