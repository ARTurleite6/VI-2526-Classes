#include "Scene/SceneBuilder.hpp"

#include "Primitive/Geometry/Mesh.hpp"
#include "Scene/Scene.hpp"

namespace VI {
Scene CreateCornellBox() {
  Scene scene;

  const int white_mat =
      scene.AddMaterial({"White Material", {0.8, 0.8, 0.8}, 1.f, 0.f});
  const int red_mat =
      scene.AddMaterial({"Red Material", {0.5, 0.1, 0.1}, 1.f, 0.f});
  const int green_mat =
      scene.AddMaterial({"Green Material", {0., 0.6, 0.}, 1.f, 0.f});
  const int blue_mat =
      scene.AddMaterial({"Blue Material", {0., 0., 0.6}, 1.f, 0.f});
  const int orange_mat =
      scene.AddMaterial({"Orange Material", {0.66, 0.44, 0.}, 1.f, 0.f});
  const int mirror_mat =
      scene.AddMaterial({"Mirror Material", {1.f, 1.f, 1.f}, 0.001f, 1.f});

  const int light_mat = scene.AddMaterial(
      {"Light", {1.f, 1.f, 1.f}, 1.f, 0.f, {1.f, 1.f, 1.f}, 5.0f});

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
                     Point{552.8, 0.0, 0.0}, Point{-100.0, 0.0, 0.0},
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
