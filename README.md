
# Tarefa: Roteiro de FreeRTOS #1 - EmbarcaTech 2025

Autor: **Ricardo Jorge Furlan**

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, 13 de junho de 2025


---


# ⚠ Disclaimer
# THE SOFTWARE IS PROVIDED "AS IS", USE AT YOUR OWN RISK
# O SOFTWARE É FORNECIDO "COMO ESTÁ", USE POR SUA CONTA E RISCO


## This code was developed for educational purposes as an assignment for Phase 2 of the residency program in the **EmbarcaTech professional training program**, with the goal of demonstrating some of the concepts learned during the residency.

## Third-Party Code

- The files for controlling the **OLED display** (`ssd1306_...` files) located in the `libs_ext` directory, including the text fonts, were based on the repository:  
  https://github.com/BitDogLab/BitDogLab-C/tree/main/display_oled_logo/inc with minor modifications.
- The `pico_sdk_import.cmake` file was **automatically generated** during project creation by the **Raspberry Pi Pico plugin**.
- The `CMakeLists.txt` file was initially **automatically generated** during project creation by the **Raspberry Pi Pico plugin**, and later edited as needed for the project.

## How to Build/Compile the Code

This software was developed for the **BitDogLab V6.3** board. This board uses the **Raspberry Pi Pico-W** board (which contains the **RP2040 microcontroller**).

For development, the **Visual Studio Code IDE** was used along with the **Raspberry Pi Pico plugin**. The SDK version used was **2.1.1**.


**Add FreeRTOS to the project:**

$ git submodule add https://github.com/FreeRTOS/FreeRTOS-Kernel.git FreeRTOS


---

# Documentação do Projeto - Sistema Multitarefa com FreeRTOS no RP2040

### Descrição Geral

Este projeto tem como objetivo desenvolver um sistema embarcado multitarefa utilizando a placa BitDogLab com o microcontrolador RP2040. A aplicação é desenvolvida em linguagem C, usando o FreeRTOS como sistema operacional de tempo real e o VSCode com suporte ao pico-sdk.
### Objetivos de Aprendizagem

- Criar e gerenciar múltiplas tarefas com o FreeRTOS
- Compreender o funcionamento do escalonador e prioridades
- Utilizar as funções vTaskCreate(), vTaskSuspend() e vTaskResume()
- Controlar periféricos GPIO no RP2040
- Estruturar projetos no VSCode com FreeRTOS e pico-sdk

### Principais Componentes do Sistema Utilizados neste  Projeto
| Módulo           | Descrição                                                   |
| ---------------- | ----------------------------------------------------------- |
| `app_semaphoros` | Gerencia as tarefas e semáforos do sistema                  |
| `config`         | Arquivo de configuração geral e definição de pinos          |
| `buttons`        | Controle dos botões A e B (suspensão e retomada de tarefas) |
| `buzzer`         | Controle dos bipes periódicos no buzzer                     |
| `led_rgb`        | Alternância cíclica entre cores no LED RGB                  |

### Funcionamento do Sistema

O sistema embarcado executa três tarefas concorrentes:

- Tarefa LED RGB: alterna entre as cores vermelho, verde e azul ciclicamente.
- Tarefa Buzzer: gera bipes em intervalos definidos.
- Tarefa Botões: lê os estados dos botões e suspende ou retoma as tarefas correspondentes:
    - Botão A: controla a tarefa do LED RGB.
    - Botão B: controla a tarefa do buzzer.

### Controle de Tarefas com e sem Semáforos

O projeto implementa duas abordagens distintas para controlar a execução das tarefas associadas ao LED RGB e ao buzzer, dependendo do modo selecionado pelo usuário na inicialização: modo normal ou modo com semáforos.
#### Modo Normal (sem semáforos)

Neste modo, o controle das tarefas é feito diretamente:

- O botão A suspende ou retoma a tarefa do LED RGB usando as chamadas vTaskSuspend() e vTaskResume().

- O botão B ativa ou desativa a execução do buzzer por meio de uma variável global buzzer_enable. Foi adotada esta abordagem para que o buzzer não fica-se travado tocando ao se suspender a tarefa do buzzer


#### Modo com Semáforos (RTOS-friendly)

Neste modo, o sistema utiliza semáforos binários para coordenar o acesso aos recursos:

- O botão A realiza xSemaphoreTake() ou xSemaphoreGive() para bloquear ou liberar o acesso ao LED RGB.
- O botão B faz o mesmo para o buzzer.

As tarefas do LED e do buzzer tentam adquirir os semáforos antes de executar. Se não obtiverem acesso, ficam bloqueadas até que o botão correspondente libere o recurso. Esse modelo oferece maior robustez e escalabilidade, sendo adequado para aplicações multitarefa reais.

### Feedback Visual

Em ambos os modos, o sistema atualiza o display OLED com o estado atual das tarefas:
- Modo normal: exibe se a tarefa do LED está suspensa e se o buzzer está habilitado.
- Modo com semáforos: exibe se o acesso ao LED ou ao buzzer está bloqueado por semáforo



### Organização dos Principais Arquivos

	├── src/
	│   ├── oled
	│	│   ├── oled
	│   ├── app_semaphoros.c
	│   ├── buttons.c
	│   ├── buzzer.c
	│   ├── led_rgb.c
	├── include/
	│   ├── app_semaphoros.h
	│   ├── buttons.h
	│   ├── buzzer.h
	│   ├── config.c
	│   ├── FreeRTOSConfig.h
	│   ├── led_rgb.h
	├── lib_ext/
	├── CMakeLists.txt
	├── main.c
	├── README.md
	├── pico_sdk_import.cmake

---

## Resposta das questões

- O que acontece se todas as tarefas tiverem a mesma prioridade?

    - Se todas as tarefas tiverem a mesma prioridade, **o escalonador do FreeRTOS aplicará um esquema de round-robin cooperativo entre as tarefas prontas para executar,** ou seja, as tarefas compartilham o tempo de CPU igualmente, revezando a execução com base em quantos ticks cada uma ocupa. Como seu projeto utiliza chamadas como vTaskDelay(), as tarefas naturalmente cedem o processador, permitindo o revezamento suave entre elas.

- Qual a tarefa que mais consome tempo de CPU?
    - **É a  tarefa de botões**, roda com mais frequência e envolve:
        Leitura de GPIOs
        Atualização do display OLED
        Lógica condicional e semáforos (no modo com semáforos)
    Obs.: As tarefas de LED RGB e buzzer são bastante leves: realizam pequenas ações (troca de cor ou beep) e dormem (vTaskDelay) por alguns milissegundos.

- Quais seriam os riscos de usar polling sem prioridades?
    - Se todas as tarefas tiverem a mesma prioridade e a frequência de polling for alta, **uma tarefa pode atrasar a execução de outra mais importante**, como o reconhecimento rápido do botão.

---

## 📜 Licença
GNU GPL-3.0.
