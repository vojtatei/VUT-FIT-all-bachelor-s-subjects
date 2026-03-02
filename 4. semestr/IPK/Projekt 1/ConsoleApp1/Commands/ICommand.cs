using IPK_Proj1.Clients;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IPK_Proj1.Commands
{
    // Defines a common interface for command classes.
    public interface ICommand
    {
        public Task Execute(Client client, string[] parameters);

        protected void ValidateArgs(string[] parameters);
    }
}
