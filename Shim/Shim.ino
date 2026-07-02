#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

#include "eeprom_utils.h"
#include "common_utils.h"

//< Déclaration des variables globales
static STATIC_CONF_FIELDS_t static_conf;
static ESP8266WebServer web_server(0);

/**
 * \fn void html_generate_exploitation_page(void)
 * \brief Fonction de generation du flux HTML/CSS/JS pour la page d'exploitation.
 */
static void html_generate_exploitation_page(void) {
  web_server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  web_server.sendContent(F("<!DOCTYPE html>"));
  web_server.sendContent(F("<html lang='fr'>"));
  web_server.sendContent(F("<head>"));
  web_server.sendContent("<title>Shim_"+String(V_LOGICIEL)+"</title>");
  web_server.sendContent(F("</head>\n"));
  web_server.sendContent(F("<body>"));
  web_server.sendContent(F("Wait for updating a new firmware !!!"));
  web_server.sendContent(F("</body>\n"));
  web_server.sendContent(F("</html>"));
  web_server.sendContent("");
}

void setup() {
  const uint8_t null_ip[4] = {0,0,0,0};
  bool dhcp_enable = 0;
  
  eeprom_init();                                                        // Initialisation du service EEPROM
                                                                        // Initialisation des structures en RAM à 0
  memset(&static_conf, 0, sizeof(STATIC_CONF_FIELDS_t));
                                                                        // Initialisation des structures des données issues de la mémoire EEPROM
  eeprom_read_data(&static_conf,EEPROM_STATIC_CFG);

  if(memcmp(&static_conf.address,null_ip,4) == 0 && 
     memcmp(&static_conf.subnet,null_ip,4)  == 0 && 
     memcmp(&static_conf.gateway,null_ip,4) == 0)
    dhcp_enable = 1;
                                                                        // La configuration réseau du SmartCharger est-elle DHCP
  if(static_conf.is_wifi_network_used)                                  // Un reseau Wifi est-il configuré ?
  {
    if(!dhcp_enable)                                                    // Une configuration manuelle est presente ?
      WiFi.config(static_conf.address,static_conf.gateway,static_conf.subnet); // Application de la configuration                                 
    WiFi.begin(static_conf.SmSsid,static_conf.SmPass);                  // Connexion au réseau
  }
  else                                                                  // Aucun reseau Wifi n'est configué                                              
    WiFi.softAP(static_conf.SmSsid,static_conf.SmPass,AP_CHANNEL,AP_VISIBILITE,AP_MAX_EXPL_CONN); // Creation du Hotspot  

  // Initialisation de la page d'exploitation
  web_server.on("/", HTTP_GET, []() {
    html_generate_exploitation_page();
  });

  ArduinoOTA.setHostname(static_conf.Hostname);                         // Initialisation du Hostname OTA
  WiFi.hostname(static_conf.Hostname);                                  // Initialisation du Hostname Web
  web_server.begin(static_conf.port);                                   // Activation du serveur Web
  ArduinoOTA.begin();                                                   // Activation du service OTA
}

void loop() {

  web_server.handleClient();                                            // Handler du service Web
  ArduinoOTA.handle();                                                  // Handler du service OTA
}
