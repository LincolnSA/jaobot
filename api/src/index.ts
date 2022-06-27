import express from "express";
import { createServer } from "http";
import { server } from "websocket";

const port = 3000;
const expressServer = express();

expressServer.use(express.json());

const httpServer = createServer(expressServer);
const websocketServer = new server({ httpServer });

expressServer.post("/webhook", (req, res) => {
  const { intent, parameters } = req.body.queryResult;
  let response = {};

  if (intent.displayName === "direct_command") {
    const { leds, actions } = parameters;

    if (leds && actions) {
      const command = leds + actions;
      websocketServer.broadcast(command);

      const typesResponse = {
        "11": "Ligando led vermelho",
        "10": "Desligando led vermelho",
        "21": "Ligando led branco",
        "20": "Desligando led branco",
        "31": "Ligando led azul",
        "30": "Desligando led azul",
        x1: "Ligando todos os leds",
        x0: "Desligando todos os leds",
      };

      response = {
        fulfillmentMessages: [
          {
            text: {
              text: [typesResponse[command]],
            },
          },
        ],
      };
    }
  }

  return res.send(response);
});

websocketServer.on("request", (request) => {
  const connection = request.accept(null, request.origin);

  connection.on("message", function (message) {
    if (message.type === "utf8") {
      console.log("Socket request: " + message.utf8Data);
    }
  });
});

httpServer.listen(port, () => console.log(`http://localhost:${port}`));
