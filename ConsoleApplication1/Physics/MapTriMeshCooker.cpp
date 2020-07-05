#include <iostream>

#include "MapTriMeshCooker.h"
#include "Physics.h"

void MapTriMeshCooker::BuildMapPhysicsMesh(Mapfile &map)
{
	physx::PxCooking* cooker = Physics::GetPxCooking();
	physx::PxPhysics* physics = Physics::GetPxPhysics();
	if (!cooker) {
		std::cout << "Error: BuildMapPhysicsMesh: No mesh cooker available" << std::endl;
		return;
	}

	// First we need to get the vert/tri count
	physx::PxU32 numVertices = 0;
	physx::PxU32 numTriangles = 0;

	for (const mapentity_t& ent : map.GetEntities()) {
		for (const mapbrush_t& brush : ent.brushes) {
			for (const mapface_t& face : brush.faces) {
				if (face.texture == "textures/skip" || face.texture == "textures/trigger") {
					continue;
				}

				if (!face.vertices_calculated.size()) {
					continue;
				}

				for (size_t i = 1; i < (int)(face.vertices_calculated.size()) - 1; i++) {
					numVertices += 3;
					numTriangles += 1;
				}
			}
		}
	}

	physx::PxVec3 *vertices = new physx::PxVec3[numVertices];
	physx::PxVec3 *triangles = new physx::PxVec3[numTriangles];
	physx::PxU32* indices = new physx::PxU32[numTriangles * 3];

	size_t vertOffset = 0;
	size_t triOffset = 0;

	for (const mapentity_t& ent : map.GetEntities()) {
		for (const mapbrush_t& brush : ent.brushes) {
			for (const mapface_t& face : brush.faces) {
				if (face.texture == "textures/skip" || face.texture == "textures/trigger") {
					continue;
				}

				if (!face.vertices_calculated.size()) {
					continue;
				}

				for (size_t i = 1; i < (int)(face.vertices_calculated.size()) - 1; i++) {
					vertices[vertOffset] = physx::PxVec3(face.vertices_calculated[0].x, face.vertices_calculated[0].y, face.vertices_calculated[0].z);
					vertices[vertOffset+1] = physx::PxVec3(face.vertices_calculated[i].x, face.vertices_calculated[i].y, face.vertices_calculated[i].z);
					vertices[vertOffset+2] = physx::PxVec3(face.vertices_calculated[i+1].x, face.vertices_calculated[i+1].y, face.vertices_calculated[i+1].z);

					indices[vertOffset] = (physx::PxU32)vertOffset;
					indices[vertOffset+1] = (physx::PxU32)vertOffset+1;
					indices[vertOffset+2] = (physx::PxU32)vertOffset+2;

					vertOffset += 3;
					triOffset += 1;
				}
			}
		}
	}

	// Setup the cooking parameters
	physx::PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = numVertices;
	meshDesc.points.data = vertices;
	meshDesc.points.stride = sizeof(physx::PxVec3);
	meshDesc.triangles.count = numTriangles;
	meshDesc.triangles.data = indices;
	meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
	
	physx::PxCookingParams params = cooker->getParams();
	params.suppressTriangleMeshRemapTable = true;
	params.meshPreprocessParams &= ~static_cast<physx::PxMeshPreprocessingFlags>(physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH);
	params.meshPreprocessParams &= ~static_cast<physx::PxMeshPreprocessingFlags>(physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE);
	params.midphaseDesc = physx::PxMeshMidPhase::eBVH33;
	params.midphaseDesc.mBVH33Desc.meshCookingHint = physx::PxMeshCookingHint::eCOOKING_PERFORMANCE;
	params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff = 0.55f;

	cooker->setParams(params);

	physx::PxTriangleMesh* triMesh = nullptr;
	physx::PxU32 meshSize = 0;

	triMesh = cooker->createTriangleMesh(meshDesc, physics->getPhysicsInsertionCallback());

	// Add to scene FIXME move elsewhere
	physx::PxRigidDynamic* meshActor = physics->createRigidDynamic(physx::PxTransform(1.0f, 1.0f, 1.0f));
	physx::PxShape* meshShape;
	if (meshActor)
	{
		meshActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

		physx::PxTriangleMeshGeometry triGeom;
		triGeom.triangleMesh = triMesh;
		meshShape = physx::PxRigidActorExt::createExclusiveShape(*meshActor, triGeom,
			*Physics::GetDefaultMat());

		Physics::GetScene()->addActor(*meshActor);
	}

}