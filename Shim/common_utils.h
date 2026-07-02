#ifndef __COMMON_UTILS__
#define __COMMON_UTILS__

#include <stdint.h>

//< MACRO de version logicielle
#define V_LOGICIEL                "v1"                  // Version Logicielle

//< MACRO du Mode Access Point
#define AP_CHANNEL                0                     // Channel ModuleTIC Access Point
#define AP_VISIBILITE             false                 // Visibilite ModuleTIC Access Point
#define AP_MAX_CONF_CONN          1                     // Nombre de connexion en mode configuration du ModuleTIC Access Point
#define AP_MAX_EXPL_CONN          5                     // Nombre de connexion en mode exploitation du ModuleTIC Access Point

//< Structure de Configurations fonctionnelles figées (Configurable uniquement à la configuration de l'équippement)
typedef struct
{
  char      Hostname[65];                               // Hostname
  char      SmSsid[33];                                 // SSID
  char      SmPass[65];                                 // Password
  uint8_t   address[4];                                 // Adresse IP Static
  uint8_t   subnet[4];                                  // Masque de sous reseau
  uint8_t   gateway[4];                                 // Passerelle
  uint16_t  port;                                       // Port de service Web
  uint16_t  portWs;                                     // Port de service WebSocket
  uint8_t   is_configured         : 1;                  // Bitfield indiquant que l'équippement est configuré
  uint8_t   is_wifi_network_used  : 1;                  // Bitfield indiquant un accrochage sur une borne Wifi
  uint8_t   RUF                   : 6;                  // Reservé Usage Future
} STATIC_CONF_FIELDS_t;

//< Enumération des types de sauvegardes en mémoire EEPROM
enum EepromDataType {
  EEPROM_STATIC_CFG,                                    // Configuration des données STATIC
};

#endif
