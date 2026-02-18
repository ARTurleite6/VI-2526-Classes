#include "Scene/SceneBuilder.hpp"

#include "Primitive/Geometry/Mesh.hpp"
#include "Primitive/Geometry/Sphere.hpp"
#include "Scene/Scene.hpp"

namespace VI {
Scene SphereScene() {
  Scene scene{};

  const int red_mat =
      scene.AddMaterial({.Name = "Red Material", .Albedo = {1.0f, 0.0f, 0.0f}});

  Sphere sphere{Point{0.0f, 0.0f, 3.0f}, 0.8f};
  scene.AddPrimitive(sphere, red_mat);

  const int light_mat = scene.AddMaterial({
      .Name = "Light",
      .EmissionColor = {1.0f, 1.0f, 1.0f},
      .EmissionPower = 10.0f,
  });

  scene.AddLight(std::make_unique<AmbientLight>(light_mat));

  return scene;
}

Scene WhittedCornellBox() {
  Scene scene;

  const int white_mat =
      scene.AddMaterial({.Name = "White", .Albedo = {0.75f, 0.75f, 0.75f}});
  const int red_mat =
      scene.AddMaterial({.Name = "Red", .Albedo = {0.65f, 0.05f, 0.05f}});
  const int green_mat =
      scene.AddMaterial({.Name = "Green", .Albedo = {0.05f, 0.30f, 0.05f}});
  const int black_mat =
      scene.AddMaterial({.Name = "Black", .Albedo = {0.02f, 0.02f, 0.02f}});
  const int blue_mat =
      scene.AddMaterial({.Name = "Dark Navy", .Albedo = {0.0f, 0.0f, 1.0f}});
  const int wood_mat =
      scene.AddMaterial({.Name = "Wood", .Albedo = {0.52f, 0.30f, 0.10f}});
  const int mirror_mat =
      scene.AddMaterial({.Name = "Mirror", .Roughness = 0.f, .Metallic = 1.f});

  // Helper: generates all 12 triangles (6 faces) of an axis-aligned box
  auto make_box = [](float x1, float y1, float z1, float x2, float y2,
                     float z2) -> std::vector<Triangle> {
    return {
        // Bottom (y=y1, normal down)
        Triangle{Point{x1, y1, z1}, Point{x1, y1, z2}, Point{x2, y1, z2},
                 Vector{0.f, -1.f, 0.f}},
        Triangle{Point{x1, y1, z1}, Point{x2, y1, z2}, Point{x2, y1, z1},
                 Vector{0.f, -1.f, 0.f}},
        // Top (y=y2, normal up)
        Triangle{Point{x1, y2, z1}, Point{x2, y2, z1}, Point{x2, y2, z2},
                 Vector{0.f, 1.f, 0.f}},
        Triangle{Point{x1, y2, z1}, Point{x2, y2, z2}, Point{x1, y2, z2},
                 Vector{0.f, 1.f, 0.f}},
        // Front (z=z1, normal toward camera)
        Triangle{Point{x1, y1, z1}, Point{x2, y1, z1}, Point{x2, y2, z1},
                 Vector{0.f, 0.f, -1.f}},
        Triangle{Point{x1, y1, z1}, Point{x2, y2, z1}, Point{x1, y2, z1},
                 Vector{0.f, 0.f, -1.f}},
        // Back (z=z2, normal away from camera)
        Triangle{Point{x1, y1, z2}, Point{x1, y2, z2}, Point{x2, y2, z2},
                 Vector{0.f, 0.f, 1.f}},
        Triangle{Point{x1, y1, z2}, Point{x2, y2, z2}, Point{x2, y1, z2},
                 Vector{0.f, 0.f, 1.f}},
        // Right face (x=x1, normal toward -X / green wall side)
        Triangle{Point{x1, y1, z1}, Point{x1, y2, z1}, Point{x1, y2, z2},
                 Vector{-1.f, 0.f, 0.f}},
        Triangle{Point{x1, y1, z1}, Point{x1, y2, z2}, Point{x1, y1, z2},
                 Vector{-1.f, 0.f, 0.f}},
        // Left face (x=x2, normal toward +X / red wall side)
        Triangle{Point{x2, y1, z1}, Point{x2, y1, z2}, Point{x2, y2, z2},
                 Vector{1.f, 0.f, 0.f}},
        Triangle{Point{x2, y1, z1}, Point{x2, y2, z2}, Point{x2, y2, z1},
                 Vector{1.f, 0.f, 0.f}},
    };
  };

  // Floor
  scene.AddPrimitive(
      Mesh{"Floor",
           std::vector<Triangle>{
               Triangle{Point{552.8f, 0.f, 0.f}, Point{-100.f, 0.f, 0.f},
                        Point{-100.f, 0.f, 859.2f}, Vector{0.f, 1.f, 0.f}},
               Triangle{Point{549.6f, 0.f, 859.2f}, Point{552.8f, 0.f, 0.f},
                        Point{-100.f, 0.f, 859.2f}, Vector{0.f, 1.f, 0.f}}}},
      white_mat);

  // Ceiling (dark/black)
  scene.AddPrimitive(
      Mesh{
          "Ceiling",
          std::vector<Triangle>{
              Triangle{Point{556.f, 548.8f, 0.f}, Point{-100.f, 548.8f, 0.f},
                       Point{-100.f, 548.8f, 459.2f}, Vector{0.f, -1.f, 0.f}},
              Triangle{Point{556.f, 548.8f, 459.2f}, Point{556.f, 548.8f, 0.f},
                       Point{-100.f, 548.8f, 459.2f}, Vector{0.f, -1.f, 0.f}}}},
      black_mat);

  // Back wall
  scene.AddPrimitive(
      Mesh{"Back Wall",
           std::vector<Triangle>{
               Triangle{Point{-100.f, 0.f, 459.2f}, Point{549.6f, 0.f, 459.2f},
                        Point{556.f, 548.8f, 459.2f}, Vector{0.f, 0.f, -1.f}},
               Triangle{Point{-100.f, 0.f, 459.2f},
                        Point{-100.f, 548.8f, 459.2f},
                        Point{556.f, 548.8f, 459.2f}, Vector{0.f, 0.f, -1.f}}}},
      white_mat);

  // Right wall (green)
  scene.AddPrimitive(
      Mesh{"Right Wall",
           std::vector<Triangle>{
               Triangle{Point{-100.f, 0.f, 0.f}, Point{-100.f, 0.f, 459.2f},
                        Point{-100.f, 548.8f, 459.2f}, Vector{1.f, 0.f, 0.f}},
               Triangle{Point{-100.f, 0.f, 0.f}, Point{-100.f, 548.8f, 0.f},
                        Point{-100.f, 548.8f, 459.2f}, Vector{1.f, 0.f, 0.f}}}},
      green_mat);

  // Left wall (red)
  scene.AddPrimitive(
      Mesh{
          "Left Wall",
          std::vector<Triangle>{
              Triangle{Point{552.8f, 0.f, 0.f}, Point{549.6f, 0.f, 459.2f},
                       Point{549.6f, 548.8f, 459.2f}, Vector{-1.f, 0.f, 0.f}},
              Triangle{Point{552.8f, 0.f, 0.f}, Point{552.8f, 548.8f, 0.f},
                       Point{549.6f, 548.8f, 459.2f}, Vector{-1.f, 0.f, 0.f}}}},
      red_mat);

  // Mirror plane on left wall (roughness=0, facing into the room)
  scene.AddPrimitive(
      Mesh{"Mirror Plane",
           std::vector<Triangle>{
               Triangle{Point{540.f, 50.f, 50.f}, Point{540.f, 50.f, 409.f},
                        Point{540.f, 488.f, 409.f}, Vector{-1.f, 0.f, 0.f}},
               Triangle{Point{540.f, 50.f, 50.f}, Point{540.f, 488.f, 409.f},
                        Point{540.f, 488.f, 50.f}, Vector{-1.f, 0.f, 0.f}}}},
      mirror_mat);

  // Dark navy box 1 (left-center of scene)
  scene.AddPrimitive(
      Mesh{"Dark Box 1", make_box(310.f, 0.f, 220.f, 440.f, 320.f, 345.f)},
      blue_mat);

  // Wooden box (right-center, closer to camera)
  scene.AddPrimitive(
      Mesh{"Wooden Box", make_box(45.f, 0.f, 60.f, 200.f, 195.f, 215.f)},
      wood_mat);

  // Metallic sphere resting on top of the wooden box
  scene.AddPrimitive(Sphere{Point{122.f, 285.f, 137.f}, 90.f}, mirror_mat);

  return scene;
}

Scene CreateCornellBox() {
  Scene scene;

  const int white_mat = scene.AddMaterial(
      {.Name = "White Material", .Albedo = {0.8f, 0.8f, 0.8f}});
  const int red_mat =
      scene.AddMaterial({.Name = "Red Material", .Albedo = {0.5f, 0.1f, 0.1f}});
  const int green_mat =
      scene.AddMaterial({.Name = "Green Material", .Albedo = {0.f, 0.6f, 0.f}});
  const int blue_mat =
      scene.AddMaterial({.Name = "Blue Material", .Albedo = {0.f, 0.f, 0.6f}});
  const int orange_mat = scene.AddMaterial(
      {.Name = "Orange Material", .Albedo = {0.66f, 0.44f, 0.f}});
  const int mirror_mat = scene.AddMaterial({.Name = "Mirror Material",
                                            .Albedo = {1.f, 1.f, 1.f},
                                            .Roughness = 0.f,
                                            .Metallic = 1.f});

  const int light_mat = scene.AddMaterial({.Name = "Light",
                                           .Albedo = {1.f, 1.f, 1.f},
                                           .EmissionColor = {1.f, 1.f, 1.f},
                                           .EmissionPower = 10.f});

  // Area Light (on the ceiling)
  {
    float y = 548.7f; // Slightly below the ceiling
    scene.AddPrimitive(Mesh{"Ceiling Light",
                            std::vector<Triangle>{Triangle{
                                                      Point{213.0f, y, 227.0f},
                                                      Point{343.0f, y, 227.0f},
                                                      Point{343.0f, y, 332.0f},
                                                      Vector{0.0f, -1.0f, 0.0f},
                                                  },
                                                  Triangle{
                                                      Point{213.0f, y, 227.0f},
                                                      Point{343.0f, y, 332.0f},
                                                      Point{213.0f, y, 332.0f},
                                                      Vector{0.0f, -1.0f, 0.0f},
                                                  }}},
                       light_mat);
  }

  // Floor
  {
    scene.AddPrimitive(
        Mesh{"Floor",
             std::vector<Triangle>{
                 Triangle{
                     Point{552.8, 0.0, 0.0},
                     Point{-100.0, 0.0, 0.0},
                     Point{-100.0, 0.0, 859.2},
                     Vector{0.0, 1.0, 0.0}, // normal for all three vertices
                 },
                 Triangle{
                     Point{549.6, 0.0, 859.2},
                     Point{552.8, 0.0, 0.0},
                     Point{-100.0, 0.0, 859.2},
                     Vector{0.0, 1.0, 0.0},
                 }}},
        white_mat);
  }
  // Ceiling
  {
    scene.AddPrimitive(
        Mesh{"Ceiling", std::vector<Triangle>{Triangle{
                                                  Point{556.0, 548.8, 0.0},
                                                  Point{-100.0, 548.8, 0.0},
                                                  Point{-100.0, 548.8, 459.2},
                                                  Vector{0.0, -1.0, 0.0},
                                              },
                                              Triangle{
                                                  Point{556.0, 548.8, 459.2},
                                                  Point{556.0, 548.8, 0.0},
                                                  Point{-100.0, 548.8, 459.2},
                                                  Vector{0.0, -1.0, 0.0},
                                              }}},
        white_mat);
  }
  // Back wall
  {
    scene.AddPrimitive(
        Mesh{"Back Wall", std::vector<Triangle>{Triangle{
                                                    Point{-100.0, 0.0, 459.2},
                                                    Point{549.6, 0.0, 459.2},
                                                    Point{556.0, 548.8, 459.2},
                                                    Vector{0.0, 0.0, -1.0},
                                                },
                                                Triangle{
                                                    Point{-100.0, 0.0, 459.2},
                                                    Point{-100.0, 548.8, 459.2},
                                                    Point{556.0, 548.8, 459.2},
                                                    Vector{0.0, 0.0, -1.0},
                                                }}},
        white_mat);
  }
  // Right Wall
  {
    scene.AddPrimitive(
        Mesh{"Right Wall",
             std::vector<Triangle>{Triangle{
                                       Point{-100.0, 0.0, 0.0},
                                       Point{-100.0, 0.0, 459.2},
                                       Point{-100.0, 548.8, 459.2},
                                       Vector{1.0, 0.0, 0.0},
                                   },
                                   Triangle{
                                       Point{-100.0, 0.0, 0.0},
                                       Point{-100.0, 548.8, 0.0},
                                       Point{-100.0, 548.8, 459.2},
                                       Vector{1.0, 0.0, 0.0},
                                   }}},
        green_mat);
  }

  { // Left Wall Mirror
    scene.AddPrimitive(Mesh{"Right Wall Mirror",
                            std::vector<Triangle>{Triangle{
                                                      Point{540, 50.0, 50.0},
                                                      Point{540, 50.0, 509.2},
                                                      Point{540, 488.8, 509.2},
                                                      Vector{-1.0, 0.0, 0.0},
                                                  },
                                                  Triangle{
                                                      Point{540, 50.0, 50.},
                                                      Point{540, 488.8, 50.},
                                                      Point{540, 488.8, 509.2},
                                                      Vector{-1.0, 0.0, 0.0},
                                                  }}},
                       mirror_mat);
  }

  // Left Wall
  {
    scene.AddPrimitive(
        Mesh{"Left Wall", std::vector<Triangle>{Triangle{
                                                    Point{552.8, 0.0, 0.0},
                                                    Point{549.6, 0.0, 459.2},
                                                    Point{549.6, 548.8, 459.2},
                                                    Vector{-1.0, 0.0, 0.0},
                                                },
                                                Triangle{
                                                    Point{552.8, 0.0, 0.0},
                                                    Point{552.8, 548.8, 0.0},
                                                    Point{549.6, 548.8, 459.2},
                                                    Vector{-1.0, 0.0, 0.0},
                                                }}},
        red_mat);
  }
  // short block
  {
    scene.AddPrimitive(
        Mesh{"Short Block",
             std::vector<Triangle>{
                 // top
                 Triangle{
                     Point{130.0, 165.0, 65.0},
                     Point{82.0, 165.0, 225.0},
                     Point{240.0, 165.0, 272.0},
                     Vector{0.0, 1.0, 0.0},
                 },
                 Triangle{
                     Point{130.0, 165.0, 65.0},
                     Point{290.0, 165.0, 114.0},
                     Point{240.0, 165.0, 272.0},
                     Vector{0.0, 1.0, 0.0},
                 },
                 // bottom
                 Triangle{
                     Point{130.0, 0.01, 65.0},
                     Point{82.0, 0.01, 225.0},
                     Point{240.0, 0.01, 272.0},
                     Vector{0.0, -1.0, 0.0},
                 },
                 Triangle{
                     Point{130.0, 0.01, 65.0},
                     Point{290.0, 0.01, 114.0},
                     Point{240.0, 0.01, 272.0},
                     Vector{0.0, -1.0, 0.0},
                 },
                 // left
                 Triangle{
                     Point{290.0, 0.0, 114.0},
                     Point{290.0, 165.0, 114.0},
                     Point{240.0, 165.0, 272.0},
                     Vector{0.8944, 0.0, 0.4472},
                 },
                 Triangle{
                     Point{290.0, 0.0, 114.0},
                     Point{240.0, 0.0, 272.0},
                     Point{240.0, 165.0, 272.0},
                     Vector{0.8944, 0.0, 0.4472},
                 },
                 // back
                 Triangle{
                     Point{240.0, 0.0, 272.0},
                     Point{240.0, 165.0, 272.0},
                     Point{82.0, 165.0, 225.0},
                     Vector{0.0, 0.0, 1.0},
                 },
                 Triangle{
                     Point{240.0, 0.0, 272.0},
                     Point{82.0, 0.0, 225.0},
                     Point{82.0, 165.0, 225.0},
                     Vector{0.0, 0.0, 1.0},
                 },
                 // right
                 Triangle{
                     Point{82.0, 0.0, 225.0},
                     Point{82.0, 165.0, 225.0},
                     Point{130.0, 165.0, 65.0},
                     Vector{-0.8944, 0.0, -0.4472},
                 },
                 Triangle{
                     Point{82.0, 0.0, 225.0},
                     Point{130.0, 0.0, 65.0},
                     Point{130.0, 165.0, 65.0},
                     Vector{-0.8944, 0.0, -0.4472},
                 },
                 // front
                 Triangle{
                     Point{130.0, 0.0, 65.0},
                     Point{130.0, 165.0, 65.0},
                     Point{290.0, 165.0, 114.0},
                     Vector{0.0, 0.0, -1.0},
                 },
                 Triangle{
                     Point{130.0, 0.0, 65.0},
                     Point{290.0, 0.0, 114.0},
                     Point{290.0, 165.0, 114.0},
                     Vector{0.0, 0.0, -1.0},
                 },
             }},
        orange_mat);
  }
  // tall block
  {
    scene.AddPrimitive(Mesh{"Tall Block",
                            std::vector<Triangle>{
                                // top
                                Triangle{
                                    Point{423.0, 330.0, 247.0},
                                    Point{265.0, 330.0, 296.0},
                                    Point{314.0, 330.0, 456.0},
                                    Vector{0.0, 1.0, 0.0},
                                },
                                Triangle{
                                    Point{423.0, 330.0, 247.0},
                                    Point{472.0, 330.0, 406.0},
                                    Point{314.0, 330.0, 456.0},
                                    Vector{0.0, 1.0, 0.0},
                                },
                                // bottom
                                Triangle{
                                    Point{423.0, 0.1, 247.0},
                                    Point{265.0, 0.1, 296.0},
                                    Point{314.0, 0.1, 456.0},
                                    Vector{0.0, -1.0, 0.0},
                                },
                                Triangle{
                                    Point{423.0, 0.1, 247.0},
                                    Point{472.0, 0.1, 406.0},
                                    Point{314.0, 0.1, 456.0},
                                    Vector{0.0, -1.0, 0.0},
                                },
                                // left
                                Triangle{
                                    Point{423.0, 0.0, 247.0},
                                    Point{423.0, 330.0, 247.0},
                                    Point{472.0, 330.0, 406.0},
                                    Vector{0.8944, 0.0, 0.4472},
                                },
                                Triangle{
                                    Point{423.0, 0.0, 247.0},
                                    Point{472.0, 0.0, 406.0},
                                    Point{472.0, 330.0, 406.0},
                                    Vector{0.8944, 0.0, 0.4472},
                                },
                                // back
                                Triangle{
                                    Point{472.0, 0.0, 406.0},
                                    Point{472.0, 330.0, 406.0},
                                    Point{314.0, 330.0, 456.0},
                                    Vector{0.0, 0.0, 1.0},
                                },
                                Triangle{
                                    Point{472.0, 0.0, 406.0},
                                    Point{314.0, 0.0, 456.0},
                                    Point{314.0, 330.0, 456.0},
                                    Vector{0.0, 0.0, 1.0},
                                },
                                // right
                                Triangle{
                                    Point{314.0, 0.0, 456.0},
                                    Point{314.0, 330.0, 456.0},
                                    Point{265.0, 330.0, 296.0},
                                    Vector{-0.8944, 0.0, -0.4472},
                                },
                                Triangle{
                                    Point{314.0, 0.0, 456.0},
                                    Point{265.0, 0.0, 296.0},
                                    Point{265.0, 330.0, 296.0},
                                    Vector{-0.8944, 0.0, -0.4472},
                                },
                                // front
                                Triangle{
                                    Point{265.0, 0.0, 296.0},
                                    Point{265.0, 330.0, 296.0},
                                    Point{423.0, 330.0, 247.0},
                                    Vector{0.0, 0.0, -1.0},
                                },
                                Triangle{
                                    Point{265.0, 0.0, 296.0},
                                    Point{423.0, 0.0, 247.0},
                                    Point{423.0, 330.0, 247.0},
                                    Vector{0.0, 0.0, -1.0},
                                },
                            }},
                       blue_mat);
  }

  return scene;
}
} // namespace VI
