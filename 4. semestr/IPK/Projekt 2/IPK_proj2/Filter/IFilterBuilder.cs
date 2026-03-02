using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IPK_proj2
{
    // Defines an interface for creating a filter string based on various command line options.
    // This filter string is used to specify which packets to capture or ignore in a network packet capture operation.
    public interface IFilterBuilder
    {
        // Method to build a filter string based on the provided CommandLineOptions.
        // This string is used to configure the filtering rules for packet capture.
        string BuildFilter(CommandLineOptions options);
    }
}
