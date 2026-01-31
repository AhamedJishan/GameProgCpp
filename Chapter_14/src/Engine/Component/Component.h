#pragma once

#include <rapidjson/document.h>

namespace jLab
{
	class Component
	{
	public:
		enum TypeID
		{
			TComponent = 0,
			TAudioComponent,
			TBoxComponent,
			TCameraComponent,
			TFollowCameraComponent,
			TFPSCameraComponent,
			TMeshComponent,
			TMirrorCameraComponent,
			TMoveComponent,
			TPointLightComponent,
			TSkinnedMeshComponent,
			TSpriteComponent,

			NUM_COMPONENT_TYPES
		};

	public:
		Component(class Actor* owner, int updateOrder = 100);
		virtual ~Component();

		virtual void Update(float deltaTime);
		virtual void Input(const struct InputState inputState);

		virtual void OnUpdateWorldTransform();

		int GetUpdateOrder() const { return mUpdateOrder; }
		class Actor* GetOwner() const { return mOwner; }

		virtual TypeID GetType() const = 0;

		// Load/Save
		virtual void LoadProperties(const rapidjson::Value& inObj);

		template<typename T>
		static Component* Create(class Actor* actor, const rapidjson::Value& inObj)
		{
			T* t = new T(actor);
			t->LoadProperties(inObj);
			return t;
		}		

	protected:
		int mUpdateOrder;
		class Actor* mOwner;
	};
}