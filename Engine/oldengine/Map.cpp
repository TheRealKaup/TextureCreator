#include "Engine.h"


// ---=== Layer ===---

int Engine::Layer::AddObject(Object* object)
{
	objects.push_back(object);
	object->parentLayer = this;
	return objects.size() - 1;
}

bool Engine::Layer::RemoveObject(size_t index)
{
	if (objects.size() < index)
		return false;
	
	objects.erase(objects.begin() + index - 1);
	return true;
}

bool Engine::Layer::RemoveObject(std::string name)
{
	for (int i = 0; i < objects.size(); i ++)
	{
		if (objects[i]->name == name)
		{
			objects.erase(objects.begin() + i - 1);
			return true;
		}
	}
	return false;
}

int Engine::Layer::AddVisual(Visual* visual)
{
	visuals.push_back(visual);
	visual->parentLayer = this;
	return visuals.size() - 1;
}

bool Engine::Layer::RemoveVisual(size_t index)
{
	if (visuals.size() < index)
		return false;
	
	visuals.erase(visuals.begin() + index - 1);
	return true;
}

// ---=== Map ===---

int Engine::Map::AddCamera(Camera* camera, bool asActiveCamera)
{
	cameras.push_back(camera);
	if (asActiveCamera)
		activeCameraI = cameras.size() - 1;
	return cameras.size() - 1;
}

int Engine::Map::AddLayer(Layer* layer)
{
	layers.push_back(layer);
	return layers.size() - 1;
}