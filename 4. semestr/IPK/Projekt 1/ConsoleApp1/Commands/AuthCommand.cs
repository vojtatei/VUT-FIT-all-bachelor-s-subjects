using IPK_Proj1.Clients;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Input;
using IPK_Proj1.Mess;

namespace IPK_Proj1.Commands
{
    // Represents an authentication command.
    public partial class AuthCommand : ICommand
    {
        // Executes the authentication command using the provided parameters.
        public async Task Execute(Client client, string[] parameters)
        {
            ValidateArgs(parameters); // Validates the command arguments.

            // Extracts username, secret, and display name from the command parameters.
            string username = parameters[0];
            string secret = parameters[1];
            string displayName = parameters[2];

            // Sets the client's display name.
            client.ChangeName(displayName);

            // Ensures the client is connected before sending the message.
            if (!client.Connected())
            {
                client.Connect();
            }

            // Creates an authentication message and sends it to the server.
            var message = new AuthMess(username, secret, displayName);
            await client.SendMessage(message);
        }

        // Validates the arguments provided to the authentication command.
        public void ValidateArgs(string[] parameters)
        {
            // Ensures there are exactly three parameters.
            if (parameters.Length != 3)
            {
                throw new ArgumentException("ERR: Incorrect number of arguments");
            }

            // Extracts the arguments for easy access.
            string username = parameters[0];
            string secret = parameters[1];
            string displayName = parameters[2];

            // Validates the username, secret, and display name using regular expressions.
            if (!MyRegex().IsMatch(username))
            {
                throw new ArgumentException("Username must contain only characters A-Z, a-z, 0-9, and a maximum of 20 characters.");
            }
            if (!MyRegex1().IsMatch(secret))
            {
                throw new ArgumentException("Secret must contain only printable characters and a maximum of 128 characters.");
            }
            if (!MyRegex2().IsMatch(displayName))
            {
                throw new ArgumentException("Display name must contain only printable characters and a maximum of 20 characters.");
            }
        }

        // Regular expression for validating the username.
        [GeneratedRegex("^[A-Za-z0-9]{1,20}$")]
        private static partial Regex MyRegex();

        // Regular expression for validating the secret.
        [GeneratedRegex("^[\x20-\x7E]{1,128}$")]
        private static partial Regex MyRegex1();

        // Regular expression for validating the display name.
        [GeneratedRegex("^[\x20-\x7E]{1,20}$")]
        private static partial Regex MyRegex2();
    }
}
