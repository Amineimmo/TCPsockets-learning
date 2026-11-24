const express = require("express");
const { WebSocketServer } = require("ws");
const wss = new WebSocketServer({ port: 55555, host: "127.0.0.1" });
const http = require("http");

const app = express();
app.use(express.text({ type: "*/*" })); // accept any body

const server = http.createServer(app);

// browser connects here
wss.on("connection", (ws) => {
  console.log("browser connected");
});

// python POSTs here
app.post("/message", (req, res) => {
  const body = req.body || "(empty)";
  console.log("python sent:", body);

  // forward to every connected browser
  wss.clients.forEach((ws) => {
    if (ws.readyState === 1) ws.send(String(body));
  });

  res.send("ok");
});

server.listen(55555, "127.0.0.1", () => {
  console.log("listening on http://127.0.0.1:55555");
});
