cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)

project(VI-RT)

add_subdirectory(external/glm)

add_executable(VI-RT src/main.cpp
    # Ray
    src/Ray/Ray.hpp
    src/Ray/Intersection.hpp

    # Renderer
    src/Renderer/Renderer.hpp
    src/Renderer/Renderer.cpp

    # Image
    src/Image/Image.hpp
    src/Image/ImagePPM.hpp
    src/Image/ImagePPM.cpp

    # Camera
    src/Camera/Camera.hpp
    src/Camera/Camera.cpp

    # Scene
    src/Scene/Scene.hpp
    src/Scene/Scene.cpp

    # Primitives
    src/Primitive/BRDF.hpp
    src/Primitive/BoundingBox.hpp
    src/Primitive/Primitive.hpp
    src/Primitive/Geometry/Geometry.hpp
    src/Primitive/Geometry/Sphere.hpp
    src/Primitive/Geometry/Sphere.cpp

    # Math
    src/Math/RGB.hpp
    src/Math/Vector.hpp
    src/Math/Random.hpp
    src/Math/Random.cpp

    # Shaders
    src/Shaders/Shader.hpp
    src/Shaders/DummyShader.hpp
    src/Shaders/AmbientShader.hpp
    src/Shaders/AmbientShader.cpp
)
target_include_directories(VI-RT PRIVATE src)
target_precompile_headers(VI-RT PRIVATE src/vi_pch.h)

target_link_libraries(VI-RT PRIVATE glm::glm)
