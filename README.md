# FreeRTOS Projekt auf dem Raspberry Pi Pico

## Überblick

Dieses Projekt verwendet **FreeRTOS** auf dem Raspberry Pi Pico, um effizientes Task-Multitasking zu ermöglichen. FreeRTOS ist ein Open-Source-Echtzeitbetriebssystem, das die Verwaltung mehrerer Tasks auf einem Kern intelligent plant, sodass die Ausführung scheinbar parallel wirkt.

## Was ist FreeRTOS?

**FreeRTOS** ist ein Echtzeitbetriebssystem, das speziell für Mikrocontroller entwickelt wurde. Es ermöglicht die Ausführung von Tasks, indem es durch den Scheduler Aufgaben basierend auf Prioritäten plant und unterbrechbar macht.

## Vorteile von FreeRTOS

- **Echtzeitfähigkeit**: Aufgaben werden innerhalb eines festgelegten Zeitrahmens ausgeführt.
- **Multitasking**: Tasks werden vom Scheduler so geplant, dass sie effizient auf einem Kern ablaufen, ohne einander zu stören.
- **Effizienz**: Geringer Ressourcenverbrauch, ideal für Mikrocontroller.
- **Portabilität**: Unterstützt viele Prozessorarchitekturen, einschließlich des RP2040.

## Warum FreeRTOS auf dem Raspberry Pi Pico?

Der **Raspberry Pi Pico** mit seinem RP2040-Chip hat zwei Cortex-M0+ Kerne, aber in diesem Projekt wird nur einer verwendet. FreeRTOS sorgt dennoch für scheinbares Multitasking, indem der Scheduler die Ausführung mehrerer Tasks effizient verwaltet. Das ist nützlich für zeitkritische Anwendungen wie Sensorsteuerung und Automatisierung.

> **Hinweis**: Ohne SMP (Symmetric Multiprocessing) laufen die Tasks nicht wirklich parallel auf beiden Kernen, sondern werden schlau gescheduled und wechseln sich ab, um eine effektive Nutzung des Prozessors zu gewährleisten.

## Projektinhalt

Dieses Projekt zeigt:
- **Task-Erstellung**: Sequentielle, aber effizient geplante Task-Ausführung.
- **Queues und Semaphores**: Kommunikation und Synchronisation zwischen Tasks.

## Setup

1. Installiere das **Pico SDK** und **FreeRTOS**.
2. Setze die Umgebungsvariablen FREERTOS_KERNEL_PATH und picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake .
3. Klone das Repository und öffne es in deiner Entwicklungsumgebung.
4. Kompiliere und lade das Projekt auf den Pico.
5. Nutze einen seriellen Monitor, um die Task-Ausgabe zu sehen.
