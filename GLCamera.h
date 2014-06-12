#ifndef _GLCAMERA_H_
#define _GLCAMERA_H_
#include <gl/glut.h>
#include <math.h>

struct Vector3f
{
	float x;
	float y;
	float z;
	Vector3f(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f) : x(x), y(y), z(z) {}

	void normalize() {
		float factor = sqrt(x*x + y*y + z*z);
		x = x/factor;
		y = y/factor;
		z = z/factor;
	}

	float dot(const Vector3f& v) const {
		return x*v.x + y*v.y + z*v.z;
	}
	Vector3f cross(const Vector3f& v) const {
		return Vector3f(y*v.z-v.y*z, z*v.x-v.z*x, x*v.y-v.x*y); 
	}
};

typedef Vector3f Vector3d;

class GLCamera
{
public:
    GLCamera();
    GLCamera(const Vector3d& pos, const Vector3d& target, const Vector3d& up);
    void setModelViewMatrix();
    void setShape(float viewAngle,float aspect,float Near,float Far);
    void slide(float du, float dv, float dn);
    void roll(float angle);
    void yaw(float angle);
    void pitch(float angle);
    float getDist();

private:
    Vector3d m_pos;
    Vector3d m_target;
    Vector3d m_up;
    Vector3d u,v,n;
};

#endif

