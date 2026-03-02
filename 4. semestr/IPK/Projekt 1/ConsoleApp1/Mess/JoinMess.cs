using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using IPK_Proj1.Factory;

namespace IPK_Proj1.Mess
{
    // Represents a message for joining a channel, used in both UDP and TCP communications.
    public class JoinMess(string channelID, string displayName) : IMessage
    {
        // The ID of the channel to join.
        public string ChannelID { get; set; } = channelID;
        // The display name of the user attempting to join the channel.
        public string DisplayName { get; set; } = displayName;
        // Indicates that a reply is expected after attempting to join a channel.
        public bool AwaitReply { get; set; } = true;

        // Converts the join message to a byte array for UDP communication, including message type, message ID, channel ID, and display name.
        public byte[] ToUDPString(ushort messageID)
        {
            List<byte> bytes =
            [
                0x03, // Message type for JOIN.
            ];

            // Convert messageID to network byte order.
            ushort networkOrderMessageID = (ushort)IPAddress.HostToNetworkOrder((short)messageID);
            bytes.AddRange(BitConverter.GetBytes(networkOrderMessageID));

            // Add channel ID and display name, each terminated with a null byte.
            bytes.AddRange(Encoding.UTF8.GetBytes(ChannelID + "\0"));
            bytes.AddRange(Encoding.UTF8.GetBytes(DisplayName + "\0"));

            return [.. bytes];
        }

        // Converts the join message to a string for TCP communication, formatting as specified.
        public string ToTCPString()
        {
            return $"JOIN {ChannelID} AS {DisplayName}\r\n";
        }
    }
}
