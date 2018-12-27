```
# Turn off HDMI & Composite output: /opt/vc/bin/tvservice -o
40 8 * * 1-5 /opt/vc/bin/tvservice -p && /home/pi/start_dashboard.sh
30 19 * * 1-5 killall xinit && /opt/vc/bin/tvservice -p

pi@raspberrypi:~ $ cat .xinitrc
unclutter &

xset s off         # don't activate screensaver
xset -dpms         # disable DPMS (Energy Star) features.
xset s noblank     # don't blank the video device

exec /home/pi/dashboard.sh

pi@raspberrypi:~ $ cat dashboard.sh
#!/bin/sh

#uzbl -u http://<IP> -c /home/pi/uzbl.conf &
#feh -F /usr/share/doc/libfreetype6/image/BlendingExamples-thumbnail.jpg

matchbox-window-manager  &
chromium-browser --noerrdialogs --disable-restore-session-state --incognito "http://<IP>" &
sleep 2
sleep 1
WID=$(xdotool search --onlyvisible --class chromium|head -1)
xdotool windowactivate ${WID}
sleep 2
xdotool key ctrl+t
sleep 2
xdotool type 'http://coderoom.free.fr/wifi/data/'
sleep  1
xdotool key "Return"
sleep 50
xdotool key F11
sleep 1
xdotool key ctrl+Tab
while true; do
sleep 250
xdotool key ctrl+Tab
#xdotool key F5
sleep 10
xdotool key ctrl+Tab
done

chvt 6
chvt 7

40 8 * * 1-5 /opt/vc/bin/tvservice -p && /home/pi/start_dashboard.sh
30 19 * * 1-5 killall xinit && /opt/vc/bin/tvservice -o
06 9 * * 1-5 DISPLAY=:0 /usr/bin/timeout 20 /usr/bin/omxplayer --no-keys -o local `/usr/local/bin/youtube-dl -f mp4 -g https://www.youtube.com/watch?v=dXPLhE_z57M`
30 9 * * 1-5 DISPLAY=:0 /usr/bin/timeout 20 /usr/bin/omxplayer --no-keys -o local `/usr/local/bin/youtube-dl -f mp4 -g https://www.youtube.com/watch?v=dXPLhE_z57M`


```
