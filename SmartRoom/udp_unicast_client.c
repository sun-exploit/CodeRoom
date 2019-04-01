void sendPacket(const char content[], IPAddress ip, uint16_t port) {
	udp.beginPacket(ip, port);
	udp.write(content);
	udp.endPacket();
}

readPacket();

sendPacket(buffer, udp.remoteIP(), udp.remotePort());

