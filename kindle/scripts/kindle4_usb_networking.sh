#!/bin/sh
# SPDX-FileCopyrightText: 2021 Sotiris Papatheodorou
# SPDX-License-Identifier: CC0-1.0
# Usage: kindle4_usb_networking.sh KINDLE_ROOT_DIR
set -eu


if [ "$#" -ne 1 ] || [ ! -d "$1" ]; then
	printf 'Usage: %s KINDLE_ROOT_DIR\n' "$(basename "$0")"
	exit 2
fi

# Read the public SSH key that will be used to connect to the kindle
ssh_public_key_file="$HOME"/.ssh/kindle.key.pub
if [ -r "$ssh_public_key_file" ]; then
	ssh_public_key=$(cat "$ssh_public_key_file")
else
	printf 'Error: Could not read the public SSH key from %s\n' "$ssh_public_key_file"
	exit 1
fi

# Extract the USB networking ZIP file
script_dir=$(dirname "$(readlink -f "$0")")
archive_dir="$script_dir/../archives"
unzip_dir=${TMPDIR:-/tmp}/kindle4_unzip
mkdir -p "$unzip_dir"
unzip -d "$unzip_dir" "$archive_dir/kindle-usbnetwork-0.57.N-k4.zip" > /dev/null

# Copy the USB networking installer to the Kindle
kindle_dir="$1"
cp "$unzip_dir/Update_usbnetwork_0.57.N_k4_install.bin" "$kindle_dir/"

printf 'USB networking installer copied into\n    %s\n' "$kindle_dir"
printf '* Unmount and disconnect the Kindle but DO NOT RESTART IT.\n'
printf '* Install USB networking using\n'
printf '      ≡ Button -> Settings -> ≡ Button -> Update Your Kindle\n'
printf '* Once the installation is finished and the Kindle is back\n'
printf '  in the main menu, connect it and mount it again.\n'
while true; do
	printf '* Type "done" once the Kindle is remounted on %s\n' "$kindle_dir"
	read -r input
	if [ "$input" = "done" ] && [ -d "$kindle_dir" ]; then
		break
	fi
done

printf '%s\n' "$ssh_public_key" > "$kindle_dir/usbnet/etc/authorized_keys"

printf '* Unmount and disconnect the Kindle.\n'
printf '* To enable USB networking, open the keyboard (⌨ Button) and enter\n'
printf '      ;debugOn\n'
printf '      ~usbNetwork\n'
printf '* Connect the Kindle. It will now appear as a\n'
printf '      Netchip Technology, Inc. Linux-USB Ethernet/RNDIS Gadget\n'
printf '  in lsusb output.\n'
printf '* Network Manager will try and fail to connect. Edit the connection \n'
printf '  IPv4 Settings, set Method to Manual and add an \n'
printf '  Address/Netmask/Gateway with values 192.168.15.201/24/192.168.15.201.\n'
printf '* Once the connection succeeds you can SSH into the Kindle using\n'
printf '      ssh -i ~/.ssh/kindle.key root@192.168.15.244\n'
printf '  or with the following command and any password\n'
printf '      ssh root@192.168.15.244\n'
printf '* To disable USB networking, open the keyboard (⌨ Button) and enter\n'
printf '      ~usbNetwork\n'
printf '      ;debugOff\n'
printf '  You might want to postpone this until the end of the setup.\n.'

# Clean up
rm -rf "$unzip_dir"

