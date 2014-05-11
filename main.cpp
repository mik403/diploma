#include "Engine.h"

int main() {

	Engine flow;

	flow.initCapture("test_video.mp4");

	while (1) {

		flow.update();
	 	flow.draw();
 
		char keyPressed = waitKey(10);

		if (keyPressed == 27) {
			break;
		}
		else if (keyPressed == 'p') {
			waitKey(1000000000);
		}

	}

}