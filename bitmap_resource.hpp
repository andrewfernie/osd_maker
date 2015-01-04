#ifndef OSD_BITMAP_MAKER_BITMAP_RESOURCE_HPP_INCLUDED
#define OSD_BITMAP_MAKER_BITMAP_RESOURCE_HPP_INCLUDED

#include <cstdint>
#include <map>
#include <set>
#include <vector>
#include "osd_image.hpp"
#include "font.hpp"

  struct bitmap_resource_t{
      bitmap_resource_t(): m_max_handle{m_min_handle}{}
      ~bitmap_resource_t(){
            for ( auto iter : m_osd_image_map){
               iter.second->destroy();
            }
            for ( auto iter : m_font_map){
                delete iter.second;
            }
      }
     // size_t get_num_fonts() const{ return m_fonts.size();}
      size_t get_num_bitmaps() const{ return m_bitmaps.size();}
      //font * get_font_at(size_t i);
       // for iterating through bitmap_handles
      bool get_bitmap_handle_at(size_t i, int & handle_out) const;
      osd_image* find_osd_image( int handle)const;
      font* find_font( int handle)const;
      // relinquish ownership of image and free its handle 
      // note that handles are replaced with invalid handles
      // use clean_bitmap_handles after iterating
      osd_bitmap* move_osd_bitmap(int handle);
      int add_bitmap( osd_bitmap*);
      int add_font_element( osd_bitmap*);
      int add_font(font*);
      void set_image_handle(int handle, osd_image* image);
      osd_bitmap* find_bitmap_by_name(std::string const & name_in)const;
      font* find_font_by_name(std::string const & name_in)const;
      
      std::string make_unique_bitmap_name(std::string const & name_in)const;
      std::string make_unique_font_name(std::string const & name_in)const;
      bool clean_bitmap_handles();
    private:
      // only bitmap handles
      std::vector<int> m_bitmaps;
      bitmap_resource_t (bitmap_resource_t const&) = delete;
      bitmap_resource_t& operator= (bitmap_resource_t const&) = delete;
      // holds font elements and bitmpas
      std::map<int,osd_image*> m_osd_image_map;
      std::map<int, font*> m_font_map;
      std::set<int> m_handles_in_use;
      int get_new_handle();
      bool free_handle(int n);
      int m_max_handle;
      static constexpr int m_min_handle = 1;
  };

#endif // OSD_BITMAP_MAKER_BITMAP_RESOURCE_HPP_INCLUDED
