// © 2020 Joseph Cameron - All Rights Reserved

#ifndef GDK_TEXT_MAP_H
#define GDK_TEXT_MAP_H

#include<gdk/texture.h>
#include<gdk/vector2.h>
#include<gdk/intvector2.h>

#include<unordered_map>
#include<memory>

namespace gdk
{
	/// \brief data type, used to render text when given to a text renderer
	/// 
	// TODO: expand to support multiple textures, so I can map codepoints to rasters in multiple textures,
	// absolutely mandatory for chinese character support
	class text_map final
	{
	public:
		using texture_ptr_type = std::shared_ptr<gdk::texture>;
		using cell_size_type = gdk::Vector2<double>;
		using texture_size_in_cells_type = gdk::IntVector2<size_t>;
		using cell_coordinate_type = gdk::IntVector2<size_t>;
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
        decltype(m_Texture) getTexture() const;

		//! texture dimensions not in texels but in number of character rasters
        decltype(m_TextureSizeInCells) getTextureSizeInCells() const;

		//! returns the cell that contains the appropriate raster
		/// \warn exception if a raster does not exist for the code point
		cell_coordinate_type get_raster_coordinate(wchar_t aCodePoint);

		text_map(texture_ptr_type aTexture,
			const texture_size_in_cells_type &aTextureSizeInCells,
			const code_point_to_cell_coordinate_map_type & aCodePointToCellCoordinateMap);

		//todo: operators
	};
}

#endif
