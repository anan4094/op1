#include "vapp.h"
VermillionApplication * VermillionApplication::s_app;

void VermillionApplication::DisplayFunc(void){
    s_app->Display();
}

void VermillionApplication::ReshapeFunc(int width,int height){
    s_app->Reshape(width, height);
}

void VermillionApplication::MainLoop(void){
    for (;;){
        GlutMainLoopEvent();
    }
}

DEBUG_OUTPUT_CALLBACK