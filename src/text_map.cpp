#include <gdk/text_map.h>

using namespace gdk;

constexpr char TAG[] = "text_map";

text_map::text_map(texture_ptr_type aTexture,
	const texture_size_in_cells_type& aTextureSizeInCells,
	const code_point_to_cell_coordinate_map_type& aCodePointToCellCoordinateMap)
: m_Texture(aTexture)
, m_TextureSizeInCells(aTextureSizeInCells)
, m_CodePointToCellCoordinateMap(aCodePointToCellCoordinateMap)
{}

text_map::cell_coordinate_type text_map::get_raster_coordinate(wchar_t aCodePoint)
{
	auto search = m_CodePointToCellCoordinateMap.find(aCodePoint);
	
	if (search != m_CodePointToCellCoordinateMap.end())
		return m_CodePointToCellCoordinateMap.at(aCodePoint);
	
	throw std::runtime_error(std::string(TAG) + 
		": text map does not contain the character: \"" + 
		/*aCodePoint +*/ "\"");
}

decltype(text_map::m_Texture) text_map::getTexture() const
{
	return m_Texture;
}

decltype(text_map::m_TextureSizeInCells) text_map::getTextureSizeInCells() const
{
	return m_TextureSizeInCells;
}
