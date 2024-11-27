#define LINE_BUF_SIZE 64   //Maximum input string length
#define ARG_BUF_SIZE 32     //Maximum argument string length
#define MAX_NUM_ARGS 3      //Maximum number of arguments

bool error_flag = false;

char line[LINE_BUF_SIZE];
char args[MAX_NUM_ARGS][ARG_BUF_SIZE];

//Function declarations
int cmd_help();
int cmd_dump();
int cmd_save();
int cmd_load();
int cmd_set();

//List of functions pointers corresponding to each command
int (*commands_func[])(){
    &cmd_help,
    &cmd_dump,
    &cmd_save,
    &cmd_load,
    &cmd_set
};

//List of command names
const char *commands_str[] = {
    "help",
    "dump",
    "save",
    "load",
    "set"
};

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
};

void parse_line();
bool read_line();
int execute();

int num_commands = sizeof(commands_str) / sizeof(char *);
int num_set_args = sizeof(set_args) / sizeof(char *);

void cli_init(){
    Serial.println("VU Meter Console.");
    Serial.println("Type \"help\" to see a list of commands.");
}


void my_cli(){
    if(!read_line()) return;
    if(!error_flag) parse_line();
    if(!error_flag) execute();

    memset(line, 0, LINE_BUF_SIZE);
    memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);

    error_flag = false;
}

bool read_line(){
    String line_string;

    while(!Serial.available()) return 0;

    if(Serial.available()){
        line_string = Serial.readStringUntil("\n");

        if(line_string.length() < LINE_BUF_SIZE){
          line_string.toCharArray(line, LINE_BUF_SIZE);
          Serial.println(line_string);
          return 1;

        } else {
          Serial.println("Input string too long.");
          error_flag = true;
        }
    }
}

void parse_line(){
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
                error_flag = true;
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
		//help_help();

	} else if(strcmp(args[1], commands_str[0]) == 0){
		//help_help();

	} else if(strcmp(args[1], commands_str[1]) == 0){
		//help_led();

	} else if(strcmp(args[1], commands_str[2]) == 0){
		//help_exit();

	} else{
		//help_help();
	}
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
    //dumpConfig();
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
    case 0:
		  Serial.print("setting amplify to: ");

      if(atof(args[2]) > 0){
        AMPLIFY = atof(args[2]);
        Serial.println(AMPLIFY, 3);
        return 1;
      }

      break;

		case 1:
      Serial.print("setting brightness to: ");
      if(atof(args[2]) > 0){
        BRIGHTNESS = atof(args[2]);
        Serial.println(BRIGHTNESS, 3);
        return 1;
      }
      break;

    case 2:
      Serial.print("setting glowness to: ");

      if(strtoul(args[2], 0, 16) > 0){
        GLOWNESS = strtoul(args[2], 0, 16);
        Serial.println(GLOWNESS, HEX);
        return 1;
      }
      break;

    case 3:
      Serial.print("setting peak_decay to: ");

      if(atof(args[2]) > 0){
        PEAK_DECAY = atof(args[2]);
        Serial.println(PEAK_DECAY, 3);
        return 1;
      }

      break;

    case 4:
      Serial.print("setting channel_decay to: ");

      if(atof(args[2]) > 0){
        CHANNEL_DECAY = atof(args[2]);
        Serial.println(CHANNEL_DECAY, 3);
        return 1;
      } 
      break;

    case 5:
		  Serial.print("setting num_leds to:");

      if(atoi(args[2]) > 0){
        NUM_LEDS = atoi(args[2]);
        Serial.println(NUM_LEDS, DEC);
        return 1;
      }
      break;

		case 6:
      Serial.print("setting lowcolor to: ");
      if(strtoul(args[2], 0, 16) > 0){
        LC = strtoul(args[2], 0, 16);
        Serial.println(LC, HEX);
        return 1;
      }

      break;

    case 7:
      Serial.print("setting midcolor to: ");

      if(strtoul(args[2], 0, 16) > 0){
        MC = strtoul(args[2], 0, 16);
        Serial.println(MC, HEX);
        return 1;
      }
      break;

    case 8:
      Serial.print("setting highcolor to:");

      if(strtoul(args[2], 0, 16) > 0){
        HC = strtoul(args[2], 0, 16);
        Serial.println(HC, HEX);
        return 1;
      }
      break;

    default:
      Serial.print(args[1]);
      Serial.println("not recognized");
      break;
	} 

  Serial.println("Invalid command. Type \"help set\" to see how to use this command.");
	return 0;
}


