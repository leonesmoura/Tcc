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
#define LAMPADAC 5 
#define INTERRUPTORLAMPADAC 4
#define VALORLAMPADAC 16

#define LAMPADAAS 14
#define INTERRUPTORLAMPADAAS 12
#define VALORLAMPADAAS 13

#define VALVULA 0

int interruptor1=0;
int interruptor2=0;

double gas=0;


const char* ssid = "Leones"; //Login da rede WIFI
const char* password = "18021965"; //Senha da rede WIFI
//Pagina Inicial em html para testes e supervisão
String form = "<html><head><title>Módulo Cozinha</title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> </head><body><h1>ESP8266 Módulo Lâmpadas Cozinha e Área de Serviço (Gas e Válvula) </h1> <p>Valores -->    /all</br><p>Lâmpada Cozinha -->    /lc</br> <p>Lâmpada Área de Serviço -->    /las </br><p>Sensor de Gás -->    /sg </br><p>Válvula Solenoide -->    /val </br></body></html>"; 


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
  WiFi.config(IPAddress(10,0,0,65),
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
    server.on("/lc", LampadaCFunction);
    server.on("/las", LampadaASFunction);
    server.on("/val", ValvulaFunction);
    server.on("/sg", GasFunction);
 
  //Inicializa o servidor.
  server.begin();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////FUNÇÕES PARA EVENTOS/////////////////////////////////////////////////////////////
void allFunction(){
  
   server.send(200, "text/plain", ( "lc:" + String(digitalRead(VALORLAMPADAC)) + "\nlas:" + String(digitalRead(VALORLAMPADAAS))  ));
}

void ValvulaFunction(){
  server.send(200, "text/plain", "Liberado por 2 segundos");
  digitalWrite(VALVULA, HIGH);
  delay(2000);
  digitalWrite(VALVULA, LOW);
  
}


void GasFunction(){
  gas=(100*analogRead(0))/1024; // Modelo com saída em percentegem de inflamibilidade 
  server.send(200, "text/plain", String(gas));
}

void LampadaCFunction(){

  if(digitalRead(VALORLAMPADAC))
  {
    digitalWrite(LAMPADAC, LOW);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAC)));
    }
    else{
   digitalWrite(LAMPADAC, HIGH);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAC)));
       }
}

void LampadaCInterrupt(){
  delayMicroseconds(100000);//evitar debounce
  static int state = 0;
  state = !digitalRead(VALORLAMPADAC);
  digitalWrite(LAMPADAC, state);
  detachInterrupt(INTERRUPTORLAMPADAC);
  interruptor1=1;

}

void LampadaASFunction(){

  if(digitalRead(VALORLAMPADAAS))
  {
    digitalWrite(LAMPADAAS, LOW);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAAS)));
    }
    else{
   digitalWrite(LAMPADAAS, HIGH);
   server.send(200, "text/plain", String(digitalRead(VALORLAMPADAAS)));
       }
}

void LampadaASInterrupt(){
  delayMicroseconds(100000); //evitar debounce
  static int state = 0;
  state = !digitalRead(VALORLAMPADAAS);
  digitalWrite(LAMPADAAS, state);
  detachInterrupt(INTERRUPTORLAMPADAAS);
  interruptor2=1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup() {
//Define portas como entrada ou saída

  pinMode(LAMPADAC, OUTPUT);
  pinMode(INTERRUPTORLAMPADAC, INPUT);
  pinMode(VALORLAMPADAC, INPUT);
  pinMode(LAMPADAAS, OUTPUT);
  pinMode(INTERRUPTORLAMPADAAS, INPUT);
  pinMode(VALORLAMPADAAS, INPUT);
  pinMode(VALVULA, OUTPUT);
  

  
  wifiConnect(ssid, password);//Conecta-se a wifi
   delay(1000);
  serverInit(); //Inicia-se o servidor
}

void loop() {
  //Interrupções Externas do ESP, acionadas com mudança de estado
  attachInterrupt(INTERRUPTORLAMPADAC, LampadaCInterrupt , CHANGE);
  attachInterrupt(INTERRUPTORLAMPADAAS, LampadaASInterrupt , CHANGE);
  
  server.handleClient(); //Deixa o servidor "ouvindo" ações

  if(interruptor1==1){
    //Get no servido para informar que o iterruptor foi acionado
    HTTPClient http; 
    http.begin("http://10.0.0.100:3000/vlc?vlc=" + String(digitalRead(VALORLAMPADAC)) );  
    http.GET();                                                                             
    interruptor1=0;
    http.end();  
    delay(180); //evitar debounce
  }

  
  if(interruptor2==1){
    //Get no servido para informar que o iterruptor foi acionado
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://10.0.0.100:3000/vlas?vlas=" + String(digitalRead(VALORLAMPADAAS)));  
    http.GET();                                                                             
    interruptor2=0;
    http.end();   
    delay(180); //evitar debounce
  }


}

