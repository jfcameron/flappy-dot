// © Joseph Cameron - All Rights Reserved

#ifndef GDK_TEXT_MAP_H
#define GDK_TEXT_MAP_H

#include <gdk/graphics/texture.h>
#include <gdk/graphics/types.h>

#include <memory>
#include <unordered_map>

namespace gdk
{
	/// \brief data type, used to render text when given to a text renderer
	///
	/// maps code points to cells within a texture containing a grid of evenly sized character rasters
	class text_map final
	{
	public:
		using texture_ptr_type = graphics::texture_ptr_type;
		using texture_size_in_cells_type = graphics::intvector2_type;
		using cell_coordinate_type = graphics::intvector2_type;
		using code_point_to_cell_coordinate_map_type = std::unordered_map<wchar_t, cell_coordinate_type>;

	private:
		//! the texture containing the rasterized codepoints
		texture_ptr_type m_Texture;

		//! size of the texture in terms of cells
		texture_size_in_cells_type m_TextureSizeInCells;

		//! maps unicode code points to the cell within the texture where the appropriate raster is found
		code_point_to_cell_coordinate_map_type m_CodePointToCellCoordinateMap;

	public:
		//! gets a ptr to the texture containing character rasters
		const texture_ptr_type &texture() const;

		//! texture dimensions not in texels but in number of character rasters
		const texture_size_in_cells_type &texture_size_in_cells() const;

		//! returns the cell that contains the appropriate raster
		/// \warning throws if a raster does not exist for the code point
		const cell_coordinate_type &raster_coordinate(wchar_t aCodePoint) const;

		text_map(texture_ptr_type aTexture,
			const texture_size_in_cells_type &aTextureSizeInCells,
			const code_point_to_cell_coordinate_map_type &aCodePointToCellCoordinateMap);
	};
}

#endif
