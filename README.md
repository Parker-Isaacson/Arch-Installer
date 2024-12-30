# Arch Installer
 Install script in C++ for Arch Linux and other configurable.

## Purpose for Creation
Arch Linux is a lightweight Linux distribution, it is my most used distribution. Sometimes it’s needed to switch computers that are running Arch on and the install process can get tedious. Following the [Installation Guide](https://wiki.archlinux.org/title/Installation_guide) provided by [archlinux.org](https://archlinux.org), or using the archinstall program also provided will work. However, both can be tedious to use. I wanted a program that installs Arch Linux efficiently with the packages and configurations that I use.

## What it does
This installer installs Arch Linux with the optional ability for having a swap partition and installing the i3-wm. It also provides the ability to add configuration files and automatically place them in their respective directories.

## How to use it
### Linux
**THIS WILL DELETE ALL DATA ON THE USB DEVICE USED ALONG WITH THE DRIVE ARCH IS INSTALLED ON**
First a bootable medium, like a USB. The drive must be partitioned to have 2 partitions, one that is 2GB and the other that takes up the remaining drive space. The partitioning can be done with any tool such as fdisk, cfidsk or any other drive partitioning tool. The first partition must have the bootable flag toggled; this can be done in fdisk. If partitioning the drive becomes two complex two drives can be used, one with the iso and the other with the installer. The partitions should be formatted as FAT32 for the bootable partition and ext4 ( or any other storage type ) for the storage partition. This can be done in the following way:
```
sudo wipefs --all /dev/sdX
sudo cfidsk /dev/sdX
sudo mkfs.fat -F 32 /dev/sdX1
sudo mkfs.ext4 -F /dev/sdx2
```

The Ach Linux iso will be needed. It can be found on [Arch Linux Downloads](https://archlinux.org/download/) and navigating to a mirror to download the newest iso.

The iso should be written the drive using the dd command in the following way:
```
sudo dd if=path/to/iso of=/dev/sdX1 status=progress oflag=sync bs=4M
```

If multiple drives are in use the dd command will also simply switch /dev/sdX1 to the root partition of the drive /dev/sdX. It is also practical to use a program like Balena Etcher to write to the drive.

Once the drive has been partitioned and the Ach Linux iso downloaded, the next step is to statically compile the installer.cpp file with the following:
```
g++ -static -std=c++11 installer.cpp -o installer
```

Next copy the executable installer over to the storage partition on the drive, this can be done by mounting the drive and copying the file. I like to use the directory /mnt/usb as the mounting position for usb devices connected to my computer, this directory can be anywhere. If the directory already exists exclude the ```--mkdir``` parameter. This can be done in the following way:
```
sudo mount --mkdir /dev/sdX2 /mnt/usb
sudo cp installer /mnt/usb
sudo umount /dev/sdX2
```

Now that the drive has the bootable Arch Linux iso, boot into it on the device Arch is to be installed on. A connect to the internet is required and can be done by using a tool like [iwctl](https://wiki.archlinux.org/title/Iwd#iwctl), the installer does not check for internet connection and will fail if it is not there. Once connected to the internet, executing the installer is next. First the drive must be mounted and then the installer can be executed. This example shows executing the installer with the ```--help```, change this using to what is needed.

Some key points about the install:
- use ```--user``` to set the username of the user profile that will be created
- use ```--pass``` to set the password of the user profile that will be created along with the root user, both passwords can be changed by running ```passwd``` while logged in as the user. This parameter does not have its input text hidden or blurred so it is recommended to set it to something easy to remember and then change it later
- use ```--drive``` to set the drive that Arch will be installed on. This is essential for making sure data is written over. use a command like ```lsblk``` or ```fdisk -l``` to figure out the drives that are connected to the computer pick the drive used for Arch, it will most likely take the form of sdX and pass the ```--drive``` parameter ```/dev/sdX``` so it installs Arch on the correct drive.
- use ```--region``` and ```--city``` to set the time zone the computer will be in. It can be changed later by linking the correct directory to /etc/localtime in the following way ```ln -sf /usr/share/zoneinfo/Region/City /etc/localtime```. The regions can be found in ```/usr/share/zoneinfo```, and cities are found in sub-directories of the region directories.
- use ```--no-wm``` and ```--no-swap``` to disable the installation of the i3-wm or creation of the swap partition respectively.

The installation can be done in the following way:
```
mount --mkidr /dev/sdX2 temp
./temp/installer --help
```

The next step is to reboot the system.

The configure.cpp file can be used after the installer is done, it should be statically compiled and the brought to the newly installed system along with a configs file with respective configs for the i3-wm. The config files I use are in the configs folder and their names must be matched or the configure.cpp file will not work.

After this is completed, the system will be able to run Arch Linux. It is possible to enter an unconfigured i3-wm by running startx.

### Windows
If this is being made on Windows and not Windows Subsystem for Linux, it will be required to have two drives.

The Ach Linux iso will be needed. It can be found on [Arch Linux Downloads](https://archlinux.org/download/) and navigating to a mirror to download the newest iso.

Then use a tool like Rufus or Balena Etcher to burn the iso onto the first drive.

The second drive will need the installer on it. The installer will need to be statically compiled, this can be done on vscode by following this guide: [Visual Studio Code for C/C++](https://code.visualstudio.com/docs/languages/cpp). Then follow a similar process as above to statically compile the installer and put it on the second drive. If the configure script is also wanted, statically compile it and place it onto the second drive along with the configs folder, and respective files.

Now that the first drive has the bootable Arch Linux iso, boot into it on the device Arch is to be installed on. A connect to the internet is required and can be done by using a tool like [iwctl](https://wiki.archlinux.org/title/Iwd#iwctl), the installer does not check for internet connection and will fail if it is not there. Once connected to the internet, executing the installer is next. First the drive must be mounted and then the installer can be executed. This example shows executing the installer with the ```--help```, change this using to what is needed.

Some key points about the install:
- use ```--user``` to set the username of the user profile that will be created
- use ```--pass``` to set the password of the user profile that will be created along with the root user, both passwords can be changed by running ```passwd``` while logged in as the user. This parameter does not have its input text hidden or blurred so it is recommended to set it to something easy to remember and then change it later
- use ```--drive``` to set the drive that Arch will be installed on. This is essential for making sure data is written over. use a command like ```lsblk``` or ```fdisk -l``` to figure out the drives that are connected to the computer pick the drive used for Arch, it will most likely take the form of sdX and pass the ```--drive``` parameter ```/dev/sdX``` so it installs Arch on the correct drive.
- use ```--region``` and ```--city``` to set the time zone the computer will be in. It can be changed later by linking the correct directory to /etc/localtime in the following way ```ln -sf /usr/share/zoneinfo/Region/City /etc/localtime```. The regions can be found in ```/usr/share/zoneinfo```, and cities are found in sub-directories of the region directories.
- use ```--no-wm``` and ```--no-swap``` to disable the installation of the i3-wm or creation of the swap partition respectively.

The installation can be done in the following way:
```
mount --mkidr /dev/sdX temp
./temp/installer --help
```

The next step is to reboot the system.

The configure.cpp file can be used after the installation is done, it should be statically compiled and then brought to the newly installed system along with a configs file with respective configs for the i3-wm. The config files I use are in the configs folder and their names must be matched or the configure.cpp file will not work.

After this is completed, the system will be able to run Arch Linux. It is possible to enter an unconfigured i3-wm by running startx.

## How it does it
The list of commands that are run is going to be listed below. What each individual command does will be written next to the command after ```#```. This list of commands can be followed to manually install Arch alongside with [Installation Guide](https://wiki.archlinux.org/title/Installation_guide). The first set of commands is for installation with the swap partition and all the packages, the second is for no swap partition and minimal packages.
With Swap & Packages
```
timedatectl # Synchronize system clock
sudo wipefs --all /dev/sdX # Wipe all partitions and file systems on the drive
echo -e "g\nn\n\n\n+1G\ny\nn\n\n\n+4G\ny\nn\n\n\n\ny\nw\nq" | sudo fdisk /dev/sdX # Command that makes 3 paritions using fdisk, 1G for boot, 4G for swap, and the rest is root
sudo mkfs.fat -F 32 /dev/sdX1 # Make the first partition into a FAT32 partition
sudo mkswap /dev/sdX2 # Make the swap partition a swap partition
sudo mkfs.ext4 -F /dev/sdX3 # Make the storage partition ext4
mount /dev/sdX3 /mnt # Mount the storage partition
swapon /dev/sdX2 # enable the swap partition

pacstrap /mnt base linux linux-firmware vim # Install needed packages, and vim so there is a text editor if things will need to be changed
genfstab -U /mnt >> /mnt/etc/fstab # Generate an fstab file
arch-chroot /mnt # Changes the root to /mnt
ln -sf /usr/share/zoneinfo/Region/City /etc/localtime # Sets the time to the input region and city
hwclock --systohc
echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen # Adds the needed locales
locale-gen # Generate the locales
echo "LANG=en_US.UTF-8" > /etc/locale.conf # Set the LANG variable
echo "hostname" > /etc/hostname # Set the hostname of the system
echo -e "127.0.0.1\tlocalhost\n::1\tlocalhost\n127.0.1.1\thostname.localdomain\thostname" > /etc/hosts # Set up the hosts file
passwd # Set the root password of root user
pacman -Sy # update needed files if possible
pacman -S --noconfirm grub efibootmgr sudo networkmanager # Install some needed packages
systemctl enable NetworkManager # Enables NetworkManager to work on startup
mkdir /boot/EFI # Make the EFI directory
mount /dev/sdX1 /boot/EFI # Mount the EFI directory
grub-install --target=x86_64-efi --efi-directory=/boot/EFI --bootloader-id=GRUB # Install grub into the EFI directory
grub-mkconfig -o /boot/grub/grub.cfg # Create the config for grub
useradd -m -G wheel user # Add the user
passwd user # Sets the user password
echo \"%wheel ALL=(ALL:ALL) ALL\" >> /etc/sudoers
pacman -S --noconfirm btop neofetch firefox gcc gdb pipewire pipewire-pulse xorg xorg-xinit i3-gaps terminator polybar picom i3status dmenu # Install the extra packages
echo "exec i3" > /home/user/.xinitrc # Create the .xinitrc file for starting the i3-wm
exit # Exit the chroot enviornment
umount -R /mnt # Recursivelly unmount all mounted partitions
```

Without Swap & Packages
```
timedatectl # Synchronize system clock
sudo wipefs --all /dev/sdX # Wipe all partitions and file systems on the drive
"echo -e \"g\nn\n\n\n+1G\ny\nn\n\n\n\ny\nw\nq\" | sudo fdisk " + drive # Command that mkes 2 partitions using fdisk, 1G for boot, and the rest is root
sudo mkfs.fat -F 32 /dev/sdX1 # Make the first partition into a FAT32 partition
sudo mkfs.ext4 -F /dev/sdX2 # Make the storage partition ext4
mount /dev/sdX2 /mnt # Mount the storage partition

pacstrap /mnt base linux linux-firmware vim # Install needed packages, and vim so there is a text editor if things will need to be changed
genfstab -U /mnt >> /mnt/etc/fstab # Generate an fstab file
arch-chroot /mnt # Changes the root to /mnt
ln -sf /usr/share/zoneinfo/Region/City /etc/localtime # Sets the time to the input region and city
hwclock --systohc
echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen # Adds the needed locales
locale-gen # Generate the locales
echo "LANG=en_US.UTF-8" > /etc/locale.conf # Set the LANG variable
echo "hostname" > /etc/hostname # Set the hostname of the system
echo -e "127.0.0.1\tlocalhost\n::1\tlocalhost\n127.0.1.1\thostname.localdomain\thostname" > /etc/hosts # Set up the hosts file
passwd # Set the root password of root user
pacman -Sy # update needed files if possible
pacman -S --noconfirm grub efibootmgr sudo networkmanager # Install some needed packages
systemctl enable NetworkManager # Enables NetworkManager to work on startup
mkdir /boot/EFI # Make the EFI directory
mount /dev/sdX1 /boot/EFI # Mount the EFI directory
grub-install --target=x86_64-efi --efi-directory=/boot/EFI --bootloader-id=GRUB # Install grub into the EFI directory
grub-mkconfig -o /boot/grub/grub.cfg # Create the config for grub
useradd -m -G wheel user # Add the user
passwd user # Sets the user password
echo \"%wheel ALL=(ALL:ALL) ALL\" >> /etc/sudoers
pacman -S --noconfirm btop neofetch gcc gdb # Install some more packages
echo "exec i3" > /home/user/.xinitrc # Create the .xinitrc file for starting the i3-wm
exit # Exit the chroot enviornment
umount -R /mnt # Recursivelly unmount all mounted partitions
```
