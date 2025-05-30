#!/bin/sh
# SPDX-FileCopyrightText: 2021 Sotiris Papatheodorou
# SPDX-License-Identifier: CC0-1.0
# Usage: kindle4_jailbreak.sh KINDLE_ROOT_DIR
set -eu

if [ "$#" -ne 1 ] || [ ! -d "$1" ]; then
	printf 'Usage: %s KINDLE_ROOT_DIR\n' "$(basename "$0")"
	exit 2
fi

# Extract the jailbreak ZIP file
script_dir=$(dirname "$(readlink -f "$0")")
archive_dir="$script_dir/../archives"
unzip_dir=${TMPDIR:-/tmp}/kindle4_unzip
mkdir -p "$unzip_dir"
unzip -d "$unzip_dir" "$archive_dir/kindle-k4-jailbreak-1.8.N.zip" > /dev/null

# Copy the jailbreak files to the Kindle
kindle_dir="$1"
cp "$unzip_dir/data.tar.gz" "$kindle_dir/"
cp "$unzip_dir/ENABLE_DIAGS" "$kindle_dir/"
cp -R "$unzip_dir/diagnostic_logs" "$kindle_dir/"

printf 'Jailbreak files copied into\n    %s\n' "$kindle_dir"
printf '* Unmount and disconnect the Kindle.\n'
printf '* Put it in Airplane Mode to prevent it from connecting to the \n'
printf '  Internet. Amazon will undo the jailbreak, USB networking and fake\n'
printf '  registration if this happens. Keep the WiFi disabled for the \n'
printf '  duration of the setup using\n'
printf '      ≡ Button -> Settings -> Airplane Mode\n'
printf '* Restart the Kindle using\n'
printf '      ≡ Button -> Settings -> ≡ Button -> Restart\n'
printf '* Once it restarts in diagnostic mode select:\n'
printf '      D) Exit, Reboot or Disable Diags\n'
printf '      R) Reboot System\n'
printf '      Q) To continue\n'
printf '* Once it restarts in normal mode you should see a document titled\n'
printf '      You are Jailbroken\n'

# Clean up
rm -rf "$unzip_dir"

