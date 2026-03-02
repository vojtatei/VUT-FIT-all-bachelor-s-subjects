using IPK_Proj1.Clients;
using IPK_Proj1.Mess;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace IPK_Proj1.Commands
{
    // Handles client's request to join a specific channel.
    public partial class JoinCommand : ICommand
    {
        // Executes join operation for a channel.
        public async Task Execute(Client client, string[] parameters)
        {
            ValidateArgs(parameters); // Validates the command arguments.

            string channelId = parameters[0];

            if (!client.Connected()) throw new Exception("ERR: Client not connected.\n");
            if (client.Name == null) throw new Exception("ERR: Client not authorized.\n");

            JoinMess message = new(channelId, client.Name);
            await client.SendMessage(message); // Sends join message to the server.
        }

        // Ensures the input channel ID is valid.
        public void ValidateArgs(string[] parameters)
        {
            if (parameters.Length != 1) throw new ArgumentException("ERR: Incorrect argument count.");
            string channelId = parameters[0];

            if (!MyRegex().IsMatch(channelId)) throw new ArgumentException("ERR: Invalid channel ID format.");
        }

        // Validates channel ID format.
        [GeneratedRegex(@"^[A-Za-z0-9\.-]{1,20}")]
        private static partial Regex MyRegex();
    }
}
