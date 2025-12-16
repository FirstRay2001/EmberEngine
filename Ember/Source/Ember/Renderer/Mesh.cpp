// Mesh.cpp
// 网格
// created by FirstRay2001, Nov/15/2025

#include "emberpch.h"
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include "Mesh.h"
#include "Model.h"
#include "Ember/Renderer/Animation/Skeleton.h"
#include "Ember/ResourceManager/ShaderLibrary.h"
#include "Ember/ResourceManager/TextureLibrary.h"

namespace Ember
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		int BoneIDs[MAX_BONE_INFLUENCE];
		float Weights[MAX_BONE_INFLUENCE];
	};

	Ref<Mesh> Mesh::CreateFromAssimpMesh_OBJ(const aiMesh* mesh, const aiScene* scene, const std::string& modelPath)
	{
		std::vector<float> vertices;
		std::vector<uint32_t> indices;
		vertices.resize(mesh->mNumVertices * 16); // 3位置 + 3法线 + 2纹理坐标 + 4骨骼ID + 4骨骼权重

		// 设置骨骼ID初始值为-1
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			float* vertexData = &vertices[i * 16];
			int* boneIDs = reinterpret_cast<int*>(&vertexData[8]);
			for (int j = 0; j < MAX_BONE_INFLUENCE; j++)
			{
				boneIDs[j] = -1;
			}
		}

		// 提取顶点数据
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex& vertex = *(Vertex*)&vertices[i * 16];
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
			{ ShaderDataType::Float2, "a_TexCoords" },
			{ ShaderDataType::Int4,   "a_BoneIDs" },
			{ ShaderDataType::Float4, "a_Weights" }
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
		Ref<Shader> shader = ShaderLibrary::Get().LoadSync("Asset/Shader/BlinnPhong.glsl"); // TODO: 暂时使用BlinnPhong着色器

		// 创建网格
		Ref<Mesh> meshRef = CreateRef<Mesh>(vertexArray, material, shader);

		meshRef->m_UseSkeleton = false;

		return meshRef;
	}

	Ref<Mesh> Mesh::CreateFromAssimpMesh_FBX(const aiMesh* mesh, const aiScene* scene, const std::string& modelPath, Ref<Model> model)
	{
		std::vector<float> vertices;
		std::vector<uint32_t> indices;
		vertices.resize(mesh->mNumVertices * 16); // 3位置 + 3法线 + 2纹理坐标 + 4骨骼ID + 4骨骼权重

		// 设置骨骼ID初始值为-1
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			float* vertexData = &vertices[i * 16];
			int* boneIDs = reinterpret_cast<int*>(&vertexData[8]);
			for (int j = 0; j < MAX_BONE_INFLUENCE; j++)
			{
				boneIDs[j] = -1;
			}
		}

		// 提取顶点数据
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex& vertex = *(Vertex*)&vertices[i * 16];
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

		// 遍历所有骨骼
		for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			aiBone* bone = mesh->mBones[boneIndex];

			// 遍历此骨骼影响的顶点
			for (unsigned int weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex)
			{
				const aiVertexWeight& weight = bone->mWeights[weightIndex];

				for (int j = 0; j < MAX_BONE_INFLUENCE; j++)
				{
					Vertex& vertex = *(Vertex*)&vertices[weight.mVertexId * 16];
					if (vertex.BoneIDs[j] == -1)
					{
						vertex.BoneIDs[j] = model->GetSkeleton()->FindIndex(bone->mName.C_Str());
						vertex.Weights[j] = weight.mWeight;
						break;
					}
				}
			}
		}

		// 创建顶点缓冲区和索引缓冲区
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(float)));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoords" },
			{ ShaderDataType::Int4,   "a_BoneIDs" },
			{ ShaderDataType::Float4, "a_Weights" }
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
				if (diffuseTexture->IsLoaded())
					material->SetAlbedoTexture(diffuseTexture);
			}

			// 高光贴图
			if (aiMat->GetTextureCount(aiTextureType_SPECULAR) > 0)
			{
				aiString str;
				aiMat->GetTexture(aiTextureType_SPECULAR, 0, &str);
				std::string texturePath = str.C_Str();
				Ref<Texture2D> specularTexture = Texture2D::Create(modelPath + "/" + texturePath);
				if (specularTexture->IsLoaded())
					material->SetSpecularTexture(specularTexture);
			}

			// 法线贴图
			if (aiMat->GetTextureCount(aiTextureType_NORMALS) > 0)
			{
				aiString str;
				aiMat->GetTexture(aiTextureType_NORMALS, 0, &str);
				std::string texturePath = str.C_Str();
				Ref<Texture2D> normalTexture = Texture2D::Create(modelPath + "/" + texturePath);
				if (normalTexture->IsLoaded())
					material->SetNormalMap(normalTexture);
			}
		}

		// 创建Shader
		Ref<Shader> shader = ShaderLibrary::Get().LoadSync("Asset/Shader/BlinnPhong.glsl"); // TODO: 暂时使用BlinnPhong着色器

		// 创建网格
		Ref<Mesh> meshRef = CreateRef<Mesh>(vertexArray, material, shader);

		meshRef->m_UseSkeleton = true; // FBX网格使用骨骼

		return meshRef;
	}
}


