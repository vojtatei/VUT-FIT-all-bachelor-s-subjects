using IPK_proj1.Mess;
using IPK_Proj1.Factory;
using IPK_Proj1.Mess;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Reflection.Metadata;
using System.Reflection.PortableExecutable;
using System.Text;
using System.Threading.Tasks;

namespace IPK_Proj1.Clients
{
    public class UDPClient : Client
    {
        private readonly UdpClient udpClient; // The underlying UDP client.
        public IPEndPoint Server { get; set; } // The server's endpoint.
        private readonly ushort Timeout; // Timeout for waiting on ACKs from the server.
        private ushort MessageId; // The ID to track messages sent.
        private readonly byte MaxRetries; // Maximum retries for sending a message.
        protected SemaphoreSlim AckSemaphore = new(1, 1); // Semaphore for managing ACKs.
        protected SemaphoreSlim ReplySemaphore = new(1, 1); // Semaphore for managing replies.
        public TaskCompletionSource<bool>? AckReceived; // TCS for ACK reception.
        public TaskCompletionSource<bool>? ReplyReceived; // TCS for reply reception.
        private bool Ack; // Indicates if an ACK was received.
        private readonly List<ushort> ReceivedMessId; // List of received message IDs to prevent processing duplicates.



        public bool IsWaitReply { get; set; } // Indicates if the client is waiting for a reply.

        // Constructor initializing the UDP client with server IP, port, timeout, and retries.
        public UDPClient(string serverIP, int port, ushort timeout, byte retries) : base(serverIP, port)
        {
            Timeout = timeout;
            MessageId = 0;
            ReceivedMessId = [];
            MaxRetries = retries;
            Ack = false;
            IsAuth = false;
            ReplyReceived = null;
            AckReceived = null;
            Server = CreateIpEndPoint(ServerPort);
            udpClient = new UdpClient(0);
        }


         // Creates an IPEndPoint from the server IP and port.
    private IPEndPoint CreateIpEndPoint(int port)
    {
        if (!IPAddress.TryParse(ServerIp, out var ip))
        {
            var hostEntry = Dns.GetHostEntry(ServerIp);
            var ipv4Addresses = hostEntry.AddressList.Where(a => a.AddressFamily == AddressFamily.InterNetwork).ToArray();
            if (ipv4Addresses.Length == 0)
            {
                throw new ArgumentException("Unable to obtain IPv4 address for server.");
            }
            ip = ipv4Addresses[0]; // Use the first available IPv4 address.
        }

        return new IPEndPoint(ip, port);
    }

        // Not implemented for UDP client. Connection is stateless.
        public override void Connect()
        {
            throw new NotImplementedException();
        }

        // Closes the UDP client and exits the environment.
        public override void Disconnect()
        {
            udpClient.Close();
            Environment.Exit(0);
        }

        // Continuously listens for messages from the server, handling them as they arrive.
        public override async Task ListenForMess()
        {
            while (true) // Or until the client is 'connected'.
            {
                UdpReceiveResult result;
                try
                {
                    result = await udpClient.ReceiveAsync();
                }
                catch (ObjectDisposedException)
                {
                    // If UdpClient is closed, exit the loop.
                    break;
                }

                var receivedBytes = result.Buffer;
                Server = result.RemoteEndPoint; // Update server endpoint if necessary.

                if (receivedBytes.Length > 0)
                {
                    try
                    {
                        // Handle the received message.
                        await HandleServerMessageUdp(receivedBytes, receivedBytes.Length);
                    }
                    catch (Exception e)
                    {
                        await Console.Error.WriteLineAsync($"ERR: {e.Message}");
                    }
                }
            }
        }


        // Handles messages received from the server over UDP.
        protected override async Task HandleServerMessageUdp(byte[] receivedBytes, int bytesRead)
        {
            // Verifies the minimum length of the received message for validity.
            if (bytesRead < 3)
            {
                Console.Error.WriteLine("ERR: Received message has an invalid format.\n");
                return;
            }

            // Validates the message type is within an expected range.
            if (receivedBytes[0] < 0 || receivedBytes[0] > 255) 
            {
                Console.Error.WriteLine($"ERR: Invalid message type: {receivedBytes[0]}\n");
                return; // Exits method early if message type is out of expected range.
            }

            // Extracts the message ID from the received bytes for message tracking and confirmation.
            ushort messageId = BitConverter.ToUInt16(receivedBytes, 1);

            // Checks for duplicate messages based on their IDs to prevent processing them multiple times.
            if (receivedBytes[0] != 0 && ReceivedMessId.Contains(messageId))
            {
                // If message ID is already received, skip further processing to avoid duplicates.
                return;
            }

            // Adds new message ID to the list of received messages if it's not a confirmation message.
            if (receivedBytes[0] != 0)
            {
                ReceivedMessId.Add(messageId);
                // Sends a confirmation back to the server acknowledging receipt of the message.
                await SendConfirmMessage(messageId);
            }

            // Processes the received message based on its type.
            switch (receivedBytes[0])
            {
                case 0: // CONFIRM message type.
                        // Signals that an acknowledgment (ACK) has been received.
                    await AckSemaphore.WaitAsync();
                    Ack = true;
                    AckReceived?.SetResult(true);
                    AckSemaphore.Release();

                    break;

                case 1: // REPLY message type.
                        // Handles processing of a reply message.
                    string replyStatus = (receivedBytes[3] == 1) ? "OK" : "NOK";
                    string replyContent = Encoding.UTF8.GetString(receivedBytes, 6, bytesRead - 6);
                    string cleanReplyContent = replyContent.Replace("\0", string.Empty); // Cleans null terminators.
                    cleanReplyContent = cleanReplyContent.Replace("\u0002", string.Empty);
                    ushort refMessageId = BitConverter.ToUInt16(receivedBytes, 4); // Extracts reference message ID.
                    await HandleReplyMessage(new ReplyMess(replyStatus, cleanReplyContent, messageId, refMessageId));
                    break;

                case 4: // Chat message type.
                        // Extracts and processes a chat message.
                    string chatDisplayName = ExtractDisplayName(receivedBytes, 3);
                    string chatContent = Encoding.UTF8.GetString(receivedBytes, chatDisplayName.Length + 3, bytesRead - (chatDisplayName.Length + 3));
                    string cleanChatContent = chatContent.Replace("\0", string.Empty); // Cleans null terminators.
                    cleanChatContent = cleanChatContent.Replace("\u0002", string.Empty);
                    HandleChatMessage(new ChatMess(chatDisplayName, cleanChatContent, messageId));
                    break;

                case 254: // Error message type.
                          // Handles processing of an error message.
                    string errorDisplayName = ExtractDisplayName(receivedBytes, 3);
                    string errorContent = Encoding.UTF8.GetString(receivedBytes, errorDisplayName.Length + 3, bytesRead - (errorDisplayName.Length + 3));
                    string cleanErrorContent = errorContent.Replace("\0", string.Empty); // Cleans null terminators.
                    cleanErrorContent = cleanErrorContent.Replace("\u0002", string.Empty);
                    await HandleErrorMessage(new ErrorMess(errorDisplayName, cleanErrorContent, messageId));
                    break;

                case 255: // BYE message type.
                          // Processes a goodbye message, indicating session termination.
                    HandleByeMessage();
                    break;

                default:
                    // Handles unknown message types.
                    Console.Error.WriteLine($"ERR: Received an unrecognized message type: {receivedBytes[0]} \n");
                    break;
            }
        }

        // Processes an error message received from the server.
        protected async Task HandleErrorMessage(ErrorMess errorMess)
        {
            // Outputs the error message received from the specified display name.
            await Console.Error.WriteAsync($"ERR FROM {errorMess.DisplayName}: {errorMess.Content}\n");


            await SendByeMessage();


        }

        public async Task SendByeMessage()
        {
            ByeMess byeMessage = new();
            ushort messageId = MessageId ; // získat další messageId
            var messageBytes = byeMessage.ToUDPString(messageId); // předat messageId
            await udpClient.SendAsync(messageBytes, messageBytes.Length, Server);
            Disconnect();
        }

        // Processes a chat message received from the server.
        protected static void HandleChatMessage(ChatMess chatMess)
        {
            // Outputs the chat message from the specified display name.
            Console.Write($"{chatMess.DisplayName}: {chatMess.Content}\n");
        }

        // Extracts the display name from the received bytes starting from the given index.
        protected static string ExtractDisplayName(byte[] receivedBytes, int index)
        {
            // Finds the end of the display name based on the null terminator.
            int endDisplayName = Array.IndexOf(receivedBytes, (byte)0, index);

            // Decodes the bytes to a string using UTF-8 encoding.
            int nameLength = endDisplayName - index;
            string displayName = Encoding.UTF8.GetString(receivedBytes, index, nameLength);
            return displayName;
        }




        // Processes a reply message received from the server.
        protected async Task HandleReplyMessage(ReplyMess replyMess)
        {
            // Checks if the reply indicates success and outputs accordingly.
            if (replyMess.Ok == "OK")
            {
                if (!IsAuth)
                {
                    IsAuth = true; // Updates authentication status if not previously authenticated.
                }

                await Console.Error.WriteLineAsync($"Success: {replyMess.Content}");
            }
            else if (replyMess.Ok == "NOK")
            {
                await Console.Error.WriteLineAsync($"Failure: {replyMess.Content}");
            }
            else
            {
                throw new Exception($"ERR: Unexpected server status code '{replyMess.Ok}'\n");
            }

            // Signals that a reply has been received if awaiting a reply.
            if (replyMess.Content.Contains("Authentication successful"))
            {
                IsAuth = true; // Update the authentication status based on reply content.
            }

            // Releases the semaphore if a reply is being awaited, indicating the reply has been received.
            ReplyReceived?.SetResult(true);
        }

        // Sends a confirmation message to the server acknowledging receipt of a message.
        protected async Task SendConfirmMessage(ushort messageId)
        {
            Byte[] bytes = new ConfirmMess().ToUDPString(messageId); // Formats the confirm message.

            try
            {
                // Attempts to send the confirm message to the server.
                await udpClient.SendAsync(bytes, bytes.Length, Server);
            }
            catch (Exception e)
            {
                Console.Error.WriteLine($"ERR: {e.Message}");
            }
        }



        // Generates and returns the next message ID.
        private ushort NextMessId()
        {
            return MessageId++;
        }


        // Receives a message from the server. Not implemented for UDP as message receipt is handled by ListenForMess().
        public override string Receive()
        {
            throw new NotImplementedException();
        }



        // Sends a message to the server using UDP.
        public override async Task SendMessage(IMessage message)
        {
            // Initial setup for acknowledgment and reply handling.
            await AckSemaphore.WaitAsync();
            AckReceived = new TaskCompletionSource<bool>();
            AckSemaphore.Release();

            // Checks if the client is authorized to send the message. Blocks unauthorized messages except for AuthMess and ByeMess.
            if (!IsAuth && !(message.GetType() == typeof(AuthMess) || message.GetType() == typeof(ByeMess)))
            {
                await Console.Error.WriteAsync($"ERR: Not auth\n");
                return;
            }

            // Sets up for reply if the message expects a reply from the server.
            if (message.AwaitReply)
            {
                await ReplySemaphore.WaitAsync();
                ReplyReceived = new TaskCompletionSource<bool>();
                ReplySemaphore.Release();
            }

            // Converts the message to its corresponding byte array format, including the message ID.
            Byte[] data = message.ToUDPString(NextMessId());
            byte counter = 0; // Tracks the number of send attempts.

            try
            {
                // Attempts to send the message up to MaxRetries times, waiting for acknowledgment after each attempt.
                while (counter < MaxRetries && !AckReceived.Task.IsCompleted)
                {
                    await udpClient.SendAsync(data, data.Length, Server); // Sends the message to the server.
                    await Task.Delay(Timeout); // Waits for acknowledgment within the specified timeout.

                    // Checks if acknowledgment has been received.
                    await AckSemaphore.WaitAsync();
                    if (Ack)
                    {
                        AckSemaphore.Release();
                        break; // Exits the loop if acknowledgment is received.
                    }
                    AckSemaphore.Release();

                    // Increments the counter and logs the retry attempt if acknowledgment is not received.
                    counter++;
                    Console.Error.WriteLine($"ERR: Message not acknowledged, retrying attempt {counter} of {MaxRetries}");
                }
            }
            catch (Exception e)
            {
                // Handles any exceptions that occur during message sending.
                await Console.Error.WriteLineAsync($"ERR: nebo tady{e.Message}");
            }

            // Checks if the message was successfully acknowledged.
            if (!AckReceived!.Task.IsCompleted || !AckReceived.Task.Result)
            {
                Console.Error.Write("ERR: Exhausted all attempts to send the message\n");
            }

            // Waits for a reply if the message expects one.
            if (message.AwaitReply)
            {
                await ReplyReceived!.Task;
            }

            // Resets the acknowledgment flag for the next message.
            Ack = false;
        }

        // Always returns true as UDP is connectionless and does not have a connected state.
        public override bool Connected()
        {
            return true;
        }

        // Placeholder implementations for abstract methods from the Client class.
        protected override void HandleReplyMessage(string status, string[] splittedMessage)
        {
            throw new NotImplementedException();
        }

        protected override void HandleChatMessage(string displayName, string[] splittedMessage)
        {
            throw new NotImplementedException();
        }

        protected override void HandleErrorMessage(string displayName, string[] splittedMessage)
        {
            throw new NotImplementedException();
        }

        protected override void HandleByeMessage()
        {
            throw new NotImplementedException();
        }

        protected override void HandleServerMessage(string message)
        {
            throw new Exception($"Unexpected server response '{message}'");
        }
    }
}
