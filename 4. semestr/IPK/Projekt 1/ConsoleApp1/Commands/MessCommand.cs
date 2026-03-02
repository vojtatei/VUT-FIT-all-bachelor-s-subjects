using IPK_Proj1.Clients;
using IPK_Proj1.Commands;
using IPK_Proj1.Mess;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Input;

namespace IPK_proj1.Commands
{
    // Handles sending of chat messages by the client.
    public class MessCommand : IPK_Proj1.Commands.ICommand
    {
        // Executes the sending of a chat message to the server.
        public async Task Execute(Client client, string[] parameters)
        {
            ValidateArgs(parameters); // Validates the message content.

            if (!client.Connected())
            {
                throw new Exception("ERR: Client is not connected to server");
            }

            ChatMess message = new(client.Name!, parameters[0]); // Creates a new chat message.
            await client.SendMessage(message); // Sends the chat message.
        }

        // Validates the input message for correct length and character set.
        public void ValidateArgs(string[] parameters)
        {
            string pattern = @"^[\x20-\x7E]*$"; // Pattern to match printable ASCII characters.

            // Throws an exception for messages that are too long or contain unexpected characters.
            if (parameters[0].Length >= 1400 || !Regex.IsMatch(parameters[0], pattern))
            {
                throw new ArgumentException("ERR: Long message or unexpected characters\n");
            }
        }
    }
}
