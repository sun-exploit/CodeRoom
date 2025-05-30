#!/bin/sh
# SPDX-FileCopyrightText: 2021 Sotiris Papatheodorou
# SPDX-License-Identifier: CC0-1.0
#
# Usage: kindle_hosts_blocking.sh [KINDLE_IP]
# KINDLE_IP defaults to 192.168.15.244 (the IP used by the Kindle 4 non-touch).

set -eu

if [ "$#" -eq 0 ]; then
	kindle_ip='192.168.15.244'
else
	kindle_ip="$1"
fi

if ! ssh -T -o ConnectTimeout=2 -i ~/.ssh/kindle.key root@"$kindle_ip" ls > /dev/null 2>&1; then
	printf 'Error: The Kindle must be connected and USB networking must be enabled\n'
	exit 1
fi

# Generate the setup script
setup_script=${TMPDIR:-/tmp}/kindle_setup.sh
cat << 'EOF' > "$setup_script"
#!/bin/sh
set -eu
/usr/sbin/mntroot rw > /dev/null 2>&1
# Backup the hosts file
cp /etc/hosts /mnt/us/backup/hosts
# Modify the hosts file
cat << 'FEO' >> /etc/hosts
127.0.0.1 amazon.ca
127.0.0.1 amazon.cn
127.0.0.1 amazon.co.jp
127.0.0.1 amazon.com
127.0.0.1 amazon.com.au
127.0.0.1 amazon.com.br
127.0.0.1 amazon.com.mx
127.0.0.1 amazon.co.uk
127.0.0.1 amazon.de
127.0.0.1 amazon.es
127.0.0.1 amazon.eu
127.0.0.1 amazon.fr
127.0.0.1 amazon.in
127.0.0.1 amazon.it
127.0.0.1 amazon.ru
127.0.0.1 cde-g7g.amazon.com
127.0.0.1 det-g7g.amazon.com
127.0.0.1 det-ta-g7g.amazon.com
127.0.0.1 device-messaging-na.amazon.com
127.0.0.1 fints-g7g.amazon.com
127.0.0.1 firs-g7g.amazon.com
127.0.0.1 firs-ta-g7g.amazon.com
127.0.0.1 fras-g7g.amazon.com
127.0.0.1 images-amazon.com
127.0.0.1 kdk-ws.amazon.com
127.0.0.1 kindle.amazon.com
127.0.0.1 kindleatt1.amazon.com
127.0.0.1 kindle-time.amazon.com
127.0.0.1 ntp-g7g.amazon.com
127.0.0.1 todo-g7g.amazon.com
127.0.0.1 w.amazon.com
127.0.0.1 www.amazon.com 
FEO
EOF

# rsync it to the Kindle
rsync --rsh "ssh -i ~/.ssh/kindle.key" "$setup_script" root@"$kindle_ip":~/ > /dev/null 2>&1

# Run the script and then delete it
ssh -i ~/.ssh/kindle.key root@"$kindle_ip" 'sh ~/kindle_setup.sh && rm ~/kindle_setup.sh'

printf 'Host blocking finished\n'
  
