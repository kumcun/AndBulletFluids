#ScreenSpaceFluidRendererGL.cpp
#FrameBufferGL.cpp
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libdemobullet
LOCAL_CFLAGS    += -DANDROID
LOCAL_SRC_FILES := Demo.cpp \
FrameBufferGL.cpp \
ScreenSpaceFluidRendererGL.cpp \
Plane.cpp \
Bullet/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
Bullet/BulletCollision/CollisionShapes/btConvexHullShape.cpp \
Bullet/BulletCollision/CollisionShapes/btSphereShape.cpp \
Bullet/BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
Bullet/BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
Bullet/BulletCollision/CollisionShapes/btCollisionShape.cpp \
Bullet/BulletCollision/CollisionShapes/btConeShape.cpp \
Bullet/BulletCollision/CollisionShapes/btCylinderShape.cpp \
Bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
Bullet/BulletCollision/CollisionShapes/btEmptyShape.cpp \
Bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
Bullet/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp \
Bullet/BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
Bullet/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
Bullet/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
Bullet/BulletCollision/CollisionShapes/btShapeHull.cpp \
Bullet/BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
Bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
Bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
Bullet/BulletCollision/CollisionShapes/btCapsuleShape.cpp \
Bullet/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
Bullet/BulletCollision/CollisionShapes/btCompoundShape.cpp \
Bullet/BulletCollision/CollisionShapes/btConvexShape.cpp \
Bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
Bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
Bullet/BulletCollision/CollisionShapes/btTriangleMesh.cpp \
Bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
Bullet/BulletCollision/CollisionShapes/btTriangleCallback.cpp \
Bullet/BulletCollision/CollisionShapes/btConcaveShape.cpp \
Bullet/BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
Bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
Bullet/BulletCollision/CollisionShapes/btBox2dShape.cpp \
Bullet/BulletCollision/CollisionShapes/btBoxShape.cpp \
Bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
Bullet/BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
Bullet/BulletCollision/CollisionShapes/btConvex2dShape.cpp \
Bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp \
Bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
Bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
Bullet/BulletCollision/BroadphaseCollision/btDispatcher.cpp \
Bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
Bullet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
Bullet/BulletCollision/BroadphaseCollision/btDbvt.cpp \
Bullet/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
Bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
Bullet/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp \
Bullet/BulletCollision/Gimpact/gim_tri_collision.cpp \
Bullet/BulletCollision/Gimpact/gim_box_set.cpp \
Bullet/BulletCollision/Gimpact/gim_contact.cpp \
Bullet/BulletCollision/Gimpact/btTriangleShapeEx.cpp \
Bullet/BulletCollision/Gimpact/btGenericPoolAllocator.cpp \
Bullet/BulletCollision/Gimpact/btContactProcessing.cpp \
Bullet/BulletCollision/Gimpact/btGImpactBvh.cpp \
Bullet/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp \
Bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp \
Bullet/BulletCollision/Gimpact/btGImpactShape.cpp \
Bullet/BulletCollision/Gimpact/gim_memory.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
Bullet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
Bullet/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
Bullet/BulletCollision/CollisionDispatch/btUnionFind.cpp \
Bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
Bullet/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
Bullet/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
Bullet/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
Bullet/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
Bullet/BulletCollision/CollisionDispatch/btManifoldResult.cpp \
Bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
Bullet/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
Bullet/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
Bullet/BulletCollision/CollisionDispatch/btCollisionObject.cpp \
Bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
Bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
Bullet/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
Bullet/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
Bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
Bullet/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
Bullet/BulletCollision/CollisionDispatch/btGhostObject.cpp \
Bullet/BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
Bullet/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
Bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
Bullet/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
Bullet/BulletMultiThreaded/btThreadSupportInterface.cpp \
Bullet/BulletMultiThreaded/SpuLibspe2Support.cpp \
Bullet/BulletMultiThreaded/SpuCollisionObjectWrapper.cpp \
Bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuCollisionShapes.cpp \
Bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/boxBoxDistance.cpp \
Bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.cpp \
Bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuContactResult.cpp \
Bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuMinkowskiPenetrationDepthSolver.cpp \
Bullet/BulletMultiThreaded/SpuSampleTask/SpuSampleTask.cpp \
Bullet/BulletMultiThreaded/SpuSampleTaskProcess.cpp \
Bullet/BulletMultiThreaded/btParallelConstraintSolver.cpp \
Bullet/BulletMultiThreaded/Win32ThreadSupport.cpp \
Bullet/BulletMultiThreaded/SequentialThreadSupport.cpp \
Bullet/BulletMultiThreaded/SpuFakeDma.cpp \
Bullet/BulletMultiThreaded/SpuContactManifoldCollisionAlgorithm.cpp \
Bullet/BulletMultiThreaded/PosixThreadSupport.cpp \
Bullet/BulletMultiThreaded/SpuCollisionTaskProcess.cpp \
Bullet/BulletMultiThreaded/btGpu3DGridBroadphase.cpp \
Bullet/BulletMultiThreaded/SpuGatheringCollisionDispatcher.cpp \
Bullet/LinearMath/btGeometryUtil.cpp \
Bullet/LinearMath/btConvexHullComputer.cpp \
Bullet/LinearMath/btConvexHull.cpp \
Bullet/LinearMath/btQuickprof.cpp \
Bullet/LinearMath/btSerializer.cpp \
Bullet/LinearMath/btAlignedAllocator.cpp \
Bullet/LinearMath/btPolarDecomposition.cpp \
Bullet/LinearMath/btVector3.cpp \
Bullet/BulletSoftBody/btSoftBodyHelpers.cpp \
Bullet/BulletSoftBody/btDefaultSoftBodySolver.cpp \
Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp \
Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.cpp \
Bullet/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp \
Bullet/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp \
Bullet/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp \
Bullet/BulletSoftBody/btSoftBody.cpp \
Bullet/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
Bullet/BulletDynamics/ConstraintSolver/btGearConstraint.cpp \
Bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
Bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
Bullet/BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
Bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp \
Bullet/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
Bullet/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
Bullet/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
Bullet/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
Bullet/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
Bullet/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
Bullet/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
Bullet/BulletDynamics/Vehicle/btWheelInfo.cpp \
Bullet/BulletDynamics/Vehicle/btRaycastVehicle.cpp \
Bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
Bullet/BulletDynamics/Dynamics/btRigidBody.cpp \
Bullet/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
Bullet/BulletDynamics/Dynamics/Bullet-C-API.cpp \
Bullet/BulletDynamics/Character/btKinematicCharacterController.cpp
BULLET_FLUIDS_FILE_LIST := $(wildcard $(LOCAL_PATH)/BulletFluids/*.cpp) \
			$(wildcard $(LOCAL_PATH)/BulletFluids/CollisionDispatch_temp/*.cpp) \
			$(wildcard $(LOCAL_PATH)/BulletFluids/Hf/*.cpp) \
			$(wildcard $(LOCAL_PATH)/BulletFluids/Sph/*.cpp) \
			$(wildcard $(LOCAL_PATH)/BulletFluids/Sph/Experimental/*.cpp)
LOCAL_SRC_FILES += $(BULLET_FLUIDS_FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES += $(LOCAL_PATH) $(LOCAL_PATH)/External/ $(LOCAL_PATH)/Bullet/ $(LOCAL_PATH)/BulletFluids/ $(LOCAL_PATH)/BulletFluids/
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv3

LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
