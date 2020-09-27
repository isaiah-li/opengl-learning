CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )

PROJECT( clouds )

#set(current_dir ${CMAKE_CURRENT_SOURCE_DIR})

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

#set(SOURCE ./Shader/*)
#source_group(main FILES ${SOURCE} )
FILE(GLOB_RECURSE shaderfiles "../Shader/*")
source_group(Shader_Files FILES ${shaderfiles})

FILE(GLOB_RECURSE Noise "../iChannels/*")
source_group(Noise_Files FILES ${Noise})

ADD_EXECUTABLE(clouds ${SRC} ${INCS} ${shaderfiles} ${Noise})

TARGET_LINK_LIBRARIES( clouds debug F:/libtools/gl/glfw-3.3.2/build/src/Debug/glfw3.lib )
TARGET_LINK_LIBRARIES( clouds optimized F:/libtools/gl/glfw-3.3.2/build/src/Release/glfw3.lib )
TARGET_LINK_LIBRARIES( clouds opengl32.lib )

TARGET_LINK_LIBRARIES( clouds ${OpenCV_LIBS} )
