# 📘 Projeto Harry - Raspberry Pi Pico W com BitDogLab

Este documento apresenta a descrição detalhada, os procedimentos e as diretrizes para a correta execução e manutenção do projeto "Harry", desenvolvido utilizando a placa Raspberry Pi Pico W em conjunto com a BitDogLab. O projeto foi implementado em linguagem C, com o uso do SDK oficial do Raspberry Pi Pico (versão 1.5.1) e o ambiente de desenvolvimento Visual Studio Code.  

O sistema realiza o controle de um display OLED SSD1306, exibe imagens binárias e reproduz uma melodia através do buzzer integrado da BitDogLab, proporcionando uma aplicação prática de conceitos fundamentais de sistemas embarcados e Internet das Coisas (IoT).  

---

## ⚙️ 1. Configuração do Ambiente

Para garantir o correto funcionamento da aplicação, siga rigorosamente as instruções descritas a seguir:

1. **Instalação do SDK do Raspberry Pi Pico (v1.5.1)**  
   - Certifique-se de que o SDK está instalado no seguinte diretório:  
     ```plaintext
     C:\Users\JC\.pico-sdk\sdk\1.5.1
     ```

2. **Instalação do Visual Studio Code (VS Code)**  
   - Instale a extensão *Raspberry Pi Pico* para habilitar o suporte ao desenvolvimento com a placa.  

3. **Configuração do `CMakeLists.txt`**  
   - Verifique se o caminho de importação do SDK está corretamente especificado.  
   - Confirme a inclusão das bibliotecas necessárias para o controle do OLED e o uso do PWM.  

4. **Compilação e Geração do Firmware**  
   - No terminal do VS Code, execute os comandos a seguir:  
     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ```

5. **Transferência do Firmware para o Raspberry Pi Pico W**  
   - Conecte a placa no modo bootloader (pressione o botão BOOTSEL ao conectar ao computador).  
   - Copie o arquivo `.uf2` gerado para a unidade de armazenamento exibida pelo sistema operacional.  

---

## 🔌 2. Conexões de Hardware (BitDogLab)

A seguir, apresentam-se as conexões necessárias entre a placa BitDogLab e os periféricos utilizados no projeto:

| **Componente**  | **Pino (GPIO)** | **Descrição**                 |
|------------------|-----------------|--------------------------------|
| OLED SSD1306 (SDA) | GPIO 14       | Linha de dados I2C              |
| OLED SSD1306 (SCL) | GPIO 15       | Linha de clock I2C              |
| Buzzer (PWM)       | Definido no `harry.h` | Saída de sinal PWM para áudio |

### 📡 Protocolo Utilizado:  
- **I2C**: para comunicação com o display OLED SSD1306 (endereço padrão: `0x3C`).  
- **PWM**: para a geração das frequências sonoras emitidas pelo buzzer.  

⚠️ **Observação:** Certifique-se de que as conexões estão corretas e que os pinos utilizados correspondem às definições presentes no código-fonte.  

---

## 🖼️ 3. Imagens (harry_black.h e harry_white.h)

As imagens exibidas no display OLED são armazenadas nos arquivos `harry_black.h` e `harry_white.h`. Estes arquivos contêm arrays binários que representam imagens monocromáticas, compatíveis com o formato suportado pelo display SSD1306.  

As imagens são exibidas através da função `ssd1306_draw_image()`, implementada no arquivo `ssd1306_i2c.c`.

### Exemplo de utilização no código:  
```c
// Exibe a imagem preta no display OLED
ssd1306_draw_image(harry_black);

// Exibe a imagem branca no display OLED
ssd1306_draw_image(harry_white);
