#pragma once

#include "PxPhysicsAPI.h"

#include "extensions/PxDefaultAllocator.h" // 16 aligned memory allocator
#include "extensions/PxDefaultErrorCallback.h" // default error callback



class Physics {
public:
	static void Initialise();

	static void Shutdown();

	static void RunStep(float deltaTime);

	inline static physx::PxPhysics* GetPxPhysics() { return mPhysics; }

	inline static physx::PxCooking* GetPxCooking() { return mCooking; }

	inline static physx::PxScene* GetScene() { return mScene; }

	inline static physx::PxMaterial* GetDefaultMat() { return mMaterial; }


protected:

	static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;
	static physx::PxFoundation* mFoundation;
	static physx::PxPvd* mPvd;
	static physx::PxPhysics* mPhysics;
	static physx::PxCooking* mCooking;

	static physx::PxMaterial* mMaterial; // Default material
	static physx::PxDefaultCpuDispatcher* mDispatcher;
	static physx::PxScene* mScene;
};