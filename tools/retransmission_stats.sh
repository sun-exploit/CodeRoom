#!/bin/bash
# Toni Comerma
# Based on EMC script for vnx
# https://community.emc.com/docs/DOC-7936

export packet_sent=1
export packets_retransmitted=1
export first=1
export current_ts=$(date +%s)
export previous_ts=0
export previous_packets_retransmitted=0
export previous_packet_sent=0
#export previous_ts=0

state_file="/var/tmp/$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"

function _reset() {
  echo "export previous_ts=0" > "${state_file}"
  echo "export previous_packets_retransmitted=0" >> "${state_file}"
  echo "export previous_packet_sent=0" >> "${state_file}"
}

function set_variable() {
  local variablename=$1
  local value=$2
  sed -i -e "s/${variablename}=.*/${variablename}=${value}/" "${state_file}"
}

[ ! -e "${state_file}" ] && _reset

. "${state_file}"
export delta=$(echo "$current_ts - $previous_ts" | bc -l)

if [ $delta -lt 0 ]; then
  _reset
  export delta=0
fi

set_variable "previous_ts" "$current_ts"

if [ -z "$1" ]
then
  echo "Select interface"
  exit
fi

case `uname` in
  Darwin) text_packet_sent="[0-9] packets sent"
          text_packet_retrans=" data packets.*retransmitted"
          ;;
  Linux)  text_packet_sent=" segments send out"
          text_packet_retrans=" segments retransmited"
          ;;
  *) echo "OS version not supported"
     exit 1
     ;;
esac
echo "
#########################################################
# Consider the following table for recommended values : #
# Above .1%                ->    Too high               #
# Between .01% and .1%     ->    Questionable           #
# Below .01%               ->    OK                     #
#########################################################
"
printf "le script est passe il y a %s secondes\n" "$delta"

DATE="date +%m/%d/%y"; TIME="date +%H:%M:%S";
result=`netstat -p tcp -s $1 |awk -v ps="$text_packet_sent" -v pr="$text_packet_retrans" '{if(match($0, ps))printf( "%d ", $1); else if(match($0,pr))printf( "%d", $1)}'`
read -r packet_sent packets_retransmitted <<< "$result"

[ $packet_sent -lt $previous_packet_sent ] && previous_packet_sent=$packet_sent
[ $packets_retransmitted -lt $previous_packets_retransmitted ] && previous_packets_retransmitted=$packets_retransmitted

if [ $packet_sent -gt $previous_packet_sent ]; then     
  printf "%d %d %d %d " $previous_packet_sent $packet_sent $previous_packets_retransmitted $packets_retransmitted|awk -v dt=$($DATE) -v tm=$($TIME) '{printf("%s %s packet_sent=%d packets_retransmitted=%d rate=%f%% \n", dt, tm, $2, $4, (( $4*1.0 - $3*1.0 ) / ( $2*1.0 - $1*1.0 )) * 100.0)}'
else
  printf "%d %d %d %d " $previous_packet_sent $packet_sent $previous_packets_retransmitted $packets_retransmitted|awk -v dt=$($DATE) -v tm=$($TIME) '{printf("%s %s packet_sent=%d packets_retransmitted=%d rate=%f%% \n", dt, tm, $2, $4, 0.0)}'
fi

echo -n "packet_sent "
echo "$packet_sent - $previous_packet_sent" | bc -l
echo -n "packets_retransmitted "
echo "$packets_retransmitted - $previous_packets_retransmitted" | bc -l

set_variable "previous_packets_retransmitted" "$packets_retransmitted"
set_variable "previous_packet_sent" "$packet_sent"
