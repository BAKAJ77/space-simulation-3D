#pragma once
#include "Engine/Graphics/MeshObject.h"
#include "Engine/Graphics/SceneCamera.h"
#include "Engine/Graphics/SceneLighting.h"

#include <assimp/scene.h>

class SceneModel
{
private:
	std::vector<MeshObject> m_meshes;
	const float m_shininess;

	glm::vec3 m_position, m_scale, m_rotationAxis;
	float m_rotationAngle;
	const std::string m_textureDir;

	const void* m_instancedArray;
	uint32_t m_numInstances;
private:
	void processNode(aiNode* node, const aiScene* scene); // Processes each node in the model
	MeshObject processMeshData(aiMesh* mesh, const aiScene* scene); // Processes and retrieves mesh's data belonging to the model

	std::vector<TextureData> getMatTextures(aiMaterial* mat, aiTextureType type) const; // Returns the textures retrieved from the aiMaterial pointer
	Material getGenericMat(aiMaterial* mat) const; // Returns a material object with ONLY phong components retrieved (and the shininess value)
public:
	SceneModel(const std::string& path, const std::string& texture_dir, float shininess = 128.0f,
		const void* instanced_array = nullptr, uint32_t num_instances = 0);
	~SceneModel();

	void setPosition(const glm::vec3& pos); // Sets the position of the model
	void setScale(const glm::vec3& scale); // Sets the scale of the model
	void setRotation(const glm::vec3& axis, float angle); // Sets the rotation state of the model

	void render(std::shared_ptr<ShaderProgram> shader, const SceneCamera& camera,
		const SpotLight* flashlight = nullptr) const; // Renders the whole model
public:
	const glm::vec3& getPosition() const; // Returns the position of model
};