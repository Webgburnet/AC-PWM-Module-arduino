/*
Pin | Interrompre # | Plateforme Arduino
---------------------------------------
2   | 0             | -Tous mais c'est INT1 sur le Leonardo
3   | 1             | -Tous mais c'est INT0 sur le Leonardo
18  | 5             | Arduino Mega uniquement
19  | 4             | Arduino Mega uniquement
20  | 3             | Arduino Mega uniquement
21  | 2             | Arduino Mega uniquement
0   | 0             | Leonardo
1   | 3             | Leonardo
7   | 4             | Leonardo
L'Arduino Due n'a pas de broches d'interruption standard car un iterrupt peut être attaché à presque toutes les broches.

Dans le programme, la broche 2 est choisie
*/
int AC_LOAD = 12;    // Sortie sur broche Opto Triac
int dimming = 128;  // Niveau du dimmer (0-128) 0 = ON, 128 = OFF
int save_dimming = 128;

void setup()
{
  Serial.begin(9600);
  pinMode(AC_LOAD, OUTPUT);// Définir la broche AC Load comme sortie
  attachInterrupt(0, zero_crosss_int, RISING);  // Choisissez l'interruption zero-cross # dans le tableau ci-dessus
  Serial.println("Fin Setup \n --");
  Serial.print("lampValue -> ");
  Serial.print((128-dimming)*100/128);
  Serial.print("% \t");
  Serial.print(dimming);
  Serial.println("");
  Serial.println("Entrée l'intensite lumineuse (0~100)");
}

// la fonction d'interruption ne doit prendre aucun paramètre et ne retourner rien
void zero_crosss_int()  // fonction à tirer au passage à zéro pour atténuer la lumière
{
  // Calcul de la Fréquence : 1 full 50Hz wave =1/50=20ms 
  // zerocrossing  (50Hz)-> 10ms (1/2 Cycle) 
  // Pour60Hz => 8.33ms (10.000/120)(1/2 Cycle) 
  // 10ms=10000us
  // (10000us - 10us) / 128 = 75 (Approx) Pour 60Hz =>65

  int dimtime = (75*dimming);    // Pour 60Hz =>65    
  delayMicroseconds(dimtime);    // Attendez de la mise en marche du TRIAC 
  digitalWrite(AC_LOAD, HIGH);   // Mise en marche du TRIAC 
  delayMicroseconds(8.33);         // triac retard de propagation (cf doc tech)
         // (pour une utilisation à 60 Hz 8,33) Certains triacs nécessitent une période plus longue
  digitalWrite(AC_LOAD, LOW);    // Ne déclenche plus le TRIAC (le prochain passage à zéro le désactivera) TRIAC
}

void loop()  {
  if(Serial.available())
  {
    int buf = Serial.parseInt();
    if(buf>100 ||buf<0)
    {
      save_dimming=dimming;
    }
    else
    {
      dimming=128-(buf*128)/100;
      save_dimming=dimming;
    }
    Serial.print("lampValue -> ");
    Serial.print(buf);
    Serial.print("% \t");
    Serial.print(dimming);
    Serial.println("");
  }
}
