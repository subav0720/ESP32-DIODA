# Mapa projektu

Projekt používá PlatformIO, prostředí `esp32dev`, framework Arduino a C++.

```text
platformio.ini                 esp32dev, Arduino, monitor 115200 baudů.
src/
  main.cpp                     Vstup Arduino: setup() a loop().
  Drivers/
    GpioDriver.h / .cpp         Inicializace výstupu a zápis HIGH/LOW.
  Middleware/
    Logger.h / .cpp             Jednoduchý sériový výpis.
  Application/
    App.h / .cpp                Inicializace a průběh aplikace.
```

`setup()` volá `appInit()`, `loop()` volá `appProcess()`.
Application může používat GPIO driver i Logger. Driver ani Logger nezávisí
na Application. Všechny implementace jsou pod `src`, kde je PlatformIO sestaví.

Application inicializuje GPIO2 jako OUTPUT s počáteční úrovní LOW a Logger
na 115200 baudů. Logger používá `Serial` (UART0 přes USB převodník desky),
bez čekání `while (!Serial)`. Po inicializaci vypíše přesně `ESP32 START`.
GPIO2 po první sekundě přepne na HIGH, po další na LOW a tak stále dokola.
Každých 5000 ms vypíše například `Uptime: 5 s`, `Uptime: 10 s` atd.
Uptime počítá přímo z `millis() / 1000`, formátuje jej pomocí `snprintf`
do pevného bufferu o 32 znacích, bez dynamického `String`.

Pin, přenosová rychlost a oba intervaly jsou konstanty v `Application/App.cpp`.
GPIO a uptime mají vlastní časovou značku. Oba intervaly používají
`(uint32_t)(now - last) >= interval`, po obsluze nastavují `last = now`.
Nečekají pomocí `delay()` ani čekacích smyček a nedohánějí zmeškané události.
Při zpoždění smyčky tedy mohou výpisy některé sekundy přeskočit.
32bitový čítač `millis()` přeteče přibližně po 49,7 dnech; zobrazovaný uptime
se může vrátit k nule. Odečítání časových značek zachovává časování přes přetečení.

`gpioInitOutput(pin, initialHigh)` připraví počáteční hodnotu před zapnutím
výstupu; `gpioWrite(pin, high)` zapisuje digitální hodnotu. `true` znamená HIGH.
Logger nabízí `loggerBegin(baud)` a `loggerLine(text)` bez vlastních front,
dynamické alokace a dalších knihoven.

Složky `include`, `lib` a `test` obsahují původní informační README.
Složka `.vscode` obsahuje nastavení editoru. `.pio` obsahuje generované sestavení.

Sestavení z kořene projektu: `pio run -e esp32dev`.
Ve VS Code také PlatformIO → Project Tasks → esp32dev → General → Build.
Build firmware do desky nenahrává.

## Ruční ověření na desce (zatím neprovedeno)

- Nahraj firmware pomocí Upload a otevři Monitor na 115200 baudů.
- S otevřeným Monitorem stiskni EN/RESET, abys zachytil i úvodní řádek.
  Ověř `ESP32 START` a pak přibližně každých pět sekund `Uptime: 5 s`,
  `Uptime: 10 s`, `Uptime: 15 s` atd. Před nimi se mohou objevit zprávy bootloaderu.
- GPIO2 ověř měřením proti GND nebo externí LED se sériovým rezistorem
  (například 1 kΩ: GPIO2 → rezistor → anoda LED, katoda → GND).
  Nepředpokládej, že je na GPIO2 vestavěná LED. Ověř po inicializaci LOW,
  přibližně po sekundě HIGH a další přepínání každou sekundu.
- GPIO2 je startovací (strapping) pin: jeho úroveň se vyhodnocuje při resetu.
  Externí zapojení nesmí při resetu vynucovat HIGH; může bránit nahrávání.
  Zapojení měň při vypnutém napájení. Na GPIO nepřiváděj 5 V.
- Ověř současné pravidelné přepínání GPIO a výpisy uptime. Jiný hardware
  firmware neovládá; Wi-Fi nepoužívá.

Úspěšný Build ověřuje sestavení programu, nikoli fyzické zapojení, sériový
převodník ani chování konkrétní desky.
