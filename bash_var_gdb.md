cat /proc/$$/maps
cat /proc/$pid/maps | grep rw-p | head -20

cat /proc/self/environ | tr '\0' '\n'

gdb -q -p <PID>
info proc mappings
find 0x1efbb000,0x1effe000,"AZERTY123"
x/200s 0x1efc6000
x/s <adresse_trouvee>
x/100gx 0x1efc1984-0x80

info files

gcore -o bash.core <PID>
grep -a -C2 "NOM_VARIABLE" bash.core.<PID>
