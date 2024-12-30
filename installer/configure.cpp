#include <iostream>
#include <cstring>

int main ( int argc, char* argv[] )
{
	std::string user = "";
	if ( argc > 1 )
		for ( int i = 0; i < argc; i++ )
		{
			if ( strcmp(argv[i], "--help") == 0 )
			{
				std::cout << "Used to configure the installed packages if installed from the installer\n\t--help\t\tLists this\n\t--user\t\tused to set the user profile that this program will run on, if not set program will not work\n";
				return 0;
			}
			else if ( strcmp(argv[i], "--user") == 0 )
			{
				user = argv[i + 1];
			}
		}
	if ( user == "" )
	{
		std::cout << "No User Set.\n";
		return 2;
	}
	int status;
	std::string commands[5] {
		"mkdir -p /home/" + user + "/.config/i3 && cp configs/i3_config /home/" + user + "/.config/i3/config",
		"mkdir -p /home/" + user + "/.config/terminator && cp configs/terminator_config /home/" + user + "/.config/terminator/config",
		"cp configs/xinitrc_config /home/" + user + "/.xinitrc",
		"cp configs/polybar_config.ini /etc/polybar/config.ini",
		"mkdir -p /home/" + user + "/.config/picom && cp configs/picom_config.conf /home/" + user + "/.config/picom/picom.conf"
	};
	for ( int i = 0; i < 5; i++ )
	{
		status = system(commands[i].c_str());
		if ( status != 0 )
		{
			std::cout << "Failed on line " << i << ". Aborting\n";
			return 1;
		}
	}
	return 0;
}
