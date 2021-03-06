#include <wx/bitmap.h>
#include <wx/rawbmp.h>
#include <wx/colour.h>

#include <quan/max.hpp>
#include <quan/gx/wxwidgets/from_wxString.hpp>
#include <quan/gx/wxwidgets/to_wxString.hpp>

#include <quan/uav/osd/dynamic/bitmap.hpp>
#include "../graphics_api/objects/db_font.hpp"

#include "../osd_bmp_app.hpp"
#include "../document.hpp"

#include "view.hpp"
#include "font_preview.hpp"
#include "font_preview/grid_cell_bmp_renderer.hpp"

using quan::gx::wxwidgets::from_wxString;
using quan::gx::wxwidgets::to_wxString;

bool font_preview::Destroy()
{
   m_grid->Destroy();
   return wxWindow::Destroy();
}

void font_preview::reset()
{
   m_current_font_handle = -1;
}

void font_preview::set_font_handle(int font_handle)
{
   m_current_font_handle = font_handle;
   this->Refresh();
}

db_font * font_preview::get_font()const
{
   if (m_current_font_handle == -1) {
      return nullptr;
   }

   return wxGetApp().get_document()->get_font(m_current_font_handle);
}

BEGIN_EVENT_TABLE(font_preview,wxWindow)

   EVT_GRID_CMD_CELL_LEFT_DCLICK(idGrid,font_preview::OnGridCellLeftDblClick)
   EVT_GRID_CMD_CELL_RIGHT_CLICK(idGrid,font_preview::OnGridCellRightClick)
END_EVENT_TABLE()

void font_preview::export_font_element_as_bitmap(int handle)
{
   document::dynamic_bitmap* bmp = wxGetApp().get_document()->get_bitmap(handle);
   if(  bmp == nullptr) {
      return;
   }
   //wxString old_name = to_wxString(bmp->get_name());
   // do a text dialog to get name
   wxTextEntryDialog dlg(this,wxT("Export Font element as Bitmap"),wxT("Enter name for new bitmap"));

   if ( dlg.ShowModal() == wxID_OK) {
      std::string name = from_wxString<char>(dlg.GetValue());
      //look for the name
      if( wxGetApp().get_document()->get_bitmap(name) != nullptr) {
         wxMessageBox(wxT("Bitmap with that name already exists"));
         return;
      }

      document::dynamic_bitmap* new_bitmap = bmp->clone();
      new_bitmap->set_name(name);
      wxGetApp().get_document()->add_bitmap(new_bitmap);
      wxGetApp().get_document()->set_modified(true);
   }
}

void font_preview::OnGridCellRightClick(wxGridEvent& event)
{

   int font_elem_handle = get_sel_font_element_handle(event);
   if ( font_elem_handle != -1) {
      wxString choices[] =
      {
         wxT("Export as Bitmap")
      };
      int style = wxDEFAULT_DIALOG_STYLE | wxOK | wxCANCEL | wxCENTRE;
      wxSingleChoiceDialog dlg {
         this
         ,wxT("Font Element Actions")
         ,wxT("Font Element Actions")
         ,sizeof(choices) / sizeof(wxString)
         ,(wxString*)choices
#if wxCHECK_VERSION(3,0,0)
         ,(void**)NULL
#else
        , NULL
#endif
         ,style
      };

      /*
      wxSingleChoice
      Dialog::wxSingleChoiceDialog(wxWindow*, const wxString&, const wxString&, int, c
      onst wxString*, char**, long int, const wxPoint&)
      */

      if (dlg.ShowModal() == wxID_OK) {
         switch (dlg.GetSelection()) {
         case 0:
            export_font_element_as_bitmap(font_elem_handle);
            break;
         case 1:
            //  delete
            break;
         }
      }
   }
}

// -1 if fail
int font_preview::get_sel_font_element_handle(wxGridEvent & event)const
{
   db_font* selected_font = this->get_font();
   if (selected_font == nullptr) {
      return -1;
   }
   int const x = event.GetCol();
   int const y = event.GetRow();
   int const num_cols = m_grid->GetNumberCols();
   int const font_pos = num_cols * y + x;
   int const ch = font_pos ;
   int font_element_handle = -1;
   if ( selected_font->get_handle_at(ch, font_element_handle)) {
      return font_element_handle;
   } else {
      return -1;
   }
}

void font_preview::OnGridCellLeftDblClick(wxGridEvent& event)
{
   int font_elem_handle = get_sel_font_element_handle(event);
   if ( font_elem_handle != -1) {
      wxGetApp().get_view()->sync_with_image_handle(font_elem_handle);
   }
}

font_preview::font_preview(wxWindow* parent)
   : wxWindow {parent, wxID_ANY},m_grid {nullptr},m_current_font_handle {-1}
{
   this->SetBackgroundColour(*wxWHITE);
   this->ClearBackground();
   wxSize cell_size{14 *2 +1,18 *2 +1};
   int num_elements = 256;
   int const num_rows = 8;
   int const rem = ((num_elements % num_rows)==0)?0:1;
   int const num_cols = (num_elements / num_rows) + rem;
   wxSize grid_size{num_cols * cell_size.x,num_rows * cell_size.y};
   this->m_grid = new wxGrid{this,idGrid,wxPoint{0,0},grid_size*2};
   m_grid->CreateGrid(num_rows,num_cols);
   m_grid->SetDefaultColSize(cell_size.x);
   m_grid->SetDefaultRowSize(cell_size.y);
   m_grid->SetRowLabelSize(0);
   m_grid->SetColLabelSize(0);
   m_grid->DisableDragGridSize();
   auto renderer = new grid_cell_bmp_renderer{};
   m_grid->SetDefaultRenderer(renderer);
   for ( int y = 0; y < num_rows; ++y) {
      for ( int x = 0; x < num_cols; ++x) {
         int ar_pos = y * num_cols + x;
         if ( ar_pos < num_elements) {
            char ch = static_cast<char>(ar_pos);
            m_grid->SetCellValue(y,x,wxString::Format(wxT("%c"),ch));
            m_grid->SetReadOnly(y,x);
         }
      }
   }
   this->Refresh();
}
