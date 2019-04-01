#include <WiFiUDP.h>

// Port d'écoute UDP
const uint16_t PORT = 4321;
// Taille du tampon de réception
const uint16_t BUFFER_SIZE = 512;
// Tampon de réception
char buffer[BUFFER_SIZE];
// Taille du paquet reçu;
uint16_t len = 0;
// L'instance du serveur UDP
WiFiUDP udp;

// Démarrage de l'écoute
udp.begin(PORT);
Serial.print("Démarrage de l'ecoute sur le port ");
Serial.println(PORT);

void readPacket() {
	len = udp.available();
	Serial.print("Paquet de ");
	Serial.print(len);
	Serial.print(" octets recu de ");
	Serial.print(udp.remoteIP());
	Serial.print(":");
	Serial.print(udp.remotePort());
	Serial.print(" contenant '");
	// Mise en tampon du paquet
	udp.read(buffer, len);
	// Affichage du contenu du paquet
	for (int i = 1; i <= len; i++) {
		Serial.print(buffer[i - 1]);
	}
	Serial.println("'");
}
void loop() {
	if (udp.parsePacket() > 0) {
		readPacket();
	}
}

