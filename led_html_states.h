#ifndef LED_HTML_TEMPLATES_H
#define LED_HTML_TEMPLATES_H

const char LED_1_ON[] = "<h1>LED#1</h1><div class=\"on\" hx-post=\"/led/1/toggle\" hx-trigger=\"click\">ON</div>";
const char LED_1_OFF[] = "<h1>LED#1</h1><div class=\"off\" hx-post=\"/led/1/toggle\" hx-trigger=\"click\">OFF</div>";

const char LED_2_ON[] = "<h1>LED#2</h1><div class=\"on\" hx-post=\"/led/2/toggle\" hx-trigger=\"click\">ON</div>";
const char LED_2_OFF[] = "<h1>LED#2</h1><div class=\"off\" hx-post=\"/led/2/toggle\" hx-trigger=\"click\">OFF</div>";

const char LED_3_ON[] = "<h1>LED#3</h1><div class=\"on\" hx-post=\"/led/3/toggle\" hx-trigger=\"click\">ON</div>";
const char LED_3_OFF[] = "<h1>LED#3</h1><div class=\"off\" hx-post=\"/led/3/toggle\" hx-trigger=\"click\">OFF</div>";

#endif // LED_HTML_TEMPLATES_H
