using System;
using System.Collections.Generic;
using System.Text;

namespace IPK_Proj1.Factory
{
    // Represents a confirmation message typically used in UDP communication to acknowledge receipt of a message.
    public class ConfirmMess : IMessage
    {
        // Indicates whether a reply is expected for this message. For ConfirmMess, it is set to false as confirmations do not require replies.
        public bool AwaitReply { get; set; } = false;

        // Converts the confirmation message to a byte array suitable for UDP communication.
        public byte[] ToUDPString(ushort messageId)
        {
            List<byte> bytesList =
            [
                0x00, // The type for CONFIRM message is 0x00.
                // Add messageId in byte order according to specification.
                // No need for network byte order conversion since messageId is expected to be in the correct order.
                .. BitConverter.GetBytes(messageId),
            ];

            return [.. bytesList];
        }

        // This method is not implemented as ConfirmMess is specific to UDP and does not have a TCP equivalent.
        public string ToTCPString()
        {
            throw new NotImplementedException("ConfirmMess is not applicable for TCP communication.");
        }
    }
}
