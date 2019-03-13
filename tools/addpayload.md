
addpayload.sh

```bash
#!/bin/bash

# Check for payload format option (default is uuencode).
uuencode=1
if [[ "$1" == '--binary' ]]; then
	binary=1
	uuencode=0
	shift
fi
if [[ "$1" == '--uuencode' ]]; then
	binary=0
	uuencode=1
	shift
fi

if [[ ! "$1" ]]; then
	echo "Usage: $0 [--binary | --uuencode] PAYLOAD_FILE"
	exit 1
fi


if [[ $binary -ne 0 ]]; then
	# Append binary data.
	sed \
		-e 's/uuencode=./uuencode=0/' \
		-e 's/binary=./binary=1/' \
			 install.sh.in >install.sh
	echo "PAYLOAD:" >> install.sh

	cat $1 >>install.sh
fi
if [[ $uuencode -ne 0 ]]; then
	# Append uuencoded data.
	sed \
		-e 's/uuencode=./uuencode=1/' \
		-e 's/binary=./binary=0/' \
			 install.sh.in >install.sh
	echo "PAYLOAD:" >> install.sh

	cat $1 | uuencode - >>install.sh
fi
```

install.sh.in

```bash
#!/bin/bash

uuencode=1
binary=0

function untar_payload()
{
	match=$(grep --text --line-number '^PAYLOAD:$' $0 | cut -d ':' -f 1)
	payload_start=$((match + 1))
	if [[ $binary -ne 0 ]]; then
		tail -n +$payload_start $0 | tar -tzvf -
	fi
	if [[ $uuencode -ne 0 ]]; then
		tail -n +$payload_start $0 | uudecode | tar -tzvf -
	fi
}

read -p "Install files? " ans
if [[ "${ans:0:1}"  ||  "${ans:0:1}" ]]; then
	untar_payload
	# Do remainder of install steps.
fi

exit 0
```

```bash
$ sh addpayload.sh --uuencode abc.tar.gz
$ cat install.sh
#!/bin/bash

... # Installer script lines (see above)
read -p "Install files? " ans
... # More installer script lines (see above)
exit 0

PAYLOAD:
begin 644 -
M'XL(`))%G$D``^W12PJ$0`Q%T2REEI!HK%J/BM`]Z(F?_?O#J8+0&=TS"8'`
M"[Q6_D\WV7V?5AH]=COWBYB9%_4J:Q$UK6J7I`&_R3+-[9B2_+YS_[F]&\8I
JXJ%874#&J_X;^H_0!V2\ZC_3/P```````````````/!D!0OB?_,`*```
`
end
```

```bash
$ sh install.sh
Install files? y
-rw-r--r-- mitch/users       0 2009-02-18 11:29 a
-rw-r--r-- mitch/users       0 2009-02-18 11:29 b
```

```bash
(gzip | base64 -w0)<<<$data
echo "H4sIABlNiVwAA/NIzcnJVyjPL8pJ4QIA1eA5twwAAAA=" | base64 -d | gunzip
```




