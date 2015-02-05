#include "panel_splitter.hpp"

panel_splitter::panel_splitter(wxWindow* parent) : wxSplitterWindow(parent, wxID_ANY)
{
   SetSashGravity(0.75);
  // SetSashPosition(2000);
   SetMinimumPaneSize(200);
   m_panel = new panel(this);

   m_font_preview = new font_preview(this);
   
   SplitHorizontally(m_panel, m_font_preview);
}