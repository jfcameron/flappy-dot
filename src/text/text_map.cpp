// © Joseph Cameron - All Rights Reserved

#include <gdk/text_map.h>

#include <stdexcept>
#include <string>

using namespace gdk;

static constexpr char TAG[] = "text_map";

text_map::text_map(texture_ptr_type aTexture,
	const texture_size_in_cells_type &aTextureSizeInCells,
	const code_point_to_cell_coordinate_map_type &aCodePointToCellCoordinateMap)
: m_Texture(aTexture)
, m_TextureSizeInCells(aTextureSizeInCells)
, m_CodePointToCellCoordinateMap(aCodePointToCellCoordinateMap)
{}

const text_map::cell_coordinate_type &text_map::raster_coordinate(wchar_t aCodePoint) const
{
	if (auto search = m_CodePointToCellCoordinateMap.find(aCodePoint); 
		search != m_CodePointToCellCoordinateMap.end()) return search->second;
	
	throw std::invalid_argument(std::string(TAG) + 
		": text map does not contain the code point: " + std::to_string(static_cast<long>(aCodePoint)));
}

const text_map::texture_ptr_type &text_map::texture() const
{
	return m_Texture;
}

const text_map::texture_size_in_cells_type &text_map::texture_size_in_cells() const
{
	return m_TextureSizeInCells;
}
