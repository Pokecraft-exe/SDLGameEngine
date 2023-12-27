#include "game.h"
#define CUDA_WARN(XXX) \
    do { if (XXX != cudaSuccess) cerr << "CUDA Error: " << \
        cudaGetErrorString(XXX) << ", at line " << __LINE__ \
        << endl; cudaDeviceSynchronize(); } while (0)

typedef vector<vec3d> Vertvec;

__global__ void calculatecuda(vec3d* vert, vec3d* outvert, vec3d* temp, mat4x4* World, mat4x4* matrix, mat4x4* projection, int* w, int* h) {
	int i = threadIdx.x;

	printf("vert[%d] : x = %d y= %d z= %d\n", i, vert[i].x, vert[i].y, vert[i].z);

	temp[0].x = vert[i].x * World->m[0][0] + vert[i].y * World->m[1][0] + vert[i].z * World->m[2][0] + vert[i].w * World->m[3][0];
	temp[0].y = vert[i].x * World->m[0][1] + vert[i].y * World->m[1][1] + vert[i].z * World->m[2][1] + vert[i].w * World->m[3][1];
	temp[0].z = vert[i].x * World->m[0][2] + vert[i].y * World->m[1][2] + vert[i].z * World->m[2][2] + vert[i].w * World->m[3][2];
	temp[0].w = vert[i].x * World->m[0][3] + vert[i].y * World->m[1][3] + vert[i].z * World->m[2][3] + vert[i].w * World->m[3][3];

	temp[1].x = temp[0].x * matrix->m[0][0] + temp[0].y * matrix->m[1][0] + temp[0].z * matrix->m[2][0] + temp[0].w * matrix->m[3][0];
	temp[1].y = temp[0].x * matrix->m[0][1] + temp[0].y * matrix->m[1][1] + temp[0].z * matrix->m[2][1] + temp[0].w * matrix->m[3][1];
	temp[1].z = temp[0].x * matrix->m[0][2] + temp[0].y * matrix->m[1][2] + temp[0].z * matrix->m[2][2] + temp[0].w * matrix->m[3][2];
	temp[1].w = temp[0].x * matrix->m[0][3] + temp[0].y * matrix->m[1][3] + temp[0].z * matrix->m[2][3] + temp[0].w * matrix->m[3][3];

	temp[0] = temp[1];

	temp[1].x = temp[0].x * projection->m[0][0] + temp[0].y * projection->m[1][0] + temp[0].z * projection->m[2][0] + temp[0].w * projection->m[3][0];
	temp[1].y = temp[0].x * projection->m[0][1] + temp[0].y * projection->m[1][1] + temp[0].z * projection->m[2][1] + temp[0].w * projection->m[3][1];
	temp[1].z = temp[0].x * projection->m[0][2] + temp[0].y * projection->m[1][2] + temp[0].z * projection->m[2][2] + temp[0].w * projection->m[3][2];
	temp[1].w = temp[0].x * projection->m[0][3] + temp[0].y * projection->m[1][3] + temp[0].z * projection->m[2][3] + temp[0].w * projection->m[3][3];

	outvert[i].x = temp[1].x / temp[1].w;
	outvert[i].y = temp[1].y / temp[1].w;
	outvert[i].z = temp[1].z / temp[1].w;
	outvert[i].x += 1.0f;
	outvert[i].y += 1.0f;
	outvert[i].x *= 0.5f * (float)*w;
	outvert[i].y *= 0.5f * (float)*h;
}
void calculatePolygonsCuda(__parameters _param, void* _this) {
	Vertvec* _verts = _param.verts;
	Vertvec _outVerts;
	Vertvec _Temp;
	mesh* _mesh = _param._mesh;
	camera* _camera = _param._camera;
	light* _light = _param._light;

	mat4x4 World, Rx, Ry, Rz, t;

	_param.Rx = &Rx;
	_param.Ry = &Ry;
	_param.Rz = &Rz;
	_param.t = &t;
	_param.World = &World;

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

	_outVerts.resize(_verts->size());
	_Temp.resize(2);

	mat4x4* gpuWorld;
	mat4x4* gpuMatrix;
	mat4x4* gpuProjection;
	vec3d* gpuVerts;
	vec3d* gpuOutVerts;
	vec3d* gpuTemp;
	int* gpuW;
	int* gpuH;

	size_t size = _mesh->vertices.size();

	cudaError_t cudaStatus;

	cudaStatus = cudaSetDevice(0);

	cudaMalloc((void**)&gpuWorld, sizeof(mat4x4));
	cudaMalloc((void**)&gpuMatrix, sizeof(mat4x4));
	cudaMalloc((void**)&gpuProjection, sizeof(mat4x4));
	cudaMalloc((void**)&gpuVerts, size * sizeof(vec3d));
	cudaMalloc((void**)&gpuOutVerts, size * sizeof(vec3d));
	cudaMalloc((void**)&gpuTemp, 2 * sizeof(vec3d));
	cudaMalloc((void**)&gpuW, sizeof(int));
	cudaMalloc((void**)&gpuH, sizeof(int));
	cudaMemcpy(gpuWorld, &World, sizeof(mat4x4), cudaMemcpyHostToDevice);
	cudaMemcpy(gpuMatrix, &_camera->_matrix, sizeof(mat4x4), cudaMemcpyHostToDevice);
	cudaMemcpy(gpuProjection, &_camera->projection, sizeof(mat4x4), cudaMemcpyHostToDevice);
	cudaMemcpy(gpuW, &((SDLGameEngine*)_this)->screen.SCREEN_WIDTH, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(gpuH, &((SDLGameEngine*)_this)->screen.SCREEN_HEIGHT, sizeof(int), cudaMemcpyHostToDevice);

	cudaStatus = cudaMemcpy(gpuVerts, &_mesh->vertices[0], size * sizeof(vec3d), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		((SDLGameEngine*)_this)->throwException("cude.cu", __FUNCTION__, "memcpy: " + string(cudaGetErrorString(cudaStatus)), false);
		goto Error;
	}

	calculatecuda<<<1, size>>>(gpuVerts, gpuOutVerts, gpuTemp, gpuWorld, gpuMatrix, gpuProjection, gpuW, gpuH);

	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		((SDLGameEngine*)_this)->throwException("cude.cu", __FUNCTION__, "gpu calculation launch failed: " + string(cudaGetErrorString(cudaStatus)), false);
		goto Error;
	}

	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		((SDLGameEngine*)_this)->throwException("cude.cu", __FUNCTION__, "cudaDeviceSynchronize returned error code " + to_string(cudaStatus) + " after launching gpu calculation !", false);
		goto Error;
	}

	vec3d* src = gpuOutVerts;
	for (Vertvec::iterator it = _mesh->vertices.begin(); it != _mesh->vertices.end(); ++it) {
		vec3d dst;

		cudaMemcpy(&dst, &src, sizeof(vec3d), cudaMemcpyDeviceToHost);

		_verts->push_back(dst);

		src += sizeof(vec3d);
	}

	goto Error;

Error:
	cudaFree(gpuWorld);
	cudaFree(gpuMatrix);
	cudaFree(gpuProjection);
	cudaFree(gpuVerts);
	cudaFree(gpuOutVerts);
	cudaFree(gpuTemp);
	cudaFree(gpuW);
	cudaFree(gpuH);
}