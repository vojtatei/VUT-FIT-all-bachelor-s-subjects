using IPK_Proj1.Clients;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using IPK_Proj1.Factory;
using IPK_proj1.Mess;
using IPK_Proj1.Mess;

namespace IPK_Proj1.Clients
{
    // Represents a TCP client for communicating with a server.
    public class TCPClient : Client
    {
        // TCP client for managing the connection.
        private readonly TcpClient tcpClient;
        // Stream for reading and writing data.
        private NetworkStream? stream;
        // Reader for the stream.
        private readonly StreamReader reader;
        // Writer for the stream.
        private readonly StreamWriter writer;

        // Constructor initializing the client with server IP and port.
        public TCPClient(string serverIP, int port) : base(serverIP, port)
        {
            tcpClient = new TcpClient(serverIP, port);
            stream = tcpClient.GetStream();
            reader = new StreamReader(stream, Encoding.UTF8);
            writer = new StreamWriter(stream, Encoding.UTF8);
        }

        // Connects the client to the server.
        public override void Connect()
        {
            try
            {
                // Connection logic is handled in the constructor.
            }
            catch (Exception e)
            {
                Console.WriteLine($"ERR: Error connecting to server: {e.Message}");
            }
        }

        // Sends a message to the server.
        public override async Task SendMessage(IMessage message)
        {
            // Prevents sending non-Bye messages when not authorized, except for Auth messages.
            if (message.GetType() == typeof(AuthMess) && IsAuth)
            {
                await Console.Error.WriteAsync($"ERR: Already authorized.\n");
                return;
            }
            
            if (!IsAuth && !(message.GetType() == typeof(AuthMess) || message.GetType() == typeof(ByeMess)))
            {
                await Console.Error.WriteAsync($"ERR: You must be authorized to send messages of type '{message.GetType().Name}'\n");
                return;
            }
            

            try
            {
                string tcpString = message.ToTCPString();
                Byte[] data = Encoding.UTF8.GetBytes(tcpString);
                await stream!.WriteAsync(data);
            }
            catch (Exception e)
            {
                await Console.Error.WriteAsync($"ERR: {e.Message}\n");
            }
        }

        // Receives a message from the server.
        public override string? Receive()
        {
            return reader.ReadLine();
        }

        // Checks if the client is connected to the server.
        public override bool Connected()
        {
            return tcpClient.Connected;
        }

        // Listens for messages from the server.
        public override async Task ListenForMess()
        {
            try
            {
                byte[] buffer = new byte[1024];
                while (stream != null)
                {
                    var bytesRead = await stream.ReadAsync(buffer);
                    if (bytesRead == 0) break; // Server closed the connection.

                    var message = Encoding.UTF8.GetString(buffer, 0, bytesRead);
                    HandleServerMessage(message);
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"ERR: Failed to receive message: {ex}");
            }
        }

        // Handles server messages by type.
        protected override void HandleServerMessage(string message)
        {
            string[] splitted = message.Split(' ');
            string msgCode = splitted[0];

            if (msgCode == "AUTH" && IsAuth)
            {
                Console.Error.WriteLine($"ERR: Already authorized.\n");
                return;
            }

            switch (msgCode)
            {
                case "REPLY":
                    HandleReplyMessage(splitted[1], splitted);
                    break;
                case "MSG":
                    HandleChatMessage(splitted[2], splitted);
                    break;
                case "ERR":
                    HandleErrorMessage(splitted[2], splitted);
                    break;
                case "BYE\r\n":
                    HandleByeMessage();
                    break;
                default:
                    Console.Error.Write($"ERR: Received an unrecognizable message format.\n");
                    SendMessage(new ByeMess()).Wait();
                    break;
            }
        }

        // Handles server reply messages.
        protected override void HandleReplyMessage(string status, string[] splittedMessage)
        {
            var msgContent = string.Join(" ", splittedMessage.Skip(3));

            if (status == "OK")
            {
                if (!IsAuth) IsAuth = true;
                Console.Error.Write($"Success: {msgContent}");
            }
            else if (status == "NOK")
            {
                Console.Error.Write($"Failure: {msgContent}");
            }
            else
            {
                throw new Exception($"ERR: Unexpected server response code '{status}'\n");
            }
        }

        // Handles chat messages from the server or other clients.
        protected override void HandleChatMessage(string displayName, string[] splittedMessage)
        {
            var msgContent = string.Join(" ", splittedMessage.Skip(4));
            Console.Write($"{displayName}: {msgContent}");
        }

        // Handles error messages from the server.
        protected override void HandleErrorMessage(string displayName, string[] splittedMessage)
        {
            var msgContent = string.Join(" ", splittedMessage.Skip(4));
            Console.Error.Write($"ERR FROM {displayName}: {msgContent}\n");
            SendMessage(new ByeMess()).Wait();
        }

        // Handles bye messages indicating disconnection.
        protected override void HandleByeMessage()
        {
            SendMessage(new ByeMess()).Wait();
            Disconnect();
        }

        // Disconnects the client from the server.
        public override void Disconnect()
        {
            if (stream != null)
            {
                stream.Close();
                stream = null;
            }
            tcpClient.Close();
        }

        protected override Task HandleServerMessageUdp(byte[] receivedBytes, int bytesRead)
        {
            throw new NotImplementedException();
        }
    }
}
