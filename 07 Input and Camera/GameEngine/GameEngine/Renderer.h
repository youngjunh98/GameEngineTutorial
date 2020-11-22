#pragma once

#include "Component.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"

class Renderer : public Component
{
public:
	Renderer ();
	virtual ~Renderer ();

	virtual void OnUpdate (float deltaTime) override;
	virtual void OnRender () override;

	void SetShaderProgram (ShaderProgram* shader);
	void SetMesh (Mesh* mesh);
	void SetTexture (Texture* texture);

private:
	ShaderProgram* m_shader;
	Mesh* m_mesh;
	Texture* m_texture;
};
