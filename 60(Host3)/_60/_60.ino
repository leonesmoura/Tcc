/**
  --------------------------------------------------------
   Autor : Leones Moura
   Trabalho de Conclusão de Curso em Engenharia Eletrônica
   Tema: Automação Residencial Utilizando IoT
   -------------------------------------------------------
*/
///////////////////////////////////////////
/////////Importa as bibliotecas///////////
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
//////////////////////////////////////////


//////////////////////////////////////////
///Definições das variáveis globais//////
#define TOMADAQ1 16
#define VALORTOMADAQ1 5

#define TOMADAQ2 4
#define VALORTOMADAQ2 0

#define TOMADAST 14
#define VALORTOMADAST 12



const char* ssid = "Leones"; //Login da rede WIFI
const char* password = "18021965"; //Senha da rede WIFI
String form = "<html><head><title>Módulo Tomadas</title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> </head><body><h1>ESP8266 Módulo Tomadas QUARTO1, QUARTO2 e SUÍTE </h1> <p>all -->    /all</br> <p>Tomada Quarto 1 -->    /tq1</br> <p>Tomada Quarto 2 -->    /tq2 </br> <p>Tomada Suíte-->    /tst </br></body></html>"; //Pagina Inicial em html para testes


////////////////////////////////////////



ESP8266WebServer server(80); //Inicializando servidor na porta 80, padrão do (HTTP)



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////FUNÇÂO PARA CONECTAR A REDE WIFI/////////////////////////////////////////////////////////////
void wifiConnect(const char* ssid, const char* password) {
  int WiFiCounter = 0;

  //Desconecta das wifi's e atualiza o modo para Station (conectado a uma rede existente)
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);

  //Inicializa a wifi com a rede e senha
  WiFi.begin(ssid, password);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(IPAddress(10, 0, 0, 60),
              IPAddress(10, 0, 0, 1), subnet);

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
void serverInit() {
  //Coloca o servidor para ouvir a página inicial
  server.on("/", []() {
    server.send(200, "text/html", form);
  });
  server.on("/all", allFunction);
  server.on("/tq1", TomadaQ1Function);
  server.on("/tq2", TomadaQ2Function);
  server.on("/tst", TomadaSTFunction);

  //Inicializa o servidor.
  server.begin();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////FUNÇÂO PARA TOMADAS/////////////////////////////////////////////////////////////
void allFunction(){
  
   server.send(200, "text/plain", ( "tq1:" + String(digitalRead(VALORTOMADAQ1)) + "\ntq2:" + String(digitalRead(VALORTOMADAQ2)) + "\ntst:" + String(digitalRead(VALORTOMADAST))  )    );
}


void TomadaQ1Function() {

  if (digitalRead(VALORTOMADAQ1))
  {
    digitalWrite(TOMADAQ1, LOW);
    server.send(200, "text/plain", String(digitalRead(VALORTOMADAQ1)));
  }
  else {
    digitalWrite(TOMADAQ1, HIGH);
    server.send(200, "text/plain", String(digitalRead(VALORTOMADAQ1)));
  }

}

void TomadaQ2Function() {

  if (digitalRead(VALORTOMADAQ2))
  {
    digitalWrite(TOMADAQ2, LOW);
    server.send(200, "text/plain", String(digitalRead(VALORTOMADAQ2)));
  }
  else {
    digitalWrite(TOMADAQ2, HIGH);
    server.send(200, "text/plain", String(digitalRead(VALORTOMADAQ2)));
  }

}

void TomadaSTFunction() {

  if (digitalRead(VALORTOMADAST))
  {
    digitalWrite(TOMADAST, LOW);
    server.send(200, "text/plain", String(digitalRead(VALORTOMADAST)));
  }
  else {
    digitalWrite(TOMADAST, HIGH);
    server.send(200, "text/plain", String(digitalRead(VALORTOMADAST)));
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup() {


  pinMode(TOMADAQ1, OUTPUT);
  pinMode(VALORTOMADAQ1, INPUT);
  pinMode(TOMADAQ2, OUTPUT);
  pinMode(VALORTOMADAQ2, INPUT);
  pinMode(TOMADAST, OUTPUT);
  pinMode(VALORTOMADAST, INPUT);
  



  wifiConnect(ssid, password);//Conecta-se a wifi
  delay(1000);
  serverInit(); //Inicia-se o servidor
}

void loop() {
  server.handleClient(); //Deixa o servidor "ouvindo" ações no servidor
}

