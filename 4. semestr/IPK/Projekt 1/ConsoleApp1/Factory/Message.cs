using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IPK_Proj1.Factory
{
    // IMessage defines the structure for message objects used in communication.
    public interface IMessage
    {
        // AwaitReply indicates whether a response is expected for the message.
        public bool AwaitReply { get; set; }

        // ToUDPString converts the message into a byte array formatted for UDP transmission.
        // The method takes a messageID to uniquely identify the message.
        public abstract byte[] ToUDPString(ushort messageID);

        // ToTCPString converts the message into a string formatted for TCP transmission.
        public abstract string ToTCPString();
    }
}

