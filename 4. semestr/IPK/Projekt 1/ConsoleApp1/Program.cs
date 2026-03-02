using System.Net.Sockets;
using System.Text;
using System.Text.RegularExpressions;
using IPK_Proj1.Clients;
using IPK_Proj1.Commands;
using IPK_Proj1.Factory;
using IPK_Proj1;

namespace IPK_Proj1;

class Program
{
    static async Task Main(string[] args)
    {
        // Parse the command line arguments to extract and validate settings.
        CommandLineSet settings = ArgParse.Parse(args);

        // If the help flag is set, the program will exit early.
        if (settings.Help)
        {
            // Display help information here if implemented.
            return;
        }

        // Create and start the chat client with the provided settings.
        ChatClient chatClient = new(settings);
        await chatClient.Start();
    }
}
