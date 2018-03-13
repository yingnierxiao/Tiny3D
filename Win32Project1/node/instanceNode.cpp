#include "instanceNode.h"
#include "../render/instanceDrawcall.h"
#include "../util/util.h"
using namespace std;

InstanceNode::InstanceNode(const VECTOR3D& position):Node(position, VECTOR3D(0, 0, 0)) {
	type = TYPE_INSTANCE;
}

InstanceNode::~InstanceNode() {
		
}

void InstanceNode::addObject(Object* object) {
	Node::addObject(object);
	if (Instance::instanceTable.find(object->mesh) == Instance::instanceTable.end())
		Instance::instanceTable[object->mesh] = 1;
	else
		Instance::instanceTable[object->mesh]++;
}

Object* InstanceNode::removeObject(Object* object) {
	Object* objToRemove = Node::removeObject(object);
	if (objToRemove)
		Instance::instanceTable[objToRemove->mesh]--;
	return objToRemove;
}

void InstanceNode::addObjects(Object** objectArray,int count) {
	for(int i=0;i<count;i++)
		this->addObject(objectArray[i]);
}

void InstanceNode::prepareDrawcall() {
	needCreateDrawcall = false;
}

void InstanceNode::updateRenderData(Camera* camera, int pass) {

}

void InstanceNode::updateDrawcall(int pass) {
	needUpdateDrawcall = false;
}
