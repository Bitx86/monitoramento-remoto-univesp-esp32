# Monitoramento Remoto (ESP32)- PI UNIVESP

## build

Primeiro, antes de iniciar a construção do projeto, deve-se dar um source no arquivo de export do ESP-IDF (da qual criei para meu computador a função "get_idf"):

```bash
source /opt/esp-idf/export.fish
```
Depois, é necessário executar

```bash
idf.py build flash monitor
```

E aí o processo de compilação e flash na memória acontecerá.
