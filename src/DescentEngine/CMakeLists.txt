cmake_minimum_required (VERSION 2.6)
project (Descent)

include_directories( ${Descent_SOURCE_DIR}/src )
include ( ../CMake_Externals.txt )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBT_NO_PROFILE=1" )

add_library(DescentEngine SHARED
	external/tinyxml2/tinyxml2.cpp

	src/Log.cpp
	src/Engines.cpp
	src/RenderEngine.cpp
	src/GameLoop.cpp
	
	src/Performance/SectionTimer.cpp
	
	src/Pathfinding/Pathfinding.cpp
	
	src/ResourceEngine/ResourceEngine.cpp
	src/ResourceEngine/ResourceEngineSDL.cpp
	
	src/SoundEngine/SoundEngineSDL.cpp
	src/Physics/PhysicsEngine.cpp
	src/Physics/EntityRigidBodyState.cpp
	
	src/Input/SDLInput.cpp
	
	src/SDL_OpenGL/SDLOpenGLInterface.cpp
	src/Common_OpenGL/OpenGL_ES_Render.cpp
	
	src/Visuals/SpriteVisual.cpp
	src/Visuals/ParticleSystemVisual.cpp
	src/Visuals/TextVisual.cpp
	
	src/EntityEngine/EntityEngine.cpp
	src/EntityEngine/Entity.cpp
	src/EntityEngine/EntityTemplate.cpp
	src/EntityEngine/MultiVisualEntity.cpp

	src/AnimationEngine/AnimationEngine.cpp
	src/AnimationEngine/EntityAnimation.cpp
	src/AnimationEngine/ParticleAnimation.cpp
	src/AnimationEngine/TextAnimation.cpp
	
	src/CinematicEngine/CinematicEngine.cpp

	src/ScriptEngine/ScriptEngine.cpp

	#bullet
	external/bullet/LinearMath/btAlignedAllocator.cpp
	#external/bullet/LinearMath/btHashMap.cpp

	#external/bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp
	#external/bullet/BulletCollision/BroadphaseCollision/btDispatcher.cpp
	#external/bullet/BulletCollision/BroadphaseCollision/btDbvt.cpp
	#external/bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp
	#external/bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp
	
	#external/bullet/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp
	#external/bullet/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp
	#external/bullet/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp
	#external/bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp
	#external/bullet/BulletCollision/CollisionDispatch/btCollisionObject.cpp
	#external/bullet/BulletCollision/CollisionDispatch/btCollisionWorld.cpp
	#external/bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp
	#external/bullet/BulletCollision/CollisionDispatch/btManifoldResult.cpp

	#external/bullet/BulletCollision/CollisionShapes/btBoxShape.cpp
	#external/bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp
	#external/bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp
	#external/bullet/BulletCollision/CollisionShapes/btConcaveShape.cpp
	#external/bullet/BulletCollision/CollisionShapes/btConvexShape.cpp
	#external/bullet/BulletCollision/CollisionShapes/btConvexInternalShape.cpp
	#external/bullet/BulletCollision/CollisionShapes/btCollisionShape.cpp
	#external/bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp
	#external/bullet/BulletCollision/CollisionShapes/btSphereShape.cpp
	#external/bullet/BulletCollision/CollisionShapes/btTriangleCallback.cpp

	#external/bullet/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp
	#external/bullet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp
	#external/bullet/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp
	#external/bullet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp
	#external/bullet/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp
	#external/bullet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp
	#external/bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp

	#external/bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp

	#external/bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp
	#external/bullet/BulletDynamics/Dynamics/btRigidBody.cpp
	
external/bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp
external/bullet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp
external/bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp
external/bullet/BulletCollision/BroadphaseCollision/btDbvt.cpp
external/bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp
external/bullet/BulletCollision/BroadphaseCollision/btDispatcher.cpp
external/bullet/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp
external/bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp
external/bullet/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp
external/bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp
external/bullet/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp
external/bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp
external/bullet/BulletCollision/CollisionDispatch/btCollisionObject.cpp
external/bullet/BulletCollision/CollisionDispatch/btCollisionWorld.cpp
external/bullet/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp
external/bullet/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btGhostObject.cpp
external/bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp
external/bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.h
external/bullet/BulletCollision/CollisionDispatch/btManifoldResult.cpp
external/bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp
external/bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp
external/bullet/BulletCollision/CollisionDispatch/btUnionFind.cpp
external/bullet/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp
external/bullet/BulletCollision/CollisionShapes/btBoxShape.cpp
external/bullet/BulletCollision/CollisionShapes/btBox2dShape.cpp
external/bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp
external/bullet/BulletCollision/CollisionShapes/btCapsuleShape.cpp
external/bullet/BulletCollision/CollisionShapes/btCollisionShape.cpp
external/bullet/BulletCollision/CollisionShapes/btCompoundShape.cpp
external/bullet/BulletCollision/CollisionShapes/btConcaveShape.cpp
external/bullet/BulletCollision/CollisionShapes/btConeShape.cpp
external/bullet/BulletCollision/CollisionShapes/btConvexHullShape.cpp
external/bullet/BulletCollision/CollisionShapes/btConvexInternalShape.cpp
external/bullet/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp
external/bullet/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp
external/bullet/BulletCollision/CollisionShapes/btConvexShape.cpp
external/bullet/BulletCollision/CollisionShapes/btConvex2dShape.cpp
external/bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp
external/bullet/BulletCollision/CollisionShapes/btCylinderShape.cpp
external/bullet/BulletCollision/CollisionShapes/btEmptyShape.cpp
external/bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp
external/bullet/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp
external/bullet/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp
external/bullet/BulletCollision/CollisionShapes/btMultiSphereShape.cpp
external/bullet/BulletCollision/CollisionShapes/btOptimizedBvh.cpp
external/bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp
external/bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp
external/bullet/BulletCollision/CollisionShapes/btShapeHull.cpp
external/bullet/BulletCollision/CollisionShapes/btSphereShape.cpp
external/bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp
external/bullet/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp
external/bullet/BulletCollision/CollisionShapes/btTetrahedronShape.cpp
external/bullet/BulletCollision/CollisionShapes/btTriangleBuffer.cpp
external/bullet/BulletCollision/CollisionShapes/btTriangleCallback.cpp
external/bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp
external/bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp
external/bullet/BulletCollision/CollisionShapes/btTriangleMesh.cpp
external/bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp
external/bullet/BulletCollision/CollisionShapes/btUniformScalingShape.cpp
external/bullet/BulletCollision/Gimpact/btContactProcessing.cpp
external/bullet/BulletCollision/Gimpact/btGenericPoolAllocator.cpp
external/bullet/BulletCollision/Gimpact/btGImpactBvh.cpp
external/bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp
external/bullet/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp
external/bullet/BulletCollision/Gimpact/btGImpactShape.cpp
external/bullet/BulletCollision/Gimpact/btTriangleShapeEx.cpp
external/bullet/BulletCollision/Gimpact/gim_box_set.cpp
external/bullet/BulletCollision/Gimpact/gim_contact.cpp
external/bullet/BulletCollision/Gimpact/gim_memory.cpp
external/bullet/BulletCollision/Gimpact/gim_tri_collision.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp
external/bullet/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp

external/bullet/BulletDynamics/Character/btKinematicCharacterController.cpp
external/bullet/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp
external/bullet/BulletDynamics/ConstraintSolver/btContactConstraint.cpp
external/bullet/BulletDynamics/ConstraintSolver/btGearConstraint.cpp
external/bullet/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp
external/bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp
external/bullet/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp
external/bullet/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp
external/bullet/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp
external/bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
external/bullet/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp
external/bullet/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp
external/bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp
external/bullet/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp
external/bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp
external/bullet/BulletDynamics/Dynamics/btRigidBody.cpp
external/bullet/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp
external/bullet/BulletDynamics/Dynamics/Bullet-C-API.cpp
external/bullet/BulletDynamics/Vehicle/btRaycastVehicle.cpp
external/bullet/BulletDynamics/Vehicle/btWheelInfo.cpp

external/bullet/LinearMath/btAlignedAllocator.cpp
external/bullet/LinearMath/btConvexHull.cpp
external/bullet/LinearMath/btConvexHullComputer.cpp
external/bullet/LinearMath/btGeometryUtil.cpp
external/bullet/LinearMath/btPolarDecomposition.cpp
external/bullet/LinearMath/btQuickprof.cpp
external/bullet/LinearMath/btSerializer.cpp
external/bullet/LinearMath/btVector3.cpp

external/lua/lapi.c
external/lua/lauxlib.c
external/lua/lbaselib.c
external/lua/lbitlib.c
external/lua/lcode.c
external/lua/lcorolib.c
external/lua/lctype.c
external/lua/ldblib.c
external/lua/ldebug.c
external/lua/ldo.c
external/lua/ldump.c
external/lua/lfunc.c
external/lua/lgc.c
external/lua/linit.c
external/lua/liolib.c
external/lua/llex.c
external/lua/lmathlib.c
external/lua/lmem.c
external/lua/loadlib.c
external/lua/lobject.c
external/lua/lopcodes.c
external/lua/loslib.c
external/lua/lparser.c
external/lua/lstate.c
external/lua/lstring.c
external/lua/lstrlib.c
external/lua/ltable.c
external/lua/ltablib.c
external/lua/ltm.c
# the compiler ...
#external/lua/luac.c
external/lua/lundump.c
external/lua/lvm.c
external/lua/lzio.c

external/luabind/src/class_info.cpp
external/luabind/src/class_registry.cpp
external/luabind/src/class_rep.cpp
external/luabind/src/class.cpp
external/luabind/src/create_class.cpp
external/luabind/src/error.cpp
external/luabind/src/exception_handler.cpp
external/luabind/src/function.cpp
external/luabind/src/inheritance.cpp
external/luabind/src/link_compatibility.cpp
external/luabind/src/object_rep.cpp
external/luabind/src/open.cpp
external/luabind/src/pcall.cpp
external/luabind/src/scope.cpp
external/luabind/src/stack_content_by_name.cpp
external/luabind/src/weak_ref.cpp
external/luabind/src/wrapper_base.cpp

external/inotify-cxx/inotify-cxx.cpp
)

target_link_libraries( DescentEngine SDL SDL_image SDL_mixer )

if(UNIX)
	target_link_libraries( DescentEngine GL )
endif()

if(WIN32)
	target_link_libraries( DescentEngine opengl32 )
endif()
 

