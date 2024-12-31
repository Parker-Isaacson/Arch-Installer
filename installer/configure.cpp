#include <iostream>
#include <cstring>

class Parameters {
	public:
		Parameters ( int argc, char* argv[] )
		{
			if ( argc > 1 )
			{
				for ( int i = 0; i < argc; i++ )
				{
					if ( strcmp(argv[i], "--help") == 0 )
					{
						std::cout << "Used to configure the installed packages if installed from the installer\n"
							"\t--help\t\tLists this\n"
							"\t--user\t\tused to set the user profile that this program will run on, if not set program will not work\n";
						quit = true;
					}
					else if ( strcmp(argv[i], "--user") == 0 )
					{
						user = argv[i + 1];
					}
				}
			}
		}
		bool quit = false;
		std::string user = "";
};

int main ( int argc, char* argv[] )
{
	Parameters params(argc, argv); // Define the params as a Parameters object
	
	if ( params.user == "" ) // If the user variable is not defined 
	{
		std::cout << "No User Set.\n";
		return 2;
	}
	
	int status = 0; // Variable for current status
	
	std::string commands[5] { // Commands that move the configuration files
		"mkdir -p /home/" + params.user + "/.config/i3 && cp configs/i3_config /home/" + params.user + "/.config/i3/config",
		"mkdir -p /home/" + params.user + "/.config/terminator && cp configs/terminator_config /home/" + params.user + "/.config/terminator/config",
		"cp configs/xinitrc_config /home/" + params.user + "/.xinitrc",
		"cp configs/polybar_config.ini /etc/polybar/config.ini",
		"mkdir -p /home/" + params.user + "/.config/picom && cp configs/picom_config.conf /home/" + params.user + "/.config/picom/picom.conf"
	};
	
	for ( int i = 0; i < 5; i++ )
	{
		status = system(commands[i].c_str()); // Execute the commands as c-string
		if ( status != 0 ) // If the command failed, abort
		{
			std::cout << "Failed on line " << i << ". Aborting\n";
			return 1;
		}
	}
	return 0;
}
