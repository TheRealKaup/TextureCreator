/*
Features:

- Collision checking
	When objects move will check for surroundings and react with user defined collider types (what can push / go through what)
	(DONE)

- Gravity
	Applies force[s] in a straight or point centered direction or to every object which has gravity enabled, given by mass and velocity

-Called functions
	Calls user defined functions when an event from a generous selection happens, also suppliend all types of data.
- Path finding when an object moves but can't go through, or gets stuck.

- Bounciness
	When an object collides, bounciness will be applied if enabled

The physics engine in KCGE has almost all the features a normal popular 2D game engine has (for example Unity), but it's only precise on int level (because of graphics), and doesn't have rotation.
I will try to get the most out of the limited graphics.
*/

#include "Engine.h"

using namespace Engine;

// Does not check if relevancy (such as if both are in the same layer or have the collider active), just compares colliders on position. This is for a specific part in the Move function.
bool Engine::AreObjectsOverlapping(Object* objA, Object* objB, bool ignoreIrrelevancy)
{
	if (!ignoreIrrelevancy)
	{
		if (!objA->colliderActive)
			return false;
		if (!objB->colliderActive)
			return false;
	}
	if (objA == NULL)
		return false;
	if (objB == NULL)
		return false;
	if (objA->collider.size() == 0)
		return false;
	if (objA->collider[0].size() == 0)
		return false;
	if (objB->collider.size() == 0)
		return false;
	if (objB->collider[0].size() == 0)
		return false;

	for (int y = 0; y < objA->collider.size(); y++)
	{
		int oY = y + objA->pos.y - objB->pos.y;
		if (oY < 0 || oY >= objB->collider.size()) // if converted y (oY) is not on range, try the next Y. oY should eventually be in the range because we checked irrelevancy earlier.
			continue;

		for (int x = 0; x < objA->collider[y].size(); x++)
		{
			int oX = x + objA->pos.x - objB->pos.x;
			if (oX < 0 || oX >= objB->collider[oY].size()) // Same as before but with x
				continue;

			if (objA->collider[y][x] && objB->collider[oY][oX]) // Overlapping
			{
				return true;
			}
		}
	}
	return false;
}

bool Engine::Object::CanMove(Vector2D dir, std::vector<Object*>* pushingObjects, std::vector<Object*>* objectsToPush)
{
	// Irrelevancy
	if (!colliderActive)
		return true;
	if (collider.size() == 0)
		return true;
	if (collider[0].size() == 0)
		return true;
	if (parentLayer->objects.size() <= 1)
		return true;
	bool blank = true;
	for (int y = 0; y < collider.size() && blank; y++)
	{
		for (int x = 0; x < collider[y].size() && blank; x++)
		{
			if (collider[y][x])
				blank = false;
		}
	}
	if (blank)
		return true;

	bool collided = false;
	bool pushed = false;
	int collisionResult = 0;
	int currentIndex = pushingObjects->size();
	Object* oObj;

	for (int o = 0; o < parentLayer->objects.size(); o++)
	{
		pushed = false;

		oObj = parentLayer->objects[o];

		// Irrelevancy
		if (oObj == this)
			continue;
		if (!oObj->colliderActive)
			continue;
		bool ignore = false;
		for (int i = 0; i < pushingObjects->size(); i++)
		{
			if (pushingObjects[0][i] == oObj || objectsToPush[0][i] == oObj)
			{
				ignore = true;
				break;
			}
		}
		if (ignore)
			continue;
		if (colliderType >= 0 && colliderType < colliderTypes.size())
		{
			if (oObj->colliderType < colliderTypes[colliderType].size())
				collisionResult = colliderTypes[colliderType][oObj->colliderType];
		}
		if (collisionResult == 2)
			continue;
		if (AreObjectsOverlapping(this, oObj))
			continue;
		
		debug_relevantCollidersInArea++;

		for (int y = 0; y < collider.size() && !pushed; y++)
		{
			int oY = y + pos.y - oObj->pos.y + dir.y;
			if (oY < 0 || oY >= oObj->collider.size())
				continue;

			for (int x = 0; x < collider[y].size() && !pushed; x++)
			{
				int oX = x + pos.x - oObj->pos.x + dir.x;
				if (oX < 0 || oX >= oObj->collider[oY].size())
					continue;

				if (collider[y][x] && oObj->collider[oY][oX])
				{
					if (collisionResult == 0)
						return false;
					else if (collisionResult == 1)
					{
						objectsToPush->push_back(oObj);
						pushingObjects->push_back(this);
						pushed = true;
					}
				}
			}
		}
	}

	if (!collided && objectsToPush->size() > 0)
	{
		for (; currentIndex < objectsToPush->size(); currentIndex++)
		{
			if (!objectsToPush[0][currentIndex]->CanMove(dir, pushingObjects, objectsToPush))
				return false;
		}
	}

	return !collided;
}

// Engine-used function, not recommended for user-use.
bool Object::Move(Vector2D dir, Object* pushingObject)
{
	debug_relevantCollidersInArea = 0;
	
	// Find irrelevancy, move and return if found

	if (!colliderActive || // Is collider active?
		collider.size() == 0 || // Does collider have size?
		collider[0].size() == 0 ||
		parentLayer->objects.size() <= 1) // Are there more objects than the moving object in the layer?
	{
		pos.x += dir.x;
		pos.y += dir.y;
		return true;
	}

	bool blank = true;
	for (int y = 0; y < collider.size() && blank; y++) // Is the collider blank? It might be active and has size but the actual collider data might be all 0, meaning no hit box.
	{
		for (int x = 0; x < collider[y].size() && blank; x++)
		{
			if (collider[y][x])
				blank = false;
		}
	}
	if (blank)
	{
		// If the collider is blank, move straight away without checking collision and return the function.
		pos.x += dir.x;
		pos.y += dir.y;
		return true;
	}
	

	// FOR OTHER OBJECTS,
		// GO OVER MOVING OBJECT'S COLLIDER,
			// CONVERT XY OF MOVING OBJECT'S COLLIDER TO OTHER OBJECT'S COLLIDER
				// IF BOTH COLLIDERS AT XY AND CONVERTED XY ARE ON, COLLIDED!
	// CHECK OVERLAP ONLY AFTER CHECKING COLLISION TO MAKE THINGS SIMPLER

	bool collided = false;
	std::vector<Object*> objectsToPush;
	std::vector<Object*> pushingObjects;
	Object* oObj;

	for (int o = 0; o < parentLayer->objects.size() && !collided; o++)
	{
		oObj = parentLayer->objects[o];

		int collisionResult = 0;
		bool pushed = false;

		if (colliderType >= 0 && colliderType < colliderTypes.size())
		{
			if (oObj->colliderType < colliderTypes[colliderType].size())
				collisionResult = colliderTypes[colliderType][oObj->colliderType];
		}

		// Find irrelevancy, continue to next object if found
		if (oObj == this || // Same object
			oObj == pushingObject || // Avoid infinte push loop, also allowing: #@# (# (objA) is moving right, pushing @ (objB) and letting @ ignore # so it will manage to move to the right, even though # is blocking [which will move after push is confirmed]) 
			AreObjectsOverlapping(this, oObj) || // Letting object that is stuck in another move out of it.
			collisionResult == 2 || // Can overlap other object
			!oObj->colliderActive || // Collider not active
			oObj->collider.size() == 0 || // Size
			oObj->collider[0].size() == 0)/* ||
			oObj->pos.y > pos.y + collider.size() + dir.y|| // Out of reach
			oObj->pos.x > pos.x + collider[0].size() + dir.x ||
			oObj->pos.y + oObj->collider.size() < pos.y + dir.y ||
			oObj->pos.x + oObj->collider[0].size() < pos.x + dir.x)*/
			continue;

		debug_relevantCollidersInArea++;

		for (int y = 0; y < collider.size() && !collided && !pushed; y++)
		{
			int oY = y + pos.y - oObj->pos.y + dir.y;
			if (oY < 0 || oY >= oObj->collider.size()) // if converted y (oY) is not on range, try the next Y. oY should eventually be in the range because we checked irrelevancy earlier.
				continue;

			for (int x = 0; x < collider[y].size() && !collided && !pushed; x++)
			{
				int oX = x + pos.x - oObj->pos.x + dir.x;
				if (oX < 0 || oX >= oObj->collider[oY].size()) // Same as before but with x
					continue;

				if (collider[y][x] && oObj->collider[oY][oX]) // Colliding
				{
					if (collisionResult == 0) // Can't push
						collided = true;
					else if (collisionResult == 1) // Can push
					{
						objectsToPush.push_back(oObj);
						pushingObjects.push_back(this);
						pushed = true;
					}
				}
			}
		}
	}

	if (!collided)
	{
		for (size_t i = 0; i < objectsToPush.size(); i++) // Check if objects can be pushed
		{
			if (!objectsToPush[i]->CanMove(dir, &pushingObjects, &objectsToPush))
				collided = true;
		}
	}
	if (!collided) // If objects can be pushed, change pos, call functions
	{
		for (size_t i = 0; i < objectsToPush.size(); i++)
		{
			objectsToPush[i]->lastCollidedObject = pushingObjects[i]; // First call OnCollision
			if (objectsToPush[i]->OnCollision)
				objectsToPush[i]->OnCollision;
			objectsToPush[i]->pos.x += dir.x; // Then add dir to pos
			objectsToPush[i]->pos.y += dir.y;
			objectsToPush[i]->lastPush = dir; // And call OnPushed
			if (objectsToPush[i]->OnPushed)
				objectsToPush[i]->OnPushed();
		}
		pos.x += dir.x;
		pos.y += dir.y;
	}
	else // If objects can't be pushed, call OnCollision
	{
		for (size_t i = 0; i < objectsToPush.size(); i++)
		{
			objectsToPush[i]->lastCollidedObject = pushingObjects[i];
			if (objectsToPush[i]->OnCollision)
				objectsToPush[i]->OnCollision;
		}
	}

	return !collided;
}


/*
Bugs:
1:	Avoiding infinte push loop also means that when:
		 ABA
		#CB
	B moves to the left, A will move, but B won't, allowing A to overlap B:
		A A
		#CB
	Possible solution:
		The problem is that an object is pushing an object that will result in the first object getting pushed. Quite an unfortunate loop, but it would be quite nice if it worked.
		The problem rises from the probability that an object spawns overlapping upon another. If one of those objects move, and they can push each other, object A will push object B, which will realize it would need
		to push object A to move, which creates the infinite push loop. To fix it, I send a pointer to Move of object B of object A, telling it to ignore object A when moving.
		To fix it, we can check if object A and object B are overlapping from the first place and if not, don't send the pointer. But -
		| Good I written all of this useless text above, because the point I was getting to is totally irrelevant. The solution lays bellow bug 2. I gave a wrong example, where C was a block.
		B can later go through A, don't worry, that was an intentional desicion you made to let objects that overlap (by accident or not) go through each other to free themselves.
	SOLVED:
		Added system mentioned in bug #2. More details in solution #2

2:	When pushing objects, can push one object, try to push another but this time the object can't be pushed, allowing:
		AB
		AC#
	A moves to the right, B will be pushed, C won't, A won't move:
		A B
		AC#
	Possible solution:
		This means a huge tree of checking again and again if all objects will be able to be at a certan position. What if B is also infront of 2 objects, where one is blocked and the other isn't?
		The problem with this system is to make it perfect, it can possibly require a sheer amount of iterations.
		But, there could be a way to make a tree that grows only the amount needed, it just needs quite a bit of planning, and if it works, there will be potentially no more problems, because all of them rise from this.
	SOLVED:
		If you tried you could possibly make a huge tree that will be seen in the FPS counter, but it completely works now, bugs #1 and #2 are now non-existent. What I had to do to make this tree-system is actually pretty simple:
		I made a function called "CanMove", which does what Move does but without moving and calling user defined functions (like OnCollision). Diagram kinda explains it:
		Move (User called) -> CanMove (Move called) for all objects to push
							  |
							  V
							  Sends vector of pushing objects, currently only original object is there.																	
							  |																																			
							  | <-----------------------------------------------------------------------------------------------------------------------------------------\
							  V																																		      |
							  Has objects in the way? -> Yes -> Can push? -> Yes -> Call CanMove on objects to move, with the tree vector added the other pushing object -/
							  |									|
							  V									V
							  No								No -> Return false, ending the tree, can't move. -> Don't move, things are on the way which can't be pushed. END
							  |
							  V
							  Return true (can move), ending or continuing CanPush tree. -> Ended the tree? -> Push all objects to push (Move object, called by Move which was called by user). END

3:	When 2 objects that were added early push an object that was added late, both objects will push it at the same time resulting the late object to be double pushed.
		BAC
		BBC
	B pushes A, resulting in both pushing C. C was added later.
		 BA C
		 BB C
	Possible solution:
		Check before pushing if the object was already pushed. Probably recheck collision which will deal another massive down in performance.

		The current problem is that we are pushing objects we already pushed. Perhaps we need to collect all objects to push, or something similar.
		Currently, we make a tree of objects, check if they can move, if they can't, move nothing. If everything can move, move it all. But we forget the tree while constructing it. Perhaps all we need is to memorize it,
		eliminate duplicates, and move. Perhaps there is even a better way - check if all can move, preorganize function calls, then change position without recalculating everything - a smarter system, that will in advance be fast.

		Move -> If nothing blocks -> For (objects to push)
									 |
									 V *With ignore list of pushing objects
									 Can Move -> Collect tree of objects, detailing what collided what to later call OnCollision and OnPushed -> Blocked? -> Yes -> Move nothing, add current object and blocking object to tree.
																																				 |
																																				 V
																																				 No
																																				 |
																																				 V
																																			---- Pushable? -> Yes -> For (objects to push) -> ...
																																			|	 |
																																			V	 V
																															Last in the tree? <- Nothing in the way? -> Return up in the tree, add current object to pushable.
																															|
																															V
																															For (tree)
																															|
																															V
	SOLVED
		blockmoving is now the new way to move multiple objects at the same time. It works great and it's bugless (so far).

		  %%
		@@%%^^
		@@##^^
		@@##

		@ @ %
		% # ^

		  ##
		######
		######
		####
*/