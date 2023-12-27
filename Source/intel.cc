#include "intel.h"
using namespace sycl;

size_t num_repetitions = 1;

typedef std::vector<vec3d> vertVector;

static auto exception_handler = [](sycl::exception_list e_list) {
	for (std::exception_ptr const& e : e_list) {
		try {
			std::rethrow_exception(e);
		}
		catch (std::exception const& e) {
#if _DEBUG
			std::cout << "Failure" << std::endl;
#endif
		}
	}
};

void calculatePolygonsIntel(__parameters _param, void* _this) {

	vertVector* _verts = _param.verts;
	mesh* _mesh = _param._mesh;
	camera* _camera = _param._camera;
	light* _light = _param._light;

	mat4x4 World, Rx, Ry, Rz, t;


	size_t vector_size = _mesh->vertices.size();

	_param.Rx = &Rx;
	_param.Ry = &Ry;
	_param.Rz = &Rz;
	_param.t = &t;
	_param.World = &World;

	_verts->resize(vector_size);

	((SDLGameEngine*)_this)->_meshApplyRotations(_param);
	((SDLGameEngine*)_this)->_meshApplyTransations(_param);

	World = ((SDLGameEngine*)_this)->_matrixMakeIdentity();
	World = Matrix_MultiplyMatrix(Ry, Rz);
	World = Matrix_MultiplyMatrix(World, Rx);
	World = Matrix_MultiplyMatrix(World, t);

	mat4x4 _cameraRotationX;
	mat4x4 _cameraRotationY;
	mat4x4 _cameraRotationZ;
	mat4x4 _cameraRotation;

	_param.Rx = &_cameraRotationX;
	_param.Ry = &_cameraRotationY;
	_param.Rz = &_cameraRotationZ;

	((SDLGameEngine*)_this)->_cameraApplyRotations(_param);

	_cameraRotation = Matrix_MultiplyMatrix(_cameraRotationZ, _cameraRotationY);
	_cameraRotation = Matrix_MultiplyMatrix(_cameraRotation, _cameraRotationZ);

	_camera->_up = { 0, 1, 0 };
	_camera->_target = { 0, 0, 1 };
	_camera->_pointAt = MultiplyMatrixVector(_cameraRotation, _camera->_target);

	_camera->_target = Vector_Add(_camera->position, _camera->_pointAt);

	mat4x4 __matrix = Matrix_PointAt(_camera->position, _camera->_target, _camera->_up);
	_camera->_matrix = __matrix; Matrix_QuickInverse(__matrix);

	size_t size = _mesh->vertices.size() - 1;

	try {
		queue q;//cpu_selector_v, exception_handler);

		// Print out the device information used for the kernel code.
		std::cout << "Running on device: "
			<< q.get_device().get_info<info::device::name>() << "\n";
		std::cout << "Vector size: " << _mesh->vertices.size() << "\n";

		// Vector addition in SYCL
		range<1> num_items{ _verts->size() };

		vertVector temp; temp.resize(2);

		buffer v_buf(_mesh->vertices);
		buffer v_temp(temp);
		buffer out_buf(_verts->data(), num_items);

		q.submit([&](handler& h) {
			accessor vaccess(v_buf, h, read_only);
			accessor temprw(v_temp, h, read_write, no_init);
			accessor outaccess(out_buf, h, write_only, no_init);

			h.parallel_for<SDLGameEngine>(num_items, [=](auto i) {
				temprw[1].x = temprw[0].x * World.m[0][0] + temprw[0].y * World.m[1][0] + temprw[0].z * World.m[2][0] + temprw[0].w * World.m[3][0];
				temprw[1].y = temprw[0].x * World.m[0][1] + temprw[0].y * World.m[1][1] + temprw[0].z * World.m[2][1] + temprw[0].w * World.m[3][1];
				temprw[1].z = temprw[0].x * World.m[0][2] + temprw[0].y * World.m[1][2] + temprw[0].z * World.m[2][2] + temprw[0].w * World.m[3][2];
				temprw[1].w = temprw[0].x * World.m[0][3] + temprw[0].y * World.m[1][3] + temprw[0].z * World.m[2][3] + temprw[0].w * World.m[3][3];

				temprw[0] = temprw[1];

				temprw[1].x = temprw[0].x * _camera->_matrix.m[0][0] + temprw[0].y * _camera->_matrix.m[1][0] + temprw[0].z * _camera->_matrix.m[2][0] + temprw[0].w * _camera->_matrix.m[3][0];
				temprw[1].y = temprw[0].x * _camera->_matrix.m[0][1] + temprw[0].y * _camera->_matrix.m[1][1] + temprw[0].z * _camera->_matrix.m[2][1] + temprw[0].w * _camera->_matrix.m[3][1];
				temprw[1].z = temprw[0].x * _camera->_matrix.m[0][2] + temprw[0].y * _camera->_matrix.m[1][2] + temprw[0].z * _camera->_matrix.m[2][2] + temprw[0].w * _camera->_matrix.m[3][2];
				temprw[1].w = temprw[0].x * _camera->_matrix.m[0][3] + temprw[0].y * _camera->_matrix.m[1][3] + temprw[0].z * _camera->_matrix.m[2][3] + temprw[0].w * _camera->_matrix.m[3][3];

				temprw[0] = temprw[1];

				outaccess[i].x = temprw[0].x * _camera->_matrix.m[0][0] + temprw[0].y * _camera->_matrix.m[1][0] + temprw[0].z * _camera->_matrix.m[2][0] + temprw[0].w * _camera->_matrix.m[3][0];
				outaccess[i].y = temprw[0].x * _camera->_matrix.m[0][1] + temprw[0].y * _camera->_matrix.m[1][1] + temprw[0].z * _camera->_matrix.m[2][1] + temprw[0].w * _camera->_matrix.m[3][1];
				outaccess[i].z = temprw[0].x * _camera->_matrix.m[0][2] + temprw[0].y * _camera->_matrix.m[1][2] + temprw[0].z * _camera->_matrix.m[2][2] + temprw[0].w * _camera->_matrix.m[3][2];
				outaccess[i].w = temprw[0].x * _camera->_matrix.m[0][3] + temprw[0].y * _camera->_matrix.m[1][3] + temprw[0].z * _camera->_matrix.m[2][3] + temprw[0].w * _camera->_matrix.m[3][3];

				outaccess[i].x = vaccess[i].x / vaccess[i].w;
				outaccess[i].y = vaccess[i].y / vaccess[i].w;
				outaccess[i].z = vaccess[i].z / vaccess[i].w;
				outaccess[i].x += 1.0f;
				outaccess[i].y += 1.0f;
				outaccess[i].x *= 0.5f * (float)((SDLGameEngine*)_this)->screen.SCREEN_WIDTH;
				outaccess[i].y *= 0.5f * (float)((SDLGameEngine*)_this)->screen.SCREEN_HEIGHT;
				});
			});
		q.wait();
		temp.clear();
	}
	catch (sycl::exception const& e) {
		cout << e.what();
		((SDLGameEngine*)_this)->throwException(__FILE__, __FUNCTION__, e.what(), false);
	}
}