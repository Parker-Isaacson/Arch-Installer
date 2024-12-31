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
						std::cout << "Program used to automate the install process for arch linux.\n"
							"\t--help\t\tOpens this menu\n"
							"\t--no-swap\tDisables the swap partition ( defualt off )\n"
							"\t--no-vm\t\tDisables the wm install and configuration ( default off )\n"
							"\t--hostname\tSets the hostname of the computer ( defualt autobuild )\n"
							"\t--region\tSets the time zone region of the computer ( default America )\n"
							"\t--city\t\tSets the time zone city of the computer ( defualt New_York )\n"
							"\t--drive\t\tSets the drive to be partitioned, formed as /dev/sdX\n"
							"\t--user\t\tSets the username of the system ( defualt = \"default\" )\n"
							"\t--pass\t\tSets the root and user passwd to the same thing ( defualt 1234 )\n"; 
							quit = true;
					}
					else if ( strcmp(argv[i], "--user") == 0 )
					{
						user = argv[i + 1];
					}
					else if ( strcmp(argv[i], "--pass") == 0 )
					{
						pass = argv[i + 1];
					}
					else if ( strcmp(argv[i], "--no-swap") == 0 )
					{
						no_swap = true;
					}
					else if ( strcmp(argv[i], "--no-wm") == 0 )
					{
						no_wm = true;
					}
					else if ( strcmp(argv[i], "--hostname") == 0)
					{
						hostname = argv[i + 1];
					}
					else if ( strcmp(argv[i], "--region") == 0 )
					{
						region = argv[i + 1];
					}
					else if ( strcmp(argv[i], "--city") == 0 )
					{
						city = argv[i + 1];
					}
					else if ( strcmp(argv[i], "--drive") == 0 )
					{
						drive = argv[i + 1];
					}
				}
			}
		}
		bool quit = false;
		bool no_swap = false; // Default includes swap partion
		bool no_wm = false; // Default includes wm
		std::string hostname = "autobuild"; // Default hostname is autobuild
		std::string region = "America"; // Default region is America
		std::string city = "New_York"; // Default city is New_York
		std::string drive = "/dev/sda"; // Default drive is /dev/sda
		std::string user = "defualt"; // Default username is default
		std::string pass = "1234"; // Default user and root password is 1234
};

int main(int argc, char* argv[])
{
	Parameters params(argc, argv); // Define the params as a Parameters object
	
	if ( params.quit == true ) // Checks if the help parameter was input
		return 0;
	
	int status = 0; // Variable for current status
	int length = 0; // Variable for length
	std::string* commands = new std::string[0] {}; // Dynamic array for storing the commands
	
	if ( !params.no_swap ) // If swap partition is allowed
	{
		commands = new std::string[8] { // Defines the commands that partition the drive and do inintial mounting
			"timedatectl",
			"sudo wipefs --all " + params.drive,
			"echo -e \"g\nn\n\n\n+1G\ny\nn\n\n\n+4G\ny\nn\n\n\n\ny\nw\nq\" | sudo fdisk " + params.drive,
			"sudo mkfs.fat -F 32 " + params.drive + "1",
			"sudo mkswap " + params.drive + "2",
			"sudo mkfs.ext4 -F " + params.drive + "3", 
			"mount " + params.drive + "3 /mnt", 
			"swapon " + params.drive + "2"
		};
		length = 8;
	}
	else
	{
		commands = new std::string[6] { // Defines the commands that partition the drive and do inintial mounting
			"timedatectl", 
			"sudo wipefs --all " + params.drive, 
			"echo -e \"g\nn\n\n\n+1G\ny\nn\n\n\n\ny\nw\nq\" | sudo fdisk " + params.drive, 
			"sudo mkfs.fat -F 32 " + params.drive + "1", 
			"sudo mkfs.ext4 -F " + params.drive + "2", 
			"mount " + params.drive + "2 /mnt"
		};
		length = 6;
	}
	
	for ( int i = 0; i < length; i++ )
	{
		status = system(commands[i].c_str()); // Execute the commands as c-string
		if ( status != 0 ) // If the command failed, abort
		{
			std::cout << "Failed on line " << i << ". Aborting.\n";
			return 1;
		}	
	}
	
	std::string packages = ""; // String for the packages to be added
	
	if ( !params.no_wm ) // If we have a window manager use the following
	{
		packages = "btop neofetch firefox gcc gdb pipewire pipewire-pulse xorg xorg-xinit i3-gaps terminator polybar picom i3status dmenu vim";
	}
	else // If no window manager don't
	{
		packages = "gcc gdb vim";
	}
	
	commands = new std::string[4] { // Defines the commands to set up the system, descriptions in README.md
		"pacstrap /mnt base linux linux-firmware",
		"genfstab -U /mnt >> /mnt/etc/fstab",
		"arch-chroot /mnt << EOF"
		"\nln -sf /usr/share/zoneinfo/" + params.region + "/" + params.city + " /etc/localtime\n"
		"hwclock --systohc\necho \"en_US.UTF-8 UTF-8\" >> /etc/locale.gen\n"
		"locale-gen\n"
		"echo \"LANG=en_US.UTF-8\" > /etc/locale.conf\n"
		"echo \"" + params.hostname + "\" > /etc/hostname\n"
		"echo -e \"127.0.0.1\tlocalhost\n::1\tlocalhost\n127.0.1.1\t" + params.hostname + ".localdomain\t" + params.hostname + "\" > /etc/hosts\n"
		"echo -e \"" + params.pass + "\n" + params.pass + "\n\" | passwd\n"
		"pacman -Sy\n"
		"pacman -S --noconfirm grub efibootmgr sudo networkmanager\n"
		"systemctl enable NetworkManager\n"
		"systemctl start NetworkManager\n"
		"mkdir /boot/EFI\nmount " + params.drive + "1 /boot/EFI\n"
		"grub-install --target=x86_64-efi --efi-directory=/boot/EFI --bootloader-id=GRUB\n"
		"grub-mkconfig -o /boot/grub/grub.cfg\n"
		"useradd -m -G wheel " + params.user + "\n"
		"echo -e \"" + params.pass + "\n" + params.pass + "\n\" | passwd " + params.user + "\n"
		"echo \"%wheel ALL=(ALL:ALL) ALL\" >> /etc/sudoers\n"
		"pacman -S --noconfirm " + packages + "\n"
		"echo \"exec i3\" > /home/" + params.user + "/.xinitrc\n"
		"exit\n"
		"EOF",
		"umount -R /mnt"
	};
	
	for ( int i = 0; i < 4; i++ )
	{
		status = system(commands[i].c_str()); // Execute the commands as c-string
		if ( status != 0 ) // If the command failed, abort
		{
			std::cout << "Failed on line " << i << ". Aborting.\n";
			return 1;
		}	
	}
	
	delete[] commands;
	
	std::cout << "Everthing installed, you need to reboot the system.\n";
	
	return 0;
}
