CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )

PROJECT( load3dmodel )

INCLUDE_DIRECTORIES(./include)
#INCLUDE_DIRECTORIES(F:/libtools/gl/glad/include)
#INCLUDE_DIRECTORIES(F:/libtools/gl/glfw-3.3.2/include)
#INCLUDE_DIRECTORIES(F:/libtools/gl/glm)
#INCLUDE_DIRECTORIES(F:/libtools/gl/assimp-5.0.0/include/)
INCLUDE_DIRECTORIES(F:/Project/opengl/LearnOpenGL/includes/)

FILE(GLOB_RECURSE INCS "*.h" "*.hpp")

#SOURCE_GROUP("Include" FILES ${INCS} )

AUX_SOURCE_DIRECTORY(./src SRC)
AUX_SOURCE_DIRECTORY(F:/libtools/gl/glad/src SRC)


#SET( OpenCV_DIR F:/libtools/opencv-2.4.13/build )
SET( OpenCV_DIR F:/libtools/opencv-4.3.0/build )
FIND_PACKAGE( OpenCV REQUIRED ) 


ADD_EXECUTABLE(load3dmodel ${SRC} ${INCS})

TARGET_LINK_LIBRARIES( load3dmodel debug F:/libtools/gl/glfw-3.3.2/build/src/Debug/glfw3.lib )
TARGET_LINK_LIBRARIES( load3dmodel optimized F:/libtools/gl/glfw-3.3.2/build/src/Release/glfw3.lib )

TARGET_LINK_LIBRARIES( load3dmodel debug F:/libtools/gl/assimp-5.0.0/build/code/Debug/assimp-vc141-mtd.lib )
TARGET_LINK_LIBRARIES( load3dmodel optimized F:/libtools/gl/assimp-5.0.0/build/code/Release/assimp-vc141-mt.lib )

TARGET_LINK_LIBRARIES( load3dmodel opengl32.lib )

TARGET_LINK_LIBRARIES( load3dmodel ${OpenCV_LIBS} )
