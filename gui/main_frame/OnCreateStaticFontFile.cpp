
#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>
#include <quan/uav/osd/detail/writable_bitmap.hpp>
#include <fstream>
#include "../../document.hpp"
#include "../../osd_bmp_app.hpp"
#include "../panel.hpp"
#include "../main_frame.hpp"
#include <wx/choicdlg.h>

using quan::gx::wxwidgets::from_wxString;
using quan::gx::wxwidgets::to_wxString;

void main_frame::OnCreateStaticFontFile(wxCommandEvent & event)
{
   wxArrayString choices = wxGetApp().get_panel()->get_font_names();
   
   wxMultiChoiceDialog dlg{
      this
      ,wxT("Choose fonts")
      ,wxT("Fonts to header")
      ,choices
   };
   if (dlg.ShowModal() == wxID_OK){

     // todo : FileDialog  put all in one file
      std::string filename 
     // = "/home/andy/cpp/projects/test/bitmap/fonts.cpp";
      = "/home/andy/cpp/projects/osd_draw/fonts.cpp";

      wxArrayInt result = dlg.GetSelections();
      // check result ok
      std::ofstream out (filename);

      out << "//Generated by OSDMaker\n\n";
      out << "#include <quan/uav/osd/api.hpp>\n";
      out << "#include <quan/uav/osd/basic_bitmap.hpp>\n";
      out << "#include <quan/uav/osd/basic_font.hpp>\n\n";
      out << "namespace {\n\n";

      // check out ok
      for ( size_t i = 0; i < result.GetCount();++i){
         std::string fnt_name = from_wxString<char>(choices[result[i]]);
         // get the font of that name
         typedef quan::uav::osd::dynamic::font dynamic_font;
         dynamic_font* fnt 
            = wxGetApp().get_document()->get_font(fnt_name);
         if ( fnt ){
            db_font const * dbf = dynamic_cast<db_font const *>(fnt);
            if ( dbf){
               dbf->output_header( fnt_name + "_type", fnt_name,out);
            }
         }
      } 

      out << "\n constexpr quan::uav::osd::font_ptr font_array[] =\n";
      out << "   {\n";
      for ( size_t i = 0; i < result.GetCount();++i){
         std::string font_name = from_wxString<char>(choices[result[i]]);
         out << "    ";
         if ( i != 0){
            out << ",";
         }
         out << "&font_" << font_name  <<'\n';
      }
      out << "   };\n} // namespace\n\n";
      out << "quan::uav::osd::font_ptr\n";
      out << "quan::uav::osd::get_font(uint32_t id)\n";
      out << "{\n";
      out << "   constexpr uint32_t size = sizeof(font_array)/sizeof(quan::uav::osd::font_ptr);\n";
      out << "   if ( id < size){\n";
      out << "      return font_array[id];\n";
      out << "   }else{\n";
      out << "      return nullptr;\n";
      out << "   }\n";
      out << "}\n";
   }
}
