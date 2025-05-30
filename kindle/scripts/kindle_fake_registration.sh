#!/bin/sh
# SPDX-FileCopyrightText: 2021 Sotiris Papatheodorou
# SPDX-License-Identifier: CC0-1.0
#
# Usage: kindle_fake_registration.sh [NAME] [KINDLE_IP]
# NAME defaults to "seal" and KINDLE_IP defaults to 192.168.15.244 (the IP used
# by the Kindle 4 non-touch).
#
# Supported devices:
# * Kindle 4 non-touch
# * Kindle 3 WiFi

set -eu

case "$#" in
	0)
		name='seal'
		kindle_ip='192.168.15.244'
		;;
	1)
		name="$1"
		kindle_ip='192.168.15.244'
		;;
	*)
		name="$1"
		kindle_ip="$2"
		;;
esac 

if ! ssh -T -o ConnectTimeout=2 -i ~/.ssh/kindle.key root@"$kindle_ip" ls > /dev/null 2>&1; then
	printf 'Error: The Kindle must be connected and USB networking must be enabled\n'
	exit 1
fi

# Generate the setup script
setup_script=${TMPDIR:-/tmp}/kindle_setup.sh
cat << EOF > "$setup_script"
#!/bin/sh
set -eu
/usr/sbin/mntroot rw > /dev/null 2>&1
# Backup the modified files
mkdir -p /mnt/us/backup/registration/
cat << 'FEO' > /mnt/us/backup/registration/reginfo
<? Xml version = "1.0" encoding = "UTF-8"?>
<response>
  <user_device_name>$name</user_device_name>
  <global_device_name>$name@kindle.com</global_device_name>
  <given_name>$name</given_name>
  <name>$name</name>
  <alias>$name</alias>
</response>
FEO
cp /opt/amazon/ebook/config/ServerConfig.conf /mnt/us/backup/registration/ServerConfig.conf.bak
# Modify the files
cp /mnt/us/backup/registration/reginfo /var/local/java/prefs/reginfo
sed -i 's/^url\\.fints/#url.fints/' /opt/amazon/ebook/config/ServerConfig.conf
EOF

# rsync it to the Kindle
rsync --rsh "ssh -i ~/.ssh/kindle.key" "$setup_script" root@"$kindle_ip":~/ > /dev/null 2>&1

# Run the script and then delete it
ssh -i ~/.ssh/kindle.key root@"$kindle_ip" 'sh ~/kindle_setup.sh && rm ~/kindle_setup.sh'

printf 'Fake registration finished\n'

