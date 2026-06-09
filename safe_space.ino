#include <WiFi.h>
#include <WebServer.h>

#define PIN_PRESSAO 35

WebServer server(80);

int pressaoADC = 0;
String estado = "Inicializando";

// Converte ADC (0-4095) para pressão simulada (80-120 kPa)
float adcToPressao(int valorADC) {
  return 80.0 + ((float)valorADC / 4095.0) * 40.0;
}

// Regras obtidas a partir do treinamento no Edge Impulse
String classificar(int leituraADC) {

  if (leituraADC <= 1300) {
    return "Seguro";
  }

  if (leituraADC <= 2700) {
    return "Alerta";
  }

  return "Critico";
}

void handleRoot() {

  String cor;
  String acao;

  if (estado == "Seguro") {

    cor = "#27ae60";
    acao = "Habitat operando normalmente.";

  } else if (estado == "Alerta") {

    cor = "#f39c12";
    acao = "Verificar pressao do habitat.";

  } else {

    cor = "#e74c3c";
    acao = "RISCO DE DESPRESSURIZACAO!";
  }

  String html = R"(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta http-equiv="refresh" content="2">
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>SafeSpace</title>

<style>

body{
font-family:Arial,sans-serif;
background:#0d1117;
color:white;
padding:20px;
text-align:center;
}

h1{
color:#58a6ff;
}

.status{
padding:20px;
border-radius:12px;
margin:20px 0;
font-size:32px;
font-weight:bold;
color:white;
}
</style>

</head>
<body>

<h1>SafeSpace</h1>

<p>Guardiao de Habitats Extremos - Setor A7</p>

<div class="status" style="background-color:
)";

  html += cor;

  html += R"(
;">
)";

  html += estado;

  html += R"(
</div>

<p>)";

  html += acao;

  html += R"(</p>

<hr>

<h2>Dados do Habitat</h2>

<p><strong>Pressao:</strong> )";

  html += String(adcToPressao(pressaoADC), 1);

  html += R"( kPa</p>

<p><strong>Leitura ADC:</strong> )";

  html += String(pressaoADC);

  html += R"(</p>

<p><strong>Setor:</strong> A7</p>

</body>
</html>
)";

  server.send(200, "text/html", html);
}

void setup() {

  Serial.begin(115200);

  WiFi.softAP("SafeSpace-Lunar", "12345678");

  Serial.println();
  Serial.println("=== SAFE SPACE ===");
  Serial.println("Access Point criado");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);

  server.begin();

  Serial.println("Servidor iniciado");
}

void loop() {

  pressaoADC = analogRead(PIN_PRESSAO);

  estado = classificar(pressaoADC);

  Serial.printf(
    "Estado: %s | Pressao: %.1f kPa | ADC: %d\n",
    estado.c_str(),
    adcToPressao(pressaoADC),
    pressaoADC
  );

  server.handleClient();

  delay(1000);
}
