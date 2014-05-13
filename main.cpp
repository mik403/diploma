#include "Engine.h"

int main() {

	Engine main_engine;

	main_engine.initCapture("test_video.mp4");

	while (1) {

		main_engine.update();
		main_engine.draw();
 
		char keyPressed = waitKey(20);

		if (keyPressed == 27) { //exit
			break;
		}
		else if (keyPressed == 'p') { //pause
			waitKey(1000000000);
		}
		else if (keyPressed == 'r') { //reset
			main_engine.resetTrackers();
		}

	}

}