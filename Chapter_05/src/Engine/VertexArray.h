#pragma once

namespace jLab
{
	class VertexArray
	{
	public:
		VertexArray(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices);
		~VertexArray();

		void SetActive();

		unsigned int GetNumVerts() { return m_NumVerts; }
		unsigned int GetNumIndices() { return m_NumIndices; }

	private:
		unsigned int m_NumVerts;
		unsigned int m_NumIndices;

		unsigned int m_VertexBuffer;
		unsigned int m_IndexBuffer;
		unsigned int m_VertexArray;
	};
}