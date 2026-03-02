using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IPK_proj2
{
    // Defines an interface for a command line parser.
    // A command line parser interprets the command line arguments provided to the application.
    public interface ICommandLineParser
    {
        // Method signature for parsing command line arguments.
        // The method is expected to return an object of CommandLineOptions which holds the parsed data.
        CommandLineOptions Parse(string[] args);
    }

}
