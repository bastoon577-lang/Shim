# Shim

Le firmware Shim litérallement "calle" en anglais est un mécanisme permettant la mise à jour par l'OTA des Modules s'éxécutant sur l'ESP8266.

Construit autour d'un fonctionnel minimal, ce firmware implémente les librairies :
* ESP8266WebServer
* ArduinoOTA

Et permet de palier à une problématique de mise à jour sans fil (OTA) qui requiert de par son mécanisme une écriture du nouveau firmware
à la suite du firmware déjà existant.

En effet, le flashage de l'ESP01 via un câble (liaison série/USB) permet d'écraser la mémoire et par conséquent 
de ne poser aucun problème vis à vis de la taille du firmware (pour peu que celui puisse rentrer dans la mémoire flash de 1 Mo).

La mise à jour par l'OTA quant à elle impose le protocole suivant:
* L'ESP01 exécute le firmware actuel (ex : SmartCharger) et reçoit le nouveau firmware par le Wifi,
* L'ESP01 doit stocker le nouveau firmware dans un espace temporaire de la mémoire flash,
* Une fois la mise à jour terminée, l'ESP01 redémarre le Bootloader et remplace l'ancien firmware par le nouveau.

En conséquence, si le firmware actuel est de 510 Ko et que le nouveau firmware est suppérieur à 500 Ko, alors la mémoire flash
devient dépassée et l'OTA renvoit une erreur avant mise à jour.

Le mécanisme Shim permet donc de libérer de l'espace mémoire flash en implémentant le strict minimum permettant:
* D'exposer un serveur Web pour vérifier que l'équippement est toujours vivant,
* D'implémenter ArduinoOTA pour satisfaire la mise à jour OTA,
* De garder la configuration Wifi du Module.

Importante, la configuration Wifi de l'équipement sur le réseau étant écrite dans l'EEPROM, le firmware Shim est compatible sur les 
équippements :
* Module TIC
* SmartCharger

Par l'exploitation de la structure STATIC_CONF_FIELDS_t exposée à l'adresse 0 de l'EEPROM par :
* [Le ModuleTIC](https://github.com/bastoon577-lang/Module_TIC/blob/main/SOFTWARE/Module_TIC/common_utils.h#L46)
* [Le SmartCharger](https://github.com/bastoon577-lang/SmartCharger_utils/blob/83a58b5db8b868ffecfff1e72e23f74d6e20f49d/smartcharger/common_utils.h#L43)

typedef struct
{
  char                  Hostname[65];                   // Hostname
  char                  SmSsid[33];                     // SSID
  char                  SmPass[65];                     // Password
  uint8_t               address[4];                     // Adresse IP Static
  uint8_t               subnet[4];                      // Masque de sous reseau
  uint8_t               gateway[4];                     // Passerelle
  uint16_t              port;                           // Port de service Web
  uint16_t              portWs;                         // Port de service WebSocket
  uint8_t               is_configured         : 1;      // Bitfield indiquant que l'équippement est configuré
  uint8_t               is_wifi_network_used  : 1;      // Bitfield indiquant un accrochage sur une borne Wifi
  uint8_t               RUF                   : 6;      // Reservé Usage Future
} STATIC_CONF_FIELDS_t;

Le firmware du Module TIC étant suffisement légé (~400 Ko); il n'est pas nécessaire de recourir au Shim, la commande de mise à jour est la suivante :
```
python espota.py -d -i 192.168.x.xxx -f ModuleTIC.ino.bin
```
En revanche, le firmware Et3k_SmartCharger.ino.bin étant plus imposant (>500 Ko), il devient nécessaire d'utiliser le Shim en effectuant les commandes suivantes:
```
python espota.py -d -i 192.168.x.xxx -f Shim.ino.bin
```
Pour libérer de l'espace mémoire, puis d'envoyer le firmware SmartCharger :
```
python espota.py -d -i 192.168.x.xxx -f Et3k_SmartCharger.ino.bin
```

 ###### Auteur : *Sébastien DALIGAULT*. 
