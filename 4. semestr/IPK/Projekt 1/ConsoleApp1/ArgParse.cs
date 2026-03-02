using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace IPK_Proj1
{
    // ArgParse is responsible for parsing command-line arguments into a structured format.
    class ArgParse
    {
        // Parses command-line arguments and returns a CommandLineSet object containing the parsed settings.
        public static CommandLineSet Parse(string[] args)
        {
            var settings = new CommandLineSet();

            // Iterates through each command-line argument.
            for (int i = 0; i < args.Length; i++)
            {
                switch (args[i])
                {
                    case "-t": // Protocol type argument.
                        if (i + 1 < args.Length && (args[i + 1] == "udp" || args[i + 1] == "tcp"))
                        {
                            i++;
                            settings.Protocol = args[i];
                        }
                        else
                        {
                            Console.WriteLine("Invalid value for protocol. Use 'tcp' or 'udp'");
                            System.Environment.Exit(1);
                        }
                        break;
                    case "-s": // Server IP or hostname argument.
                        if (i + 1 < args.Length)
                        {
                            i++;
                            settings.ServerIp = args[i];
                        }
                        else
                        {
                            Console.WriteLine("IP address or hostname not specified");
                            System.Environment.Exit(1);
                        }
                        break;

                    case "-p": // Server port argument.
                        if (i + 1 < args.Length && ushort.TryParse(args[i + 1], out ushort parsedPort))
                        {
                            settings.ServerPort = parsedPort;
                            i++;
                        }
                        else
                        {
                            Console.WriteLine("Invalid or missing value for port");
                            System.Environment.Exit(1);
                        }
                        break;
                    case "-r": // Number of retries argument.
                        if (i + 1 < args.Length && byte.TryParse(args[i + 1], out byte parsedRetries))
                        {
                            settings.Retries = parsedRetries;
                            i++;
                        }
                        else
                        {
                            Console.WriteLine("Invalid or missing value for retries");
                            System.Environment.Exit(1);
                        }
                        break;

                    case "-d": // Timeout duration argument.
                        if (i + 1 < args.Length && ushort.TryParse(args[i + 1], out ushort parsedTimeout))
                        {
                            settings.Timeout = parsedTimeout;
                            i++;
                        }
                        else
                        {
                            Console.WriteLine("Invalid or missing value for timeout");
                            System.Environment.Exit(1);
                        }
                        break;

                    case "-h": // Help argument.
                        settings.Help = true;
                        break;

                    default: // Unrecognized argument.
                        Console.WriteLine($"Unknown argument: {args[i]}");
                        System.Environment.Exit(1);
                        break;
                }
            }
            return settings; // Returns the structured settings parsed from the command-line arguments.
        }
    }
}
