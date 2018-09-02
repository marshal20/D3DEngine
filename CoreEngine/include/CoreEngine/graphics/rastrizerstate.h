#pragma once

struct ID3D11RasterizerState;

namespace ce
{
	class RasterizerState
	{
	public:
		enum class Cull { Back = 1, Front, None };
		enum class Fill { Solid = 1, Wireframe };
		enum class Rotation { CW = 1, CCW };

	private:
		ID3D11RasterizerState* m_resterizer_state = nullptr;

	public:
		RasterizerState();
		~RasterizerState();

		void init(Cull cull_mode, Fill fill_mode = Fill::Solid, Rotation front_face = Rotation::CCW);
		void cleanup();

		void use();
	};
}
