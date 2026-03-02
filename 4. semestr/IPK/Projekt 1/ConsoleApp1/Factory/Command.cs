using IPK_Proj1.Commands;

namespace IPK_Proj1.Factory
{
    // CommandFactory creates command objects based on input command names.
    public class CommandFactory
    {
        // GetCommand returns a command object for the specified command name.
        public static ICommand GetCommand(string name)
        {
            // The switch statement matches the command name to instantiate the appropriate command object.
            return name.ToLower() switch
            {
                "auth" => new AuthCommand(),// Returns a new authentication command object.
                "help" => new HelpCommand(),// Returns a new help command object.
                "join" => new JoinCommand(),// Returns a new join command object.
                "rename" => new RenameCommand(),// Returns a new rename command object.
                _ => throw new NotImplementedException("ERR: Bad instruction, try /help"),// If the command name is unrecognized, throws an exception prompting to try /help.
            };
        }
    }
}
