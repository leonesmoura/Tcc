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
//////////////////////////////////////////


//////////////////////////////////////////
///Definições das variáveis globais//////
#define LAMPADAQ1 5 
#define INTERRUPTORLAMPADAQ1 4
#define VALORLAMPADAQ1 16

#define LAMPADAQ2 14
#define INTERRUPTORLAMPADAQ2 12
#define VALORLAMPADAQ2 13

#define LAMPADAS2 1
#define INTERRUPTORLAMPADAQ2 0
#define VALORLAMPADAQ2 3

int interruptor1=0;
int interruptor2=0;
int interruptor3=0;


const char* ssid = "Leones"; //Login da rede WIFI
const char* password = "18021965"; //Senha da rede WIFI
//Pagina Inicial em html para testes e supervisão
String form = "<html><head><title>Módulo Sala</title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> </head><body><h1>ESP8266 Módulo Lâmpadas QUARTO1 e QUARTO2 </h1> <p>Valores -->    /all</br><p>Lâmpada Quarto 1 -->    /lq1</br> <p>Lâmpada Quarto 2 -->    /lq2 </br></body></html>"; 


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
  WiFi.config(IPAddress(10,0,0,51),
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
    server.on("/lq1", LampadaQ1Function);
    server.on("/lq2", LampadaQ2Function);
    server.on("/ls2", LampadaQ1Function);
 
  //Inicializa o servidor.
  server.begin();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////FUNÇÂO PARA LAMPADAS/////////////////////////////////////////////////////////////
void allFunction(){
  
   server.send(200, "text/plain", ( "lq1:" + String(digitalRead(VALORLAMPADAQ1)) + "\nlq2:" + String(digitalRead(VALORLAMPADAQ2)) + "\nls2:" + String(digitalRead(VALORLAMPADAS2))  )    );
}

void LampadaQ1Function(){

  if(digitalRead(VALORLAMPADAQ1))
  {
    digitalWrite(LAMPADAQ1, LOW);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAQ1)));
    }
    else{
   digitalWrite(LAMPADAQ1, HIGH);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAQ1)));
       }
}

void LampadaQ1Interrupt(){
  delayMicroseconds(100000);//evitar debounce
  static int state = 0;
  state = !digitalRead(VALORLAMPADAQ1);
  digitalWrite(LAMPADAQ1, state);
  detachInterrupt(INTERRUPTORLAMPADAQ1);
  interruptor1=1;

}

void LampadaQ2Function(){

  if(digitalRead(VALORLAMPADAQ2))
  {
    digitalWrite(LAMPADAQ2, LOW);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAQ2)));
    }
    else{
   digitalWrite(LAMPADAQ2, HIGH);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAQ2)));
       }
}

void LampadaQ2Interrupt(){
  delayMicroseconds(100000); //evitar debounce
  static int state = 0;
  state = !digitalRead(VALORLAMPADAQ2);
  digitalWrite(LAMPADAQ2, state);
  detachInterrupt(INTERRUPTORLAMPADAQ2);
  interruptor2=1;
}


void LampadaS2Function(){

  if(digitalRead(VALORLAMPADAS2))
  {
    digitalWrite(LAMPADAS2, LOW);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAS2)));
    }
    else{
   digitalWrite(LAMPADAS2, HIGH);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAS2)));
       }
}

void LampadaS2Interrupt(){
  delayMicroseconds(100000); //evitar debounce
  static int state = 0;
  state = !digitalRead(VALORLAMPADAS2);
  digitalWrite(LAMPADAS2, state);
  detachInterrupt(INTERRUPTORLAMPADAS2);
  interruptor3=1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup() {
//Define portas como entrada ou saída

  pinMode(LAMPADAQ1, OUTPUT);
  pinMode(INTERRUPTORLAMPADAQ1, INPUT);
  pinMode(VALORLAMPADAQ1, INPUT);
  pinMode(LAMPADAQ2, OUTPUT);
  pinMode(INTERRUPTORLAMPADAQ2, INPUT);
  pinMode(VALORLAMPADAQ2, INPUT);
  pinMode(LAMPADAS2, OUTPUT);
  pinMode(INTERRUPTORLAMPADAS2, INPUT);
  pinMode(VALORLAMPADAS2, INPUT);
  

  
  wifiConnect(ssid, password);//Conecta-se a wifi
   delay(1000);
  serverInit(); //Inicia-se o servidor
}

void loop() {
  //Interrupções Externas do ESP, acionadas com mudança de estado
  attachInterrupt(INTERRUPTORLAMPADAQ1, LampadaQ1Interrupt , CHANGE);
  attachInterrupt(INTERRUPTORLAMPADAQ2, LampadaQ2Interrupt , CHANGE);
  attachInterrupt(INTERRUPTORLAMPADAS2, LampadaQ2Interrupt , CHANGE);
   
  
  server.handleClient(); //Deixa o servidor "ouvindo" ações

  if(interruptor1==1){
    //Get no servido para informar que o iterruptor foi acionado
    HTTPClient http; 
    http.begin("http://10.0.0.100:3000/vlq1?vlq1=" + String(digitalRead(VALORLAMPADAQ1)) );  
    http.GET();                                                                             
    interruptor1=0;
    http.end();  
    delay(180); //evitar debounce
  }

  
  if(interruptor2==1){
    //Get no servido para informar que o iterruptor foi acionado
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://10.0.0.100:3000/vlq2?vlq2=" + String(digitalRead(VALORLAMPADAQ2)));  
    http.GET();                                                                             
    interruptor2=0;
    http.end();   
    delay(180); //evitar debounce
  }


  if(interruptor3==1){
    //Get no servido para informar que o iterruptor foi acionado
    HTTPClient http; 
    http.begin("http://10.0.0.100:3000/vls2?vls2=" + String(digitalRead(VALORLAMPADAS2)));  
    http.GET();                                                                            
    interruptor3=0;
    http.end();   
    delay(180); //evitar debounce
  }
}

