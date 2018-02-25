/*
 *  Created by TheCircuit
*/
        #include <FirebaseArduino.h>
        #define SS_PIN 4  //D2
        #define RST_PIN 5 //D1
        #define LEDGREEN 15 //d8
        #define ledred 0 //d3
        #define rrelay 16 //D0
        
        #include <SPI.h>
        #include <MFRC522.h>
        #include <ESP8266WiFi.h>

              #define FIREBASE_HOST "esp8266-6e172.firebaseio.com"
              #define FIREBASE_AUTH "xp4NMecBicPMawr64mBY0pNjnpAUJuC4chJGjpDJ"
        
            MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

            int out = 0;
            #define WIFI_SSID "DekBanBan"
            #define WIFI_PASSWORD "sinptv_1999"

            const String UserCardNumber = "User";
      
void setup() 
{
            Serial.begin(115200);   // Initiate a serial communication
            SPI.begin();      // Initiate  SPI bus
            mfrc522.PCD_Init();   // Initiate MFRC522
            pinMode(LEDGREEN,OUTPUT);
            pinMode(ledred,OUTPUT);
            pinMode(rrelay,OUTPUT);
          
            Wificonnection();
            Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
            Firebase.remove(UserCardNumber);
           //  Firebase.setString(UserCardNumber + "/name", CardName);
}
void loop() 
{
      digitalWrite(rrelay,HIGH);
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
        static int Count = 0;
  if (content.substring(1) == "F9 11 7D 9E") //change UID of the card that you want to give access
  {       
         const String UserCardNumber = "User";
         const String CardName = " piti " + UserCardNumber;
          int ID = 0;
          int value =0;
          const String Address = " Facuty of Engineering ";
          if (value == 0){
                      
              StaticJsonBuffer<200> jsonBuffer;
              JsonObject& valueObject = jsonBuffer.createObject();
              valueObject["Name"] = CardName;
              valueObject["ID"] = ID;
              valueObject["Address"] = Address;
            //  Firebase.set(UserCardNumber + "/001", valueObject);
             Firebase.push(UserCardNumber + "/001", valueObject);

            
         /* Firebase.setString(UserCardNumber + "/Address: ", Address);
              Firebase.setString(UserCardNumber + "/Name", CardName);
               Firebase.setInt(UserCardNumber + "/ID : ", ID);*/
              
             Serial.println(" Card Access ");
              Serial.println(" Welcome Mr.Piti ");
              Serial.println(" Have FUN ");
              Serial.println();
              digitalWrite(rrelay,LOW);
              digitalWrite(LEDGREEN,HIGH);
              delay(1500);
              digitalWrite(LEDGREEN,LOW);
              delay(5000);
              digitalWrite(rrelay,HIGH);
                   

         
      }
  }

  else if (content.substring(1) == "65 C3 33 3B") //change UID of the card that you want to give access
  {       
         const String UserCardNumber = "User";
         const String CardName = " sinkerd " + UserCardNumber;
          int ID = 1;
          int value =0;
          const String Address = " Facuty of Engineering ";
          if (value == 0){
            
             StaticJsonBuffer<200> jsonBuffer;
              JsonObject& valueObject = jsonBuffer.createObject();
              valueObject["Name"] = CardName;
              valueObject["ID"] = ID;
              valueObject["Address"] = Address;
            //  Firebase.set(UserCardNumber + "/001", valueObject);
             Firebase.push(UserCardNumber + "/002", valueObject);
            
         /* Firebase.setString(UserCardNumber + "/Address: ", Address);
              Firebase.setString(UserCardNumber + "/Name", CardName);
               Firebase.setInt(UserCardNumber + "/ID : ", ID);*/
              
             Serial.println(" Card access ");
              Serial.println(" Welcome Mr.Sinkerd ");
              Serial.println(" Have FUN ");
              Serial.println();
              digitalWrite(LEDGREEN,HIGH);
               digitalWrite(rrelay,LOW);
              delay(1500);
              digitalWrite(LEDGREEN,LOW);
               delay(5000);
              digitalWrite(rrelay,HIGH);
                   
          
             

         
      }
  }
  else {
          Serial.println("Card Not Access");
            digitalWrite(ledred,HIGH);
             delay(1000);
          digitalWrite(ledred,LOW);
          const String UserCardNumber = "User";
        const String Empty = " User Out of system ";

        StaticJsonBuffer<200> jsonBuffer;
              JsonObject& Emptys = jsonBuffer.createObject();
              Emptys ["Name"] = Empty;
       Firebase.push(UserCardNumber + "/No USER: ", Emptys);
       
  }
}
 
  

 
      
      
      void Wificonnection(){
          WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
          Serial.print("Connecting");
          while(WiFi.status()!=WL_CONNECTED)
          {
            Serial.print(".");
            delay(500);
            
          }
          Serial.println();
          Serial.print("Connected: ");
          Serial.println(WiFi.localIP());
          Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
          
      }
