using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using IPK_Proj1.Factory;

namespace IPK_Proj1.Clients
{
    // Defines an abstract base class for a client that can communicate with a server.
    abstract public partial class Client(string serverIp, int port)
    {
        // IP address of the server.
        protected string ServerIp { get; set; } = serverIp;

        // Port number on which the server is listening.
        protected int ServerPort { get; set; } = port;

        // Indicates whether the client has been authorized by the server.
        public bool IsAuth { get; set; }

        // Represents the name of the client.
        public string? Name { get; set; }

        // Changes the name of the client to a new name, ensuring it only contains printable characters and is at most 20 characters long.
        public void ChangeName(string newName)
        {
            if (!MyRegex().IsMatch(newName))
            {
                throw new ArgumentException("ERR: Name must contain only printable characters and the maximum length is 20.\n");
            }

            Name = newName;
        }

        // Abstract method for connecting to the server.
        public abstract void Connect();

        // Abstract method for sending a message to the server.
        public abstract Task SendMessage(IMessage message);

        // Abstract method for listening for messages from the server.
        public abstract Task ListenForMess();

        // Abstract method for receiving a message from the server.
        public abstract string? Receive();

        // Abstract method for disconnecting from the server.
        public abstract void Disconnect();

        // Abstract method for checking if the client is connected to the server.
        public abstract bool Connected();

        // Abstract method for handling server messages.
        protected abstract void HandleServerMessage(string message);

        // Abstract method for handling server messages received via UDP.
        protected abstract Task HandleServerMessageUdp(byte[] receivedBytes, int bytesRead);

        // Abstract methods for handling different types of messages from the server.
        protected abstract void HandleReplyMessage(string status, string[] splittedMessage);
        protected abstract void HandleChatMessage(string displayName, string[] splittedMessage);
        protected abstract void HandleErrorMessage(string displayName, string[] splittedMessage);
        protected abstract void HandleByeMessage();


        [GeneratedRegex("^[\x20-\x7E]{1,20}$")]
        private static partial Regex MyRegex();
    }
}
