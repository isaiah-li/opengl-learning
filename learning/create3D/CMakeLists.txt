CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )

PROJECT( create3D )

INCLUDE_DIRECTORIES(./include)
INCLUDE_DIRECTORIES(F:/libtools/gl/glad/include)
INCLUDE_DIRECTORIES(F:/libtools/gl/glfw-3.3.2/include)
INCLUDE_DIRECTORIES(F:/libtools/gl/glm)

FILE(GLOB_RECURSE INCS "*.h" "*.hpp")

#SOURCE_GROUP("Include" FILES ${INCS} )

AUX_SOURCE_DIRECTORY(./src SRC)
AUX_SOURCE_DIRECTORY(F:/libtools/gl/glad/src SRC)


#SET( OpenCV_DIR F:/libtools/opencv-2.4.13/build )
SET( OpenCV_DIR F:/libtools/opencv-4.3.0/build )
FIND_PACKAGE( OpenCV REQUIRED ) 


ADD_EXECUTABLE(create3D ${SRC} ${INCS})

TARGET_LINK_LIBRARIES( create3D debug F:/libtools/gl/glfw-3.3.2/build/src/Debug/glfw3.lib )
TARGET_LINK_LIBRARIES( create3D optimized F:/libtools/gl/glfw-3.3.2/build/src/Release/glfw3.lib )
TARGET_LINK_LIBRARIES( create3D opengl32.lib )

TARGET_LINK_LIBRARIES( create3D ${OpenCV_LIBS} )
