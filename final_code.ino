#define ENA 9                  // Pin de commande de driver 
#define IN3 5                  // IN3 , IN4 Pins pour Moteur
#define IN4 4
#define CHA 2                  // Pin interruption encodeur
#define CHB 3                  // Pin encodeur
#define a  0.1462              // Les coefs a,b,c,d,f du l'equation du correcteur
#define b  0.1448
#define c  0.0014
#define d  1.7293
#define f 0.7293
#define K 1
#define TIME_INTERVAL 10.0     // temps d'echantionnallage en ms
#define TICKS_TURN 100.0       // Resolution du codeur 100 impulsion/tour
float e[3]={0} ;               // tableau des erreurs   
float y[3]={0} ;               //tableau du sortie du correcteurs
float refdeg=3000;
float ref=refdeg/9.54929;        // reference : vitesse en rad/s
float com=0 ;                  // Sortie de correcteur instantanee
float vitesse=0;               // Vitesse du moteur rad/s
unsigned long ticksCounter = 0;          // C'est un compteur qui calcul nb du ticks
unsigned long prevTicksValue = 0;   
float numberOfTurns = 0;                        // nb du tour du moteur
float motorSpeed = 0;                  
void asserv(float e[],float y[],float v){       // Fonction d'asservissement du vitesse 
  e[2]=ref-v; //y[2] par calcul encodeur        // Erreur a l'instant n 

  /*Serial.print(e[0]);
  Serial.print(e[1]);
  Serial.println(e[2]);*/
  com=a*e[2]-b*e[1]+c*e[0]+d*y[1]-f*y[0];       // sotie du correcteur a l'instant n
  com=constrain(com,0,255);                     // contraint un nombre a rester dans un intervalle ex dans moteur [0 , 255]
  e[0]=e[1];                                    // decalage des valeurs dans le tableau des erreurs et le tableau du sortie de correcteur
  e[1]=e[2];
  y[0]=y[1];
  y[1]=com;

} 
float calcul_vit(){                                 // fonction qui calcul vitesse
  int numberOfTicks = ticksCounter ;   
  ticksCounter = 0;
  numberOfTurns = numberOfTicks / TICKS_TURN;             
  return (numberOfTurns * (60000 / TIME_INTERVAL)) ;         // La vitesse en tr/min
}
void countTick() {           // interruption qui calcul nb de ticks
  ticksCounter++;
}
void turnON(float com){             // demarre le moteur
 digitalWrite(IN3,HIGH);
 digitalWrite(IN4,LOW);
 analogWrite(ENA,com);
   }
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // initialize pins
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(CHA, INPUT);
  attachInterrupt(digitalPinToInterrupt(CHA), countTick, FALLING );     // fonction d'interruption 
  analogWrite(ENA, 0);
  //e[0]=ref;               // Erreur = ref - vitesse  et puisque au debut on initialise la vitesse = 0 alors Erreur = ref
  //e[1]=ref; 
  //e[2]=ref;
  

}
void loop() {
  // put your main code here, to run repeatedly:
  vitesse=calcul_vit()/9.54929;    // Conversion du vitesse tr/min en rad/s
  asserv(e,y,vitesse);             // fonction qui asservie la vitesse du moteur
  /*Serial.println(com);*/
  turnON(com);
  Serial.println(vitesse*9.549);
  //Serial.print(refdeg);
  delay(TIME_INTERVAL);
}
