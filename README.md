# SafeSpace – Guardião de Habitats Extremos

## Visão Geral

O SafeSpace é uma solução de monitoramento para habitats espaciais extremos, como colônias na Lua ou em Marte. O objetivo é identificar rapidamente possíveis anomalias atmosféricas que possam comprometer a segurança dos ocupantes.

A solução utiliza um ESP32 conectado a um sensor simulado (potenciômetro no Wokwi) para representar variações de pressão atmosférica dentro do habitat. Os dados coletados são utilizados para treinar um modelo de classificação no Edge Impulse. Posteriormente, as regras aprendidas pelo modelo são convertidas para estruturas condicionais (`if/else`) executadas diretamente no ESP32.

O status do ambiente é disponibilizado em tempo real por meio de um Web Server embarcado, permitindo que qualquer astronauta conectado à rede local visualize a situação atual do habitat.

---

# Objetivo

Desenvolver um sistema embarcado capaz de classificar o ambiente interno de uma colônia espacial em três estados:

* Seguro
* Alerta
* Crítico

A classificação deve ocorrer localmente no ESP32, sem depender de processamento em nuvem.

---

# Alinhamento com a ODS 11

## ODS 11 – Cidades e Comunidades Sustentáveis

O projeto contribui para a criação de habitats resilientes e seguros em ambientes extremos, promovendo:

* Monitoramento contínuo das condições ambientais.
* Detecção rápida de situações de risco.
* Redução da dependência de infraestrutura externa.
* Maior confiabilidade operacional por meio de processamento local.

---

# Arquitetura da Solução

```text
Potenciometro
(Simulacao de Pressao)
          |
          v
        ESP32
          |
          v
  Classificacao Local
      (TinyML)
          |
          v
      Web Server
          |
          v
 Astronauta acessa
  via navegador
```

---

# Tecnologias Utilizadas

## Hardware

* ESP32
* Potenciômetro (simulando pressão atmosférica)

## Software

* Wokwi
* Edge Impulse
* Arduino IDE / ESP32 Framework
* HTML
* CSS
* Wi-Fi Access Point

---

# Engenharia de Dados

## Coleta dos Dados

Os dados foram gerados utilizando o simulador Wokwi.

O potenciômetro foi utilizado para representar diferentes níveis de pressão atmosférica dentro do habitat.

As leituras foram obtidas através da função:

```cpp
analogRead(GPIO34)
```

gerando valores entre:

```text
0 e 4095
```

---

## Classes Definidas

### Seguro

Faixa:

```text
0 - 1300
```

Representa pressão dentro dos níveis aceitáveis.

---

### Alerta

Faixa:

```text
1301 - 2700
```

Representa uma possível anomalia que exige monitoramento.

---

### Crítico

Faixa:

```text
2701 - 4095
```

Representa risco elevado de falha atmosférica ou despressurização.

---

## Dataset

Foram gerados três conjuntos de dados:

```text
seguro.csv
alerta.csv
critico.csv
```

Cada arquivo contém amostras de leitura do sensor dentro de sua respectiva faixa operacional.

Exemplo:

```csv
timestamp,pressao
0,450
1,520
2,610
3,780
4,1200
```

---

# TinyML com Edge Impulse

## Importação dos Dados

Os arquivos CSV foram importados para o Edge Impulse utilizando as labels:

* seguro
* alerta
* critico

---

## Configuração do Impulse

Estrutura utilizada:

```text
Input
  |
Raw Data
  |
Classification
```

---

## Treinamento

Configuração:

```text
Epochs: 50
```

O modelo foi treinado para identificar padrões correspondentes aos três estados do habitat.

---

## Inferência Embarcada

Em vez de exportar a biblioteca completa do Edge Impulse, as regras aprendidas foram convertidas para lógica condicional.

Exemplo:

```cpp
if (leituraADC <= 1300)
{
    estado = "Seguro";
}
else if (leituraADC <= 2700)
{
    estado = "Alerta";
}
else
{
    estado = "Critico";
}
```

Essa abordagem reduz o consumo de memória e atende aos requisitos da atividade.

---

# Web Server ESP32

O ESP32 atua como Access Point próprio.

Configuração:

```text
SSID: SafeSpace-Lunar
Senha: 12345678
```

Ao acessar o endereço IP do ESP32, o usuário visualiza:

* Estado atual do habitat
* Pressão simulada
* Leitura ADC
* Setor monitorado
* Recomendações de ação

---

# Estados Operacionais

## Seguro

Condição:

```text
ADC <= 1300
```

Mensagem:

```text
Habitat operando normalmente.
```

---

## Alerta

Condição:

```text
1301 <= ADC <= 2700
```

Mensagem:

```text
Verificar pressao do habitat.
```

---

## Crítico

Condição:

```text
ADC > 2700
```

Mensagem:

```text
RISCO DE DESPRESSURIZACAO!
```

---

# Fluxo de Funcionamento

1. O potenciômetro gera uma leitura simulando a pressão do habitat.
2. O ESP32 realiza a leitura analógica.
3. A leitura é classificada localmente.
4. O estado é atualizado.
5. O Web Server publica as informações.
6. O astronauta acessa o status via navegador.

---

# Resultados Esperados

| Leitura ADC | Estado  |
| ----------- | ------- |
| 0 – 1300    | Seguro  |
| 1301 – 2700 | Alerta  |
| 2701 – 4095 | Crítico |

---

# Possíveis Evoluções

* Utilização de sensores reais de pressão atmosférica.
* Integração com sensores de temperatura e radiação.
* Registro histórico das medições.
* Sistema de alarmes sonoros.
* Dashboard remoto em nuvem.
* Comunicação entre múltiplos módulos de habitat.

---

# Conclusão

O SafeSpace demonstra a aplicação de TinyML em sistemas embarcados para monitoramento de habitats extremos. A solução realiza classificação local em tempo real utilizando um ESP32, reduzindo dependência de infraestrutura externa e aumentando a confiabilidade operacional em cenários críticos como colônias lunares e marcianas.
