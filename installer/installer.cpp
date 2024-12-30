#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

int main(int argc, char* argv[])
{
	bool no_swap = false;
	bool no_wm = false;
	std::string hostname = "autobuild"; // can be changed whenever echo "hostname" > /etc/hostname
	std::string region = "America";
	std::string city = "New_York"; // can be changed by ln -sf /usr/share/zoneinfo/region/city /etc/localtime
			       // then running hwclock --systohc
	std::string drive = "/dev/sda";
	std::string user = "defualt";
	std::string pass = "1234";
	if ( argc > 1 )
		for ( int i = 0; i < argc; i++ )
		{
			if ( strcmp(argv[i], "--help") == 0 )
			{
				std::cout << "Program used to automate the install process for arch linux.\n\t--help\t\tOpens this menu\n\t--no-swap\tDisables the swap partition ( defualt off )\n\t--no-vm\t\tDisables the wm install and configuration ( default off )\n\t--hostname\tSets the hostname of the computer ( defualt autobuild )\n\t--region\tSets the time zone region of the computer ( default America )\n\t--city\t\tSets the time zone city of the computer ( defualt New_York )\n\t--drive\t\tSets the drive to be partitioned, formed as /dev/sdX\n\t--user\t\tSets the username of the system ( defualt = \"default\" )\n\t--pass\t\tSets the root and user passwd to the same thing ( defualt 1234 )\n"; 
				return 0;
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
	std::cout << no_swap << no_wm << hostname << region << city << drive << user << pass << std::endl;
	int status = 0;
	if ( !no_swap )
	{
		std::string commands[8] = {"timedatectl", "sudo wipefs --all " + drive, "echo -e \"g\nn\n\n\n+1G\ny\nn\n\n\n+4G\ny\nn\n\n\n\ny\nw\nq\" | sudo fdisk " + drive, "sudo mkfs.fat -F 32 " + drive + "1", "sudo mkswap " + drive + "2", "sudo mkfs.ext4 -F " + drive + "3", "mount " + drive + "3 /mnt", "swapon " + drive + "2"};
		for ( int i = 0; i < 8; i++ )
		{
			status = system(commands[i].c_str());
			if ( status != 0 )
			{
				std::cout << "Failed on line " << i << ". Aborting.\n";
				return 1;
			}	
		}
	}
	else
	{
		std::string commands[6] = {"timedatectl", "sudo wipefs --all " + drive, "echo -e \"g\nn\n\n\n+1G\ny\nn\n\n\n\ny\nw\nq\" | sudo fdisk " + drive, "sudo mkfs.fat -F 32 " + drive + "1", "sudo mkfs.ext4 -F " + drive + "2", "mount " + drive + "2 /mnt"};
		for ( int i = 0; i < 6; i++ )
		{
			status = system(commands[i].c_str());
			if ( status != 0 )
			{
				std::cout << "Failed on line " << i << ". Aborting.\n";
				return 1;
			}	
		}
	}
	if ( !no_wm )
	{
		std::string commands[4] ={
			"pacstrap /mnt base linux linux-firmware vim",
			"genfstab -U /mnt >> /mnt/etc/fstab",
			"arch-chroot /mnt <<EOF\nln -sf /usr/share/zoneinfo/" + region + "/" + city + " /etc/localtime\nhwclock --systohc\necho \"en_US.UTF-8 UTF-8\" >> /etc/locale.gen\nlocale-gen\necho \"LANG=en_US.UTF-8\" > /etc/locale.conf\necho \"" + hostname + "\" > /etc/hostname\necho -e \"127.0.0.1\tlocalhost\n::1\tlocalhost\n127.0.1.1\t" + hostname + ".localdomain\t" + hostname + "\" > /etc/hosts\necho -e \"" + pass + "\n" + pass + "\n\" | passwd\npacman -Sy\npacman -S --noconfirm grub efibootmgr sudo networkmanager\nsystemctl enable NetworkManager\nsystemctl start NetworkManager\nmkdir /boot/EFI\nmount " + drive + "1 /boot/EFI\ngrub-install --target=x86_64-efi --efi-directory=/boot/EFI --bootloader-id=GRUB\ngrub-mkconfig -o /boot/grub/grub.cfg\nuseradd -m -G wheel " + user + "\necho -e \"" + pass + "\n" + pass + "\n\" | passwd " + user + "\necho \"%wheel ALL=(ALL:ALL) ALL\" >> /etc/sudoers\npacman -S --noconfirm btop neofetch firefox gcc gdb pipewire pipewire-pulse xorg xorg-xinit i3-gaps terminator polybar picom i3status dmenu\necho \"exec i3\" > /home/" + user + "/.xinitrc\nexit\nEOF",
			"umount -R /mnt"
		};
		for ( int i = 0; i < 4; i++ )
		{
			status = system(commands[i].c_str());
			if ( status != 0 )
			{
				std::cout << "Failed on line " << i << ". Aborting.\n";
				return 1;
			}	
		}
	}
	else
	{
		std::string commands[4] ={
			"pacstrap /mnt base linux linux-firmware vim",
			"genfstab -U /mnt >> /mnt/etc/fstab",
			"arch-chroot /mnt <<EOF\nln -sf /usr/share/zoneinfo/" + region + "/" + city + " /etc/localtime\nhwclock --systohc\necho \"en_US.UTF-8 UTF-8\" >> /etc/locale.gen\nlocale-gen\necho \"LANG=en_US.UTF-8\" > /etc/locale.conf\necho \"" + hostname + "\" > /etc/hostname\necho -e \"127.0.0.1\tlocalhost\n::1\tlocalhost\n127.0.1.1\t" + hostname + ".localdomain\t" + hostname + "\" > /etc/hosts\necho -e \"" + pass + "\n" + pass + "\n\" | passwd\npacman -Sy\npacman -S --noconfirm grub efibootmgr sudo networkmanager\nsystemctl enable NetworkManager\nsystemctl start NetworkManager\nmkdir /boot/EFI\nmount " + drive + "1 /boot/EFI\ngrub-install --target=x86_64-efi --efi-directory=/boot/EFI --bootloader-id=GRUB\ngrub-mkconfig -o /boot/grub/grub.cfg\nuseradd -m -G wheel " + user + "\necho -e \"" + pass + "\n" + pass + "\n\" | passwd " + user + "\necho \"%wheel ALL=(ALL:ALL) ALL\" >> /etc/sudoers\npacman -S --noconfirm btop neofetch gcc gdb\nexit\nEOF",
			"umount -R /mnt"
		};
		for ( int i = 0; i < 4; i++ )
		{
			status = system(commands[i].c_str());
			if ( status != 0 )
			{
				std::cout << "Failed on line " << i << ". Aborting.\n";
				return 1;
			}	
		}
	}
	std::cout << "Everthing installed, you need to reboot the system and configure the configs\n" << user << " | " << pass << " <- your user account\n";
	return 0;
}
