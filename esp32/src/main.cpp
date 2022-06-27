
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

const char *ssid = "";                  // Nome Wifi
const char *password = "";              // Senha Wifi
const char *websocket_server_host = ""; // Ip da sua máquina se estiver no modo develop
const int websocket_server_port = 3000; // Porta do servidor

const int LED_BLINK = 2;
const int LED_01 = 13; // Led vermelho
const int LED_02 = 12; // Led branco
const int LED_03 = 27; // Led azul

unsigned long messageInterval = 5000;
#define DEBUG_SERIAL Serial

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{

  switch (type)
  {
  case WStype_DISCONNECTED:
  {
    DEBUG_SERIAL.println("Socket Disconnected!");
    digitalWrite(LED_BLINK, LOW);
  }
  break;
  case WStype_CONNECTED:
  {
    DEBUG_SERIAL.println("Socket Connected!");
    digitalWrite(LED_BLINK, HIGH);

    webSocket.sendTXT("Connected");
  }
  break;
  case WStype_TEXT:
  {

    DEBUG_SERIAL.printf("Server Request: %s\n", payload);

    const char *command = (char *)payload;

    if (strcmp(command, "11") == 0)
    {
      webSocket.sendTXT("Acender Led Vermelho");
      digitalWrite(LED_01, HIGH);
      return;
    }
    if (strcmp(command, "10") == 0)
    {
      webSocket.sendTXT("Apagar Led Vermelho");
      digitalWrite(LED_01, LOW);
      return;
    }
    if (strcmp(command, "21") == 0)
    {
      webSocket.sendTXT("Acender Led Branco");
      digitalWrite(LED_02, HIGH);
      return;
    }
    if (strcmp(command, "20") == 0)
    {
      webSocket.sendTXT("Apagar Led Branco");
      digitalWrite(LED_02, LOW);
      return;
    }
    if (strcmp(command, "31") == 0)
    {
      webSocket.sendTXT("Acender Led Azul");
      digitalWrite(LED_03, HIGH);
      return;
    }
    if (strcmp(command, "30") == 0)
    {
      webSocket.sendTXT("Apagar Led Azul");
      digitalWrite(LED_03, LOW);
      return;
    }
    if (strcmp(command, "x1") == 0)
    {
      webSocket.sendTXT("Acender todos os Leds");
      digitalWrite(LED_01, HIGH);
      digitalWrite(LED_02, HIGH);
      digitalWrite(LED_03, HIGH);
      return;
    }
    if (strcmp(command, "x0") == 0)
    {
      webSocket.sendTXT("Apagar todos os Leds");
      digitalWrite(LED_01, LOW);
      digitalWrite(LED_02, LOW);
      digitalWrite(LED_03, LOW);
      return;
    }
  }
  break;
  case WStype_BIN:
  case WStype_PING:
  case WStype_PONG:
  case WStype_ERROR:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
    break;
  }
}

void setup()
{
  DEBUG_SERIAL.begin(115200);
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println();

  pinMode(LED_BLINK, OUTPUT);
  pinMode(LED_01, OUTPUT);
  pinMode(LED_02, OUTPUT);
  pinMode(LED_03, OUTPUT);

  for (uint8_t t = 5; t > 0; t--)
  {
    DEBUG_SERIAL.printf("Starting at %d...\n", t);
    DEBUG_SERIAL.flush();
    delay(1000);
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    DEBUG_SERIAL.println("Connecting WIFI...");
  }

  // DEBUG_SERIAL.print("Local IP: ");
  // DEBUG_SERIAL.println(WiFi.localIP());

  DEBUG_SERIAL.println("WIFI Connected!");

  webSocket.begin(websocket_server_host, websocket_server_port, "/");
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
}