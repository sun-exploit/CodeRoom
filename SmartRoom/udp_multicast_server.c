// Adresse IP multicast
const IPAddress IP_MCAST(239, 0, 0, 50);

// Démarrage de l'écoute
udp.begin(PORT);
Serial.print("Démarrage de l'ecoute sur le port ");
Serial.println(PORT);

// Démarrage de l'écoute en multicast
udp.beginMulticast(WiFi.localIP(), IP_MCAST, PORT);
Serial.print("Demarrage de l'ecoute sur les adresses ");
Serial.print(WiFi.localIP());
Serial.print(" et ");
Serial.print(IP_MCAST);
Serial.print(" sur le port ");
Serial.println(PORT);

// Commande 'HELO'
const char CMD_HELO[] = "HELO";
// Commande 'subscribe'
const char CMD_SUBS[] = "SUBS";
// Commande 'EXIT'
const char CMD_EXIT[] = "EXIT";
// Contenu ACK
const char CONTENT_ACK[] = "ACK";
// Contenu NACK
const char CONTENT_NACK[] = "NACK";

void sendBackAck() {
	sendPacket(CONTENT_ACK, udp.remoteIP(), udp.remotePort());
}

void sendBackAck() {
	sendPacket(CONTENT_NACK, udp.remoteIP(), udp.remotePort());
}

void unsubscribe() {
	// On arrête toutes les écoutes
	udp.stopAll();
}

void subscribe(IPAddress mip) {
	udp.beginMulticast(WiFi.localIP(), mip, PORT);
	Serial.print("Ecoute multicast l'adresses ");
	Serial.print(mip);
	Serial.print(" sur le port ");
	Serial.println(PORT);
}

void readPacket() {
	// Récupération de la taille du paquet
	len = udp.available();
	// Mise en tampon du paquet
	udp.read(buffer, len);
	// Ajout du terminateur de chaîne
	buffer[len] = '\0';
	if (strcmp(buffer, CMD_HELO) == 0) {
		// Commande 'HELO', réponse en envoyant un ACK
		sendBackAck();
	} else if (udp.destinationIP() != IP_MCAST) {
		// La paquet n'est pas à destination de l'adresse de découverte
		if (strcmp(buffer, CMD_EXIT) == 0) {
			// Commande 'EXIT', désinscription du groupe et ACK
			sendBackAck();
			// On arrête toutes les écoutes
			unsubscribe();
			// On démarre l'écoute sur le groupe de découverte
			subscribe(IP_MCAST);
		} else if (strncmp(buffer, CMD_SUBS, 4) == 0) {
			// Commande 'subscribe'
			IPAddress mip;
			// Test de validité de l'adresse
			if (mip.fromString(buffer + strlen(CMD_SUBS))) {
				sendBackAck();
				// On arrête toutes les écoutes
				unsubscribe();
				// On démarre l'écoute sur le groupe de découverte
				subscribe(IP_MCAST);
				// On démarre l'écoute sur le nouveau groupe
				subscribe(mip);
			} else {
				sendBackNack();
			}
		}
	} else {
		// Commande non reconnue
		sendBackNack();
	}
}


