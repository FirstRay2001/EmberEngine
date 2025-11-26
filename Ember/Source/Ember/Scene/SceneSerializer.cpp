// SceneSerializer.cpp
// 场景序列化器
// created by FirstRay2001, Nov/20/2025

#include "emberpch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Component.h"
#include "Ember/ResourceManager/TextureLibrary.h"
#include "Ember/ResourceManager/ShaderLibrary.h"
#include "Ember/Core/Timer.h"

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <glm/glm.hpp>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		// 将 glm::vec3 编码为 YAML 节点
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		// 将 YAML 节点解码为 glm::vec3
		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		// 将 glm::vec4 编码为 YAML 节点
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		// 将 YAML 节点解码为 glm::vec4
		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	Emitter& operator<<(Emitter& out, const glm::vec3& vec)
	{
		out << Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(Emitter& out, const glm::vec4& vec)
	{
		out << Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
		return out;
	}
}

namespace Ember
{
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) :
		m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "EntityID" << YAML::Value << (uint64_t)(uint32_t)entity;

		// 标签组件
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "Tag";
			out << YAML::BeginMap;
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}

		// 变换组件
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "Transform";
			out << YAML::BeginMap;
			auto& position = entity.GetComponent<TransformComponent>().Position;
			auto& rotation = entity.GetComponent<TransformComponent>().Rotation;
			auto& scale = entity.GetComponent<TransformComponent>().Scale;
			out << YAML::Key << "Position" << YAML::Value << position;
			out << YAML::Key << "Rotation" << YAML::Value << rotation;
			out << YAML::Key << "Scale" << YAML::Value << scale;
			out << YAML::EndMap;
		}

		// Mesh组件
		if (entity.HasComponent<MeshComponent>())
		{
			out << YAML::Key << "Mesh";
			out << YAML::BeginMap;
			auto& mesh = entity.GetComponent<MeshComponent>().GetMesh();

			// VertexArray参数序列化
			{
				out << YAML::Key << "VertexArray";
				out << YAML::BeginMap;
				auto primitiveType = mesh.GetVertexArray()->GetPrimitiveType();
				std::string primitiveTypeName;
				switch (primitiveType)
				{
				case VertexArray::PrimitiveType::Cube:
					primitiveTypeName = "Cube";
					break;
				case VertexArray::PrimitiveType::Sphere:
					primitiveTypeName = "Sphere";
					break;
				default:
					primitiveTypeName = "None";
					break;
				};

				// 根据不同图元类型序列化不同参数
				out << YAML::Key << "Type" << YAML::Value << primitiveTypeName;
				if (primitiveType == VertexArray::PrimitiveType::Cube)
				{
					auto scale = mesh.GetVertexArray()->GetCubeScale();
					out << YAML::Key << "Scale" << YAML::Value << scale;
				}
				else if (primitiveType == VertexArray::PrimitiveType::Sphere)
				{
					auto radius = mesh.GetVertexArray()->GetSphereRadius();
					out << YAML::Key << "Radius" << YAML::Value << radius;
					auto sectorCount = mesh.GetVertexArray()->GetSphereSectorCount();
					out << YAML::Key << "SectorCount" << YAML::Value << sectorCount;
					auto stackCount = mesh.GetVertexArray()->GetSphereStackCount();
					out << YAML::Key << "StackCount" << YAML::Value << stackCount;
				}
				out << YAML::EndMap;
			}
			
			// 材质参数序列化
			{
				out << YAML::Key << "Material";
				out << YAML::BeginMap;
				auto& material = mesh.GetMaterial();
				out << YAML::Key << "Name" << YAML::Value << material->GetName();

				// 根据不同着色模型序列化不同参数
				const auto shadingModel = material->GetShadingModel();
				if (shadingModel == Material::ShadingModel::BlinnPhong)
				{
					out << YAML::Key << "ShadingModel" << YAML::Value << "BlinnPhong";

					// 漫反射颜色
					out << YAML::Key << "Albedo" << YAML::Value << material->GetAlbedo();
					if (material->HasAlbedoTexture())
						out << YAML::Key << "AlbedoTexturePath" << YAML::Value << material->GetAlbedoTexture()->GetPath();

					// 高光色
					out << YAML::Key << "SpecularColor" << YAML::Value << material->GetSpecularColor();
					if (material->HasSpecularTexture())
						out << YAML::Key << "SpecularTexturePath" << YAML::Value << material->GetSpecularTexture()->GetPath();

					// 高光强度
					out << YAML::Key << "Shininess" << YAML::Value << material->GetShininess();
					if (material->HasShininessTexture())
						out << YAML::Key << "ShininessTexturePath" << YAML::Value << material->GetShininessTexture()->GetPath();

					// 法线贴图
					if (material->HasNormalMap())
						out << YAML::Key << "NormalMapPath" << YAML::Value << material->GetNormalMap()->GetPath();

					// 自发光
					out << YAML::Key << "Emissive" << YAML::Value << material->GetEmissive();
					if (material->HasEmissiveTexture())
						out << YAML::Key << "EmissiveTexturePath" << YAML::Value << material->GetEmissiveTexture()->GetPath();

					// 透明度
					out << YAML::Key << "Opacity" << YAML::Value << material->GetOpacity();
				}
				else if (shadingModel == Material::ShadingModel::PBR)
				{
					out << YAML::Key << "ShadingModel" << YAML::Value << "PBR";

					// 漫反射颜色
					out << YAML::Key << "Albedo" << YAML::Value << material->GetAlbedo();
					if (material->HasAlbedoTexture())
						out << YAML::Key << "AlbedoTexturePath" << YAML::Value << material->GetAlbedoTexture()->GetPath();

					// 镜面反射色
					out << YAML::Key << "SpecularColor" << YAML::Value << material->GetSpecularColor();

					// 光泽度
					out << YAML::Key << "Glossiness" << YAML::Value << material->GetGlossiness();
					if (material->HasSpecularGlossinessTexture())
						out << YAML::Key << "SpecularGlossinessTexturePath" << YAML::Value << material->GetSpecularGlossinessTexture()->GetPath();

					// 法线贴图
					if (material->HasNormalMap())
						out << YAML::Key << "NormalMapPath" << YAML::Value << material->GetNormalMap()->GetPath();

					// 自发光
					out << YAML::Key << "Emissive" << YAML::Value << material->GetEmissive();
					if (material->HasEmissiveTexture())
						out << YAML::Key << "EmissiveTexturePath" << YAML::Value << material->GetEmissiveTexture()->GetPath();

					// 透明度
					out << YAML::Key << "Opacity" << YAML::Value << material->GetOpacity();
				}
				else
				{
					out << YAML::Key << "ShadingModel" << YAML::Value << "Unlit";
					out << YAML::Key << "Albedo" << YAML::Value << material->GetAlbedo();
					if (material->HasAlbedoTexture())
						out << YAML::Key << "AlbedoTexturePath" << YAML::Value << material->GetAlbedoTexture()->GetPath();
					if (material->HasNormalMap())
						out << YAML::Key << "NormalMapPath" << YAML::Value << material->GetNormalMap()->GetPath();
					out << YAML::Key << "Emissive" << YAML::Value << material->GetEmissive();
					if (material->HasEmissiveTexture())
						out << YAML::Key << "EmissiveTexturePath" << YAML::Value << material->GetEmissiveTexture()->GetPath();
					out << YAML::Key << "Opacity" << YAML::Value << material->GetOpacity();
				}
				out << YAML::EndMap;
			}
			

			// Shader参数序列化
			{
				out << YAML::Key << "ShaderPath" << YAML::Value << mesh.GetShader()->GetFilepath();

			}

			out << YAML::EndMap;
		}

		// Model组件
		if (entity.HasComponent<ModelComponent>())
		{
			out << YAML::Key << "Model";
			out << YAML::BeginMap;
			auto& model = entity.GetComponent<ModelComponent>().m_Model;
			out << YAML::Key << "ModelPath" << YAML::Value << (model ? model->GetPath() : "None");
			out << YAML::EndMap;
		}

		// Grid组件
		if (entity.HasComponent<GridComponent>())
		{
			out << YAML::Key << "Grid";
			out << YAML::BeginMap;

			// 网格线框Shader路径
			auto& gridComp = entity.GetComponent<GridComponent>();
			out << YAML::Key << "ShaderPath" << YAML::Value << gridComp.m_Shader->GetFilepath();

			// 网格线框VertexArray参数
			auto gridVAO = gridComp.m_Grid;
			out << YAML::Key << "VertexArray";
			out << YAML::BeginMap;
			auto gridSize = gridVAO->GetGridSize();
			auto gridDivisions = gridVAO->GetGridDivisions();
			out << YAML::Key << "Size" << YAML::Value << gridSize;
			out << YAML::Key << "Divisions" << YAML::Value << gridDivisions;
			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		// 摄像机组件
		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "Camera";
			out << YAML::BeginMap;

			const auto& camera = entity.GetComponent<CameraComponent>().m_Camera;
			out << YAML::Key << "Aspect" << YAML::Value << camera.GetAspectRatio();
			out << YAML::Key << "FOV" << YAML::Value << camera.GetFov();
			out << YAML::Key << "NearClip" << YAML::Value << camera.GetNearClip();
			out << YAML::Key << "FarClip" << YAML::Value << camera.GetFarClip();
			out << YAML::EndMap;
		}

		// 点光源组件
		if (entity.HasComponent<PointLightComponent>())
		{
			out << YAML::Key << "PointLight";
			out << YAML::BeginMap;
			auto& pointLight = entity.GetComponent<PointLightComponent>().m_PointLight;
			out << YAML::Key << "Ambient" << YAML::Value << pointLight.Ambient;
			out << YAML::Key << "Diffuse" << YAML::Value << pointLight.Diffuse;
			out << YAML::Key << "Specular" << YAML::Value << pointLight.Specular;
			out << YAML::Key << "Constant" << YAML::Value << pointLight.Constant;
			out << YAML::Key << "Linear" << YAML::Value << pointLight.Linear;
			out << YAML::Key << "Quadratic" << YAML::Value << pointLight.Quadratic;
			out << YAML::EndMap;
		}

		// 平行光组件
		if (entity.HasComponent<DirectionalLightComponent>())
		{
			out << YAML::Key << "DirectionalLight";
			out << YAML::BeginMap;
			auto& dirLight = entity.GetComponent<DirectionalLightComponent>().m_DirectionalLight;
			out << YAML::Key << "Ambient" << YAML::Value << dirLight.Ambient;
			out << YAML::Key << "Diffuse" << YAML::Value << dirLight.Diffuse;
			out << YAML::Key << "Specular" << YAML::Value << dirLight.Specular;
			out << YAML::Key << "Direction" << YAML::Value << dirLight.Direction;
			out << YAML::EndMap;
		}

		// 天空盒组件
		if (entity.HasComponent<SkyboxComponent>())
		{
			out << YAML::Key << "Skybox";
			out << YAML::BeginMap;
			auto skyboxPath = entity.GetComponent<SkyboxComponent>().m_Cubemap->GetPath();
			out << YAML::Key << "SkyboxPath" << YAML::Value << skyboxPath;
			auto shaderPath = entity.GetComponent<SkyboxComponent>().m_Shader->GetFilepath();
			out << YAML::Key << "ShaderPath" << YAML::Value << shaderPath;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::DeserializeEntity(const YAML::Node& entityNode, bool UseTransform)
	{
		uint64_t entityID = entityNode["EntityID"].as<uint64_t>();
		std::string tag = "Untitled";

		// 标签组件
		if (entityNode["Tag"])
			tag = entityNode["Tag"]["Tag"].as<std::string>();
		Entity entity = m_Scene->CreateEntity(tag);

		// 变换组件
		if (entityNode["Transform"] && UseTransform)
		{
			auto position = entityNode["Transform"]["Position"].as<glm::vec3>();
			auto rotation = entityNode["Transform"]["Rotation"].as<glm::vec3>();
			auto scale = entityNode["Transform"]["Scale"].as<glm::vec3>();
			auto& transform = entity.GetComponent<TransformComponent>();	// 已经创建Transform
			transform.Position = position;
			transform.Rotation = rotation;
			transform.Scale = scale;
		}

		// Mesh组件
		if (entityNode["Mesh"])
		{
			// VertexArray
			auto vaNode = entityNode["Mesh"]["VertexArray"];
			std::string vaType = vaNode["Type"].as<std::string>();
			Ref<VertexArray> vertexArray;
			if (vaType == "Cube")
			{
				auto scale = vaNode["Scale"].as<glm::vec3>();
				vertexArray = VertexArray::CreateCube(scale);
			}
			else if (vaType == "Sphere")
			{
				float radius = vaNode["Radius"].as<float>();
				int sectorCount = vaNode["SectorCount"].as<int>();
				int stackCount = vaNode["StackCount"].as<int>();
				vertexArray = VertexArray::CreateSphere(radius, sectorCount, stackCount);
			}
			else if (vaType == "Model")
			{
				// TODO: 模型加载
				vertexArray = VertexArray::CreateCube(glm::vec3(1.0f));
			}

			// 材质
			auto matNode = entityNode["Mesh"]["Material"];
			auto material = Material::Create(matNode["Name"].as<std::string>());
			std::string shadingModel = matNode["ShadingModel"].as<std::string>();
			if (shadingModel == "BlinnPhong")
			{
				// 漫反射颜色
				material->SetAlbedo(matNode["Albedo"].as<glm::vec3>());
				if (matNode["AlbedoTexturePath"])
				{
					std::string albedoTexPath = matNode["AlbedoTexturePath"].as<std::string>();
					auto albedoTexture = TextureLibrary::Get().LoadSync(albedoTexPath);
					material->SetAlbedoTexture(albedoTexture);
				}

				// 高光色
				material->SetSpecularColor(matNode["SpecularColor"].as<glm::vec3>());
				if (matNode["SpecularTexturePath"])
				{
					std::string specularTexPath = matNode["SpecularTexturePath"].as<std::string>();
					auto specularTexture = TextureLibrary::Get().LoadSync(specularTexPath);
					material->SetSpecularTexture(specularTexture);
				}

				// 高光强度
				material->SetShininess(matNode["Shininess"].as<float>());
				if (matNode["ShininessTexturePath"])
				{
					std::string shininessTexPath = matNode["ShininessTexturePath"].as<std::string>();
					auto shininessTexture = TextureLibrary::Get().LoadSync(shininessTexPath);
					material->SetShininessTexture(shininessTexture);
				}

				// 法线贴图
				if (matNode["NormalMapPath"])
				{
					std::string normalMapPath = matNode["NormalMapPath"].as<std::string>();
					auto normalMap = TextureLibrary::Get().LoadSync(normalMapPath);
					material->SetNormalMap(normalMap);
				}

				// 自发光
				material->SetEmissive(matNode["Emissive"].as<glm::vec3>());
				if (matNode["EmissiveTexturePath"])
				{
					std::string emissiveTexPath = matNode["EmissiveTexturePath"].as<std::string>();
					auto emissiveTexture = TextureLibrary::Get().LoadSync(emissiveTexPath);
					material->SetEmissiveTexture(emissiveTexture);
				}

				// 透明度
				material->SetOpacity(matNode["Opacity"].as<float>());
			}
			else if (shadingModel == "PBR")
			{
				// TODO: PBR
			}
			else
			{
				// TODO: Unlit
			}

			// Shader
			std::string shaderPath = entityNode["Mesh"]["ShaderPath"].as<std::string>();
			auto shader = ShaderLibrary::Get().LoadSync(shaderPath);

			// 添加Mesh组件
			entity.AddComponent<MeshComponent>(vertexArray, material, shader);
		}

		// Model组件
		if (entityNode["Model"])
		{
			std::string modelPath = entityNode["Model"]["ModelPath"].as<std::string>();
			auto& modelComp = entity.AddComponent<ModelComponent>(modelPath);
		}

		// Grid组件
		if (entityNode["Grid"])
		{
			auto& gridNode = entityNode["Grid"];

			// Shader
			std::string shaderPath = gridNode["ShaderPath"].as<std::string>();
			auto shader = ShaderLibrary::Get().LoadSync(shaderPath);

			// VertexArray
			auto vaNode = gridNode["VertexArray"];
			float size = vaNode["Size"].as<float>();
			int divisions = vaNode["Divisions"].as<int>();
			auto gridVAO = VertexArray::CreateGrid(size, divisions);

			// 添加Grid组件
			auto& gridComp = entity.AddComponent<GridComponent>();
			gridComp.m_Shader = shader;
			gridComp.m_Grid = gridVAO;
		}

		// Camera组件
		if (entityNode["Camera"])
		{
			auto& cameraComp = entity.AddComponent<CameraComponent>();
			float aspect = entityNode["Camera"]["Aspect"].as<float>();
			float fov = entityNode["Camera"]["FOV"].as<float>();
			float nearClip = entityNode["Camera"]["NearClip"].as<float>();
			float farClip = entityNode["Camera"]["FarClip"].as<float>();
			//cameraComp.m_Camera.SetAspectRatio(aspect);
			cameraComp.m_Camera.SetFov(fov);
			cameraComp.m_Camera.SetNearClip(nearClip);
			cameraComp.m_Camera.SetFarClip(farClip);
		}

		// 点光源组件
		if (entityNode["PointLight"])
		{
			auto& pointLightComp = entity.AddComponent<PointLightComponent>();
			auto& pointLight = pointLightComp.m_PointLight;
			pointLight.Ambient = entityNode["PointLight"]["Ambient"].as<glm::vec3>();
			pointLight.Diffuse = entityNode["PointLight"]["Diffuse"].as<glm::vec3>();
			pointLight.Specular = entityNode["PointLight"]["Specular"].as<glm::vec3>();
			pointLight.Constant = entityNode["PointLight"]["Constant"].as<float>();
			pointLight.Linear = entityNode["PointLight"]["Linear"].as<float>();
			pointLight.Quadratic = entityNode["PointLight"]["Quadratic"].as<float>();
		}

		// 平行光组件
		if (entityNode["DirectionalLight"])
		{
			auto& dirLightComp = entity.AddComponent<DirectionalLightComponent>();
			auto& dirLight = dirLightComp.m_DirectionalLight;
			dirLight.Ambient = entityNode["DirectionalLight"]["Ambient"].as<glm::vec3>();
			dirLight.Diffuse = entityNode["DirectionalLight"]["Diffuse"].as<glm::vec3>();
			dirLight.Specular = entityNode["DirectionalLight"]["Specular"].as<glm::vec3>();
			dirLight.Direction = entityNode["DirectionalLight"]["Direction"].as<glm::vec3>();
		}

		// 天空盒组件
		if (entityNode["Skybox"])
		{
			auto& skyboxComp = entity.AddComponent<SkyboxComponent>();
			std::string skyboxPath = entityNode["Skybox"]["SkyboxPath"].as<std::string>();
			skyboxComp.m_Cubemap = CubemapTexture::Create(skyboxPath);
			std::string shaderPath = entityNode["Skybox"]["ShaderPath"].as<std::string>();
			skyboxComp.m_Shader = ShaderLibrary::Get().LoadSync(shaderPath);
		}
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		auto view = m_Scene->m_Registry.view<TagComponent>();
		for (auto e : view)
		{
			Entity entity(e, m_Scene.get());
			if (!entity) continue;
			SerializeEntity(out, entity);
		}

		out << YAML::EndMap;

		// 输出到文件
		std::ofstream ofs(filepath);
		ofs << out.c_str();

	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		EMBER_NOT_IMPLEMENTED();
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		Timer timer;

		// 读取文件，检查文件格式
		YAML::Node data = YAML::LoadFile(filepath);
		if (!data["Scene"])
			return false;

		// 读取实体数据
		auto entities = data["Entities"];
		for (auto entityNode : entities)
		{
			DeserializeEntity(entityNode);
		}

		EMBER_CORE_TRACE("Scene deserialized in {0} ms", timer.ElapsedMillis());

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		EMBER_NOT_IMPLEMENTED();
		return false;
	}

	void SceneSerializer::SerializePrefab(const uint32_t entityID, const std::string& filepath)
	{
		YAML::Emitter out;
		Entity entity((entt::entity)entityID, m_Scene.get());
		SerializeEntity(out, entity);

		// 输出到文件
		std::ofstream ofs(filepath);
		ofs << out.c_str();

	}

	bool SceneSerializer::DeserializePrefab(const std::string& filepath)
	{
		// 读取文件，检查文件格式
		YAML::Node data = YAML::LoadFile(filepath);
		if (!data["EntityID"])
			return false;

		bool bUseTransform = false;	// 不使用Prefab的Transform
		DeserializeEntity(data, bUseTransform);

		return true;
	}
}
