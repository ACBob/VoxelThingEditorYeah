// -- Model
// Handles various OpenGL functions for rendering models.
// (sets up the modelview matrix, loads textures, etc.)
// Holds vertices and things like that.

// TODO: not immediate-mode OpenGL

#include <QObject>

#include <QGLContext>

#include <QVector>

#include "../world/vector.hpp"

class Model : public QObject
{
	Q_OBJECT

  public:
	struct Vertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	};

	struct Face
	{
		int v1, v2, v3;
	};

	Model( QObject *parent = 0 );
	~Model();

	void loadFromObj( const QString &filename );
	bool saveToObj( const QString &filename );

	void render( QGLContext *context );

	void update(); // Refreshes the buffers.

	QVector<Vertex> m_vertices;
	QVector<Face> m_faces;

	Vector3f m_position;
};
