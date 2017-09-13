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
#define LAMPADAST 5
#define INTERRUPTORLAMPADAST 4
#define VALORLAMPADAST 16

#define LAMPADAB 14
#define INTERRUPTORLAMPADAB 12
#define VALORLAMPADAB 13


#define LEDIR 15

int interruptor1=0;
int interruptor2=0;

//Definindo saída de Sinal IR
IRsend irsend(LEDIR);  



const char* ssid = "Leones"; //Login da rede WIFI
const char* password = "18021965"; //Senha da rede WIFI
//Pagina Inicial em html para testes e supervisão
String form = "<html><head><title>Módulo Suíte</title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> </head><body><h1>ESP8266 Módulo Lâmpadas Suíte, Banheiro e IR  </h1><p> Valores -->    /all</br>Lâmpada Suíte 1 -->    /lst</br>Lâmpada Banheiro -->    /lb </br>Liga/Desliga IR -->    /power</br>Subir canal IR -->    /ch+</br>Descer canal IR -->    /ch-</br>Subir volume IR -->    /v+</br>Descer volume IR -->    /v-</br>Confirma IR -->    /ok</br>Cima IR-->    /up</br>Baixo IR-->    /down</br>Esquerda IR-->    /left</br>Direita IR-->    /right</br>Entradas IR-->    /sorce</br> </p></body></html>"; //Pagina Inicial em html para testes



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
  WiFi.config(IPAddress(10,0,0,70),
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
    server.on("/lst", LampadaSTFunction);
    server.on("/lb", LampadaBFunction);
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
   server.send(200, "text/plain", ( "lst:" + String(digitalRead(VALORLAMPADAST)) + "\nlb:" + String(digitalRead(VALORLAMPADAB))    )    );
}

void PowerIR(){
  irsend.sendNEC(0xE0E040BF, 32);
  server.send(200, "text/plain", "Power");
  }

void VupIR(){
  irsend.sendNEC(0xE0E0E01F, 32);
   server.send(200, "text/plain", "V+");
}


void VdownIR(){
  irsend.sendNEC(0xE0E0D02F, 32);
  server.send(200, "text/plain", "V-");
}

void CHupIR(){
  irsend.sendNEC(0xFF609F, 32);
  server.send(200, "text/plain", "CH+");
}


void CHdownIR(){
  irsend.sendNEC(0xFF6897, 32);
  server.send(200, "text/plain", "CH-");
}

void UPIR(){
  irsend.sendNEC(0xE0E006F9, 32);
  server.send(200, "text/plain", "UP");
}


void DOWNIR(){
  irsend.sendNEC(0xE0E08679, 32);
  server.send(200, "text/plain", "DOWN");
}


void LEFTIR(){
  irsend.sendNEC(0xE0E0A659, 32);
  server.send(200, "text/plain", "LEFT");
}


void RIGHTIR(){
  irsend.sendNEC(0xE0E046B9, 32);
  server.send(200, "text/plain", "RIGHT");
}


void OKIR(){
  irsend.sendNEC(0xE0E016E9, 32);
  server.send(200, "text/plain", "OK");
}


void SORCEIR(){
  irsend.sendNEC(0xE0E0807F, 32);
  server.send(200, "text/plain", "SORCE");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////FUNÇÕES PARA LAMPADAS/////////////////////////////////////////////////////////////

void LampadaSTFunction(){

  if(digitalRead(VALORLAMPADAST))
  {
    digitalWrite(LAMPADAST, LOW);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAST)));
    }
    else{
   digitalWrite(LAMPADAST, HIGH);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAST)));
       }
       
}

void LampadaSTInterrupt(){
  delayMicroseconds(100000);//evitar debounce
  static int state = 0;
  state = !digitalRead(VALORLAMPADAST);
  digitalWrite(LAMPADAST, state);
  detachInterrupt(INTERRUPTORLAMPADAST);
  interruptor1=1;
  
}

void LampadaBFunction(){

  if(digitalRead(VALORLAMPADAB))
  {
    digitalWrite(LAMPADAB, LOW);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAB)));
    }
    else{
   digitalWrite(LAMPADAB, HIGH);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAB)));
       }
     
}

void LampadaBInterrupt(){
  delayMicroseconds(100000);//evitar debounce
  static int state = 0;
  state = !digitalRead(VALORLAMPADAB);
  digitalWrite(LAMPADAB, state);
  detachInterrupt(INTERRUPTORLAMPADAB);
interruptor2=1;

}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup() {


//Define portas como entrada ou saída
  pinMode(LAMPADAST, OUTPUT);
  pinMode(INTERRUPTORLAMPADAST, INPUT);
  pinMode(VALORLAMPADAST, INPUT);
  
  pinMode(LAMPADAB, OUTPUT);
  pinMode(INTERRUPTORLAMPADAB, INPUT);
  pinMode(VALORLAMPADAB, INPUT);

   irsend.begin();
   wifiConnect(ssid, password);//Conecta-se a wifi
   delay(1000);
   serverInit(); //Inicia-se o servidor
}

void loop() {
  //Interrupções Externas do ESP, acionadas com mudança de estado
  attachInterrupt(INTERRUPTORLAMPADAST, LampadaSTInterrupt , CHANGE);
  attachInterrupt(INTERRUPTORLAMPADAB, LampadaBInterrupt , CHANGE);

  server.handleClient(); //Deixa o servidor "ouvindo" ações

  
  
  if(interruptor1==1){
     //Get no servido para informar que o iterruptor foi acionado
    HTTPClient http;  
    http.begin("http://10.0.0.100:3000/vlst?vlst=" + String(digitalRead(VALORLAMPADAST)) ); 
    http.GET();                                                            
    interruptor1=0;
    http.end();   //Close connection
  }

  
  if(interruptor2==1){
     //Get no servido para informar que o iterruptor foi acionado
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://10.0.0.100:3000/vlb?vlb=" + String(digitalRead(VALORLAMPADAB)));  
    http.GET();                                                               
    interruptor2=0;
    http.end();   //Close connection
  }


}

