/***************************************************************
 * Name:      OsdBmpApp.h
 * Purpose:   Defines Application Class
 * Author:    Andy (kwikius@yahoo.com)
 * Created:   2013-03-06
 * Copyright: Andy (http://www.softpainter.org)
 * License:
 **************************************************************/

#ifndef QUANTRACKER_SIMAPP_H
#define QUANTRACKER_SIMAPP_H

#include <wx/app.h>
#include <wx/timer.h>

#include <quan/serial_port.hpp>
#include <wx/thread.h>
#include <wx/config.h>

class document;
class main_frame;
class panel;
struct view;
struct bitmap_preview;

class OsdBmpApp : public wxApp
{
public:
   OsdBmpApp();
   virtual bool OnInit();
   ~OsdBmpApp();
   document* get_document()const {assert(m_document);return m_document;}
   main_frame* get_main_frame()const{assert(m_frame); return m_frame;}
   view * get_view()const;
   panel* get_panel() const;

   bitmap_preview* get_bitmap_preview()const;
    
   wxConfig* get_config()const {return m_app_config;}

   void set_sp(quan::serial_port* sp_in)
   {
      assert(! have_sp());
      m_sp = sp_in;
   }
   void close_sp()
   {
      assert( have_sp());
      delete m_sp;
      m_sp = nullptr;
   }

  bool have_sp()const { return m_sp != nullptr;}
  quan::serial_port* get_sp()const 
  {
       assert( have_sp());
       return m_sp;
  }
  
  wxCriticalSection m_sp_CS;
  
private:
   main_frame* m_frame;
   document* m_document;
   quan::serial_port* m_sp;
   wxConfig* m_app_config;
   void init_mainframe();
};

DECLARE_APP(OsdBmpApp)

#endif // QUANTRACKER_SIMAPP_H
