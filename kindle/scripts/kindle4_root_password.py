#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2021 Sotiris Papatheodorou
# SPDX-License-Identifier: CC0-1.0
# https://www.sven.de/kindle/
# http://www.mobileread.com/forums/showthread.php?p=1873256

import hashlib
import os.path
import re
import sys

def usage():
    print("Usage: {} SERIAL".format(os.path.basename(sys.argv[0])))
    print("Show possible root passwords for a Kindle 4 with serial number SERIAL.")
    print("The serial number is shown on the second settings page.")
    print("It consists of 16 hexadecimal digits.")

def regularize_serial(serial):
    """Remove spaces and capitalize letters"""
    return "".join(serial.split()).upper()

def valid_serial(serial):
    """Test that the serial contains only 16 of the characters 0-9 and A-F"""
    if re.search("^[0-9A-F]{16}$", serial) is None:
        return False
    else:
        return True



if len(sys.argv) != 2:
    usage()
    sys.exit(2)

serial = regularize_serial(sys.argv[1])

if not valid_serial(serial):
    print("Error: invalid serial number, expected 16 hexadecimal digits")
    sys.exit(2)

serial_hash = hashlib.md5((serial + "\n").encode("UTF-8")).hexdigest()
passwords = [serial_hash[13:16], serial_hash[7:10], serial_hash[7:11]]
passwords = ["fiona" + x for x in passwords]
passwords.insert(0, "mario")

print("Possible root passwords:")
for password in passwords:
    print(password)
print("\nUnprivileged login:")
print("User: framework")
print("Password: mario")

