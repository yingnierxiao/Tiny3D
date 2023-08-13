#include "oit.h"
#include "../render/shaderscontainer.h"

int counterIndex = 0, linkedListIndex = 0;

Oit::Oit() {
	oitCounter = new RenderBuffer(1, false);
	oitCounter->setBufferData(GL_ATOMIC_COUNTER_BUFFER, counterIndex, GL_UNSIGNED_INT, 1, 1, GL_DYNAMIC_DRAW, NULL);
	oitList = NULL;
	oitHeader = NULL;
}

Oit::~Oit() {
	delete oitCounter;
	if (oitList) delete oitList; oitList = NULL;
	if (oitHeader) delete oitHeader; oitHeader = NULL;
}

void Oit::resize(int width, int height) {
	scrWidth = width, scrHeight = height;
	if (oitHeader) delete oitHeader;
	oitHeader = new Image2D(scrWidth, scrHeight, UINT_PRE, 1, 0, NEAREST, WRAP_CLAMP_TO_EDGE);
	if (oitList) delete oitList;
	oitList = new RenderBuffer(1, false);
	oitList->setBufferData(GL_SHADER_STORAGE_BUFFER, linkedListIndex, GL_UNSIGNED_INT, scrWidth * scrHeight * MAX_OIT_LAYER, 4, GL_STREAM_DRAW, NULL);
}

void Oit::useOitCounter(int loc) {
	if (oitCounter) oitCounter->setShaderBase(GL_ATOMIC_COUNTER_BUFFER, counterIndex, loc);
}

void Oit::unuseOitCounter(int loc) {
	if (oitCounter) oitCounter->unbindShaderBase(GL_ATOMIC_COUNTER_BUFFER, counterIndex, loc);
}

void Oit::useOitList(int loc) {
	if (oitList) oitList->setShaderBase(GL_SHADER_STORAGE_BUFFER, linkedListIndex, loc);
}

void Oit::unuseOitList(int loc) {
	if (oitList) oitList->unbindShaderBase(GL_SHADER_STORAGE_BUFFER, linkedListIndex, loc);
}

void Oit::resetOit(Render* render, Shader* shader) {
	// clear atomic counter
	uint zeroData = 0;
	if (oitCounter) oitCounter->updateBufferData(counterIndex, 1, &zeroData);
	
	// clear head image
	render->useShader(shader);
	if (!shader->isTexBinded(oitHeader->hnd)) shader->setHandle64("headPointers", oitHeader->hnd);
	glDispatchCompute(scrWidth, scrHeight, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Oit::beginRenderOit(Render* render, RenderState* state, Shader* shader) {
	render->setDepthWrite(false);
	render->useShader(shader);
	useOitCounter(0);
	useOitList(0);
	shader->setUint("uMaxNodes", scrWidth * scrHeight * MAX_OIT_LAYER);
	if (!shader->isTexBinded(oitHeader->hnd)) shader->setHandle64("headPointers", oitHeader->hnd);
}

void Oit::endRenderOit(Render* render) {
	unuseOitCounter(0);
	unuseOitList(0);
	render->setDepthWrite(true);

	//uint* data = (uint*)malloc(scrWidth * scrHeight * sizeof(uint));
	//oitHeader->readData(sizeof(uint), data);
	//printf("header: %d %d %d %d\n", data[0], data[1], data[2], data[3]);
	//free(data);
}

void Oit::blendOit(Render* render, RenderState* state, Shader* shader) {
	render->useShader(shader);
	useOitList(0);
	if (!shader->isTexBinded(oitHeader->hnd)) shader->setHandle64("headPointers", oitHeader->hnd);
	// todo draw screen board
	unuseOitList(0);
}