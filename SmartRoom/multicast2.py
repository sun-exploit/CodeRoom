# Knihovny pro praci se siti a JSON
import socket
import struct
import json

# Multicastova IP adresa a UDP port
# Viz https://cs.wikipedia.org/wiki/IP_multicast
multicast_ip = "224.0.0.0"
multicast_port = 8201

# Nastaveni UDP multicastu pro poslech dat jen v LAN a
# z nastavene multicastove IP adresy a portu
spojeni = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
spojeni.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
spojeni.bind((multicast_ip, multicast_port))
mreq = struct.pack("4sL", socket.inet_aton(multicast_ip), socket.INADDR_ANY)
spojeni.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

print("Posloucham na 224.0.0.0:8201...")
print("")

# Nekonecna smycka, dokud program neukoncim
while True:
    # Pokud dorazila zprava, uloz ji a adresu odesilatele
    # 1024 predstavuje velikost pameti pro zpravu, tedy nejvyse 1 024 B
    zprava, adresa = spojeni.recvfrom(1024)
    print("Prichozi zprava od " + adresa[0] + ": " + zprava)
    # Pokud zprava obahuje slovo REGISTRACE,
    # vyseparuj z ni jednotlive hodnoty oddelene znakem /,
    # uloz je do JSON objektu a vypis na monitor
    if "REGISTRACE" in zprava:
        zprava = zprava.split("/")
        zarizeni = {}
        zarizeni["Nazev"] = zprava[1]
        zarizeni["Role"] = int(zprava[2])
        zarizeni["Akce"] = zprava[3]
        zarizeni["IP"] = adresa[0]
        zarizeni["MAC"] = zprava[4].upper()

        print("")
        print("Nove zarizeni touzi po registraci:")
        print("Nazev: " + zarizeni["Nazev"])
        print("Role: " + str(zarizeni["Role"]))
        print("Akce: " + zarizeni["Akce"])
        print("IP adresa: " + zarizeni["IP"])
        print("MAC adresa: " + zarizeni["MAC"])
        print("")

        # Uloz popis zarizeni v JSON do souboru krabicky/macadresa.krabicka
        with open("krabicky/" + zarizeni["MAC"] + ".krabicka", "w") as soubor:
            json.dump(zarizeni, soubor)

        # Odesli zpet zpravu s podpisem centraly,
        # aby si krabicka mohla ulozit jeji IP adresu a napriste s ni komunikovala
        # treba skrze HTTP
        print("Posilam krabicce svuj podpis, at vi, jakou mam IP adresu")
        print("")
        spojeni.sendto("JA JSEM TADY VELKY SEF", adresa)
