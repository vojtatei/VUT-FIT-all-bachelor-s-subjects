using IPK_Proj1;
using IPK_Proj1.Commands;
using IPK_Proj1.Clients;
using IPK_Proj1.Factory;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using IPK_Proj1.Mess;
using IPK_proj1.Mess;
using IPK_proj1.Commands;

namespace IPK_Proj1
{
    public class ChatClient(CommandLineSet settings)
    {
        private readonly Client client = CreateClient(settings);

        private readonly CommandFactory commandFactory = new();


        private bool Exit = false;

        public async Task Start()
        {
            // Handle the Cancel (Ctrl+C) event to perform a graceful shutdown.
            Console.CancelKeyPress += async (sender, e) =>
            {
                e.Cancel = true;
                await HandleExit();
            };

            while (true)
            {
                // Listen for incoming messages in the background.
                var listen = client.ListenForMess();

                string? input = Console.ReadLine();

                // If input is null, initiate exit procedure.
                if (input == null)
                {
                    await HandleExit();
                    break;
                }

                // Skip empty inputs.
                if (string.IsNullOrEmpty(input))
                {
                    continue;
                }

                // If input starts with '/', it's a command.
                if (input.StartsWith('/'))
                {
                    await HandleComm(input);
                }
                else
                {
                    // Treat any other input as a chat message.
                    MessCommand message = new();
                    await message.Execute(client, [input]);
                }
            }
        }

        private async Task HandleExit()
        {
            // Prevent multiple exit attempts.
            if (Exit)
            {
                return;
            }

            Exit = true;

            // Send a 'Bye' message to the server before disconnecting.
            await client.SendMessage(new ByeMess());

            // Disconnect the client and exit the application.
            client.Disconnect();
            Environment.Exit(0);
        }

        private async Task HandleComm(string input)
        {
            // Parse the input to get the command name and parameters.
            string[] splitInput = input[1..].Split(' ');
            string commandName = splitInput[0];
            string[] parameters = splitInput.Skip(1).ToArray();

            try
            {
                // Fetch and execute the command based on the command name.
                ICommand command = CommandFactory.GetCommand(commandName);
                await command.Execute(client, parameters);
            }
            catch (System.Exception e)
            {
                // Handle any errors during command execution.
                await Console.Error.WriteLineAsync(e.Message);
            }
        }

        private static Client CreateClient(CommandLineSet set)
        {
            // Factory method to create a Client based on the protocol specified in the settings.
            if (set.Protocol == "tcp")
            {
                return new TCPClient(set.ServerIp, set.ServerPort);
            }
            else if (set.Protocol == "udp")
            {
                return new UDPClient(set.ServerIp, set.ServerPort, set.Timeout, set.Retries);
            }
            else
            {
                Console.Error.WriteLine("Invalid protocol: " + set.Protocol);
                throw new ArgumentException("Unsupported protocol");
            }
        }
    }
}
