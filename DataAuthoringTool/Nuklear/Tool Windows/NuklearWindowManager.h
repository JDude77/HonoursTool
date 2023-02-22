#pragma once
#ifndef NUKLEAR_WINDOW_MANAGER
#define NUKLEAR_WINDOW_MANAGER
#include "NuklearWindow.h"
#include <vector>

#include "../../Data/Shared/DataInterfaces.h"
using std::vector;

class NuklearWindowManager
{
public:
	NuklearWindowManager(){};
	bool RenderAllActiveWindows(nk_context* nuklearContext)
	{
		for (NuklearWindow* window : activeNuklearWindows_)
		{
			if(const bool success = RenderWindow(nuklearContext, window); !success) return false;
		}//End for
	}//End RenderAllActiveWindows
	bool CreateNewWindow(char* windowTitle, struct nk_rect windowStartSize){return true;}

private:
	vector<NuklearWindow*> activeNuklearWindows_;
	bool RenderWindow(nk_context* nuklearContext, NuklearWindow* nuklearWindow)
	{
		shared_ptr<PrimaryData> windowData = nuklearWindow->GetWindowData();
		if(windowData != nullptr)
		{
			if(nk_begin(nuklearContext, nuklearWindow->GetWindowTitle(), nk_rect(25, 50, 100, 100),
		    NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
		    NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE))
			{
				//Render Header
				//Set name, set ID, set member template type
				nk_layout_row_dynamic(nuklearContext, 24, 4);
					//Name
					nk_label(nuklearContext, "Name: ", NK_TEXT_LEFT);
					nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, windowData->GetNameBuffer(), windowData->GetNameBufferCurrentLength(), windowData->GetBufferMax(), nk_filter_default);

					//ID
					nk_label(nuklearContext, "ID: ", NK_TEXT_LEFT);
					nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, windowData->GetIDBuffer(), windowData->GetIDBufferCurrentLength(), windowData->GetBufferMax(), nk_filter_default);
				
				//Render Body

				//Render Footer
				//Save, export, load, delete buttons
				nk_layout_row_dynamic(nuklearContext, 24, 1);
					if(nk_button_label(nuklearContext, "SAVE")) windowData->Save();
					if(nk_button_label(nuklearContext, "LOAD")) windowData->Load();
					if(nk_button_label(nuklearContext, "EXPORT")) windowData->Export();
					if(nk_button_label(nuklearContext, "DELETE")) windowData->Delete();
				
			}//End if
			nk_end(nuklearContext);
		}//End if
		else
		{
			return false;
		}//End else
		
		return true;
	}//End RenderWindow
};
#endif