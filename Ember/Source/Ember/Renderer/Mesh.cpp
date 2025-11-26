// Mesh.cpp
// 网格
// created by FirstRay2001, Nov/15/2025

#include "emberpch.h"
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include "Mesh.h"
#include "Ember/ResourceManager/ShaderLibrary.h"
#include "Ember/ResourceManager/TextureLibrary.h"

namespace Ember
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	Ref<Mesh> Mesh::CreateFromAssimpMesh(const aiMesh* mesh, const aiScene* scene, const std::string& modelPath)
	{
		std::vector<float> vertices;
		std::vector<uint32_t> indices;
		vertices.resize(mesh->mNumVertices * 8); // 3位置 + 3法线 + 2纹理坐标

		if (mesh->mNumVertices == 23253)
		{
			EMBER_CORE_WARN("23253");
		}

		// 提取顶点数据
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			if (i == 7625)
			{
				EMBER_CORE_WARN("Processing vertex 7625");
			}

			Vertex& vertex = *(Vertex*)&vertices[i * 8];
			vertex.Position = {
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			};
			if (mesh->HasNormals())
			{
				vertex.Normal = {
					mesh->mNormals[i].x,
					mesh->mNormals[i].y,
					mesh->mNormals[i].z
				};
			}
			else
			{
				vertex.Normal = { 0.0f, 0.0f, 0.0f };
			}

			if (mesh->HasTextureCoords(0))
			{
				vertex.TexCoords = {
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				};
			}
			else
			{
				vertex.TexCoords = { 0.0f, 0.0f };
			}
		}

		// 提取索引数据
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// 创建顶点缓冲区和索引缓冲区
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(float)));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoords" }
			});
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size()));

		// 创建顶点数组对象
		Ref<VertexArray> vertexArray = VertexArray::Create();
		vertexArray->AddVertexBuffer(vertexBuffer);
		vertexArray->SetIndexBuffer(indexBuffer);
		vertexArray->Unbind(); // 解绑 防止OpenGL状态污染

		// 创建材质
		Ref<Material> material = CreateRef<Material>();
		
		// 提取材质
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
			// 漫反射贴图
			if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString str;
				aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
				std::string texturePath = str.C_Str();
				Ref<Texture2D> diffuseTexture = Texture2D::Create(modelPath + "/" + texturePath);
				if(diffuseTexture->IsLoaded())
					material->SetAlbedoTexture(diffuseTexture);
			}
			// 高光贴图
			if (aiMat->GetTextureCount(aiTextureType_SPECULAR) > 0)
			{
				aiString str;
				aiMat->GetTexture(aiTextureType_SPECULAR, 0, &str);
				std::string texturePath = str.C_Str();
				Ref<Texture2D> specularTexture = Texture2D::Create(modelPath + "/" + texturePath);
				if(specularTexture->IsLoaded())
					material->SetSpecularTexture(specularTexture);
			}
			// 法线贴图
			if (aiMat->GetTextureCount(aiTextureType_NORMALS) > 0)
			{
				aiString str;
				aiMat->GetTexture(aiTextureType_NORMALS, 0, &str);
				std::string texturePath = str.C_Str();
				Ref<Texture2D> normalTexture = Texture2D::Create(modelPath + "/" + texturePath);
				if(normalTexture->IsLoaded())
					material->SetNormalMap(normalTexture);
			}
		}

		// 创建Shader
		Ref<Shader> shader = ShaderLibrary::Get().LoadSync("Asset/Shader/BlinnPhong.glsl"); // 暂时使用BlinnPhong着色器

		// 创建网格
		Ref<Mesh> meshRef = CreateRef<Mesh>(vertexArray, material, shader);

		return meshRef;
	}
}


