using IPK_proj2;
using System;

namespace NetworkSniffer
{
    // Defines the main entry class of the packet sniffer application.
    public class Program
    {
       
        // The main entry method for the application.
        static void Main(string[] args)
        {
            try
            {
                // Setup a handler to gracefully handle cancellation requests (like Ctrl+C from the console).
                Console.CancelKeyPress += new ConsoleCancelEventHandler(CancelHandler);

                // Initialize a command line parser and parse the arguments passed to the application.
                var commandLineParser = new CommandLineParser();
                var options = commandLineParser.Parse(args);

                // Setup the packet sniffer with the necessary components to capture and filter network packets.
                var packetSniffer = new PacketSniffer(
                    new PacketCapture(), new FilterBuilder(), commandLineParser);

                // Start the packet sniffing process using the parsed command line options.
                packetSniffer.Start(options);
            }
            catch (ArgumentException ex)
            {
                // If an error occurs, display it and exit the application with an error code.
                Console.WriteLine($"Error: {ex.Message}");
                Environment.Exit(1); // Exit the program with status code 1 indicating an error.
            }
        }

        static void CancelHandler(object? sender, ConsoleCancelEventArgs args)
        {
            // Set the Cancel property to true to prevent the console from terminating the application.
            args.Cancel = true;
            // Terminate the application with exit code 0 indicating a normal termination.
            Environment.Exit(0);
        }
    }
}
