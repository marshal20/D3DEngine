#pragma once
// This file holdes all classes that manage states:
// - BlendState
// - DepthStencilState

#include <memory>

#include "pointerutil.h"

struct ID3D11BlendState;
struct ID3D11DepthStencilState;

/* 
TODO: 
- sampler state
*/


class BlendState
{
public:
	BlendState();
	~BlendState() = default;

	void use();

private:
	InterPtr<ID3D11BlendState> m_pBlendState;
};

class DepthStencilState
{
public:
	DepthStencilState();
	~DepthStencilState() = default;

	void use();

private:
	InterPtr<ID3D11DepthStencilState> m_pDepthStencilState;
};