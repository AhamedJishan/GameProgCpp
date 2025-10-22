#include "Model.h"

#include <glm/glm.hpp>

#include "Mesh.h"

namespace jLab
{
	Model::Model(const std::string& filename, Game* game, bool flipUvs, Skeleton* skeleton)
		:mGame(game)
		,mFlipUVs(flipUvs)
		,mSkeleton(skeleton)
		,mIsSkinned(false)
		,mAABB(glm::vec3(std::numeric_limits<float>::infinity()), glm::vec3(-std::numeric_limits<float>::infinity()))
	{
		mDirectory = filename.substr(0, filename.find_last_of('/') + 1);

		if (mSkeleton)
			mIsSkinned = true;

		Load();
		GenerateAABB();
	}

	Model::~Model()
	{
	}
	
	void Model::Load()
	{
	}
	
	void Model::GenerateAABB()
	{
		for (Mesh* mesh : mMeshes)
		{
			const std::vector<Vertex> vertices = mesh->GetVertices();
			for (Vertex vertex : vertices)
				mAABB.UpdateMinMax(vertex.Position);
		}
	}
}