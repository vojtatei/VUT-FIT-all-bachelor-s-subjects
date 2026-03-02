using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using IPK_Proj1.Clients;

namespace IPK_Proj1.Commands
{
    // Represents a command to display help or usage information.
    public class HelpCommand : ICommand
    {
        public Task Execute(Client client, string[] parameters)
        {
            Console.WriteLine("/auth <arg1> <arg2> <arg3>, where first argument is xlogin00, second is secret and third is display name\n/join <arg1>, where argument is channel where do you want to join\n/rename <arg1>, where argument is the new name that you want to set");
            return Task.CompletedTask;
        }

        public void ValidateArgs(string[] args) 
        {
        }
    }
}
