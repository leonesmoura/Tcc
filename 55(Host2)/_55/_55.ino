/**
 *--------------------------------------------------------
 * Autor : Leones Moura  
 * Trabalho de Conclusão de Curso em Engenharia Eletrônica
 * Tema: Automação Residencial Utilizando IoT
 * -------------------------------------------------------
 */
///////////////////////////////////////////
/////////Importa as bibliotecas///////////
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>
//////////////////////////////////////////


//////////////////////////////////////////
///Definições das variáveis globais//////
#define LAMPADAS1 5
#define INTERRUPTORLAMPADAS1 4
#define VALORLAMPADAS1 16

#define LAMPADAV 14
#define INTERRUPTORLAMPADAV 12
#define VALORLAMPADAV 13


#define LEDIR 15

int interruptor1=0;
int interruptor2=0;

//Definindo saída de Sinal IR
IRsend irsend(LEDIR);  



const char* ssid = "Leones"; //Login da rede WIFI
const char* password = "18021965"; //Senha da rede WIFI
//Pagina Inicial em html para testes e supervisão
String form = "<html><head><title>Módulo Sala</title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> </head><body><h1>ESP8266 Módulo Lâmpadas SALA1, SALA2 e IR  </h1><p> Valores -->    /all</br>Lâmpada Sala 1 -->    /ls1</br>Lâmpada Varanda -->    /lv </br>Liga/Desliga IR -->    /power</br>Subir canal IR -->    /ch+</br>Descer canal IR -->    /ch-</br>Subir volume IR -->    /v+</br>Descer volume IR -->    /v-</br>Confirma IR -->    /ok</br>Cima IR-->    /up</br>Baixo IR-->    /down</br>Esquerda IR-->    /left</br>Direita IR-->    /right</br>Entradas IR-->    /sorce</br> </p></body></html>"; //Pagina Inicial em html para testes



////////////////////////////////////////



ESP8266WebServer server(80); //Define servidor na porta padrão http (80)



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////FUNÇÂO PARA CONECTAR A REDE WIFI/////////////////////////////////////////////////////////////
void wifiConnect(const char* ssid, const char* password){
  int WiFiCounter = 0;

  //Desconecta das wifi's e atualiza o modo para Station (conectado a uma rede existente)
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); //Define o modo Station


  //Inicializa a wifi com a rede e senha
  WiFi.begin(ssid, password);
  //Define um ip fixo junto com a mascara de sub-rede e gateway
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(IPAddress(10,0,0,55),
              IPAddress(10,0,0,1), subnet);
  //Aguarda conectar-se
while (WiFi.status() != WL_CONNECTED && WiFiCounter < 30) {
    delay(1000);
    WiFiCounter++;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////FUNÇÂO PARA INICIAR O SERVIDOR/////////////////////////////////////////////////////////////
void serverInit(){
//Coloca o servidor para ouvir a página inicial
  server.on("/", [](){
   server.send(200, "text/html", form);
  });
  //Define caminhos para chamar eventos
    server.on("/all", allFunction);
    server.on("/ls1", LampadaS1Function);
    server.on("/lv", LampadaVFunction);
    server.on("/power", PowerIR );
    server.on("/v+", VupIR);
    server.on("/v-", VdownIR);
    server.on("/ch+", CHupIR);
    server.on("/ch-", CHdownIR);
    server.on("/up", UPIR);
    server.on("/down", DOWNIR);
    server.on("/left", LEFTIR);
    server.on("/right", RIGHTIR);
    server.on("/ok", OKIR);
    server.on("/sorce", SORCEIR);  

  //Inicializa o servidor.
  server.begin();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////FUNÇÕES PARA IR/////////////////////////////////////////////////////////////

void allFunction(){
   server.send(200, "text/plain", ( "ls1:" + String(digitalRead(VALORLAMPADAS1)) + "\nlv:" + String(digitalRead(VALORLAMPADAV))    )    );
}

void PowerIR(){
  irsend.sendNEC(0xFDC03F, 32);
  server.send(200, "text/plain", "Power");
  }

void VupIR(){
  irsend.sendNEC(0xFD6897, 32);
   server.send(200, "text/plain", "V+");
}


void VdownIR(){
  irsend.sendNEC(0xFD58A7, 32);
  server.send(200, "text/plain", "V-");
}

void CHupIR(){
  irsend.sendNEC(0xFD28D7, 32);
  server.send(200, "text/plain", "CH+");
}


void CHdownIR(){
  irsend.sendNEC(0xFD18E7, 32);
  server.send(200, "text/plain", "CH-");
}

void UPIR(){
  irsend.sendNEC(0xFDB847, 32);
  server.send(200, "text/plain", "UP");
}


void DOWNIR(){
  irsend.sendNEC(0xFDA25D, 32);
  server.send(200, "text/plain", "DOWN");
}


void LEFTIR(){
  irsend.sendNEC(0xFD02FD, 32);
  server.send(200, "text/plain", "LEFT");
}


void RIGHTIR(){
  irsend.sendNEC(0xFD42BD, 32);
  server.send(200, "text/plain", "RIGHT");
}


void OKIR(){
  irsend.sendNEC(0xFD827D, 32);
  server.send(200, "text/plain", "OK");
}


void SORCEIR(){
  irsend.sendNEC(0xFD7887, 32);
  server.send(200, "text/plain", "SORCE");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////FUNÇÕES PARA LAMPADAS/////////////////////////////////////////////////////////////

void LampadaS1Function(){

  if(digitalRead(VALORLAMPADAS1))
  {
    digitalWrite(LAMPADAS1, LOW);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAS1)));
    }
    else{
   digitalWrite(LAMPADAS1, HIGH);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAS1)));
       }
       
}

void LampadaS1Interrupt(){
  delayMicroseconds(100000);//evitar debounce
  static int state = 0;
  state = !digitalRead(VALORLAMPADAS1);
  digitalWrite(LAMPADAS1, state);
  detachInterrupt(INTERRUPTORLAMPADAS1);
  interruptor1=1;
  
}

void LampadaVFunction(){

  if(digitalRead(VALORLAMPADAV))
  {
    digitalWrite(LAMPADAV, LOW);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAV)));
    }
    else{
   digitalWrite(LAMPADAV, HIGH);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAV)));
       }
     
}

void LampadaVInterrupt(){
  delayMicroseconds(100000);//evitar debounce
  static int state = 0;
  state = !digitalRead(VALORLAMPADAV);
  digitalWrite(LAMPADAV, state);
  detachInterrupt(INTERRUPTORLAMPADAV);
interruptor2=1;

}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup() {


//Define portas como entrada ou saída
  pinMode(LAMPADAS1, OUTPUT);
  pinMode(INTERRUPTORLAMPADAS1, INPUT);
  pinMode(VALORLAMPADAS1, INPUT);
  
  pinMode(LAMPADAV, OUTPUT);
  pinMode(INTERRUPTORLAMPADAV, INPUT);
  pinMode(VALORLAMPADAV, INPUT);

   irsend.begin();
   wifiConnect(ssid, password);//Conecta-se a wifi
   delay(1000);
   serverInit(); //Inicia-se o servidor
}

void loop() {
  //Interrupções Externas do ESP, acionadas com mudança de estado
  attachInterrupt(INTERRUPTORLAMPADAS1, LampadaS1Interrupt , CHANGE);
  attachInterrupt(INTERRUPTORLAMPADAV, LampadaVInterrupt , CHANGE);

  server.handleClient(); //Deixa o servidor "ouvindo" ações

  
  
  if(interruptor1==1){
     //Get no servido para informar que o iterruptor foi acionado
    HTTPClient http;  
    http.begin("http://10.0.0.100:3000/vls1?vls1=" + String(digitalRead(VALORLAMPADAS1)) ); 
    http.GET();                                                            
    interruptor1=0;
    http.end();   //Close connection
  }

  
  if(interruptor2==1){
     //Get no servido para informar que o iterruptor foi acionado
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://10.0.0.100:3000/vlv?vlv=" + String(digitalRead(VALORLAMPADAV)));  
    http.GET();                                                               
    interruptor2=0;
    http.end();   //Close connection
  }


}

