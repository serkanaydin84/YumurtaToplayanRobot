#define kled 11  //Kırmızı ledlerimizi bağladık
#define yled 10
#define mz80_SagOn A0  //sağ ön sensörümüzü A0 numaralı pine bağladık sensörümüzü
#define mz80_SolOn A1  //sol ön sensörümüzü A1 numaralı pine bağladık sensörümüzü
#define mz80_SagArka A2  //sağ arka sensörümüzü A2 numaralı pine bağladık sensörümüzü

void setup() {
  pinMode(kled,OUTPUT); // ledleri çıkış olarak tanımladık
  pinMode(yled,OUTPUT); // ledleri çıkış olarak tanımladık
  pinMode(mz80_SagOn, INPUT);  //Kızılotesi sensoru giriş olarak tanımladık
  pinMode(mz80_SolOn, INPUT);  //Kızılotesi sensoru giriş olarak tanımladık
  pinMode(mz80_SagArka, INPUT);  //Kızılotesi sensoru giriş olarak tanımladık
  
  Serial.begin(9600);
}

void loop() {
  Serial.print("Sağ Ön : ");
  Serial.println(analogRead(mz80_SagOn));
  Serial.print("Sol Ön : ");
  Serial.println(analogRead(mz80_SolOn));
  Serial.print("Sağ Arka : ");
  Serial.println(analogRead(mz80_SagArka));
  //delay(1000);
  if (analogRead(mz80_SagOn) < 1000 || analogRead(mz80_SolOn) < 1000 || analogRead(mz80_SagArka) < 1000) //engel yoksa kırmızı ledi söndür
  { 
    digitalWrite(kled, 1);
    digitalWrite(yled, 0);
  }
  else
  { //engel varsa kırmızı ledi yak
    digitalWrite(kled, 0);
    digitalWrite(yled, 1);
  }
}
