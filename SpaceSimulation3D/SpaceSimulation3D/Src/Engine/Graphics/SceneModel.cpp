#include "SceneModel.h"
#include "Engine/Utils/LoggingManager.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>

SceneModel::SceneModel(const std::string& path, const std::string& texture_dir, float shininess,
	const void* instanced_array, uint32_t num_instances) :
	m_shininess(shininess), m_position(glm::vec3(1.0f)), m_scale(glm::vec3(1.0f)),
	m_rotationAxis(glm::vec3(1.0f)), m_rotationAngle(0.0f), m_textureDir(texture_dir), 
	m_instancedArray(instanced_array), m_numInstances(num_instances)
{
	// Load the model data from file
	Assimp::Importer importer;
	const aiScene* modelScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!modelScene || modelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !modelScene->mRootNode)
		OutputLog("An error occurred while loading the model from path: " + path, Logging::Severity::FATAL);
	else
		this->processNode(modelScene->mRootNode, modelScene);
}

SceneModel::~SceneModel() {}

void SceneModel::processNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh the current node is pointing to
	for (uint32_t index = 0; index < node->mNumMeshes; index++)
		m_meshes.emplace_back(this->processMeshData(scene->mMeshes[node->mMeshes[index]], scene));

	// Iterate through each of the child nodes of the current nodes (and so on)
	for (uint32_t index = 0; index < node->mNumChildren; index++)
		this->processNode(node->mChildren[index], scene);
}

MeshObject SceneModel::processMeshData(aiMesh* mesh, const aiScene* scene)
{
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	Material material;

	// Retrieve each vertex's data in the mesh
	for (uint32_t index = 0; index < mesh->mNumVertices; index++)
	{
		VertexData vertex;

		auto& vertexPos = vertex.m_vertexPos;
		vertexPos.x = mesh->mVertices[index].x;
		vertexPos.y = mesh->mVertices[index].y;
		vertexPos.z = mesh->mVertices[index].z;

		auto& normalPos = vertex.m_normalPos;
		normalPos.x = mesh->mNormals[index].x;
		normalPos.y = mesh->mNormals[index].y;
		normalPos.z = mesh->mNormals[index].z;

		if (mesh->mTextureCoords[0])
		{
			auto& texturePos = vertex.m_texturePos;
			texturePos.x = mesh->mTextureCoords[0][index].x;
			texturePos.y = mesh->mTextureCoords[0][index].y;
		}
		else
			vertex.m_texturePos = glm::vec2(0.0f);

		vertices.emplace_back(vertex);
	}

	// Next we retrieve the indices data in the mesh
	for (uint32_t index = 0; index < mesh->mNumFaces; index++)
	{
		aiFace face = mesh->mFaces[index];
		for (uint32_t count = 0; count < face.mNumIndices; count++)
			indices.emplace_back(face.mIndices[count]);
	}

	// Finally we retrieve the mesh's material
	material.shininess = m_shininess;

	if (mesh->mMaterialIndex >= 0)
	{
		auto& textures = material.m_textures;
		
		auto diffuseTextures = this->getMatTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
		auto specularTextures = this->getMatTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());

		if (textures.empty())
			material = this->getGenericMat(scene->mMaterials[mesh->mMaterialIndex]);
	}

	return MeshObject(vertices, indices, material, m_instancedArray, m_numInstances);
}

std::vector<TextureData> SceneModel::getMatTextures(aiMaterial* mat, aiTextureType type) const
{
	std::vector<TextureData> textures;
	for (uint32_t index = 0; index < mat->GetTextureCount(type); index++)
	{
		aiString path;
		mat->GetTexture(type, index, &path);

		TextureData textureData;
		if (type == aiTextureType_DIFFUSE)
			textureData.m_type = "DIFFUSE_TEXTURE";
		else if (type == aiTextureType_SPECULAR)
			textureData.m_type = "SPECULAR_TEXTURE";

		textureData.m_texture = std::make_shared<TextureComponent>(m_textureDir + "/" + std::string(path.C_Str()));
		//textureData.m_texture->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		textures.emplace_back(textureData);
	}

	return textures;
}

Material SceneModel::getGenericMat(aiMaterial* mat) const
{
	Material material;
	aiColor3D temp;

	mat->Get(AI_MATKEY_COLOR_AMBIENT, temp);
	material.m_ambient = glm::vec3(temp.r, temp.g, temp.b);
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, temp);
	material.m_diffuse = glm::vec3(temp.r, temp.g, temp.b);
	mat->Get(AI_MATKEY_COLOR_SPECULAR, temp);
	material.m_specular = glm::vec3(temp.r, temp.g, temp.b);

	return material;
}

void SceneModel::setPosition(const glm::vec3& pos)
{
	m_position = pos;
}

void SceneModel::setScale(const glm::vec3& scale)
{
	m_scale = scale;
}

void SceneModel::setRotation(const glm::vec3& axis, float angle)
{
	m_rotationAxis = axis;
	m_rotationAngle = angle;
}

void SceneModel::render(std::shared_ptr<ShaderProgram> shader, const SceneCamera& camera, 
	const SpotLight* flashlight) const
{
	glm::mat4 model;
	model = glm::translate(model, m_position);
	model = glm::scale(model, m_scale);
	model = glm::rotate(model, glm::radians(m_rotationAngle), m_rotationAxis);

	shader->bindProgram();
	shader->setUniform("model", model);
	shader->setUniform("cameraPos", camera.getPosition());

	shader->setUniform("sunlight.direction", glm::vec3(0.0f, -0.3f, 0.65f));
	shader->setUniform("sunlight.ambient", glm::vec3(0.0f));
	shader->setUniform("sunlight.diffuse", glm::vec3(1.0f));
	shader->setUniform("sunlight.specular", glm::vec3(0.5f));

	// Setup the other lighting uniforms in shader
	if (flashlight)
	{
		shader->setUniform("flashlight.enabled", flashlight->m_enabled);

		shader->setUniform("flashlight.position", flashlight->m_position);
		shader->setUniform("flashlight.direction", flashlight->m_direction);

		shader->setUniform("flashlight.ambient", flashlight->m_ambient);
		shader->setUniform("flashlight.diffuse", flashlight->m_diffuse);
		shader->setUniform("flashlight.specular", flashlight->m_specular);

		shader->setUniform("flashlight.constant", flashlight->m_constant);
		shader->setUniform("flashlight.linear", flashlight->m_linear);
		shader->setUniform("flashlight.specular", flashlight->m_specular);

		shader->setUniform("flashlight.innerCutOff", flashlight->m_innerCutOff);
		shader->setUniform("flashlight.outerCutOff", flashlight->m_outerCutOff);
	}

	// Render all meshes in model
	for (auto& mesh : m_meshes)
		mesh.render(shader);
}

const glm::vec3& SceneModel::getPosition() const
{
	return m_position;
}