#include <sycl/sycl.hpp>
#include <vector>
#include <iostream>
#include <string>
#include "maths_engine.h"
#include <thread>
#if FPGA_HARDWARE || FPGA_EMULATOR || FPGA_SIMULATOR
#include <sycl/ext/intel/fpga_extensions.hpp>
#endif
using namespace sycl;

size_t num_repetitions = 1;
typedef std::vector<float> Vector;
typedef std::vector<vec3d> vertVector;
typedef std::vector<mat4x4> worldVector;
typedef std::vector<camera> cameraVector;

static auto exception_handler = [](sycl::exception_list e_list) {
    for (std::exception_ptr const& e : e_list) {
        try {
            std::rethrow_exception(e);
        }
        catch (std::exception const& e) {
#if _DEBUG
            std::cout << "Failure" << std::endl;
#endif
            std::terminate();
        }
    }
    };

extern void _main();

std::atomic<__parameters> param;
std::atomic<void*> pointer;
std::atomic<bool> called;
std::atomic<bool> quit;

void calculatePolygonsIntel(__parameters p, void* _this) {
    param.store(p);
    pointer.store(_this);
    called.store(true);
    while (called);
}

SYCL_EXTERNAL
void MultiplyMatrixVectorIntel(vec3d* i, vec3d* o, mat4x4* m)
{
    o->x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + m->m[3][0];
    o->y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + m->m[3][1];
    o->z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + m->m[3][2];
    o->w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + m->m[3][3];

    if (o->w != 0.0f)
    {
        o->x /= o->w; o->y /= o->w; o->z /= o->w;
    }
}

void VectorCalculate(queue& q, const vertVector& in_vector, vertVector& temp_vector, vertVector& temp_vector2,
    vertVector* out_parallel, const worldVector& world, const cameraVector& cam, const Vector& screenS) {
    range<1> num_items{ in_vector.size() };

    buffer in_buf(in_vector);
    buffer temp_buf(temp_vector);
    buffer temp_buf2(temp_vector2);
    buffer world_buf(world);
    buffer cam_buf(cam);
    buffer screen_buf(screenS);
    buffer out_buf(out_parallel->data(), num_items);

    for (size_t i = 0; i < num_repetitions; i++) {
        q.submit([&](handler& h) {
            // Create an accessor for each buffer with access permission: read, write or
            // read/write. The accessor is a mean to access the memory in the buffer.
            accessor in(in_buf, h, read_only);
            accessor temprw(temp_buf, h, read_write);
            accessor temprw2(temp_buf2, h, read_write);
            accessor World(world_buf, h, read_only);
            accessor _camera(cam_buf, h, read_only);
            accessor sizes(screen_buf, h, read_only);

            accessor out(out_buf, h, read_write, no_init);

            //sycl::stream cout(1024, 256, h);

            h.parallel_for(num_items, [=](auto i) {
                temprw[i].x = in[i].x * World[0].m[0][0] + in[i].y * World[0].m[1][0] + in[i].z * World[0].m[2][0] + in[i].w * World[0].m[3][0];
                temprw[i].y = in[i].x * World[0].m[0][1] + in[i].y * World[0].m[1][1] + in[i].z * World[0].m[2][1] + in[i].w * World[0].m[3][1];
                temprw[i].z = in[i].x * World[0].m[0][2] + in[i].y * World[0].m[1][2] + in[i].z * World[0].m[2][2] + in[i].w * World[0].m[3][2];
                temprw[i].w = in[i].x * World[0].m[0][3] + in[i].y * World[0].m[1][3] + in[i].z * World[0].m[2][3] + in[i].w * World[0].m[3][3];

                //temprw[1].x = temprw[0].x * _camera[0]._matrix.m[0][0] + temprw[0].y * _camera[0]._matrix.m[1][0] + temprw[0].z * _camera[0]._matrix.m[2][0] + temprw[0].w * _camera[0]._matrix.m[3][0];
                //temprw[1].y = temprw[0].x * _camera[0]._matrix.m[0][1] + temprw[0].y * _camera[0]._matrix.m[1][1] + temprw[0].z * _camera[0]._matrix.m[2][1] + temprw[0].w * _camera[0]._matrix.m[3][1];
                //temprw[1].z = temprw[0].x * _camera[0]._matrix.m[0][2] + temprw[0].y * _camera[0]._matrix.m[1][2] + temprw[0].z * _camera[0]._matrix.m[2][2] + temprw[0].w * _camera[0]._matrix.m[3][2];
                //temprw[1].w = temprw[0].x * _camera[0]._matrix.m[0][3] + temprw[0].y * _camera[0]._matrix.m[1][3] + temprw[0].z * _camera[0]._matrix.m[2][3] + temprw[0].w * _camera[0]._matrix.m[3][3];

                MultiplyMatrixVectorIntel((vec3d*)&temprw[i], (vec3d*)&out[i], (mat4x4*)&_camera[0].projection);

                out[i].x ++;
                out[i].y ++;
                out[i].x *= 0.5f * sizes[0];
                out[i].y *= 0.5f * sizes[1];

                //cout << "in[" << (int)i.get_id() << "] X= " << out[i].x << " Y= " << out[i].y << " Z= " << out[i].z << '\n';
                });
            });
    };
    // Wait until compute tasks on GPU done
    q.wait();
}

void terminateProgram() {
    quit = true;
}

extern void GPUThrow(void*, string);

int main(int argc, char* argv[]) {
    auto gpuselector = gpu_selector_v;

    Vector sizes;
    vertVector verts, temp, temp2;
    worldVector World;
    cameraVector _camera;
    
    quit = false;
    sizes.resize(2);
    World.resize(1);
    _camera.resize(1);

    std::thread t = std::thread(_main);

    queue gpu(gpuselector, exception_handler);
    std::cout << "Running on device: "
        << gpu.get_device().get_info<info::device::name>() << "\n";

    // wait for invocation (SDLGameEngine::_calculatePolygons() -> calculatePolygonsIntel())
    while (!quit) {
        if (called.load()) {
            try {
                size_t vector_size = param.load()._mesh->vertices.size();
                param.load().verts->resize(vector_size);
                temp.resize(vector_size);
                temp2.resize(vector_size);

                World[0] = *param.load().World;
                _camera[0] = *param.load()._camera;
                sizes[0] = param.load().sx;
                sizes[1] = param.load().sy;

                //std::cout << "Vector size: " << vector_size << "\n";
                VectorCalculate(gpu, param.load()._mesh->vertices, temp, temp2, param.load().verts, World, _camera, sizes);
                //param.verts->insert(param.verts->begin(), verts.begin(), verts.end());
                called.store(false);
            }
            catch (sycl::exception const& e) {
                std::cout << "An exception is caught in GPU.\n";
                called.store(false);
                GPUThrow(pointer, e.what());
            }
        }
    }

    std::cout << "Vector add successfully completed on device.\n";

    t.join();

    std::exit(0);

    return 0;
}