// -- VoxelThingYeah's vector class
#pragma once

#include <math.h>

#define DEG2RAD ( M_PI / 180.0 )
#define RAD2DEG ( 180.0 / M_PI )

// Forward declarations
template <typename T> class Vector3;
template <typename T> class Vector4;

// QT:
#include <QDebug>

// Generic vector class of 3 elements
// Defines the math operations
// Woah mamma, that's very verbose!
// Thanks C++ for making me do this
template <typename T> class Vector3
{
  public:
	T x, y, z;

	Vector3() : x( 0 ), y( 0 ), z( 0 ){};
	Vector3( T i ) : x( i ), y( i ), z( i ){};
	Vector3( T x, T y ) : x( x ), y( y ), z( 0 ){};
	Vector3( T x, T y, T z ) : x( x ), y( y ), z( z ){};
	Vector3( const Vector3<T> &v ) : x( v.x ), y( v.y ), z( v.z ){};

	Vector3<T> &operator=( const Vector3<T> &v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3<T> &operator+=( const Vector3<T> &v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3<T> &operator-=( const Vector3<T> &v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3<T> &operator*=( const Vector3<T> &v )
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vector3<T> &operator/=( const Vector3<T> &v )
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	Vector3<T> &operator*=( T s )
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vector3<T> &operator/=( T s )
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	Vector3<T> operator+( const Vector3<T> &v ) const { return Vector3<T>( x + v.x, y + v.y, z + v.z ); }

	Vector3<T> operator-( const Vector3<T> &v ) const { return Vector3<T>( x - v.x, y - v.y, z - v.z ); }

	Vector3<T> operator*( const Vector3<T> &v ) const { return Vector3<T>( x * v.x, y * v.y, z * v.z ); }

	Vector3<T> operator/( const Vector3<T> &v ) const { return Vector3<T>( x / v.x, y / v.y, z / v.z ); }

	Vector3<T> operator*( T s ) const { return Vector3<T>( x * s, y * s, z * s ); }

	Vector3<T> operator/( T s ) const { return Vector3<T>( x / s, y / s, z / s ); }

	T Magnitude() const { return sqrt( x * x + y * y + z * z ); }

	Vector3<T> Normal() const
	{
		T m = Magnitude();
		return Vector3<T>( x / m, y / m, z / m );
	}

	Vector3<T> Floor() const { return Vector3<T>( floor( x ), floor( y ), floor( z ) ); }
	Vector3<T> Ceil() const { return Vector3<T>( ceil( x ), ceil( y ), ceil( z ) ); }

	Vector3<T> Lerp( const Vector3<T> &v, T t ) const
	{
		return Vector3<T>( x + ( v.x - x ) * t, y + ( v.y - y ) * t, z + ( v.z - z ) * t );
	}

	Vector3<T> Lerp( const T v, T t ) const
	{
		return Vector3<T>( x + ( v - x ) * t, y + ( v - y ) * t, z + ( v - z ) * t );
	}

	Vector3<T> Inverse() const { return Vector3<T>( -x, -y, -z ); }

	Vector3<T> Rotate( const Vector3<T> &v, T angle ) const
	{
		T ca = cos( angle );
		T sa = sin( angle );
		return Vector3<T>( x * ca + y * sa, -x * sa + y * ca, z );
	}
	// It's assumed here that the vector is in degrees
	Vector3<T> Rotate( const Vector3<T> &v ) const
	{
		return RotateAxis( 0, v.x * DEG2RAD ).RotateAxis( 1, v.y * DEG2RAD ).RotateAxis( 2, v.z * DEG2RAD );
	}

	Vector3<T> RotateAxis( int axis, T angle ) const
	{
		switch ( axis )
		{
			case 0:
				return Vector3<T>( x, y * cos( angle ) - z * sin( angle ), y * sin( angle ) + z * cos( angle ) );
			case 1:
				return Vector3<T>( x * cos( angle ) + z * sin( angle ), y, -x * sin( angle ) + z * cos( angle ) );
			case 2:
				return Vector3<T>( x * cos( angle ) - y * sin( angle ), x * sin( angle ) + y * cos( angle ), z );
		}
		return Vector3<T>( x, y, z );
	}

	Vector3<T> LookingAt( const Vector3<T> &v ) const { return Vector3<T>( v.x - x, v.y - y, v.z - z ); }
	Vector3<T> RotateTowards( const Vector3<T> &v ) const { return Rotate( LookingAt( v ), acos( Dot( v ) ) ); }

	T Dot( const Vector3<T> &v ) const { return x * v.x + y * v.y + z * v.z; }

	Vector3<T> Cross( const Vector3<T> &v ) const
	{
		return Vector3<T>( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
	}

	T Distance( const Vector3<T> &v ) const { return ( *this - v ).Magnitude(); }

	// Math hurts my head
	// Copied the project/unproject from the glu library (roughly)
	// Substituted a Matrix4x4 with a Vector4<Vector4<T>>

	// Takes a point in screen space and returns a point in world space
	Vector3<T> UnProject( const Vector3<T> &v, const Vector4<Vector4<T>> &m, const Vector4<T> &viewport ) const
	{
		Vector4<T> v2 = Vector4<T>( v.x, v.y, v.z, 1.0f );
		v2			  = m * v2;
		v2 /= v2.w;
		v2 = v2 * viewport;
		return Vector3<T>( v2.x, v2.y, v2.z );
	}

	// Takes a point in world space and returns a point in screen space
	// TODO: is this correct?
	Vector3<T> Project( const Vector3<T> &v, const Vector4<Vector4<T>> &m, const Vector4<T> &viewport ) const
	{
		Vector4<T> v2 = Vector4<T>( v.x, v.y, v.z, 1.0f );
		v2			  = m * v2;
		v2 /= v2.w;
		v2 = v2 * viewport;
		return Vector3<T>( v2.x, v2.y, v2.z );
	}

	// Comparison

	bool operator==( const Vector3<T> &v ) const { return x == v.x && y == v.y && z == v.z; }
	bool operator!=( const Vector3<T> &v ) const { return x != v.x || y != v.y || z != v.z; }
	bool operator>( const Vector3<T> &v ) const { return x > v.x && y > v.y && z > v.z; }
	bool operator<( const Vector3<T> &v ) const { return x < v.x || y < v.y || z < v.z; }
	bool operator>=( const Vector3<T> &v ) const { return x >= v.x && y >= v.y && z >= v.z; }
	bool operator<=( const Vector3<T> &v ) const { return x <= v.x && y <= v.y && z <= v.z; }

	// Conversion to arbitrary types
	template <typename U> operator Vector3<U>() const { return Vector3<U>( (U)x, (U)y, (U)z ); }

	// QT:
	// purely for qDebug()
	friend QDebug operator<<( QDebug dbg, const Vector3<T> &v )
	{
		dbg.nospace() << "Vector3<" << typeid( T ).name() << ">(" << v.x << ", " << v.y << ", " << v.z << ")";
		return dbg.space();
	}
};

template <typename T> Vector3<T> inline RandomVector3( T min, T max )
{
	return Vector3<T>( rand() * ( max - min ) / RAND_MAX + min, rand() * ( max - min ) / RAND_MAX + min,
					   rand() * ( max - min ) / RAND_MAX + min );
}

template <typename T> Vector3<T> inline RandomVector3( Vector3<T> min, Vector3<T> max )
{
	return Vector3<T>( rand() * ( max.x - min.x ) / RAND_MAX + min.x, rand() * ( max.y - min.y ) / RAND_MAX + min.y,
					   rand() * ( max.z - min.z ) / RAND_MAX + min.z );
}

using Vector3f = Vector3<float>;
using Vector3i = Vector3<int>;
using Vector3c = Vector3<char>;

// Generic vector class of 4 elements
// (It's like Vector3 but with w, and some limited functions)
template <typename T> class Vector4
{
  public:
	T x, y, z, w;

	Vector4() : x( 0 ), y( 0 ), z( 0 ), w( 0 ) {}
	Vector4( T i ) : x( i ), y( i ), z( i ), w( i ){};
	Vector4( T x, T y ) : x( x ), y( y ), z( 0 ), w( 0 ){};
	Vector4( T x, T y, T z ) : x( x ), y( y ), z( z ), w( 0 ){};
	Vector4( T x, T y, T z, T w ) : x( x ), y( y ), z( z ), w( w ) {}
	Vector4( const Vector4<T> &v ) : x( v.x ), y( v.y ), z( v.z ), w( v.w ) {}
	Vector4( const Vector3<T> &v ) : x( v.x ), y( v.y ), z( v.z ), w( 0 ) {}

	Vector4<T> operator+( const Vector4<T> &v ) const { return Vector4<T>( x + v.x, y + v.y, z + v.z, w + v.w ); }
	Vector4<T> operator-( const Vector4<T> &v ) const { return Vector4<T>( x - v.x, y - v.y, z - v.z, w - v.w ); }
	Vector4<T> operator*( const Vector4<T> &v ) const { return Vector4<T>( x * v.x, y * v.y, z * v.z, w * v.w ); }
	Vector4<T> operator/( const Vector4<T> &v ) const { return Vector4<T>( x / v.x, y / v.y, z / v.z, w / v.w ); }

	Vector4<T> operator+( T v ) const { return Vector4<T>( x + v, y + v, z + v, w + v ); }
	Vector4<T> operator-( T v ) const { return Vector4<T>( x - v, y - v, z - v, w - v ); }
	Vector4<T> operator*( T v ) const { return Vector4<T>( x * v, y * v, z * v, w * v ); }
	Vector4<T> operator/( T v ) const { return Vector4<T>( x / v, y / v, z / v, w / v ); }

	Vector4<T> &operator=( const Vector4<T> &v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}
	Vector4<T> &operator+=( const Vector4<T> &v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	Vector4<T> &operator-=( const Vector4<T> &v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	Vector4<T> &operator*=( const Vector4<T> &v )
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}
	Vector4<T> &operator/=( const Vector4<T> &v )
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	Vector4<T> &operator+=( T v )
	{
		x += v;
		y += v;
		z += v;
		w += v;
		return *this;
	}
	Vector4<T> &operator-=( T v )
	{
		x -= v;
		y -= v;
		z -= v;
		w -= v;
		return *this;
	}
	Vector4<T> &operator*=( T v )
	{
		x *= v;
		y *= v;
		z *= v;
		w *= v;
		return *this;
	}
	Vector4<T> &operator/=( T v )
	{
		x /= v;
		y /= v;
		z /= v;
		w /= v;
		return *this;
	}

	bool operator==( const Vector4<T> &v ) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
	bool operator!=( const Vector4<T> &v ) const { return x != v.x || y != v.y || z != v.z || w != v.w; }
	bool operator>( const Vector4<T> &v ) const { return x > v.x && y > v.y && z > v.z && w > v.w; }
	bool operator<( const Vector4<T> &v ) const { return x < v.x || y < v.y || z < v.z || w < v.w; }
	bool operator>=( const Vector4<T> &v ) const { return x >= v.x && y >= v.y && z >= v.z && w >= v.w; }
	bool operator<=( const Vector4<T> &v ) const { return x <= v.x && y <= v.y && z <= v.z && w <= v.w; }

	// Conversion to Vector3
	operator Vector3<T>() const { return Vector3<T>( x, y, z ); }

	// Arbitrary conversions
	template <typename U> operator Vector3<U>() const { return Vector3<U>( x, y, z ); }
	template <typename U> operator Vector4<U>() const { return Vector4<U>( x, y, z, w ); }

	// QT:
	// purely for qDebug()
	friend QDebug operator<<( QDebug dbg, const Vector4<T> &v )
	{
		dbg.nospace() << "Vector4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return dbg.space();
	}
};

template <typename T> Vector4<T> inline RandomVector4( T min, T max )
{
	return Vector4<T>( rand() * ( max - min ) / RAND_MAX + min, rand() * ( max - min ) / RAND_MAX + min,
					   rand() * ( max - min ) / RAND_MAX + min, rand() * ( max - min ) / RAND_MAX + min );
}

template <typename T> Vector4<T> inline RandomVector4( Vector4<T> min, Vector4<T> max )
{
	return Vector4<T>( rand() * ( max.x - min.x ) / RAND_MAX + min.x, rand() * ( max.y - min.y ) / RAND_MAX + min.y,
					   rand() * ( max.z - min.z ) / RAND_MAX + min.z, rand() * ( max.w - min.w ) / RAND_MAX + min.w );
}

using Vector4f = Vector4<float>;
using Vector4i = Vector4<int>;
using Vector4c = Vector4<char>;