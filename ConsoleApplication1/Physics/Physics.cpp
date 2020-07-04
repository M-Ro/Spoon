#include <iostream>

#include "Physics.h"

#include "pvd/PxPvd.h"

#define PVD_HOST "127.0.0.1"

physx::PxDefaultErrorCallback Physics::gDefaultErrorCallback;
physx::PxDefaultAllocator Physics::gDefaultAllocatorCallback;

physx::PxFoundation* Physics::mFoundation;

physx::PxPvd* Physics::mPvd;
physx::PxPhysics* Physics::mPhysics;
physx::PxCooking* Physics::mCooking;

void Physics::Initialise()
{
	mFoundation = nullptr;
	mPvd = nullptr;
	mPhysics = nullptr;
	mCooking = nullptr;

	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

	if (!mFoundation) {
		std::cout << "PhysX: PxCreateFoundation failed!" << std::endl;
		return;
	}

	bool recordMemoryAllocations = true;

#ifdef _DEBUG
	mPvd = PxCreatePvd(*mFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
		physx::PxTolerancesScale(), recordMemoryAllocations, mPvd);
	if (!mPhysics) {
		std::cout << "Physx: PxCreatePhysics failed!" << std::endl;
		return;
	}

	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, physx::PxCookingParams(physx::PxTolerancesScale()));
	if (!mCooking) {
		std::cout << "Physx: PxCreateCooking failed!" << std::endl;
		return;
	}

	// Initalise extensions, no idea if we need these yet
	if (!PxInitExtensions(*mPhysics, mPvd)) {
		std::cout << "Physx: PxInitExtensions failed!" << std::endl;
		return;
	}
}

void Physics::Shutdown()
{
	PxCloseExtensions();

	if (mCooking) {
		mCooking->release();
	}

	if (mPhysics) {
		mPhysics->release();
	}

	if (mPvd) {
		mPvd->release();
	}

	if (mFoundation) {
		mFoundation->release();
	}
}