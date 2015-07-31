#include "ImageGeneratorThreadWorker.h"
#include "ImageGeneratorThreadController.h"

wxThread::ExitCode ImageGeneratorThreadWorker::Entry(){
	//generate the image and write to file
	ImageGenerator imageGenerator;

	//set the colour function used with the heatmap
	imageGenerator.setColourProvider(imageOptions.colourProvider);
	imageGenerator.setInvertColours(imageOptions.invertColours);
	imageGenerator.setFunction(imageOptions.heatMapFunc, imageOptions.xMin, imageOptions.xMax, imageOptions.yMin, imageOptions.yMax);
	imageGenerator.setSSAALevel(imageOptions.ssaaLevel);

	//generate the final image
	for (int i = this->startRow; i < this->endRow; i++){
		imageGenerator.generateImageRow(this->outputImage, i);
		
		//update the thread controller with our progress, and check if we should cancel the image generation
		bool shouldContinue = this->controller->onWorkerThreadProgressUpdate(this->threadId, i - this->startRow, this->endRow - this->startRow);
		if (!shouldContinue)
			break;
	}

	return (wxThread::ExitCode) 0;
}