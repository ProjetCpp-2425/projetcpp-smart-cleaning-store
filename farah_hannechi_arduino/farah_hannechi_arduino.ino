// definition des pins de buzzer et de capteur ultrason
const int trigPin = 9;      
const int echoPin = 10;    
const int buzzerPin = 8;  

// definition de variables
long duration;            
int productCount = 0;      //compteur pour nombre de produits
bool productDetected = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600); // communication serie
  //Serial.println("Product Counter Initialized");
}

void loop() {
  if(Serial.available()>0){ //partie QT
 String   receiveddata =Serial.readStringUntil('\n');
    int receivedv =receiveddata.toInt();
    productCount=receivedv;
  }//fin Qt
  digitalWrite(trigPin, LOW);//pour eviter l'interference
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  if (duration > 0 && duration <= 300) { //distance sup 300
    if (!productDetected) {            // Ensure each product is counted once
      productCount++;                
      digitalWrite(buzzerPin, HIGH);  
      delay(100); //temps de buzzing                     
      digitalWrite(buzzerPin, LOW);    
      productDetected = true;          
      //Serial.print("Product Count: ");
      Serial.println(productCount);    
    }
  } else {
    productDetected = false; // Reset detection for the next object
  }

  delay(100);
}
