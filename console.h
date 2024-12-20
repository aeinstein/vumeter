#ifndef CONSOLE_H
#define CONSOLE_H

#ifdef OLED
  #include "menu.h"
#endif

#define LINE_BUF_SIZE 64   //Maximum input string length
#define ARG_BUF_SIZE 32     //Maximum argument string length
#define MAX_NUM_ARGS 3      //Maximum number of arguments

bool newData = false;

char line[LINE_BUF_SIZE];
char args[MAX_NUM_ARGS][ARG_BUF_SIZE];

//Function declarations
int cmd_help();
int cmd_dump();
int cmd_save();
int cmd_load();
int cmd_set();

#ifdef OLED
  int cmd_menu();
#endif

void help_help();
void help_load();
void help_save();
void help_set();
void help_set_amplify();
void help_set_brightness();
void help_set_mode();


//List of functions pointers corresponding to each command
int (*commands_func[])(){
    &cmd_help,
    &cmd_dump,
    &cmd_save,
    &cmd_load,
    &cmd_set,
  #ifdef OLED
    &cmd_menu
  #endif
};

//List of command names
const char *commands_str[] = {
    "help",
    "dump",
    "save",
    "load",
    "set",
  #ifdef OLED
    "menu"
  #endif
};

int num_commands = sizeof(commands_str) / sizeof(char *);

//List of set sub commands
const char *set_args[] = {
  "amplify",
  "brightness",
  "glowness",
  "peak_decay",
  "channel_decay",
  "num_leds",
  "lowcolor",
  "midcolor",
  "highcolor",
  "peak_indicator",
  "mode",
};

int num_set_args = sizeof(set_args) / sizeof(char *);

void parse_line();
bool read_line();
int execute();

const char *menu_args[] = {
  "up",
  "down",
  "left",
  "right",
  "click"
};

int num_menu_args = sizeof(menu_args) / sizeof(char *);

void cli_init(){
  Serial.println("VU Meter Console.");
  Serial.println("Type \"help\" to see a list of commands.");
}

void my_cli(){
  if(!read_line()) return;
  if(!newData) return;
  parse_line();
  execute();

  memset(line, 0, LINE_BUF_SIZE);
  memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);
}

bool read_line() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      line[ndx] = rc;
      ndx++;

      if (ndx >= LINE_BUF_SIZE) 
        ndx = LINE_BUF_SIZE - 1;

    } else {
      line[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
      return 1;
    }
  }

  return 0;
}

void parse_line(){
  if(!newData) return;
  newData = false;

  char *argument;
  int counter = 0;

  argument = strtok(line, " ");

  while((argument != NULL)){
    if(counter < MAX_NUM_ARGS){
      if(strlen(argument) < ARG_BUF_SIZE){
        strcpy(args[counter], argument);
        argument = strtok(NULL, " ");
        counter++;

      } else{
        Serial.println("Input string too long.");
        newData = false;
        break;
      }
      
    } else {
        break;
    }
  }
}

int execute(){
    for(int i=0; i < num_commands; i++){
      if(strcmp(args[0], commands_str[i]) == 0){
          return(*commands_func[i])();
      }
    }

    Serial.print("Invalid command: ");
    Serial.print(args[0]);
    Serial.println(". Type \"help\" for more.");
    return 0;
}

int cmd_help(){
	if(args[1] == NULL){
		help_help();

	} else if(strcmp(args[1], commands_str[0]) == 0){
		help_help();

	} else if(strcmp(args[1], commands_str[1]) == 0){
		//help_dump();

	} else if(strcmp(args[1], commands_str[2]) == 0){
		//help_save();

  } else if(strcmp(args[1], commands_str[3]) == 0){
		//help_load();

  } else if(strcmp(args[1], commands_str[4]) == 0){
    /*
    if(strcmp(args[2], "amplify") == 0) help_set_amplify();
    if(strcmp(args[2], "brightness") == 0) help_set_brightness();
    if(strcmp(args[2], "mode") == 0) help_set_mode();
    if(strcmp(args[2], "amplify") == 0) help_set_amplify();
    if(strcmp(args[2], "brightness") == 0) help_set_brightness();
    if(strcmp(args[2], "mode") == 0) help_set_mode();
    if(strcmp(args[2], "amplify") == 0) help_set_amplify();
    if(strcmp(args[2], "brightness") == 0) help_set_brightness();
*/
	} else{
		help_help();
	}

  return 1;
}

int cmd_dump(){
  dumpConfig();
  return 1;
}

int cmd_save(){
	saveSettings();
  Serial.println("Config saved");
  return 1;
}

int cmd_load(){
  if(loadSettings()){
    Serial.println("Config loaded");
    #ifdef OLED
    handle_menu(1);
    #endif
    return 1;
  }

  return 0;
}

int cmd_set(){
  int arg = -1;

  for(int i=0; i < num_set_args; i++){
    if(strcmp(args[1], set_args[i]) == 0) {
      arg = i;
      break;
    }
  }

  switch(arg){
    #ifdef CALIBRATION_POTI
    case 0:
		  Serial.print("setting amplify to: ");

      if(atof(args[2]) > 0){
        AMPLIFY = atof(args[2]);
        Serial.println(AMPLIFY, 3);
      }

      break;
    #endif
		case 1:
      Serial.print("setting brightness to: ");
      if(atof(args[2]) > 0){
        BRIGHTNESS = atof(args[2]);
        Serial.println(BRIGHTNESS, 3);
      }
      break;

    case 2:
      Serial.print("setting glowness to: ");

      if(strtoul(args[2], 0, 16) > 0){
        GLOWNESS = strtoul(args[2], 0, 16);
        Serial.println(GLOWNESS, HEX);
      }
      break;

    case 3:
      Serial.print("setting peak_decay to: ");

      if(atof(args[2]) > 0){
        PEAK_DECAY = atof(args[2]);
        Serial.println(PEAK_DECAY, 3);
      }

      break;

    case 4:
      Serial.print("setting channel_decay to: ");

      if(atof(args[2]) > 0){
        CHANNEL_DECAY = atof(args[2]);
        Serial.println(CHANNEL_DECAY, 3);
      }
      break;

    case 5:
		  Serial.print("setting num_leds to:");

      if(atoi(args[2]) > 0){
        NUM_LEDS = atoi(args[2]);
        Serial.println(NUM_LEDS, DEC);
        initLeds();
      }
      break;

		case 6:
      Serial.print("setting lowcolor to: ");
      if(strtoul(args[2], 0, 16) > 0){
        LC = strtoul(args[2], 0, 16);
        Serial.println(LC, HEX);
      }

      break;

    case 7:
      Serial.print("setting midcolor to: ");

      if(strtoul(args[2], 0, 16) > 0){
        MC = strtoul(args[2], 0, 16);
        Serial.println(MC, HEX);
      }
      break;

    case 8:
      Serial.print("setting highcolor to:");

      if(strtoul(args[2], 0, 16) > 0){
        HC = strtoul(args[2], 0, 16);
        Serial.println(HC, HEX);
      }
      break;

    case 9:
      Serial.print("setting peak indicator to: ");

      if(strcmp(args[2], "on") == 0) {
        Serial.println("on");
        PEAK_INDICATOR = true;

      } else {
        Serial.println("off");
        PEAK_INDICATOR = false;
      }

      break;

    case 10:
      Serial.print("setting mode to: ");

      if(strcmp(args[2], "dual") == 0) {
        Serial.println("Dual");
        MODE = DUAL;

      } else if(strcmp(args[2], "mirror") == 0) {
        Serial.println("Mirror");
        MODE = MIRROR;

      } else if(strcmp(args[2], "stripped") == 0) {
        Serial.println("Stripped");
        MODE = STRIPPED;

      } else {
        Serial.println("Folded");
        MODE = FOLDED;
      }

      initLeds();
      break;

    default:
      Serial.println("Invalid command. Type \"help set\" to see how to use this command.");
	    return 0;
	}
  
  #ifdef OLED
    handle_menu(true);
  #endif

  return 1;
}

#ifdef OLED
int cmd_menu(){
  int arg = -1;

  for(int i=0; i < num_menu_args; i++){
    if(strcmp(args[1], menu_args[i]) == 0) {
      arg = i;
      break;
    }
  }

  switch(arg){
    case 0: // UP
      handle_menu_up();
      break;

    case 1: // DOWN
      handle_menu_down();
      break;

    case 2: // LEFT
      handle_menu_left();
      break;

    case 3: // RIGHT
      handle_menu_right();
      break;

    default:  // CLICK
      handle_menu_click();
      break;
  }

  return 1;
}
#endif

void help_set(){
  Serial.println("Change config values.");

  Serial.println("The following parameters are available:");

  for(int i=0; i < num_set_args; i++){
      Serial.print("  ");
      Serial.println(set_args[i]);
  }
}

void help_help(){
  Serial.println("The following commands are available:");

  for(int i=0; i < num_commands; i++){
      Serial.print("  ");
      Serial.println(commands_str[i]);
  }
  Serial.println("");
  Serial.println("You can for instance type \"help set\" for more info on the set command.");
}

void help_load(){
  Serial.println("Load stored config from eeprom.");
}

void help_save(){
  Serial.println("Store config to eeprom.");
}

#endif
