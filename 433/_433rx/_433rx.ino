// 433 MHz Přijímač

// připojení knihovny
#include <VirtualWire.h>

void setup()
{
  // inicializace komunikace po sériové lince
  Serial.begin(9600);
  // nastavení typu bezdrátové komunikace
  vw_set_ptt_inverted(true);
  // nastavení rychlosti přenosu v bitech za sekundu
  vw_setup(2000);
  // nastavení čísla datového pinu pro přijímač
  vw_set_rx_pin(2);
  // nastartování komunikace po nastaveném pinu
  vw_rx_start();
}

void loop()
{
  // vytvoření proměnných pro uložení
  // přijaté zprávy a její délky,
  // délka je maximálně 78 znaků
  uint8_t zprava[VW_MAX_MESSAGE_LEN];
  uint8_t delkaZpravy = VW_MAX_MESSAGE_LEN;
  // v případě přijetí zprávy se vykoná tato if funkce
  if (vw_get_message(zprava, &delkaZpravy)) {
    // rozsvícení LED diody při příjmu (nepovinné)
    digitalWrite(13, true);
    // vytištění celé zprávy po znacích
    // pomocí for cyklu
    for (int i = 0; i < delkaZpravy; i++) {
      Serial.print((char)zprava[i]);
    }
    // ukončení vypsaného řádku pomocí println
    Serial.println("");
    // zhasnutí LED diody při příjmu (nepovinné)
    digitalWrite(13, false);
  }
}
