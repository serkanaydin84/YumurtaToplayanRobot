#include <Servo.h>

// ***************************
// renk sensörü tanımlamaları
#define csOrta_s0 13
#define csOrta_s1 12
#define csOrta_s2 8
#define csOrta_s3 7
#define csOrta_out 4
#define csArka_s0 13
#define csArka_s1 12
#define csArka_s2 8
#define csArka_s3 7
#define csArka_out 2

// MZ-80 Sensörleri için tanımlamalarımızı yapıyoruz
#define mz80_SagOn A0     //sağ ön sensörümüzü A0 numaralı pine bağladık sensörümüzü
#define mz80_SolOn A1     //sol ön sensörümüzü A1 numaralı pine bağladık sensörümüzü
#define mz80_SagArka A2   //sağ arka sensörümüzü A2 numaralı pine bağladık sensörümüzü
#define mz80_SolArka A3   //sol arka sensörümüzü A3 numaralı pine bağladık sensörümüzü
#define mz80_Arka A4      //arka sensörümüzü A4 numaralı pine bağladık sensörümüzü

// DC MOTOR TANIMLAMALARI
#define enA 0
#define enB 1
#define in1 5
#define in2 6
#define in3 10
#define in4 11

Servo servoOrta;
Servo servoArka;

int degreeOrta, degreeArka;          // Servoların hareket derecelerini tutar
int yumurtaKirmizi, yumurtaYesil, yumurtaMavi;  // Ortadaki renk sönsöründen gelen renklerini tutar
int zeminKirmizi, zeminYesil, zeminMavi;        // arkadaki renk sönsöründen gelen renklerini tutar
int yumurtaSayisi = 0;                          // Kaç tane yumurta topladık

int sagOnMz80;      // Sağ öndeki mesafe sensörünün değerini tutar
int solOnMz80;      // Sol öndeki mesafe sensörünün değerini tutar
int sagArkaMz80;    // Sağ arkadaki mesafe sensörünün değerini tutar
int solArkaMz80;    // Sol arkadaki mesafe sensörünün değerini tutar
int arkaMz80;       // Arkadaki mesafe sensörünün değerini tutar

int hiz = 255;
int yavas = 100;

bool start = true;        // ilk başta zemini algılayacak ve bu değişken bir defalığına bizimRengimiz değişkenine zemin değerini atadıktan sonra false değerini alarak yarışma sonuna kadar değişmeyecek
bool koseneGit = false;   // yumurta sayısı yeteri kadar olduğunda ya da belli bir süre geçtiğinde köşesine gidip yumurtaları bırakmak için tanımlandı
bool ilkHareket = true;   // köşede rengimizi okuduktan sonra false değeri alacak. ilk hareketi vermek için tanımlandı
bool koseyeGit = false;    // robotumuzun köşeye gitmesini istediğimizde true yapılır

String bizimRengimiz;     // arkadaki renk sensörünün okuduğu değer atanacak
String yumurtaRengi;      // ortadaki renk sensörünün okuduğu değer atanacak
String zeminRengi;        // arkadaki renk sensörünün okuduğu değer atanacak,

long rnd;       // RASTGELE DÖNMEK İÇİN TANIMLANDI


// ************************************************************************************
// ****  SETUP ************************************************************************
// ************************************************************************************

void setup() {
  // *************************************
  // RENK SENSÖR AYARLARI
  pinMode(csOrta_s0, OUTPUT);  pinMode(csOrta_s1, OUTPUT);  pinMode(csOrta_s2, OUTPUT);  pinMode(csOrta_s3, OUTPUT);  pinMode(csOrta_out, INPUT);
  pinMode(csArka_s0, OUTPUT);  pinMode(csArka_s1, OUTPUT);  pinMode(csArka_s2, OUTPUT);  pinMode(csArka_s3, OUTPUT);  pinMode(csArka_out, INPUT);
  
  // Arduino için Frekans değerini ayarladık
  digitalWrite(csOrta_s0, HIGH);  digitalWrite(csOrta_s1, LOW);  digitalWrite(csArka_s0, HIGH);  digitalWrite(csArka_s1, LOW);
  //pinMode(kled, OUTPUT);  pinMode(yled, OUTPUT);  pinMode(mled, OUTPUT);

  // **************************************
  // MZ-80 MESAFE SENSÖR AYARLARI
  pinMode(mz80_SagOn, INPUT);     //Kızılotesi sensoru giriş olarak tanımladık
  pinMode(mz80_SolOn, INPUT);     //Kızılotesi sensoru giriş olarak tanımladık
  pinMode(mz80_SagArka, INPUT);   //Kızılotesi sensoru giriş olarak tanımladı
  pinMode(mz80_SolArka, INPUT);   //Kızılotesi sensoru giriş olarak tanımladık
  pinMode(mz80_Arka, INPUT);      //Kızılotesi sensoru giriş olarak tanımladı

  // **************************************
  // SERVO TANIMLARI
  servoOrta.attach(3);    // Orta Servo tanımlanıyor
  servoArka.attach(9);    // Arka Servo tanımlanıyor

  // **************************************
  // DC MOTOR TANIMLARI
  pinMode(enA, OUTPUT);  pinMode(in1, OUTPUT);  pinMode(in2, OUTPUT);   // SOL MOTOR
  pinMode(enB, OUTPUT);  pinMode(in3, OUTPUT);  pinMode(in4, OUTPUT);   // SAĞ MOTOR
      
  Serial.begin(9600);
}


// ************************************************************************************
// ****  LOOP  ************************************************************************
// ************************************************************************************
void loop() {
  // RENK SENSÖRLERİ ZEMİN VE YUMURTA RENKLERİNİ OKUYOR
  //kalibreEt();
  //kalibreEtZemin();
  ortaRenkSensorOku();
  arkaRenkSensorOku();
  yumurtaRengiOku();
  zeminRengiOku();

  delay(100);

  //servoOrtaKalibreEt();
  
  //servoOrtaDon(yumurtaRengi);

  mz80lerden_GelenDegerler();


  // DENEME AMACIYLA YAZILDI
  if (yumurtaSayisi >= 4) {
    dur(hiz);
  } else {

  // HAYDEEE GAZAN MÜBAREK OLSUN
  if(ilkHareket == true) {
    ilkHareket = false;
    delay(1000);
    ileri(hiz);
  } else {

    // TOPLANAN YUMURTA SAYISI 3'Ü GEÇİNCE KÖŞEYE GİTMESİ SAĞLANIYOR
    if (yumurtaSayisi >= 3) {
      koseyeGit = true;
    }
    
    // KÖŞEYE GİTME İŞLEMİ YAPILIYOR
    if (koseyeGit == true) {
      
      // KÖŞE RENGİMİZ BULUNDU. OLEEEEY. ICIH YUMURTLA BAKEM :-)
      if(bizimRengimiz == zeminRengi) {
        yumurtla();
        koseyeGit = false;
      } else {
        ileri(hiz);

        // EĞER ŞANSA KÖŞE RENGİMİZ BULUNURSA. ICIH YUMURTLA BAKEM :-)
        if(bizimRengimiz == zeminRengi) {
          yumurtla();
        }
      }
      
    } else {

      // ENGEL YOK BAS GAZA
      if(sagOnMz80 > 1000 && solOnMz80 > 1000) {
        ileri(yavas);
      } 
      
      // SOLA DÖN
      else if(sagOnMz80 < 1000 || solOnMz80 < 1000 && sagArkaMz80 < 1000) { 
        geri(hiz);
        delay(250);
        solaDon(yavas); 
      } 
      
      // SAĞA DÖN
      else if (sagOnMz80 < 1000 || solOnMz80 < 1000 && solArkaMz80 < 1000) {  
        geri(hiz);
        delay(250);
        sagaDon(yavas);
      } 
      
      // ENGEL VARSA YAVAŞŞŞŞŞŞŞŞŞ, KAFANA GÖRE TAKIL YANİ DÖN
      else {        
        geri(hiz);
        delay(250);

        // RASTGELE DÖN
        rnd = random(0, 50);
        if (rnd % 2 == 0) {
          sagaDon(yavas);}
        else {
          solaDon(yavas);}
      }
    }

    
    
  }
  }

 
  Serial.println("BİZİM RENK : " + bizimRengimiz);
  Serial.println("zemin renk : " + zeminRengi);   
  Serial.println("yumurta renk : " + yumurtaRengi);   
  Serial.println("yumurta SAYISI : " + yumurtaSayisi);   
}

// SICAK VE POHLU YUMURTALAR GELİYOOOOOOOOOOORR
void yumurtla() {
  dur(hiz);
  servoArkaDon(zeminRengi);
  delay(2000);
  ileri(hiz); 
}

// MESAFE SENSÖRLERİNDEN GELEN DEĞERLERİ GLOBAL DEĞİŞKENLERE ATANIYOR
void mz80lerden_GelenDegerler() {
  sagOnMz80 = analogRead(mz80_SagOn);
  solOnMz80 = analogRead(mz80_SolOn);
  sagArkaMz80 = analogRead(mz80_SagArka);
  solArkaMz80 = analogRead(mz80_SolArka);
  arkaMz80 = analogRead(mz80_Arka);
  /*
  Serial.print("Sağ Ön : ");    Serial.println(sagOnMz80);
  Serial.print("Sol Ön : ");    Serial.println(solOnMz80);
  Serial.print("Sağ Arka : ");  Serial.println(sagArkaMz80);
  Serial.print("Sol Arka : ");  Serial.println(solArkaMz80);
  Serial.print("Arka : ");      Serial.println(arkaMz80);
  */
}

// **************************************************************
// MOTOR HAREKETLERİ
// **************************************************************

void ileri(int pwm) {
  sagDcIleri(pwm);
  solDcIleri(pwm);
}

void solaDon(int pwm) {
  solDcGeri(pwm);
  sagDcIleri(pwm);
}

void sagaDon(int pwm){
  sagDcGeri(pwm);
  solDcIleri(pwm);
}

void dur(int pwm) {
  sagDcDur(pwm);
  solDcDur(pwm);
}

void geri(int pwm) {
  sagDcGeri(pwm);
  solDcGeri(pwm);
}

void yavasla(int pwm) {
  ileri(pwm);
}

void hizlan(int pwm) {
  ileri(pwm);
}

void sagDcIleri(int pwm) {
    analogWrite(enB, pwm); // PWM signal L298N ye gönderiliyor
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}
void sagDcGeri(int pwm) {
    analogWrite(enB, pwm); // PWM signal L298N ye gönderiliyor
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}
void sagDcDur(int pwm) {
    analogWrite(enB, pwm); // PWM signal L298N ye gönderiliyor
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);  
}

void solDcIleri(int pwm) {
    analogWrite(enA, pwm); // PWM signal L298N ye gönderiliyor
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
}
void solDcGeri(int pwm) {
    analogWrite(enA, pwm); // PWM signal L298N ye gönderiliyor
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
}
void solDcDur(int pwm) {
    analogWrite(enA, pwm); // PWM signal L298N ye gönderiliyor
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
}





// **************************************************************
// gelen yumurta veya zemin renk değerlerine göre orta servo ayarlanıyor
void servoOrtaDon(String renk) {
  if (renk == "beyaz") {  // düz ol
    degreeOrta = 90;
    servoOrta.write(degreeOrta);
  } else if (bizimRengimiz == renk) {  // sola dön
    degreeOrta = 0;
    servoOrta.write(degreeOrta);
    yumurtaSayisi++;  // toplanlanan kendi yumurta sayımızı arttırıyoruz
  } else {  // sağa dön
    degreeOrta = 180;
    servoOrta.write(degreeOrta);
  }
}

// kendi rengimize geldiğinde açılacak
// rakip veya beyaz renge geldiğinde kapanacak
void servoArkaDon(String renk) {
  if (renk == "beyaz" || renk != bizimRengimiz) {
    degreeArka = 80;
    servoArka.write(degreeArka);
  } else {
    degreeArka = 20;
    servoArka.write(degreeArka);
  }
}

// ************************************************************************************
// RENK SEMSÖRLERİ ZEMİN VE YUMURTA RENKERİNE GÖRE DEĞERLERİ GLOBAL DEĞİŞKENLERE ATIYOR
// ************************************************************************************
void ortaRenkSensorOku() {
  delay(100);
  // Kırmızı rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, LOW);
  yumurtaKirmizi = pulseIn(csOrta_out, LOW);
  yumurtaKirmizi = map(yumurtaKirmizi, 48, 180, 0, 100);
  /*
  Serial.print("Kırmızı renk: ");
  Serial.print(yumurtaKirmizi);
  Serial.print("\t");
  */
  delay(100);

  
  // Yesil rengi belirleme
  digitalWrite(csOrta_s2, HIGH);
  digitalWrite(csOrta_s3, HIGH);
  yumurtaYesil = pulseIn(csOrta_out, LOW);
  yumurtaYesil = map(yumurtaYesil, 43, 210, 0, 100);
  /*
  Serial.print("Yeşil renk: ");
  Serial.print(yumurtaYesil);
  Serial.print("\t");
  */
  delay(100);

  
  // Mavi rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, HIGH);
  yumurtaMavi = pulseIn(csOrta_out, LOW);
  yumurtaMavi = map(yumurtaMavi, 40, 190, 0, 100);
  /*
  Serial.print("Mavi renk: ");
  Serial.print(yumurtaMavi);
  Serial.println("\t");
  */
  delay(100);
}


void arkaRenkSensorOku() {
  delay(50);
  // Kırmızı ZEMİN rengi belirleme
  digitalWrite(csArka_s2, LOW);
  digitalWrite(csArka_s3, LOW);
  zeminKirmizi = pulseIn(csArka_out, LOW);
  zeminKirmizi = map(zeminKirmizi, 48, 180, 0, 100);
  /*
  Serial.print("ZEMİN Kırmızı renk: ");
  Serial.print(zeminKirmizi);
  Serial.print("\t");
  */
  delay(50);

  
  // Yesil ZEMİN rengi belirleme
  digitalWrite(csArka_s2, HIGH);
  digitalWrite(csArka_s3, HIGH);
  zeminYesil = pulseIn(csArka_out, LOW);
  zeminYesil = map(zeminYesil, 43, 210, 0, 100);
  /*
  Serial.print("ZEMİN Yeşil renk: ");
  Serial.print(zeminYesil);
  Serial.print("\t");
  */
  delay(50);

  
  // Mavi ZEMİN rengi belirleme
  digitalWrite(csArka_s2, LOW);
  digitalWrite(csArka_s3, HIGH);
  zeminMavi = pulseIn(csArka_out, LOW);
  zeminMavi = map(zeminMavi, 40, 190, 0, 100);
  /*
  Serial.print("ZEMİN Mavi renk: ");
  Serial.print(zeminMavi);
  Serial.println("\t");
  */
  delay(50);
}

void yumurtaRengiOku() {
  // Orta Sensör beyaz zemin algılanıyor
  if (yumurtaKirmizi < 0 and yumurtaMavi < 0 and yumurtaYesil < 0) {
    yumurtaRengi = "beyaz";
    //Serial.println("Orta Sensör Beyaz zemin algıladı");
  }
  // Orta Sensör kırmızı algılanıyor
  else if (yumurtaKirmizi<90 and yumurtaKirmizi < yumurtaMavi and yumurtaKirmizi < yumurtaYesil) {
    yumurtaRengi = "kirmizi";
    //Serial.println("Orta Sensör Kırmızı yumurta algıladı");
  }
  // Orta Sensör mavi algılanıyor
  else if (yumurtaMavi < yumurtaKirmizi and yumurtaMavi < yumurtaYesil ) {
    yumurtaRengi = "mavi";
    //Serial.println("Orta Sensör Mavi yumurta algıladı");
  }
}

void zeminRengiOku() {
  // Arka Sensör beyaz zemin algılanıyor
  if (zeminKirmizi < 0 and zeminMavi < 0 and zeminYesil < 0) {
    zeminRengi = "beyaz";
    //Serial.println("Arka Sensör Beyaz ZEMİN algıladı : " + bizimRengimiz);
  }
  // Arka Sensör kırmızı zemin algılanıyor
  else if (zeminKirmizi<90 and zeminKirmizi < zeminMavi and zeminKirmizi < zeminYesil) {
    zeminRengi = "kirmizi";
    if (start == true) {  // ilk sefer çalışacak sonra çalışmayacak
      bizimRengimiz = "kirmizi";
      start = false;  
    }
    //Serial.println("Arka Sensör Kırmızı ZEMİN algıladı : " + bizimRengimiz);
  }
  // Arka Sensör mavi zemin algılanıyor
  else if (zeminMavi < zeminKirmizi and zeminMavi < zeminYesil ) {
    zeminRengi = "mavi";
    if (start == true) {  // ilk sefer çalışacak sonra çalışmayacak
      bizimRengimiz = "mavi";
      start = false;  
    }
    //Serial.println("Arka Sensör Mavi ZEMİN algıladı : " + bizimRengimiz);
  }
}







/* ******************** */
/* KALİBRASYON AYARLARI */
/* ******************** */

void servoOrtaKalibreEt() {
  degreeOrta = 90;
  servoOrta.write(degreeOrta);
  servoArka.write(100);
}

void kalibreEt() {
  // Kırmızı rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, LOW);
  yumurtaKirmizi = pulseIn(csOrta_out, LOW);
  Serial.print("Kırmızı renk: ");
  Serial.print(yumurtaKirmizi);
  Serial.print("\t");
  delay(1000);

  
  // Yesil rengi belirleme
  digitalWrite(csOrta_s2, HIGH);
  digitalWrite(csOrta_s3, HIGH);
  yumurtaYesil = pulseIn(csOrta_out, LOW);
  Serial.print("Yeşil renk: ");
  Serial.print(yumurtaYesil);
  Serial.print("\t");
  delay(1000);

  
  // Mavi rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, HIGH);
  yumurtaMavi = pulseIn(csOrta_out, LOW);
  Serial.print("Mavi renk: ");
  Serial.print(yumurtaMavi);
  Serial.println("\t");
  delay(1000);
}


void kalibreEtZemin() {
  // Kırmızı rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, LOW);
  zeminKirmizi = pulseIn(csArka_out, LOW);
  Serial.print("ZEMIN Kırmızı renk: ");
  Serial.print(zeminKirmizi);
  Serial.print("\t");
  delay(1000);

  
  // Yesil rengi belirleme
  digitalWrite(csOrta_s2, HIGH);
  digitalWrite(csOrta_s3, HIGH);
  zeminYesil = pulseIn(csArka_out, LOW);
  Serial.print("ZEMIN Yeşil renk: ");
  Serial.print(zeminYesil);
  Serial.print("\t");
  delay(1000);

  
  // Mavi rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, HIGH);
  zeminMavi = pulseIn(csArka_out, LOW);
  Serial.print("ZEMIN Mavi renk: ");
  Serial.print(zeminMavi);
  Serial.println("\t");
  delay(1000);
}
