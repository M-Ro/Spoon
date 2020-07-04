#pragma once

#include "PxPhysicsAPI.h"

#include "extensions/PxDefaultAllocator.h" // 16 aligned memory allocator
#include "extensions/PxDefaultErrorCallback.h" // default error callback



class Physics {
public:
	static void Initialise();

	static void Shutdown();

protected:

	static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;
	static physx::PxFoundation* mFoundation;
	static physx::PxPvd* mPvd;
	static physx::PxPhysics* mPhysics;
	static physx::PxCooking* mCooking;
};