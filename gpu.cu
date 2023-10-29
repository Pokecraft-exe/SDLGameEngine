#include "game.h"

__global__ void SDLGameEngine::calculateGPU(vec3d* vert, mat4x4* World, mat4x4* matrix, mat4x4* projection) {
	int i = threadIdx.x;

	vec3d Transformed = MultiplyMatrixVector(*World, vert[i]);

	vec3d Viewed = MultiplyMatrixVector(*matrix, Transformed);

	vert[i] = MultiplyMatrixVector(*projection, Viewed);

	vert[i] = Vector_Div(vert[i], vert[i].w);
	vert[i].x += 1.0f;
	vert[i].y += 1.0f;
	vert[i].x *= 0.5f * (float)screen.SCREEN_WIDTH;
	vert[i].y *= 0.5f * (float)screen.SCREEN_HEIGHT;

}
void SDLGameEngine::_calculatePolygons(__parameters _param) {

	vector<vec3d>* _verts = _param.verts;
	mesh* _mesh = _param._mesh;
	camera* _camera = _param._camera;
	light* _light = _param._light;

	mat4x4 World, Rx, Ry, Rz, t;

	_param.Rx = &Rx;
	_param.Ry = &Ry;
	_param.Rz = &Rz;
	_param.t = &t;
	_param.World = &World;

	_meshApplyRotations(_param);
	_meshApplyTransations(_param);

	World = _matrixMakeIdentity();
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

	_cameraApplyRotations(_param);

	_cameraRotation = Matrix_MultiplyMatrix(_cameraRotationZ, _cameraRotationY);
	_cameraRotation = Matrix_MultiplyMatrix(_cameraRotation, _cameraRotationZ);

	_camera->_up = { 0, 1, 0 };
	_camera->_target = { 0, 0, 1 };
	_camera->_pointAt = MultiplyMatrixVector(_cameraRotation, _camera->_target);

	_camera->_target = Vector_Add(_camera->position, _camera->_pointAt);

	mat4x4 __matrix = Matrix_PointAt(_camera->position, _camera->_target, _camera->_up);
	_camera->_matrix = __matrix; Matrix_QuickInverse(__matrix);

	mat4x4* gpuWorld;
	mat4x4* gpuMatrix;
	mat4x4* gpuProjection;
	vec3d* gpuVerts;

	size_t size = _mesh->vertices.size();

	cudaError_t cudaStatus;

	cudaStatus = cudaSetDevice(0);

	cudaStatus = cudaMalloc((void**)&gpuWorld, sizeof(mat4x4));
	cudaStatus = cudaMalloc((void**)&gpuMatrix, sizeof(mat4x4));
	cudaStatus = cudaMalloc((void**)&gpuProjection, sizeof(mat4x4));
	cudaStatus = cudaMalloc((void**)&gpuVerts, size * sizeof(vec3d));
	cudaStatus = cudaMemcpy(gpuWorld, &World, sizeof(mat4x4), cudaMemcpyHostToDevice);
	cudaStatus = cudaMemcpy(gpuMatrix, &_camera->_matrix, sizeof(mat4x4), cudaMemcpyHostToDevice);
	cudaStatus = cudaMemcpy(gpuProjection, &_camera->projection, sizeof(mat4x4), cudaMemcpyHostToDevice);
	cudaStatus = cudaMemcpy(gpuVerts, _mesh->vertices.begin()._Ptr, size * sizeof(vec3d), cudaMemcpyHostToDevice);

	calculateGPU << < 1, size >> > (gpuVerts, gpuWorld, gpuMatrix, gpuProjection);

	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "gpu calculation launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto Error;
	}

	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching gpu calculation!\n", cudaStatus);
		goto Error;
	}

	for (int i = 0; i < size; i++) {
		vec3d v;
		cudaStatus = cudaMemcpy(&v, (i * sizeof(vec3d)) + gpuVerts, sizeof(vec3d), cudaMemcpyDeviceToHost);
		_verts->push_back(v);
	}
Error:
	cudaFree(gpuWorld);
	cudaFree(gpuMatrix);
	cudaFree(gpuProjection);
	cudaFree(gpuVerts);
}