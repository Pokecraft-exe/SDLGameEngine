#include "game.h"

extern void terminateProgram();

SDLGameEngine::SDLGameEngine() {
}
void SDLGameEngine::GameThread() {
	if (!OnUserCreates()) quit = true;

	auto time1 = std::chrono::system_clock::now();
	auto time0 = std::chrono::system_clock::now();
	while (!quit) {
		do {
			if (quit) return;
			time1 = std::chrono::system_clock::now();
			std::chrono::duration<float> _elapsedTime = time1 - time0;
			time0 = time1;
			fElapsedTime = _elapsedTime.count();
			_fps = 1 / fElapsedTime;
		} while (OnUserUpdate(fElapsedTime) && !quit);
	}

	if (!screen.isPersistant()) screen.quit();
	terminateProgram();
	return;
}
mat4x4 SDLGameEngine::_matrixMakeIdentity()
{
	mat4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}
void SDLGameEngine::_meshApplyRotations(__parameters r_mesh) {
	r_mesh.Rx->m[0][0] = 1.0f;
	r_mesh.Rx->m[1][1] = cosf(r_mesh._mesh->RotationX);
	r_mesh.Rx->m[1][2] = sinf(r_mesh._mesh->RotationX);
	r_mesh.Rx->m[2][1] = -sinf(r_mesh._mesh->RotationX);
	r_mesh.Rx->m[2][2] = cosf(r_mesh._mesh->RotationX);
	r_mesh.Rx->m[3][3] = 1.0f;

	r_mesh.Ry->m[0][0] = cosf(r_mesh._mesh->RotationY);
	r_mesh.Ry->m[0][2] = sinf(r_mesh._mesh->RotationY);
	r_mesh.Ry->m[2][0] = -sinf(r_mesh._mesh->RotationY);
	r_mesh.Ry->m[1][1] = 1.0f;
	r_mesh.Ry->m[2][2] = cosf(r_mesh._mesh->RotationY);
	r_mesh.Ry->m[3][3] = 1.0f;

	r_mesh.Rz->m[0][0] = cosf(3.14 + r_mesh._mesh->RotationZ);
	r_mesh.Rz->m[0][1] = sinf(3.14 + r_mesh._mesh->RotationZ);
	r_mesh.Rz->m[1][0] = -sinf(3.14 + r_mesh._mesh->RotationZ);
	r_mesh.Rz->m[1][1] = cosf(3.14 + r_mesh._mesh->RotationZ);
	r_mesh.Rz->m[2][2] = 1.0f;
	r_mesh.Rz->m[3][3] = 1.0f;
}
void SDLGameEngine::_cameraApplyRotations(__parameters r_camera) {
	r_camera.Rx->m[0][0] = 1;
	r_camera.Rx->m[1][1] = cosf(r_camera._camera->RotationX);
	r_camera.Rx->m[1][2] = sinf(r_camera._camera->RotationX);
	r_camera.Rx->m[2][1] = -sinf(r_camera._camera->RotationX);
	r_camera.Rx->m[2][2] = cosf(r_camera._camera->RotationX);
	r_camera.Rx->m[3][3] = 1;

	r_camera.Ry->m[0][0] = cosf(r_camera._camera->RotationY);
	r_camera.Ry->m[0][2] = sinf(r_camera._camera->RotationY);
	r_camera.Ry->m[2][0] = -sinf(r_camera._camera->RotationY);
	r_camera.Ry->m[1][1] = 1.0f;
	r_camera.Ry->m[2][2] = cosf(r_camera._camera->RotationY);
	r_camera.Ry->m[3][3] = 1.0f;

	r_camera.Rz->m[0][0] = cosf(r_camera._camera->RotationZ);
	r_camera.Rz->m[0][1] = sinf(r_camera._camera->RotationZ);
	r_camera.Rz->m[1][0] = -sinf(r_camera._camera->RotationZ);
	r_camera.Rz->m[1][1] = cosf(r_camera._camera->RotationZ);
	r_camera.Rz->m[2][2] = 1;
	r_camera.Rz->m[3][3] = 1;
}
void SDLGameEngine::_meshApplyTransations(__parameters t_mesh) {
	t_mesh.t->m[0][0] = 1.0f;
	t_mesh.t->m[1][1] = 1.0f;
	t_mesh.t->m[2][2] = 1.0f;
	t_mesh.t->m[3][3] = 1.0f;
	t_mesh.t->m[3][0] = t_mesh._mesh->origin.x;
	t_mesh.t->m[3][1] = t_mesh._mesh->origin.y;
	t_mesh.t->m[3][2] = t_mesh._mesh->origin.z;
}
void SDLGameEngine::_drawPolygons(camera* _camera, light* _light, vector<vec3d>* _verts, vector<polygon>* _polys) {
	for (size_t i = 0; i < _polys->size(); i++) {
		vector<vec3d> verts;
		material m;
		for (size_t id : _polys->at(i).p) {
			//cout << id << " " << _verts->size() << endl;
			verts.push_back(_verts->at(id));
			m = _polys->at(i).m;
		}
		vec3d normal = calculate_normal(verts);
		float l = sqrtf(_light->orientation.x * _light->orientation.x +
			_light->orientation.y * _light->orientation.y +
			_light->orientation.z * _light->orientation.z);
		_light->orientation.x /= l; _light->orientation.y /= l; _light->orientation.z /= l;

		float dp = normal.x * _light->orientation.x + normal.y * _light->orientation.y + normal.z * _light->orientation.z;

		Color c = 0;

		c.R(dp * 255.0f);
		c.G(dp * 255.0f);
		c.B(dp * 255.0f);

		if (Vector_DotProduct(normal, _camera->_pointAt) < 0.0f) {
			screen.fillPolygon(verts, c);
			screen.drawPolygon(verts, 0xff6666);
		}
	}
}
void SDLGameEngine::start() {
	int cudaDevice = 0;
	cudaGetDeviceCount(&cudaDevice);

	if (cudaDevice > 0) {
		currentGpu = gpu::Cuda;

		//_calculatePolygons = &calculatePolygonsCuda;

		cout << "Switching to cuda device gpu" << std::endl;
	}
	else {
		currentGpu = gpu::Intel;

		_calculatePolygons = &calculatePolygonsIntel;

		cout << "Switching to intel device gpu" << std::endl;
	}

	GameThread();
}
void SDLGameEngine::CreateNewScreen(string Name, int w, int h) {
	screen = window(Name.c_str(), w, h);
}
float SDLGameEngine::fps() {
	return _fps;
}
mesh SDLGameEngine::getMesh(string sFilename) {
	ifstream f(sFilename);
	bool file_read = false;
	mesh _mesh;
	if (!f.is_open()) return _mesh;

	// Local cache of verts
	vector<vec3d> verts;
	vector<vec3d> normals;

	while (!f.eof())
	{
		char line[128];
		f.getline(line, 128);

		strstream s;
		s << line;
		char junk;

		if (line[0] == 'v')
		{
			if (line[1] == 'n') {
				vec3d n;
				s >> junk >> n.x >> n.y >> n.z;
				normals.push_back(n);
			}
			else {
				vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
		}

		if (line[0] == 'f')
		{
			polygon _polygon;
			vector<int> f;
			s >> junk;
			vector<string> fi;
			string fs = "";
			while (!s.eof()) {
				fs = "";
				s >> fs;
				fi.push_back(fs);
			}
			for (string& _s : fi) {
				if (_s.find("/") == -1) {
					f.push_back(stoi(_s));
				}
				else {
					f.push_back(stoi(_s.substr(0, _s.find('/'))));
					/*if (normals.size() > 0) {
						_polygon.normal = normals[stoi(_s.substr(_s.rfind('/') + 1))-1];
						_polygon.has_normal = true;
					}*/
				}
			}

			for (int i : f) {
				_polygon.p.push_back(i - 1);
			}
			_mesh.polygons.push_back(_polygon);

			file_read = true;
		}
	}

	_mesh.vertices = verts;

	f.close();
	if (file_read) std::cout << "file \"" << sFilename << "\" read successfully, imported "
		<< verts.size() << " vertices, " << normals.size() << " normals and "
		<< _mesh.polygons.size() << " polygones!" << std::endl;
	return _mesh;
}
void SDLGameEngine::render(mesh* _mesh, camera* _camera, light* _light) {

	vector<vec3d> verts;
	vector<polygon> polys = _mesh->polygons;
	vector<polygon> clipped_polys;
	__parameters param = { &verts, _mesh, _camera, _light }; param.end = false;

	mat4x4 World, Rx, Ry, Rz, t;

	param.Rx = &Rx;
	param.Ry = &Ry;
	param.Rz = &Rz;
	param.t = &t;
	param.World = &World;

	size_t vector_size = _mesh->vertices.size();

	verts.resize(vector_size);

	_meshApplyRotations(param);
	_meshApplyTransations(param);

	World = _matrixMakeIdentity();
	World = Matrix_MultiplyMatrix(Ry, Rz);
	World = Matrix_MultiplyMatrix(World, Rx);
	World = Matrix_MultiplyMatrix(World, t);

	mat4x4 _cameraRotationX;
	mat4x4 _cameraRotationY;
	mat4x4 _cameraRotationZ;
	mat4x4 _cameraRotation;

	param.Rx = &_cameraRotationX;
	param.Ry = &_cameraRotationY;
	param.Rz = &_cameraRotationZ;

	_cameraApplyRotations(param);

	_cameraRotation = Matrix_MultiplyMatrix(_cameraRotationZ, _cameraRotationY);
	_cameraRotation = Matrix_MultiplyMatrix(_cameraRotation, _cameraRotationZ);

	_camera->_up = { 0, 1, 0 };
	_camera->_target = { 0, 0, 1 };
	_camera->_pointAt = MultiplyMatrixVector(_cameraRotation, _camera->_target);

	_camera->_target = Vector_Add(_camera->position, _camera->_pointAt);

	mat4x4 __matrix = Matrix_PointAt(_camera->position, _camera->_target, _camera->_up);
	_camera->_matrix = __matrix; Matrix_QuickInverse(__matrix);

	param.World = &World;

	param.sx = screen.sizex();
	param.sy = screen.sizey();

	// delegates the triangle calculation to another thread while triangulating
	_calculatePolygons(param, (void*)this);

	/*cout << "VERTS SIZE: " << verts.size() << std::endl;

	int i = 0;
	for (vec3d v : verts) {
		cout << "v[" << i << "] x = " << v.x << " y = " << v.y << " z = " << v.z << std::endl;
		i++;
	}

	vec3d plane_n = { 0.0f, 0.0f, 1.0f };
	vec3d plane_p = { 0.0f, 0.0f, 0.5f };

	auto dist = [&](vec3d& p)
	{
		vec3d n = p; n.normalise();
		return (n.z - Vector_DotProduct(plane_n, plane_p));
	};

	/*for (polygon& p : polys) {
		/*for (size_t& i : p.p) {
			vec3d& v = verts[i];
			if (v.x <= 1) v.x = 1;
			if (v.x >= screen.SCREEN_WIDTH - 1) { v.x = screen.SCREEN_WIDTH - 1; }
			if (v.y <= 1) v.y = 1;
			if (v.y >= screen.SCREEN_HEIGHT - 1) { v.y = screen.SCREEN_HEIGHT - 1; }
		}
		polygon p1, p2;
		int num = polygon_ClipAgainstPlane(plane_p, plane_n, &p, &p1, &p2, &verts);

		if (num == 1) {
			clipped_polys.push_back(p1);
		}
		else if (num == 2) {
			clipped_polys.push_back(p1);
			clipped_polys.push_back(p2);
		}
	}*/

	/*for (auto& triToRaster : polys)
	{
		// Clip triangles against all four screen edges, this could yield
		// a bunch of triangles, so create a queue that we traverse to
		//  ensure we only test new triangles generated against planes
		polygon clipped[2];
		list<polygon> listTriangles;

		// Add initial triangle
		listTriangles.push_back(triToRaster);
		int nNewTriangles = 1;

		for (int p = 0; p < 4; p++)
		{
			int nTrisToAdd = 0;
			while (nNewTriangles > 0)
			{
				// Take triangle from front of queue
				polygon test = listTriangles.front();
				listTriangles.pop_front();
				nNewTriangles--;

				// Clip it against a plane. We only need to test each
				// subsequent plane, against subsequent new triangles
				// as all triangles after a plane clip are guaranteed
				// to lie on the inside of the plane. I like how this
				// comment is almost completely and utterly justified
				switch (p)
				{
				case 0:	nTrisToAdd = polygon_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, &test, &clipped[0], &clipped[1], &verts); break;
				case 1:	nTrisToAdd = polygon_ClipAgainstPlane({ 0.0f, (float)screen.SCREEN_HEIGHT - 10, 0.0f }, { 0.0f, -1.0f, 0.0f }, &test, &clipped[0], &clipped[1], &verts); break;
				case 2:	nTrisToAdd = polygon_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, &test, &clipped[0], &clipped[1], &verts); break;
				case 3:	nTrisToAdd = polygon_ClipAgainstPlane({ (float)screen.SCREEN_WIDTH - 10, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, &test, &clipped[0], &clipped[1], &verts); break;
				}

				// Clipping may yield a variable number of triangles, so
				// add these new ones to the back of the queue for subsequent
				// clipping against next planes
				for (int w = 0; w < nTrisToAdd; w++)
					listTriangles.push_back(clipped[w]);
			}
			nNewTriangles = listTriangles.size();
		}

		for (polygon& p : listTriangles) {
			clipped_polys.push_back(p);
		}
	}*/
	auto sortPolys = [&](polygon& t1, polygon& t2)
		{
			size_t size1 = t1.p.size() - 1;
			size_t size2 = t2.p.size() - 1;
			float z1 = 0.0f;
			float z2 = 0.0f;
			for (int i = 0; i < size1; i++) {
				z1 += verts[t1.p[i]].z;
			}
			z1 /= size1 + 1;
			for (int i = 0; i < size2; i++) {
				z2 += verts[t2.p[i]].z;
			}
			z2 /= size2 + 1;
			return z1 > z2;
		};

	//sort(polys.begin(), polys.end(), sortPolys);

		// delegates the triangle drawing to another thread

	_drawPolygons(_camera, _light, &verts, &polys);
}
void SDLGameEngine::moveMeshX(mesh* t_mesh, float unit) {
	t_mesh->origin.x += unit;
}
void SDLGameEngine::moveMeshY(mesh* t_mesh, float unit) {
	t_mesh->origin.y += unit;
}
void SDLGameEngine::moveMeshZ(mesh* t_mesh, float unit) {
	t_mesh->origin.z += unit;
}
void SDLGameEngine::rotateMeshX(mesh* r_mesh, float degree) {
	r_mesh->RotationX = degree * 3.14159f / 180;
}
void SDLGameEngine::rotateMeshY(mesh* r_mesh, float degree) {
	r_mesh->RotationY = degree * 3.14159f / 180;
}
void SDLGameEngine::rotateMeshZ(mesh* r_mesh, float degree) {
	r_mesh->RotationZ = degree * 3.14159f / 180;
}
void SDLGameEngine::throwException(string file, string function, string str, bool ishost) {
	string message = "";
	if (ishost) message = "Exception from CPU";
	else		message = "Exception from GPU";

	int len = str.length() * 8;

	window screen2("Error", len, 100);

	screen2.fill(0xffffff);
	screen2.DrawString(file + "::" + function + "()", 10, 5, 0xff0000, 2);
	screen2.DrawString(str, 10, 25, 0xff0000, 1);
	screen2.DrawString(message, 10, 45, 0xff0000, 2);
	screen2.update();
	screen2.ShowCursor();
	quit = false;

	while (1) {

		while (screen.PollEvent()) {
			if (screen.e.type == QUIT) {
				quit = true;
			}
		}
		while (screen2.PollEvent()) {
			if (screen2.e.type == QUIT) {
				quit = true;
			}
		}
	}
}

void GPUThrow(void* ptr, string str) {
	((SDLGameEngine*)ptr)->throwException("intel.cc", "VectorCalculate()", str, false);
}