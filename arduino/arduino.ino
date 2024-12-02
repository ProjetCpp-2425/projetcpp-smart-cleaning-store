// Définition des pins du buzzer et du capteur ultrason
const int trigPin = 9;  // Pin de déclenchement
const int echoPin = 10; // Pin de réception d'écho
const int buzzerPin = 8; // Pin connectée au buzzer

// Définition des variables globales
long duration; // Temps de l'écho
bool clientDetected = false; // Client détecté ou non
unsigned long lastDetectionTime = 0; // Temps de la dernière détection
const unsigned long detectionInterval = 10000; // Intervalle entre deux détections

void setup() {
  pinMode(trigPin, OUTPUT); // Déclaration de la pin trig comme sortie
  pinMode(echoPin, INPUT);  // Déclaration de la pin echo comme entrée
  pinMode(buzzerPin, OUTPUT); // Déclaration de la pin buzzer comme sortie
  Serial.begin(9600); // Initialisation de la communication série
}

void loop() {
  unsigned long currentTime = millis();

  // Vérifier si une commande est reçue via le port série
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Lire la commande reçue
    command.trim();
    if (command == "ACTIVATE_BUZZER") {
      digitalWrite(buzzerPin, HIGH); // Activer le buzzer
      delay(100); // Durée de 100 ms
      digitalWrite(buzzerPin, LOW); // Désactiver le buzzer
    }
  }

  // Vérifier si 10 secondes se sont écoulées depuis la dernière détection
  if (currentTime - lastDetectionTime >= detectionInterval) {
    // Générer une impulsion pour le capteur ultrason
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Lire la durée de l'écho
    duration = pulseIn(echoPin, HIGH);

    // Si la distance est inférieure à 300 (environ 50 cm)
    if (duration > 0 && duration <= 300 && !clientDetected) {
      Serial.println("Detected"); // Envoyer un message à Qt
      clientDetected = true; // Marquer le client comme détecté
      lastDetectionTime = currentTime; // Mettre à jour l'heure de détection
    }

    // Réinitialiser la détection si la distance est supérieure à 300
    if (duration > 300) {
      clientDetected = false;
    }
  }
}
