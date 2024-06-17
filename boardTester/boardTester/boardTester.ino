int qntPins = 10;
int inPin[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
int outPin[] = {22, 23, 24, 25, 26, 27, 28, 29,30, 31};
int mapper[] = {0,0,0,0,0,0,0,0,0,0};
#define UPPER_VOLTAGE 950
#define LOWER_VOLTAGE 74
void setup() {
  Serial.begin(9600);
  analogReference(DEFAULT);
  for(int i=0;i<qntPins;i++){
    pinMode(inPin[i], INPUT);
    pinMode(outPin[i], OUTPUT);
  }
  int correctPins = 0;
  for(int i=0;i<qntPins;i++){
    digitalWrite(outPin[i],1);
    delay(1000);
    int which = -1;
    for(int j=0;j<qntPins;j++){
        int value = analogRead(inPin[j]) > UPPER_VOLTAGE;
        if(value) {
          if(which != -1) {
            which = -1;
            break;
          }
          which = j;
        }
    }
    digitalWrite(outPin[i],0);
    Serial.println("pin " + String(outPin[i]) + " goes to "+ String(which));
    if(which == -1) {
      outPin[i] = -1;
      correctPins--;
    }
    correctPins++;
    mapper[i] = which;
  }
  if(correctPins==0){
    for(;;){
        Serial.println("Erro ao mapear");
        delay(1000);
    }
  }
  for(int i=0;i<qntPins;i++){
    if(outPin[i] != -1) continue;
    for(int j = i+1;j<qntPins;j++){
      if(outPin[j] != -1){
        outPin[i] = outPin[j];
        mapper[i] = mapper[j];
        outPin[j] = -1;
        break;
      }
    }
  }
  qntPins = correctPins;
  Serial.print("Current pins: ");
  for(int i=0;i<qntPins;i++){
    Serial.print(String(outPin[i]) + "("+String(mapper[i])+") ");
  }
  Serial.println();
}

void loop() {
    bool error = false;
    for(int mask=0;!error && mask< (1<<qntPins);mask++){
        //Serial.print("Out: ");
        for(int i=0;i<qntPins;i++){
            if(outPin[i] == -1) continue;
            int value = !!(mask&(1<<i));
            digitalWrite(outPin[i], value);
            //Serial.print(char('0'+value));
            int analogVal = analogRead(inPin[mapper[i]]); 
            int inVal = analogVal >= UPPER_VOLTAGE ? 1 : (analogVal <= LOWER_VOLTAGE ? 0 : -1);
            if(inVal != value){
                error = true;
                break;
            }
        }
    }
    if(error){
        for(;;){
            Serial.println("Erro em teste");
            delay(100);
            Serial.println("Erro em teste");
            delay(1000);
        }
    }
}
