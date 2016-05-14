#include <iostream>

#include <linux/input.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

Mix_Chunk* chunk;

void end();
bool init(int argc,char *argv[],int* fileDescripter);
void loop(int fileDescripter);

int main(int argc,char *argv[]){
	int fileDescripter;
	Mix_Chunk* chunk;
	if(!init(argc,argv,&fileDescripter))	
		return -1;
	loop(fileDescripter);
}

void sighandler(int signal){
	end();
}

void loop(int fileDescripter){
	for(;;){
		struct input_event event;
		if (read(fileDescripter,&event,sizeof(event)) != sizeof(event))
			exit(EXIT_FAILURE);
		if (event.type == EV_KEY && event.value == 1){
			Mix_PlayChannel(-1,chunk,0);
		}
	}
}

bool init(int argc,char *argv[],int *fileDescripter){
	if (SDL_Init(SDL_INIT_AUDIO) == -1){
		std::cerr << "Failed init SDL" << std::endl;
		return false;
	}
	if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024) == -1){
		std::cerr << "Failed open audio" << std::endl;
		return false;
	}
	if (argc > 1){
		chunk = Mix_LoadWAV(argv[1]);
		if (chunk == nullptr){
			std::cerr << "Failed open " << argv[1] << std::endl;
			return false;
		}
	}else{
		chunk = Mix_LoadWAV("./default.wav");	
		if (chunk == nullptr){
			std::cerr << "Failed open default.wav" << std::endl;
			return false;
		}
	}
	*fileDescripter = open("/dev/input/event0",O_RDONLY);
	if (*fileDescripter == -1){
		std::cerr << "Cannot read /dev/input/event0" << std::endl;
		return false;
	}
	if (SIG_ERR == signal(SIGHUP,sighandler) && SIG_ERR == signal(SIGINT,sighandler)){
		std::cerr << "Failed set signalhandler" << std::endl;
		return false;
	}
	return true;
}

void end(){
	Mix_FreeChunk(chunk);
	Mix_Quit();
	SDL_Quit();
	std::cout << "exited with code 0" << std::endl;
}
