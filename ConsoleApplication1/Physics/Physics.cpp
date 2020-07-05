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

physx::PxMaterial* Physics::mMaterial; // Default material
physx::PxDefaultCpuDispatcher* Physics::mDispatcher;
physx::PxScene* Physics::mScene;

void Physics::Initialise()
{
	mFoundation = nullptr;
	mPvd = nullptr;
	mPhysics = nullptr;
	mCooking = nullptr;
	mMaterial = nullptr;
	mDispatcher = nullptr;
	mScene = nullptr;

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

	// Initialise the scene
	physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	mDispatcher = physx::PxDefaultCpuDispatcherCreate(2); // 2 thread cpu dispatcher

	sceneDesc.cpuDispatcher = mDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	mScene = mPhysics->createScene(sceneDesc);

	physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// Create default material
	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
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

void Physics::RunStep(float deltaTime)
{
	if (!mScene) {
		return;
	}

	mScene->simulate(deltaTime);
	mScene->fetchResults(true);
}