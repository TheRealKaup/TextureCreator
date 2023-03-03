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
	return false;
}

/*
bool Engine::Object::CanMove(Vector2D dir, std::vector<Object*>* pushingObjects, std::vector<Object*>* objectsToPush, std::vector<Object*>* blockingObjectd)
{
	// Search for the object in the blocking list.
	// If it's there, check if any collider can result in a push.
	//		If something can push, proceed into checking it out.
	//		If nothing can push, ignore, because nothing can undo the block.

	Object* oObj;

	// For other objects in layer
	for (int o = 0; o < parentLayer->objects.size(); o++)
	{
		oObj = parentLayer->objects[o];

		// If object is in pushingObjects, ignore because already moving + prevent infinite push sequence
		// If object is in objectsToPush, ignore because alreayd moving
		bool ignore = false;
		for (size_t i = 0; i < pushingObjects->size(), !ignore; i++)
			ignore = (oObj == (*pushingObjects)[i] || oObj == (*objectsToPush)[i]);
		if (ignore)
			continue;



		if (colliderType >= 0 && colliderType < colliderTypes.size())
		{
			if (oObj->colliderType < colliderTypes[colliderType].size())
				collisionResult = colliderTypes[colliderType][oObj->colliderType];
		}
		if (collisionResult == 2)
			continue;
		
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
*/

bool Object::Move(Vector2D dir)
{
	// Only supporting SIMPLE COLLLIDERS.

	bool collided = false;
	std::vector<Object*> objectsToPush; // Objects that will be pushed.
	std::vector<Object*> pushingObjects; // The objects that push the objects that will be pushed.
	// These are INDEX!
	std::vector<size_t> collidersToPush; // Colliders that will be pushed (used for event calling).
	std::vector<size_t> pushingColliders; // Colliders that push the colliders that will be pushed (used for event calling).
	std::vector<Object*> blockingObjects; // The objects that currently block, and will be checked if could be pushed.
	Object* oObj; // other object
	Collider* cldr; // collider
	Collider* oCldr; // other collider

	/*
	Summery:
	Check for possible collisions with all the colliders of the object with other colliders of other objects.
	If a collider is futured to collider with an object, check if the other collider blocks, or the collider can push the other collider.
	If the collider is getting blocked, add the other object to the blockingObjects list. That list represents all the objects that could be
		blocking. Could be because there might be another collider that can push that object.
		If at the end of the tree there are still blockingObjects, then the object can't be moved.
	If the collider can push, add the other object to the objectsToPush list. This list is just to memorize what to push later.
	
	Go through: OTHER OBJECTS. We want to eliminate the objects in the layer one by one.
		Go through: COLLIDERS. We want to use the moving object's colliders against the other object's colliders.
			Go throught: OTHER COLLIDERS. Do whatever is necessary to know the result of collision.
				The moment there is a push result, the object is pushed, and move to the next object.
				If there is a block, continue searching for a push.
					If there's a block and no push, pass the blocking object into the tree to hope that an object in objectsToPush can push it.
	*/
	

	// Other other objects
	for (size_t o = 0; o < parentLayer->objects.size(); o++)
	{
		oObj = parentLayer->objects[o];

		// Continue if it is the same object
		if (oObj == this)
			continue;
		
		// 2=overlap, 1=push, 0=block
		// The moving object will test all of its colliders in order to try to push the other object.
		// That means that there is a result per object, not per collider.
		// At the end of going through all the other object's collider
		unsigned char result = 2;
		size_t c = 0;
		size_t oc = 0;
		// Colliders
		for (c = 0; c < colliders.size(); c++)
		{
			cldr = &colliders[c];

			// Continue if the collider isn't active or 0
			if (!cldr->active || cldr->size.x == 0 || cldr->size.y == 0)
				continue;

			// Other object's colliders
			for (oc = 0; oc < oObj->colliders.size(); oc++)
			{
				oCldr = &oObj->colliders[oc];

				// Continue if the other collider isn't active or 0
				if (!oCldr->active || oCldr->size.x == 0 || oCldr->size.y == 0)
					continue;

				// Continue if the result of collison will be overlap (overlapping is not an event and is ignored).
				unsigned char collisionResult = 2;
				if (cldr->type >= 0 && cldr->type < colliderTypes.size())
				{
					if (oCldr->type >= 0 && oCldr->type < colliderTypes[cldr->type].size())
						collisionResult = colliderTypes[cldr->type][oCldr->type];
				}
				if (collisionResult == 2 || (result == 0 && collisionResult == 0)) // Continue also if there already is a block, and next collider could potenially block too (which is meaningless at this point).
					continue;

				// Comparing the distance between the colliders size can answer if the colliders will be
				// in each other (collision)
				int cldrPosX = cldr->pos.x + pos.x + dir.x;
				int oCldrPosX = oCldr->pos.x + oObj->pos.x;
				int cldrPosY = cldr->pos.y + pos.y + dir.y;
				int oCldrPosY = oCldr->pos.y + oObj->pos.y;

				if (cldrPosX < oCldrPosX + oCldr->size.x && cldrPosX + cldr->size.x > oCldrPosX
					&& cldrPosY < oCldrPosY + oCldr->size.y && cldrPosY + cldr->size.y > oCldrPosY)
				{
					// Collision!

					// Note to where I was when I left (I think..?):
					// I think I finished this point in the function, which is when collision has been detected and
					// now it is time to add the object to the blocking/pushed list.
					// Seems like the next thing to do is process objectsToPush and hope that blockingObjects will be cleared
					// throughout the pushing tree. Note that currently I am not memorizing which exact colliders are pushing and 
					// being pushed, and instead only the objects (the groups). I also don't support complex colliders RN, but for testing
					// the new system of tree, simple colliders are just fine.

					// At the end of the other object iteration, result can only be:
					// 2 = Unset, which means the object is completely overlappable or out of reach.
					// 1 = To push, and the moment (result = collisionResult) == 1, the iteration is ended (break), so it won't be accidently set to something else.
					// 0 = Blocking, which means all of the colliders of other object were checked, and non of them can be pushed.
					result = collisionResult;
					// Can push
					if (collisionResult == 1)
						break;
				}
			}
		}

		if (result == 0)
			blockingObjects.push_back(oObj); // This object was concluded to be blocking. Pass this list to objectsToPush and maybe they can push these and allow movement.
		else if (result == 1)
		{
			objectsToPush.push_back(oObj); // This is the list of objects we wish to try to push to clear path for the original object to move.
			pushingObjects.push_back(this); // This is the list of objects THAT PUSH. This is used to prevent an infinite push sequence.
			collidersToPush.push_back(c); // This is the list of colliders that were found as can be pushed. Used for calling user-defined functions later.
			pushingColliders.push_back(oc); // This is the list of the colliders that were able to push the pushed colliders. Used for calling user-defined functions later.
		}
	}

	for (size_t i = 0; i < objectsToPush.size(); i++)
	{
		break; // disabled for testing
		// NOTE: not passing collidersToPush and pushingColliders rn.
		//objectsToPush[i]->CanMove(dir, &pushingObjects, &objectsToPush, &blockingObjects);
		// try to move objectsToPush and don't forget to ask for those objects to push blockingObjects!
	
		// Throughout the for-loop objects will be removed/added to blockingObjects.
	}

	// No objects are blocking. They were never found OR were all pushed.
	if (blockingObjects.size() == 0)
	{
		// Move and call OnEvents
		for (size_t i = 0; i < objectsToPush.size(); i++)
		{
			objectsToPush[i]->pos.x += dir.x;
			objectsToPush[i]->pos.y += dir.y;
			objectsToPush[i]->lastPush = dir;
			objectsToPush[i]->theOtherObject = pushingObjects[i];
			objectsToPush[i]->theColliderIndex = collidersToPush[i];
			objectsToPush[i]->theOtherColliderIndex = pushingColliders[i];
			if (objectsToPush[i]->OnPushed)
				objectsToPush[i]->OnPushed();

			pushingObjects[i]->lastPush = dir;
			pushingObjects[i]->theOtherObject = objectsToPush[i];
			pushingObjects[i]->theColliderIndex = pushingColliders[i];
			pushingObjects[i]->theOtherColliderIndex = collidersToPush[i];
			if (pushingObjects[i]->OnPush)
				pushingObjects[i]->OnPush();
			// OnEvent functions are called only for objects that are pushed or blocking.
		}
		pos.x += dir.x;
		pos.y += dir.y;

		return true;
	}

	// Failed to move, blocked.
	return false;
}

/*
Since an object has multiple colliders, things can't work like they worked before. And in addition, now there are 2 types
of colliders, simple and complex. Simple is a rectangle which is faster to process, and complex is a 2D bool array, which is slower
to process but allows complex shapes.

Idea:

Object is a gruop. If one of the colliders gets pushed, all of the other colliders in the group get pushed too.

If another collider is blocking the collider, the reference to the other collider should be the other object, becuase if any
of the other object's collider manage to get pushed, then the whole object moves.

So it is really just like an object moves with one collider, except the collider is split into different types, allowing
different positioning to affect the movement differently.


The first loop (Object::Move) is the loop that checks if any other objects are in the way of the object.
	If a blocking other object is found add the other collider (for proper OnEvent calling) and other object (for possible pushing). 
	If a pushable other object is found add the other collider and the other object to try push.
	If an overlappable other object is found, ignore.

	What's important here is that if we get to another object that got other colliders infront of the colliders,
		If one of the colliders can push any of the other colliders, then the other object will be pushed, memories it to try pushing it later.
		If non of the colliders can push any of the other colliders, then the other object is blocking, memories it to hope for another object that can push it.
	It's like the object is smart and searches for the best collider to push the other object.

At the end, there should be no blocking objects

Objects that are already in the pushing sequence should be ignored.


bool AreCollidersOverlapping(Collider* collider, Vector2D dir, Collider* otherCollider)

bool Move()
{
	std::vector<Collider*> blockingColliders;

	For (colliders)
	{
		For (other objects)
		{
			For (other_object.collider)
			{
				If (collisionResult == 2) // Overlapped
					continue;
				If (AreCollidersOverlapping(collider, dir, other_collider))
				{
					If (collisionResult == 0) // Blocked
					{
						blockingColliders.pushBack(other_collider);
					}
				}
			}
		}
	}
}

*/


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