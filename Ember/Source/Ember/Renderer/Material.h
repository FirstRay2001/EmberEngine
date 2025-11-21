// Material.h
// 材质类
// created by FirstRay2001, Nov/6/2025

#pragma once

#include "Ember/Core/Core.h"
#include <glm/glm.hpp>

namespace Ember
{
	class Texture2D;
	class Shader;

	class Material
	{
	public:
		// 着色模型
		enum class ShadingModel
		{
			Unlit,
			BlinnPhong,
			PBR
		};


		Material(const std::string& name = "DefaultMaterial");

		// 获取材质名称
		const std::string& GetName() const { return m_Name; }

		// 获取着色模型
		ShadingModel GetShadingModel() const { return m_ShadingModel; }

		// 设置着色模型
		void SetShadingModelBlinnPhong() { m_ShadingModel = ShadingModel::BlinnPhong; }
		void SetShadingModelPBR() { m_ShadingModel = ShadingModel::PBR; }

		// 材质参数接口
		glm::vec3&		GetAlbedo()														{ return m_Albedo; }
		void			SetAlbedo(const glm::vec3& albedo)								{ m_Albedo = albedo; }
		Ref<Texture2D>& GetAlbedoTexture()												{ return m_AlbedoTexture; }
		void			SetAlbedoTexture(const Ref<Texture2D>& texture)					{ m_AlbedoTexture = texture; }
		Ref<Texture2D>& GetNormalMap()													{ return m_NormalMap; }
		void			SetNormalMap(const Ref<Texture2D>& texture)						{ m_NormalMap = texture; }
		glm::vec3&		GetEmissive()													{ return m_Emissive; }
		void			SetEmissive(const glm::vec3& emissive)							{ m_Emissive = emissive; }
		Ref<Texture2D>& GetEmissiveTexture()											{ return m_EmissiveTexture; }
		void			SetEmissiveTexture(const Ref<Texture2D>& texture)				{ m_EmissiveTexture = texture; }
		float&			GetOpacity()													{ return m_Opacity; }
		void			SetOpacity(float opacity)										{ m_Opacity = opacity; }
		glm::vec3&		GetSpecularColor()												{ return m_SpecularColor; }
		void			SetSpecularColor(const glm::vec3& color)						{ m_SpecularColor = color; }
		Ref<Texture2D>& GetSpecularTexture()											{ return m_SpecularTexture; }
		void			SetSpecularTexture(const Ref<Texture2D>& texture)				{ m_SpecularTexture = texture; }
		float&			GetShininess()													{ return m_Shininess; }
		void			SetShininess(float shininess)									{ m_Shininess = shininess; }
		Ref<Texture2D>& GetShininessTexture()											{ return m_ShininessTexture; }
		void			SetShininessTexture(const Ref<Texture2D>& texture)				{ m_ShininessTexture = texture; }
		float&			GetGlossiness()													{ return m_Glossiness; }
		void			SetGlossiness(float glossiness)									{ m_Glossiness = glossiness; }
		Ref<Texture2D>& GetSpecularGlossinessTexture()									{ return m_SpecularGlossinessTexture; }
		void			SetSpecularGlossinessTexture(const Ref<Texture2D>& texture)		{ m_SpecularGlossinessTexture = texture; }

		// 判断是否使用某个纹理
		bool HasAlbedoTexture() const { return m_AlbedoTexture != nullptr; }
		bool HasNormalMap() const { return m_NormalMap != nullptr; }
		bool HasEmissiveTexture() const { return m_EmissiveTexture != nullptr; }
		bool HasSpecularTexture() const { return m_SpecularTexture != nullptr; }
		bool HasShininessTexture() const { return m_ShininessTexture != nullptr; }
		bool HasSpecularGlossinessTexture() const { return m_SpecularGlossinessTexture != nullptr; }

		// 应用到Shader
		void ApplyToShader(const Ref<Shader>& shader) const;

		// 创建材质
		static Ref<Material> Create(const std::string& name = "DefaultMaterial");

	protected:
		std::string m_Name;
		ShadingModel m_ShadingModel = ShadingModel::BlinnPhong;

		// ============ 通用参数 ====================
		glm::vec3			m_Albedo				= { 0.8f, 0.8f, 0.8f };
		Ref<Texture2D>		m_AlbedoTexture			= nullptr;
		Ref<Texture2D>		m_NormalMap				= nullptr;
		glm::vec3			m_Emissive				= { 0.0f, 0.0f, 0.0f };
		Ref<Texture2D>		m_EmissiveTexture		= nullptr;
		float				m_Opacity				= 1.0f;

		// ========= Phong/Blinn-Phong参数 =========
		glm::vec3 			m_SpecularColor			= { 1.0f, 1.0f, 1.0f };
		Ref <Texture2D>		m_SpecularTexture		= nullptr;
		float				m_Shininess				= 32.0f;
		Ref<Texture2D>		m_ShininessTexture		= nullptr;

		// ============ PBR参数 ====================
		float				m_Glossiness			= 0.5f;
		Ref<Texture2D>		m_SpecularGlossinessTexture = nullptr;
	};
}