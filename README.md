# arduino-relay-server
Servidor de Rele e RF433 para comunicar com MQTT

O Uso que faço dessa aplicação é Comunicar uma relay board de 8 portas, um transmissor RF433 e um receptor RF433 com o Home Assistant.


O que o sistema faz?.
-------------

- Recebe sinal do alarme Sonoff DW1 por RF433 e notifica o Home Assistant através do MQTT que me envia a notificação pelo telegram
- Aciona os relays para ligar e desligar dispositivos proximos inclusive um computador direto no powerbutton através do MQTT que eu envio pelo Home assistant. [Placa de Relê](https://pt.aliexpress.com/item/New-5V-8-Channel-Relay-Module-Board-for-Arduino-PIC-AVR-MCU-DSP-ARM-Electronic/1803836311.html?spm=a2g03.search0204.3.51.1e6a2c1fja9bO8&s=p&ws_ab_test=searchweb0_0,searchweb201602_1_10065_10068_10547_319_10059_10884_317_10548_10887_10696_321_322_10084_453_10083_454_10103_10618_10307_537_536_10902,searchweb201603_16,ppcSwitch_0&algo_expid=2ee3298b-6564-47e9-a5cb-90d7c8b86073-8&algo_pvid=2ee3298b-6564-47e9-a5cb-90d7c8b86073&transAbTest=ae803_4)
- Transmite sinal RF433 [Modulo de recepção e transmissao RF](https://pt.aliexpress.com/item/Lote-1-1-par-2-pcs-433-mhz-RF-transmissor-e-receptor-M-dulo-kit-de/32896066427.html?spm=a2g03.search0204.3.270.519ce2f68P2u5z&transAbTest=ae803_4&ws_ab_test=searchweb0_0%2Csearchweb201602_1_10065_10068_10547_319_10059_10884_317_10548_10887_10696_321_322_10084_453_10083_454_10103_10618_10307_537_536_10902%2Csearchweb201603_16%2CppcSwitch_0&algo_pvid=fc5f06ee-4e91-409e-a736-f9f7d2a0fe64&algo_expid=fc5f06ee-4e91-409e-a736-f9f7d2a0fe64-37) para acionar modulos de relays remotos [QIACHIP-Universal](https://pt.aliexpress.com/item/QIACHIP-Universal-433-MHz-Wireless-Switch-Controle-Remoto-AC-85-V-110-V-220-V-1CH/32870812191.html?spm=a2g0s.9042311.0.0.2798b90aaA1KEm "QIACHIP Universal") que funcionam como um Sonoff so que mais burros.

![](https://github.com/slayrj/arduino-relay-server/blob/master/sketch.png)


Usei o Pino A0 pois minha placa é de 8 rele se usasse em linha reta com o Flat iria atingir o pino de interrupção usado pelo Receptor de RF.

- Imagem do Projeto atualmente, (falta mudanças estéticas, em breve farei.)

![](https://github.com/slayrj/arduino-relay-server/blob/master/projeto-15-03-2019.jpg)
