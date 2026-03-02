using IPK_Proj1.Clients;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IPK_Proj1.Commands
{
    // Handles the renaming of the client's display name.
    public class RenameCommand : ICommand
    {
        // Executes the command to change the client's display name.
        public Task Execute(Client client, string[] parameters)
        {
            // Extracts the new display name from command parameters.
            string displayName = parameters[0];

            // Changes the client's display name to the new value.
            client.ChangeName(displayName);

            return Task.CompletedTask; // Indicates that the operation has completed.
        }

        // Validation for the arguments. Implement as needed.
        public void ValidateArgs(string[] parameters)
        {
            // Implementation of argument validation if required.
        }
    }
}
