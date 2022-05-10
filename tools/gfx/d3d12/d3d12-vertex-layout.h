// d3d12-vertex-layout.h
#pragma once

#include "../renderer-shared.h"
#include "d3d12-buffer.h"
#include <d3d12.h>

namespace gfx
{
namespace d3d12
{

using namespace Slang;

class InputLayoutImpl : public InputLayoutBase
{
public:
    List<D3D12_INPUT_ELEMENT_DESC> m_elements;
    List<UINT> m_vertexStreamStrides;
    List<char> m_text; ///< Holds all strings to keep in scope
};

} // namespace d3d12
} // namespace gfx
